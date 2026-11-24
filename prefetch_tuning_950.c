// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright(c) 2019 Huawei Technologies Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * Create: 2026-11-24
 * Description: prefetch_tuning_950 is the prefetch tuning module for the
 *              Kunpeng 950 platform. It provides the reg_prefetch_ctl
 *              device attribute to set/clear bit 0 (the first bit, the
 *              CPU prefetch control bit) of the S3_1_c15_c6_4
 *              (CPUPREFCTRL_EL1) register on all online CPUs.
 */

#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/cpumask.h>
#include <linux/percpu.h>
#include <linux/smp.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <asm/sysreg.h>

#ifndef is_affinity_mask_valid
#define is_affinity_mask_valid(val) 1
#endif

/*
 * Value written to reg_prefetch_ctl: the desired state of bit 0 of
 * S3_1_c15_c6_4.  Bit 0 set (1) disables CPU prefetch, bit 0 cleared
 * (0) enables it, so the names describe the bit state rather than the
 * prefetch state.
 */
enum {
    REG_BIT_CLEAR = 0,
    REG_BIT_SET
};

/*
 * Bit 0 (the first bit) of S3_1_c15_c6_4 (CPUPREFCTRL_EL1) on Kunpeng
 * 950 is the CPU prefetch control bit:
 * 1--CPU prefetch disable, 0--CPU prefetch enable.
 */
#define REG_PREFETCH_CTL_CTRL (1UL << 0)

static cpumask_var_t prefetch_cpumask_value;
static DEFINE_MUTEX(prefetch_mtx);

static ssize_t prefetch_ctl_store(struct device* dev,
                                  struct device_attribute* attr, const char* buf, size_t count);
static ssize_t prefetch_ctl_show(struct device* dev,
                                 struct device_attribute* attr, char* buf);

/* Create device attribute files */
static DEVICE_ATTR(reg_prefetch_ctl, S_IRUGO|S_IWUSR,
                   prefetch_ctl_show, prefetch_ctl_store);

static struct attribute *prefetch_950_attrs[] = {
    &dev_attr_reg_prefetch_ctl.attr,
    NULL,
};

static const struct attribute_group prefetch_950_attr_group = {
    .attrs = prefetch_950_attrs,
};

static const struct attribute_group *attr_groups[] = {
    &prefetch_950_attr_group,
    NULL,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "prefetch",
    .groups = attr_groups,
};

/* 1--set bit 0 of S3_1_c15_c6_4, 0--clear bit 0 of S3_1_c15_c6_4 */
static void prefetch_ctl_set(void *dummy)
{
    int *value = (int *)dummy;

    if (*value == REG_BIT_SET)
        sysreg_clear_set(S3_1_c15_c6_4, 0, REG_PREFETCH_CTL_CTRL);
    else if (*value == REG_BIT_CLEAR)
        sysreg_clear_set(S3_1_c15_c6_4, REG_PREFETCH_CTL_CTRL, 0);

    return;
}

static void prefetch_ctl_get(void *dummy)
{
    int *value = this_cpu_ptr((int __percpu *)dummy);
    u64 reg_value = read_sysreg(S3_1_c15_c6_4);

    *value = (int)(reg_value & REG_PREFETCH_CTL_CTRL);
    return;
}

/* 0--bit 0 of S3_1_c15_c6_4 is cleared, 1--bit 0 is set */
static ssize_t prefetch_ctl_store(struct device* dev,
                                  struct device_attribute* attr, const char* buf, size_t count)
{
    ssize_t ret = -1;
    int value = -1;

    ret = kstrtoint(buf, 0, &value);
    if (ret || (value != 0 && value != 1)) {
        pr_err("invalid input!\n");
        return -EINVAL;
    }

    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, prefetch_ctl_set, &value, 1);
    mutex_unlock(&prefetch_mtx);

    return count;
}

/* Longest line emitted for one merged CPU run:
 * "cpu(" + up to 10 digits + '-' + up to 10 digits + "): x.\n" */
#define PREFETCH_CTL_RUN_LINE_MAX 32

/* Longest fragment emitted by prefetch_ctl_cpulist() for one range:
 * ',' + up to 10 digits + '-' + up to 10 digits. */
#define PREFETCH_CTL_LIST_MAX 24

