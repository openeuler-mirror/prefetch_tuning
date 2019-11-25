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
 */

#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cpumask.h>
#include <linux/miscdevice.h>

#ifndef is_affinity_mask_valid
#define is_affinity_mask_valid(val) 1
#endif

#ifdef CONFIG_ARCH_HISI
typedef struct {
	long cpuprefctrl_el1;
	long adps_lld_ddr_el1;
	long adpp_l1v_mop_el1;
	long adps_lld_l3_el1;
} cfg_t;
#else
typedef long cfg_t;
#endif

static cpumask_var_t mask_value;
cfg_t __percpu *cur_cfg;
cfg_t __percpu *old_cfg;
static DEFINE_MUTEX(prefetch_mtx);

#ifdef CONFIG_ARCH_HISI
static cfg_t cfg[] = {
	[0] = {.cpuprefctrl_el1 = 0x112f8127f,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[1] = {.cpuprefctrl_el1 = 0x112f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[2] = {.cpuprefctrl_el1 = 0x112f81254,
			.adps_lld_ddr_el1 = 0x4d34a200,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[3] = {.cpuprefctrl_el1 = 0xb52f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[4] = {.cpuprefctrl_el1 = 0x112f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[5] = {.cpuprefctrl_el1 = 0x3012f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[6] = {.cpuprefctrl_el1 = 0x3012f81254,
			.adps_lld_ddr_el1 = 0x4d142000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[7] = {.cpuprefctrl_el1 = 0x3012f81254,
			.adps_lld_ddr_el1 = 0x4d142000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x4d145100},
	[8] = {.cpuprefctrl_el1 = 0x4112f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[9] = {.cpuprefctrl_el1 = 0x112f81260,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[10] = {.cpuprefctrl_el1 = 0x112f81260,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x658e5700},
	[11] = {.cpuprefctrl_el1 = 0x3412f81254,
			.adps_lld_ddr_el1 = 0x4d142000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[12] = {.cpuprefctrl_el1 = 0x3412F81260,
			.adps_lld_ddr_el1 = 0x4d12000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[13] = {.cpuprefctrl_el1 = 0x112f81240,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[14] = {.cpuprefctrl_el1 = 0x112f81240,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x69154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[15] = {.cpuprefctrl_el1 = 0x80110f81380,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
};

static void set_prefetch(void* dummy)
{
    cfg_t cfg = *((cfg_t *)dummy);

    __asm__ __volatile__(
	    "msr S3_1_c15_c6_4, %0 \n"
	    :
	    : "r"(cfg.cpuprefctrl_el1)
	    :
    );

    __asm__ __volatile__(
	    "msr S3_1_c15_c7_1, %0 \n"
	    :
	    : "r"(cfg.adps_lld_ddr_el1)
	    :
    );

    __asm__ __volatile__(
	    "msr S3_1_c15_c6_5, %0 \n"
	    :
	    : "r"(cfg.adpp_l1v_mop_el1)
	    :
    );

    __asm__ __volatile__(
	    "msr S3_1_c15_c7_0, %0 \n"
	    :
	    : "r"(cfg.adps_lld_l3_el1)
	    :
    );
}

static void get_prefetch(void* dummy)
{
    cfg_t *pcfg = this_cpu_ptr((cfg_t __percpu *)dummy);
    cfg_t cfg;

    __asm__ __volatile__(
	    "mrs %0, S3_1_c15_c6_4 \n"
	    : "=r"(cfg.cpuprefctrl_el1)
	    :
	    :
    );

    __asm__ __volatile__(
	    "mrs %0, S3_1_c15_c7_1 \n"
	    : "=r"(cfg.adps_lld_ddr_el1)
	    :
	    :
    );

    __asm__ __volatile__(
	    "mrs %0, S3_1_c15_c6_5 \n"
	    : "=r"(cfg.adpp_l1v_mop_el1)
	    :
	    :
    );

    __asm__ __volatile__(
	    "mrs %0, S3_1_c15_c7_0 \n"
	    : "=r"(cfg.adps_lld_l3_el1)
	    :
	    :
    );

    *pcfg = cfg;
}
#else
static cfg_t cfg[] = {};

static void set_prefetch(void* dummy)
{
}

static void get_prefetch(void* dummy)
{
}
#endif

static void reset_prefetch(void* dummy)
{
    cfg_t *pcfg = this_cpu_ptr((cfg_t __percpu *)dummy);
	set_prefetch(pcfg);
}

static ssize_t prefetch_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count)
{
	ssize_t ret;
	int value;

    ret = kstrtouint(buf, 0, &value);
    if (ret) {
		pr_err("invalid input\n");
		return count;
	}

	mutex_lock(&prefetch_mtx);
	if (value < ARRAY_SIZE(cfg)) {
		on_each_cpu_mask(mask_value, set_prefetch, &cfg[value], 1);
	} else {
		pr_err("value %d is out of range\n", value);
	}
	mutex_unlock(&prefetch_mtx);

	return count;
}

static ssize_t prefetch_show(struct device* dev,
		struct device_attribute* attr, char* buf)
{
	int cpu, policy;
	int count = 0;

	mutex_lock(&prefetch_mtx);
	on_each_cpu_mask(mask_value, get_prefetch, cur_cfg, 1);

	for_each_cpu(cpu, mask_value) {
		for (policy = ARRAY_SIZE(cfg) - 1; policy >= 0; policy--) {
			if (!memcmp(&cfg[policy], per_cpu_ptr(cur_cfg, cpu),
				sizeof(cfg_t)))
				break;
		}
		count += scnprintf(buf + count, PAGE_SIZE, "cpu(%d): %d\n",
					cpu, policy);
	}
	mutex_unlock(&prefetch_mtx);

	return count;
}

static ssize_t prefetch_mask_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count)
{
	int ret;

	mutex_lock(&prefetch_mtx);
	ret = cpulist_parse(buf, mask_value);
	if (ret) {
		pr_err("cpulist_parse error: %d\n", ret);
		goto prefetch_mask_end;
	}

	if (!is_affinity_mask_valid(mask_value)) {
		pr_err("mask value error\n");
		goto prefetch_mask_end;
	}

prefetch_mask_end:
	mutex_unlock(&prefetch_mtx);
	return count;
}

static ssize_t prefetch_mask_show(struct device* dev,
				struct device_attribute* attr, char* buf)
{
	ssize_t ret;
	mutex_lock(&prefetch_mtx);
	ret = cpumap_print_to_pagebuf(true, buf, mask_value);
	mutex_unlock(&prefetch_mtx);
	return ret;
}

/*
 * prefetch policy, can be 0~15:
 * 0: disable; 1~15: different thresholds for sms,amop algrithom;
 */
static DEVICE_ATTR(policy, S_IRUGO|S_IWUSR,
		prefetch_show, prefetch_store);

/* assign the effective cpu, can be a list */
static DEVICE_ATTR(cpumask, S_IRUGO|S_IWUSR,
		prefetch_mask_show, prefetch_mask_store);

static struct attribute *prefetch_attrs[] = {
    &dev_attr_policy.attr,
    &dev_attr_cpumask.attr,
    NULL,
};

static const struct attribute_group prefetch_attr_group = {
    .attrs = prefetch_attrs,
};

static const struct attribute_group *prefetch_attr_groups[] = {
    &prefetch_attr_group,
    NULL,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "prefetch",
	.groups = prefetch_attr_groups,
};

static int __init prefetch_init(void)
{
	int ret;
	int cpu;

	/* mask: initial a mask */
    if (!alloc_cpumask_var(&mask_value, GFP_KERNEL)) {
        ret = -ENOMEM;
        goto err_mask_alloc;
	}

	cpumask_clear(mask_value);
	for_each_online_cpu(cpu)
		cpumask_set_cpu(cpu, mask_value);

    if (!is_affinity_mask_valid(mask_value)) {
        pr_err("incalid affinity_mask\n");
        ret = -EINVAL;
        goto err_mask_init;
    }

	cur_cfg = alloc_percpu(cfg_t);
    if (!cur_cfg) {
        pr_err("alloc_percpu fail\n");
        ret = -ENOMEM;
        goto err_mask_init;
    }

	old_cfg = alloc_percpu(cfg_t);
    if (!old_cfg) {
        pr_err("alloc_percpu fail\n");
        ret = -ENOMEM;
        goto err_cfg_alloc;
    }

	on_each_cpu(get_prefetch, old_cfg, 1);

	/* initial prefetch misc and initial prefetch_ops */
	ret = misc_register(&misc);
    if (ret < 0) {
        pr_err("misc register fail\n");
        goto err_misc_reg;
    }

	return 0;

err_misc_reg:
	free_percpu(old_cfg);
err_cfg_alloc:
	free_percpu(cur_cfg);
err_mask_init:
	free_cpumask_var(mask_value);
err_mask_alloc:
	return ret;
}

static void __exit prefetch_exit(void)
{
	misc_deregister(&misc);
	on_each_cpu(reset_prefetch, old_cfg, 1);
	free_percpu(old_cfg);
	free_percpu(cur_cfg);
	free_cpumask_var(mask_value);
}

module_init(prefetch_init);
module_exit(prefetch_exit);
MODULE_LICENSE("GPL");
