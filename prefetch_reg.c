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
 * Create: 2020-07-02
 * Author: Liqiang (liqiang9102@gitee)
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
#include <asm/io.h>
#include "prefetch_mod.h"

#ifdef CONFIG_ARCH_HISI
#define PREFETCH_POLICY_MAX 15
static cfg_t prefetch_cfg[] = {
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

void set_prefetch(void* dummy)
{
    cfg_t *cfg = (cfg_t *)dummy;
    unsigned long read_uniq = read_sysreg(S3_1_c15_c6_4);
    if (cfg == NULL)
        return;
    read_uniq &= CACHE_READUNIQ_CTRL;
    write_sysreg(cfg->cpuprefctrl_el1 | read_uniq, S3_1_c15_c6_4);
    write_sysreg(cfg->adps_lld_ddr_el1, S3_1_c15_c7_1);
    write_sysreg(cfg->adpp_l1v_mop_el1, S3_1_c15_c6_5);
    write_sysreg(cfg->adps_lld_l3_el1, S3_1_c15_c7_0);
    return;
}

void get_prefetch(void* dummy)
{
    cfg_t *pcfg = this_cpu_ptr((cfg_t __percpu *)dummy);
    pcfg->cpuprefctrl_el1 = read_sysreg(S3_1_c15_c6_4);
    pcfg->adps_lld_ddr_el1 = read_sysreg(S3_1_c15_c7_1);
    pcfg->adpp_l1v_mop_el1 = read_sysreg(S3_1_c15_c6_5);
    pcfg->adps_lld_l3_el1 = read_sysreg(S3_1_c15_c7_0);
    
    /* Ignore READUNIQ bit */
    pcfg->cpuprefctrl_el1 &= ~(CACHE_READUNIQ_CTRL);
    return;
}

void read_unique_set(void *dummy)
{
    int *value = (int *)dummy;
    if (*value == ENABLE)
        sysreg_clear_set(S3_1_c15_c6_4, 0, CACHE_READUNIQ_CTRL);
    else if (*value == DISABLE)
        sysreg_clear_set(S3_1_c15_c6_4, CACHE_READUNIQ_CTRL, 0);
    
    return;
}

static const u64 readUniqueOffset = 40;
void read_unique_get(void *dummy)
{
    int *value = this_cpu_ptr((int __percpu *)dummy);
    u64 reg_value = read_sysreg(S3_1_c15_c6_4);
    *value = (reg_value >> readUniqueOffset) & 0x1;
    return;
}

static unsigned long skt_offset = 0x200000000000ULL;
static unsigned nr_skt = 2, totem_num = 1;
void initial_cpu_info(void)
{
	u32 midr = read_cpuid_id();
	unsigned cvariant = 0x1, core_per_skt = 48;
	int max_cpu = nr_cpu_ids;
	cvariant = MIDR_VARIANT(midr);
	if (cvariant == 0x1)
		skt_offset = 0x200000000000ULL;
	else
		skt_offset = 0x400000000000ULL;
	if (max_cpu == 24 || max_cpu == 32 || max_cpu == 8 || max_cpu == 12) {
        nr_skt = 1;
        totem_num = 1;
    } else {
        core_per_skt = (max_cpu % 48 == 0) ? 48 : 64;
        nr_skt = max_cpu / core_per_skt;
        totem_num = 2;
    }
}

/*To modify the L3 register. Traverse the socket and totem（skt_idx, die_idx）*/
/* 0--unlimit 1--limit*/ 
static const u32 iocapacityLimitOffset = 13;
int iocapacity_limit_get(void *dummy)
{
	unsigned long *addr = (unsigned long *)dummy;
    u32 reg_value = readl_reg(*addr);
    int value = (reg_value >> iocapacityLimitOffset) & 0x1;
    return value;
}

void iocapacity_limit_set(void *dummy)
{
    int *value = (int *)dummy;
	unsigned int die_idx = 0, skt_idx = 0;
	for (skt_idx = 0; skt_idx < nr_skt; skt_idx++) {
		for (die_idx = 0; die_idx < 2; die_idx++) { 
            unsigned long base = skt_idx * skt_offset, base2 = 0;
            unsigned val = 0;
			if ((totem_num == 1) && (die_idx == 1))
                continue;
            if (die_idx == 1)
                base += TOTEM_OFFSET;
			base2 = (unsigned long)ioremap(base + TB_L3T0_BASE, REG_RANGE);
            if (!base2)
				return;
            val = readl_reg(base2 + L3T_DYNAMIC_CTRL);
			if (*value == ENABLE)
            	val |= (1<<iocapacityLimitOffset);
			else if (*value == DISABLE)
				val &= (~(1<<iocapacityLimitOffset));
            writel_reg(val, base2 + L3T_DYNAMIC_CTRL);
            iounmap((volatile void*)base2);
        }
    }
    return;
}

unsigned long get_skt_offset(void) 
{
	return skt_offset;
} 

unsigned get_nr_skt(void)
{
	return nr_skt;
}

unsigned get_totem_num(void)
{
	return totem_num;
}

#else
#define PREFETCH_POLICY_MAX 0
static cfg_t prefetch_cfg[] = {};

void set_prefetch(void* dummy)
{
    return;
}

void get_prefetch(void* dummy)
{
    return;
}

void read_unique_set(void *dummy)
{
    return;
}

void read_unique_get(void *dummy)
{
    return;
}

void iocapacity_limit_set(void *dummy)
{
	return;
}

void initial_cpu_information(void)
{
	return;
}

int iocapacity_limit_get(void *dummy)
{
	return -1;
}

unsigned long get_skt_offset(void) 
{
	return 0;
} 

unsigned get_nr_skt(void)
{
	return 0;
}

unsigned get_totem_num(void)
{
	return 0;
}

#endif

int prefetch_policy_num(void)
{
    return ARRAY_SIZE(prefetch_cfg);
}

cfg_t *prefetch_policy(int policy)
{
    if (policy < 0 || policy > PREFETCH_POLICY_MAX)
        return NULL;
    return &prefetch_cfg[policy];
}

void reset_prefetch(void* dummy)
{
    cfg_t *pcfg = this_cpu_ptr((cfg_t __percpu *)dummy);
	set_prefetch(pcfg);
}

MODULE_LICENSE("GPL");