/*
 * sysfs passes only one page (PAGE_SIZE) to show().  One line per CPU
 * needs ~13 bytes per CPU and does not fit any more on machines with
 * several hundred CPUs: the output is cut in the middle of a line (for
 * example after "cpu(322): 0. cpu(32").  Consecutive CPUs holding the
 * same value are therefore merged into one line, so that the state of
 * every CPU still fits into the page:
 *
 *     cpu(0-327): 0.
 *     cpu(1): 1.
 *
 * A single CPU keeps the old "cpu(N): V." format.
 */
static int prefetch_ctl_emit_run(char* buf, int count, int first, int last, int val)
{
    count += scnprintf(buf + count, PAGE_SIZE - count, "cpu(%d", first);
    if (last != first)
        count += scnprintf(buf + count, PAGE_SIZE - count, "-%d", last);
    count += scnprintf(buf + count, PAGE_SIZE - count, "): %d.\n", val);
    return count;
}

/*
 * Print mask as a kernel-style CPU list ("0-3,7,9-11") at buf + count
 * and return the new count.  Self-contained helper: cpulist_scnprintf()
 * is not declared by all kernels (it was removed in v5.14), so the
 * module must not depend on it.
 */
static int prefetch_ctl_cpulist(char* buf, int count,
                                const struct cpumask* mask)
{
    int cpu = -1;
    int start = -1;   /* first CPU of the range being printed */
    int prev = -1;    /* last CPU of the range being printed  */
    int printed = 0;  /* a range has been printed already     */

    for_each_cpu(cpu, mask) {
        if (start < 0) {
            start = prev = cpu;
            continue;
        }
        if (cpu != prev + 1) {
            /* Stop at a range boundary when the page runs out of
             * space, so the size argument passed to every
             * scnprintf() below stays positive and no range is cut
             * in the middle. */
            if (count > (int)PAGE_SIZE - PREFETCH_CTL_LIST_MAX)
                return count;
            if (printed)
                count += scnprintf(buf + count, PAGE_SIZE - count, ",");
            if (start == prev)
                count += scnprintf(buf + count, PAGE_SIZE - count,
                                   "%d", start);
            else
                count += scnprintf(buf + count, PAGE_SIZE - count,
                                   "%d-%d", start, prev);
            printed = 1;
            start = cpu;
        }
        prev = cpu;
    }
    if (start >= 0) {
        if (printed && count > (int)PAGE_SIZE - PREFETCH_CTL_LIST_MAX)
            return count;
        if (printed)
            count += scnprintf(buf + count, PAGE_SIZE - count, ",");
        if (start == prev)
            count += scnprintf(buf + count, PAGE_SIZE - count, "%d", start);
        else
            count += scnprintf(buf + count, PAGE_SIZE - count,
                               "%d-%d", start, prev);
    }
    return count;
}

/*
 * Fallback for the pathological case in which even the merged lines do
 * not fit into the page (the bit changes from CPU to CPU): report one
 * CPU list per value instead, so that every CPU is still visible.  Each
 * CPU of the mask shows up in exactly one of the two lists.
 * Called with prefetch_mtx held.
 */
static int prefetch_ctl_show_cpulist(char* buf, int __percpu *cur)
{
    cpumask_var_t ones, zeros;
    int cpu = -1;
    int count = 0;

    if (!alloc_cpumask_var(&ones, GFP_KERNEL) ||
        !alloc_cpumask_var(&zeros, GFP_KERNEL)) {
        free_cpumask_var(ones);
        free_cpumask_var(zeros);
        pr_err("alloc_cpumask_var fail\n");
        return -ENOMEM;
    }

    cpumask_clear(ones);
    cpumask_clear(zeros);
    for_each_cpu(cpu, prefetch_cpumask_value) {
        if (*per_cpu_ptr(cur, cpu))
            cpumask_set_cpu(cpu, ones);
        else
            cpumask_set_cpu(cpu, zeros);
    }

    /* A header plus one list fragment must still fit into the page;
     * prefetch_ctl_cpulist() applies the same check to its own
     * fragments, so the size argument of every scnprintf() call
     * below stays positive. */
    if (!cpumask_empty(ones) &&
        count <= (int)PAGE_SIZE - PREFETCH_CTL_LIST_MAX) {
        count += scnprintf(buf + count, PAGE_SIZE - count, "cpu(1): ");
        count = prefetch_ctl_cpulist(buf, count, ones);
        if (count < (int)PAGE_SIZE - 1)
            count += scnprintf(buf + count, PAGE_SIZE - count, "\n");
    }
    if (!cpumask_empty(zeros) &&
        count <= (int)PAGE_SIZE - PREFETCH_CTL_LIST_MAX) {
        count += scnprintf(buf + count, PAGE_SIZE - count, "cpu(0): ");
        count = prefetch_ctl_cpulist(buf, count, zeros);
        if (count < (int)PAGE_SIZE - 1)
            count += scnprintf(buf + count, PAGE_SIZE - count, "\n");
    }

    free_cpumask_var(ones);
    free_cpumask_var(zeros);
    return count;
}

