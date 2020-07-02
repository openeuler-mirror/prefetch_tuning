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
#include "prefetch_mod.h"

#ifndef is_affinity_mask_valid
#define is_affinity_mask_valid(val) 1
#endif

static cpumask_var_t prefetch_cpumask_value;
static cfg_t __percpu *cur_cfg;
static cfg_t __percpu *old_cfg;
static DEFINE_MUTEX(prefetch_mtx);

static ssize_t cache_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count);
static ssize_t cache_show(struct device* dev,
		struct device_attribute* attr, char* buf);
static ssize_t prefetch_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count);
static ssize_t prefetch_show(struct device* dev,
		struct device_attribute* attr, char* buf);
static ssize_t prefetch_mask_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count);
static ssize_t prefetch_mask_show(struct device* dev,
				struct device_attribute* attr, char* buf);

/* Device create */
static DEVICE_ATTR(cache, S_IRUGO|S_IWUSR,
        cache_show, cache_store);

static DEVICE_ATTR(policy, S_IRUGO|S_IWUSR,
		prefetch_show, prefetch_store);

static DEVICE_ATTR(cpumask, S_IRUGO|S_IWUSR,
		prefetch_mask_show, prefetch_mask_store);


static struct attribute *prefetch_attrs[] = {
    &dev_attr_policy.attr,
    &dev_attr_cpumask.attr,
    &dev_attr_cache.attr,
    NULL,
};

static const struct attribute_group prefetch_attr_group = {
    .attrs = prefetch_attrs,
};

static const struct attribute_group *attr_groups[] = {
    &prefetch_attr_group,
    NULL,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "prefetch",
	.groups = attr_groups,
};

/* 0--close, 1--open */
static ssize_t cache_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count)
{
    ssize_t ret;
    int value = -1;

    ret = kstrtouint(buf, 0, &value);
    if (ret || (value != 0 && value != 1)) {
        pr_err("invalid input(0-close,1-open,other-invalid)!\n");
        return count;
    }

    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, cache_set, &value, 1);
    mutex_unlock(&prefetch_mtx);

    return count;
}

static ssize_t cache_show(struct device* dev,
		struct device_attribute* attr, char* buf)
{
    int cpu;
    int count = 0;

    int __percpu *cur = alloc_percpu(int);
    if (!cur) {
        pr_err("alloc_percpu fail\n");
        return -ENOMEM;
    }
    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, cache_get, cur, 1);
    
    for_each_cpu(cpu, prefetch_cpumask_value) {
        int *ptr = per_cpu_ptr(cur, cpu);
        count += scnprintf(buf + count, PAGE_SIZE, "cpu(%d): %s.\n",
                    cpu, (ptr == NULL) ? "n/a" : ((*ptr == 0) ? "close" : "open"));
    }
    mutex_unlock(&prefetch_mtx);
    free_percpu(cur);
    return count;
}


static ssize_t prefetch_store(struct device* dev,
		struct device_attribute* attr, const char* buf, size_t count)
{
	ssize_t ret;
	int policy = -1;
    
    ret = kstrtouint(buf, 0, &policy);
    if (ret) {
		pr_err("invalid input\n");
		return count;
	}

	mutex_lock(&prefetch_mtx);
	if (policy < prefetch_policy_num()) {
		on_each_cpu_mask(prefetch_cpumask_value, set_prefetch, &policy, 1);
	} else {
		pr_err("policy %d is out of range\n", policy);
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
	on_each_cpu_mask(prefetch_cpumask_value, get_prefetch, cur_cfg, 1);
	for_each_cpu(cpu, prefetch_cpumask_value) {
        cfg_t *this_cfg = per_cpu_ptr(cur_cfg, cpu);
		for (policy = prefetch_policy_num() - 1; policy >= 0; policy--) {
			if (!memcmp(prefetch_policy(policy), this_cfg, sizeof(cfg_t)))
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
	ret = cpulist_parse(buf, prefetch_cpumask_value);
	if (ret) {
		pr_err("cpulist_parse error: %d\n", ret);
		goto prefetch_mask_end;
	}

	if (!is_affinity_mask_valid(prefetch_cpumask_value)) {
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
	ret = cpumap_print_to_pagebuf(true, buf, prefetch_cpumask_value);
	mutex_unlock(&prefetch_mtx);
	return ret;
}

/*
 * prefetch policy, can be 0~15:
 * 0: disable; 1~15: different thresholds for sms,amop algrithom;
 */


static int __init prefetch_init(void)
{
	int ret;
	int cpu;
	/* mask: initial a mask */
    if (!alloc_cpumask_var(&prefetch_cpumask_value, GFP_KERNEL)) {
        ret = -ENOMEM;
        goto err_mask_alloc;
	}

	cpumask_clear(prefetch_cpumask_value);
	for_each_online_cpu(cpu)
		cpumask_set_cpu(cpu, prefetch_cpumask_value);

    if (!is_affinity_mask_valid(prefetch_cpumask_value)) {
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
	free_cpumask_var(prefetch_cpumask_value);
err_mask_alloc:
	return ret;
}

static void __exit prefetch_exit(void)
{
	misc_deregister(&misc);
	on_each_cpu(reset_prefetch, old_cfg, 1);
	free_percpu(old_cfg);
	free_percpu(cur_cfg);
	free_cpumask_var(prefetch_cpumask_value);
}

module_init(prefetch_init);
module_exit(prefetch_exit);
MODULE_LICENSE("GPL");
