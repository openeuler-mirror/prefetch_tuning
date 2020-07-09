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
#ifndef __PREFETCH_TUNING__
#define __PREFETCH_TUNING__

enum {
    DISABLE = 0,
    ENABLE
};

#define CACHE_READUNIQ_CTRL (1L << 40)
#define TB_L3T0_BASE		0x90180000
#define L3T_DYNAMIC_CTRL	0x400
#define TOTEM_OFFSET		0x8000000
#define REG_RANGE		0x5000
#define writel_reg(val, addr) (*(volatile unsigned int *)(addr) = (val))
#define readl_reg(addr) (*(volatile unsigned int*)(addr))

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

extern void set_prefetch(void* dummy);
extern void get_prefetch(void* dummy);
extern void read_unique_set(void *dummy);
extern void read_unique_get(void *dummy);
extern void reset_prefetch(void* dummy);
extern void initial_cpu_info(void);
extern int iocapacity_limit_get(void *dummy);
extern void iocapacity_limit_set(void *dummy);
unsigned get_totem_num(void);
unsigned get_nr_skt(void);

unsigned long get_skt_offset(void); 
extern int prefetch_policy_num(void);
extern cfg_t *prefetch_policy(int policy);

#endif