/* 0--bit 0 of S3_1_c15_c6_4 is cleared, 1--bit 0 is set */
static ssize_t prefetch_ctl_show(struct device* dev,
                                 struct device_attribute* attr, char* buf)
{
    int cpu = -1;
    int count = 0;
    int runs = 0;    /* number of merged lines            */
    int first = -1;  /* first CPU of the run being merged */
    int last = -1;   /* last CPU of the run being merged  */
    int val = 0;     /* value of the run being merged     */

    int __percpu *cur = alloc_percpu(int);
    if (!cur) {
        pr_err("alloc_percpu fail\n");
        return -ENOMEM;
    }
    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, prefetch_ctl_get, cur, 1);

    /* Pass 1: count the merged lines needed by the output. */
    for_each_cpu(cpu, prefetch_cpumask_value) {
        int cur_val = *per_cpu_ptr(cur, cpu);

        if (first < 0) {
            first = last = cpu;
            val = cur_val;
        } else if (cur_val != val || cpu != last + 1) {
            runs++;
            first = cpu;
            val = cur_val;
        }
        last = cpu;
    }
    if (first >= 0)
        runs++;

    if ((unsigned long)runs * PREFETCH_CTL_RUN_LINE_MAX > PAGE_SIZE - 1) {
        /* Even the merged lines may not fit into the page. */
        count = prefetch_ctl_show_cpulist(buf, cur);
    } else {
        /* Pass 2: emit the merged lines. */
        first = -1;
        for_each_cpu(cpu, prefetch_cpumask_value) {
            int cur_val = *per_cpu_ptr(cur, cpu);

            if (first < 0) {
                first = last = cpu;
                val = cur_val;
            } else if (cur_val != val || cpu != last + 1) {
                count = prefetch_ctl_emit_run(buf, count, first, last, val);
                first = cpu;
                val = cur_val;
            }
            last = cpu;
        }
        if (first >= 0)
            count = prefetch_ctl_emit_run(buf, count, first, last, val);
    }

    mutex_unlock(&prefetch_mtx);
    free_percpu(cur);
    return count;
}

static int __init prefetch_950_init(void)
{
#ifdef CONFIG_ARCH_HISI
    int ret = -1;
    int cpu = -1;

    u32 implementor = read_cpuid_implementor();
    if (implementor != 0x48) {
        printk(KERN_ALERT "This is not HiSilicon CPU core;\n");
        return -ENODEV;
    }

    /* mask: initial a mask with all online CPUs */
    if (!alloc_cpumask_var(&prefetch_cpumask_value, GFP_KERNEL)) {
        ret = -ENOMEM;
        goto err_mask_alloc;
    }

    cpumask_clear(prefetch_cpumask_value);
    for_each_online_cpu(cpu)
    cpumask_set_cpu(cpu, prefetch_cpumask_value);

    if (!is_affinity_mask_valid(prefetch_cpumask_value)) {
        pr_err("invalid affinity_mask\n");
        ret = -EINVAL;
        goto err_mask_init;
    }

    /* initial prefetch misc and initial prefetch_950 */
    ret = misc_register(&misc);
    if (ret < 0) {
        pr_err("misc register fail\n");
        goto err_misc_reg;
    }

    return 0;

err_misc_reg:
err_mask_init:
    free_cpumask_var(prefetch_cpumask_value);
err_mask_alloc:
    return ret;
#else
    return 0;
#endif
}

static void __exit prefetch_950_exit(void)
{
#ifdef CONFIG_ARCH_HISI
    misc_deregister(&misc);
    free_cpumask_var(prefetch_cpumask_value);
#endif
}

module_init(prefetch_950_init);
module_exit(prefetch_950_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kunpeng 950 prefetch tuning module");
