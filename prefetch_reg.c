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
 *         Liuke (liuke20@gitee)
 *         Wangwuzhe (wangwuzhe@gitee)
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

static DEFINE_MUTEX(l3t_dctrl_mtx);
static DEFINE_MUTEX(l3t_dauctrl1_mtx);
static DEFINE_MUTEX(l3t_dauctr0_mtx);
static DEFINE_MUTEX(l3t_prefetch_mtx);
static DEFINE_MUTEX(l3t_pnumconf1_mtx);
static DEFINE_MUTEX(l3t_sctrl_mtx);
static DEFINE_MUTEX(hha_ctrl_mtx);
static DEFINE_MUTEX(hha_ccctrl_mtx);
static DEFINE_MUTEX(hha_dirctrl_mtx);
static DEFINE_MUTEX(hha_funcdis_mtx);
static DEFINE_MUTEX(hha_ddrlevel_mtx);
static DEFINE_MUTEX(com_msd1ctrl_mtx);
static DEFINE_MUTEX(mn_dctrl_mtx);

unsigned int read_reg(void *addr, int bitstart, int bitend);
void write_reg(void *addr, unsigned setval, unsigned bitstart, unsigned bitend);

#define PREFETCH_POLICY_MAX 15
/* */
static cfg_t prefetch_cfg[] = {
    [0] = {
        .cpuprefctrl_el1 = 0x112f8127f,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [1] = {
        .cpuprefctrl_el1 = 0x112f81254,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [2] = {
        .cpuprefctrl_el1 = 0x112f81254,
        .adps_lld_ddr_el1 = 0x4d34a200,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [3] = {
        .cpuprefctrl_el1 = 0xb52f81254,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [4] = {
        .cpuprefctrl_el1 = 0x112f81254,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29080082a880,
        .adps_lld_l3_el1 = 0x65965700
    },
    [5] = {
        .cpuprefctrl_el1 = 0x3012f81254,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29080082a880,
        .adps_lld_l3_el1 = 0x65965700
    },
    [6] = {
        .cpuprefctrl_el1 = 0x3012f81254,
        .adps_lld_ddr_el1 = 0x4d142000,
        .adpp_l1v_mop_el1 = 0x29080082a880,
        .adps_lld_l3_el1 = 0x65965700
    },
    [7] = {
        .cpuprefctrl_el1 = 0x3012f81254,
        .adps_lld_ddr_el1 = 0x4d142000,
        .adpp_l1v_mop_el1 = 0x29080082a880,
        .adps_lld_l3_el1 = 0x4d145100
    },
    [8] = {
        .cpuprefctrl_el1 = 0x4112f81254,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [9] = {
        .cpuprefctrl_el1 = 0x112f81260,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [10] = {
        .cpuprefctrl_el1 = 0x112f81260,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x658e5700
    },
    [11] = {
        .cpuprefctrl_el1 = 0x3412f81254,
        .adps_lld_ddr_el1 = 0x4d142000,
        .adpp_l1v_mop_el1 = 0x29080082a880,
        .adps_lld_l3_el1 = 0x65965700
    },
    [12] = {
        .cpuprefctrl_el1 = 0x3412F81260,
        .adps_lld_ddr_el1 = 0x4d12000,
        .adpp_l1v_mop_el1 = 0x29080082a880,
        .adps_lld_l3_el1 = 0x65965700
    },
    [13] = {
        .cpuprefctrl_el1 = 0x112f81240,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [14] = {
        .cpuprefctrl_el1 = 0x112f81240,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x69154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
    [15] = {
        .cpuprefctrl_el1 = 0x80110f81380,
        .adps_lld_ddr_el1 = 0x6554a000,
        .adpp_l1v_mop_el1 = 0x29154332a840,
        .adps_lld_l3_el1 = 0x65965700
    },
};

/* locate register bits */
static FuncStruct Funcs[] = {
    [IOCAPACITY_LIMIT_EN_ORDER] = {
        .StartBit = IOCAPACITY_LIMIT_EN_START,
        .EndBit = IOCAPACITY_LIMIT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "iocapacity_limit_en"
    },
    [TAG_REP_ALG_ORDER] = {
        .StartBit = TAG_REP_ALG_START,
        .EndBit = TAG_REP_ALG_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 3,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "tag_rep_alg"
    },
    [SQMERGE_EN_ORDER] = {
        .StartBit = SQMERGE_EN_START,
        .EndBit = SQMERGE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "sqmerge_en"
    },
    [PREFETCH_DROP_HHA_EN_ORDER] = {
        .StartBit = PREFETCH_DROP_HHA_EN_START,
        .EndBit = PREFETCH_DROP_HHA_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "prefetch_drop_hha_en"
    },
    [RAMSWAP_FULL_SHUT_EN_ORDER] = {
        .StartBit = RAMSWAP_FULL_SHUT_EN_START,
        .EndBit = RAMSWAP_FULL_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "ramswap_full_shut_en"
    },
    [PRIME_DROP_MASK_EN_ORDER] = {
        .StartBit = PRIME_DROP_MASK_EN_START,
        .EndBit = PRIME_DROP_MASK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "prime_drop_mask_en"
    },
    [SEQUENCE_OPT_EN_ORDER] = {
        .StartBit = SEQUENCE_OPT_EN_START,
        .EndBit = SEQUENCE_OPT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "sequence_opt_en"
    },
    [PREFETCH_ULT_DDR_ORDER] = {
        .StartBit = PREFETCH_ULT_DDR_START,
        .EndBit = PREFETCH_ULT_DDR_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 3,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_utl_ddr"
    },
    [PREFETCH_ULT_DDR_EN_ORDER] = {
        .StartBit = PREFETCH_ULT_DDR_EN_START,
        .EndBit = PREFETCH_ULT_DDR_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_utl_ddr_en"
    },
    [PREFETCH_ULT_L3T_ORDER] = {
        .StartBit = PREFETCH_ULT_L3T_START,
        .EndBit = PREFETCH_ULT_L3T_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 3,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_utl_l3t"
    },
    [PREFETCH_UTL_L3T_EN_ORDER] = {
        .StartBit = PREFETCH_UTL_L3T_EN_START,
        .EndBit = PREFETCH_UTL_L3T_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_utl_l3t_en"
    },
    [PREFETCH_START_LEVEL_ORDER] = {
        .StartBit = PREFETCH_START_LEVEL_START,
        .EndBit = PREFETCH_START_LEVEL_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 31,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_start_level"
    },
    [RDMERGE_UPGRADE_EN_ORDER] = {
        .StartBit = RDMERGE_UPGRADE_EN_START,
        .EndBit = RDMERGE_UPGRADE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "rdmerge_upgrade_en"
    },
    [DDR_COMPRESS_OPT_EN_ORDER] = {
        .StartBit = DDR_COMPRESS_OPT_EN_START,
        .EndBit = DDR_COMPRESS_OPT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "ddr_compress_opt_en"
    },
    [SNPSLEEP_EN_ORDER] = {
        .StartBit = SNPSLEEP_EN_START,
        .EndBit = SNPSLEEP_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "snpsleep_en"
    },
    [PREFETCHTGT_EN_ORDER] = {
        .StartBit = PREFETCHTGT_EN_START,
        .EndBit = PREFETCHTGT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "prefetchtgt_en"
    },
    [CPU_PF_LQOS_EN_ORDER] = {
        .StartBit = CPU_PF_LQOS_EN_START,
        .EndBit = CPU_PF_LQOS_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "cpu_pf_lqos_en"
    },
    [REFILLSIZE_COM_ADA_EN_ORDER] = {
        .StartBit = REFILLSIZE_COM_ADA_EN_START,
        .EndBit = REFILLSIZE_COM_ADA_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "refillsize_com_ada_en"
    },
    [REFILLSIZE_PRE_ADA_EN_ORDER] = {
        .StartBit = REFILLSIZE_PRE_ADA_EN_START,
        .EndBit = REFILLSIZE_PRE_ADA_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "refillsize_pre_ada_en"
    },
    [PREFETCH_OVERIDE_LEVEL_ORDER] = {
        .StartBit = PREFETCH_OVERIDE_LEVEL_START,
        .EndBit = PREFETCH_OVERIDE_LEVEL_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 15,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_overide_level"
    },
    [PREFETCH_VAGUE_EN_ORDER] = {
        .StartBit = PREFETCH_VAGUE_EN_START,
        .EndBit = PREFETCH_VAGUE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_vague_en"
    },
    [PREFETCH_CORE_EN_ORDER] = {
        .StartBit = PREFETCH_CORE_EN_START,
        .EndBit = PREFETCH_CORE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 15,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_core_en"
    },
    [PREFETCH_MATCH_EN_ORDER] = {
        .StartBit = PREFETCH_MATCH_EN_START,
        .EndBit = PREFETCH_MATCH_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_match_en"
    },
    [REG_CTRL_PREFETCH_DROP_ORDER] = {
        .StartBit = REG_CTRL_PREFETCH_DROP_START,
        .EndBit = REG_CTRL_PREFETCH_DROP_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_prefetch_drop"
    },
    [REG_CTRL_DMCASSIGN_ORDER] = {
        .StartBit = REG_CTRL_DMCASSIGN_START,
        .EndBit = REG_CTRL_DMCASSIGN_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_dmcassign"
    },
    [REG_CTRL_RDATABYP_ORDER] = {
        .StartBit = REG_CTRL_RDATABYP_START,
        .EndBit = REG_CTRL_RDATABYP_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_rdatabyp"
    },
    [REG_DIR_REPLACE_ALG_ORDER] = {
        .StartBit = REG_DIR_REPLACE_ALG_START,
        .EndBit = REG_DIR_REPLACE_ALG_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 3,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_dir_replace_alg"
    },
    [PREFETCH_COMB_ORDER] = {
        .StartBit = PREFETCH_COMB_START,
        .EndBit = PREFETCH_COMB_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "prefetch_comb"
    },
    [REG_FUNCDIS_COMB_ORDER] = {
        .StartBit = PREFETCH_FUNCDIS_COMB_START,
        .EndBit = PREFETCH_FUNCDIS_COMB_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_comb"
    },
    [REG_NOSNP_ATOMIC_BYPASS_EN_ORDER] = {
        .StartBit = REG_NOSNP_ATOMIC_BYPASS_EN_START,
        .EndBit = REG_NOSNP_ATOMIC_BYPASS_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "reg_nosnp_atomic_bypass_en"
    },
    [REG_RO_ALLOC_SHUT_EN_ORDER] = {
        .StartBit = REG_RO_ALLOC_SHUT_EN_START,
        .EndBit = REG_RO_ALLOC_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "reg_ro_alloc_shut_en"
    },
    [REG_WRFULL_HIT_SHUT_EN_ORDER] = {
        .StartBit = REG_WRFULL_HIT_SHUT_EN_START,
        .EndBit = REG_WRFULL_HIT_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "reg_wrfull_hit_shut_en"
    },
    [REQ_CONFLICT_EN_ORDER] = {
        .StartBit = REQ_CONFLICT_EN_START,
        .EndBit = REQ_CONFLICT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "req_conflict_en"
    },
    [LOWER_POWER_EN_ORDER] = {
        .StartBit = LOWER_POWER_EN_START,
        .EndBit = LOWER_POWER_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "lower_power_en"
    },
    [DATACLEAN_SHUT_EN_ORDER] = {
        .StartBit = DATACLEAN_SHUT_EN_START,
        .EndBit = DATACLEAN_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "dataclean_shut_en"
    },
    [ARB_FLUSH_SHUT_EN_ORDER] = {
        .StartBit = ARB_FLUSH_SHUT_EN_START,
        .EndBit = ARB_FLUSH_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "arb_flush_shut_en"
    },
    [PGNT_ARB_EXAT_SHUT_EN_ORDER] = {
        .StartBit = PGNT_ARB_EXAT_SHUT_EN_START,
        .EndBit = PGNT_ARB_EXAT_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "pgnt_arb_exat_shut_en"
    },
    [FAST_EXTER_SHUT_EN_ORDER] = {
        .StartBit = FAST_EXTER_SHUT_EN_START,
        .EndBit = FAST_EXTER_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "fast_exter_shut_en"
    },
    [FAST_DATA_SHUT_EN_ORDER] = {
        .StartBit = FAST_DATA_SHUT_EN_START,
        .EndBit = FAST_DATA_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "fast_data_shut_en"
    },
    [PEND_DATA_SHUT_EN_ORDER] = {
        .StartBit = PEND_DATA_SHUT_EN_START,
        .EndBit = PEND_DATA_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "pend_data_shut_en"
    },
    [RAMFWD_SHUT_EN_ORDER] = {
        .StartBit = RAMFWD_SHUT_EN_START,
        .EndBit = RAMFWD_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "ramfwd_shut_en"
    },
    [READS_UPGRADE_EN_ORDER] = {
        .StartBit = READS_UPGRADE_EN_START,
        .EndBit = READS_UPGRADE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "reads_upgrade_en"
    },
    [RDMERGE_PIPE_EN_ORDER] = {
        .StartBit = RDMERGE_PIPE_EN_START,
        .EndBit = RDMERGE_PIPE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "rdmerge_pipe_en"
    },
    [SPILL_EN_ORDER] = {
        .StartBit = SPILL_EN_START,
        .EndBit = SPILL_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "spill_en"
    },
    [SPILL_SHARE_EN_ORDER] = {
        .StartBit = SPILL_SHARE_EN_START,
        .EndBit = SPILL_SHARE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "spill_shared_en"
    },
    [SPILL_INSTR_EN_ORDER] = {
        .StartBit = SPILL_INSTR_EN_START,
        .EndBit = SPILL_INSTR_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "spill_instr_en"
    },
    [SQRDMERGE_EN_ORDER] = {
        .StartBit = SQRDMERGE_EN_START,
        .EndBit = SQRDMERGE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "sqrdmerge_en"
    },
    [PREFETCH_DROP_EN_ORDER] = {
        .StartBit = PREFETCH_DROP_EN_START,
        .EndBit = PREFETCH_DROP_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "prefetch_drop_en"
    },
    [DATAPULL_EN_ORDER] = {
        .StartBit = DATAPULL_EN_START,
        .EndBit = DATAPULL_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "datapull_en"
    },
    [MKINVLD_EN_ORDER] = {
        .StartBit = MKINVLD_EN_START,
        .EndBit = MKINVLD_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "mkinvld_en"
    },
    [RAMTHR_EN_ORDER] = {
        .StartBit = RAMTHR_EN_START,
        .EndBit = RAMTHR_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "ramthr_en"
    },
    [RSPERR_EN_ORDER] = {
        .StartBit = RSPERR_EN_START,
        .EndBit = RSPERR_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "rsperr_en"
    },
    [FORCE_CQ_CLK_EN_ORDER] = {
        .StartBit = FORCE_CQ_CLK_EN_START,
        .EndBit = FORCE_CQ_CLK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "force_cq_clk_en"
    },
    [RDNOSNP_NCA_SHUT_EN_ORDER] = {
        .StartBit = RDNOSNP_NCA_SHUT_EN_START,
        .EndBit = RDNOSNP_NCA_SHUT_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "rdnosnp_nca_shut_en"
    },
    [WRFULL_CREATE_EN_ORDER] = {
        .StartBit = WRFULL_CREATE_EN_START,
        .EndBit = WRFULL_CREATE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "wrfull_create_en"
    },
    [CLEANUNIQUE_DATA_EN_ORDER] = {
        .StartBit = CLEANUNIQUE_DATA_EN_START,
        .EndBit = CLEANUNIQUE_DATA_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "cleanunique_data_en"
    },
    [LOCK_SHARE_REQ_EN_ORDER] = {
        .StartBit = LOCK_SHARE_REQ_EN_START,
        .EndBit = LOCK_SHARE_REQ_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "lock_share_req_en"
    },
    [ATOMIC_MONITOR_EN_ORDER] = {
        .StartBit = ATOMIC_MONITOR_EN_START,
        .EndBit = ATOMIC_MONITOR_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL0,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctr0_mtx,
        .Name = "atomic_monitor_en"
    },
    [PREFETCH_CLR_LEVEL_ORDER] = {
        .StartBit = PREFETCH_CLR_LEVEL_START,
        .EndBit = PREFETCH_CLR_LEVEL_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PREFETCH,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &l3t_prefetch_mtx,
        .Name = "prefetch_clr_level"
    },
    [REG_CTRL_SPILLPREFETCH_ORDER] = {
        .StartBit = REG_CTRL_SPILLPREFETCH_START,
        .EndBit = REG_CTRL_SPILLPREFETCH_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_spillprefetch"
    },
    [REG_CTRL_MPAMEN_ORDER] = {
        .StartBit = REG_CTRL_MPAMEN_START,
        .EndBit = REG_CTRL_MPAMEN_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_mpamen"
    },
    [REG_CTRL_MPAMQOS_ORDER] = {
        .StartBit = REG_CTRL_MPAMQOS_START,
        .EndBit = REG_CTRL_MPAMQOS_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_mpamqos"
    },
    [REG_CTRL_POISON_ORDER] = {
        .StartBit = REG_CTRL_POISON_START,
        .EndBit = REG_CTRL_POISON_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_poison"
    },
    [REG_CTRL_COMPRESS_SPEC_ORDER] = {
        .StartBit = REG_CTRL_COMPRESS_SPEC_START,
        .EndBit = REG_CTRL_COMPRESS_SPEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_compress_spec"
    },
    [REG_CTRL_WRITEEVICT_DROP_ORDER] = {
        .StartBit = REG_CTRL_WRITEEVICT_DROP_START,
        .EndBit = REG_CTRL_WRITEEVICT_DROP_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_writeevict_drop"
    },
    [REG_CTRL_EXCL_CLEAR_DIS_ORDER] = {
        .StartBit = REG_CTRL_EXCL_CLEAR_DIS_START,
        .EndBit = REG_CTRL_EXCL_CLEAR_DIS_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_excl_clear_dis"
    },
    [REG_CTRL_EXCL_EVENTEN_ORDER] = {
        .StartBit = REG_CTRL_EXCL_EVENTEN_START,
        .EndBit = REG_CTRL_EXCL_EVENTEN_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_excl_eventen"
    },
    [REG_CTRL_ECCEN_ORDER] = {
        .StartBit = REG_CTRL_ECCEN_START,
        .EndBit = REG_CTRL_ECCEN_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ctrl_mtx,
        .Name = "reg_ctrl_eccen"
    },
    [SEQUENCE_SHAPE_EN_ORDER] = {
        .StartBit = SEQUENCE_SHAPE_EN_START,
        .EndBit = SEQUENCE_SHAPE_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "sequence_shape_en"
    },
    [MPAM_PORTION_EN_ORDER] = {
        .StartBit = MPAM_PORTION_EN_START,
        .EndBit = MPAM_PORTION_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "mpam_portion_en"
    },
    [MPAM_CAPACITY_EN_ORDER] = {
        .StartBit = MPAM_CAPACITY_EN_START,
        .EndBit = MPAM_CAPACITY_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "mpam_capacity_en"
    },
    [ECCCHK_EN_ORDER] = {
        .StartBit = ECCCHK_EN_START,
        .EndBit = ECCCHK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "eccchk_en"
    },
    [REFILL_1024_RELAX_EN_ORDER] = {
        .StartBit = REFILL_1024_RELAX_EN_START,
        .EndBit = REFILL_1024_RELAX_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "refill_1024_relax_en"
    },
    [LOOKUP_THR_EN_ORDER] = {
        .StartBit = LOOKUP_THR_EN_START,
        .EndBit = LOOKUP_THR_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "lookup_thr_en"
    },
    [SNPUNIQUE_STASH_EN_ORDER] = {
        .StartBit = SNPUNIQUE_STASH_EN_START,
        .EndBit = SNPUNIQUE_STASH_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "snpunique_stash_en"
    },
    [PRIME_TIMEOUT_MASK_EN_ORDER] = {
        .StartBit = PRIME_TIMEOUT_MASK_EN_START,
        .EndBit = PRIME_TIMEOUT_MASK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "prime_timeout_mask_en"
    },
    [PRIME_SLEEP_MASK_EN_ORDER] = {
        .StartBit = PRIME_SLEEP_MASK_EN_START,
        .EndBit = PRIME_SLEEP_MASK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "prime_sleep_mask_en"
    },
    [PRIME_EXTEND_MASK_EN_ORDER] = {
        .StartBit = PRIME_EXTEND_MASK_EN_START,
        .EndBit = PRIME_EXTEND_MASK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "prime_extend_mask_en"
    },
    [FORCE_INTL_ALLOCATE_FAIL_ORDER] = {
        .StartBit = FORCE_INTL_ALLOCATE_FAIL_START,
        .EndBit = FORCE_INTL_ALLOCATE_FAIL_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "force_intl_allocate_fail"
    },
    [CPU_WRITE_UNIQUE_STREAM_EN_ORDER] = {
        .StartBit = CPU_WRITE_UNIQUE_STREAM_EN_START,
        .EndBit = CPU_WRITE_UNIQUE_STREAM_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "cpu_write_unique_stream_en"
    },
    [CPU_VIC_LQOS_EN_ORDER] = {
        .StartBit = CPU_VIC_LQOS_EN_START,
        .EndBit = CPU_VIC_LQOS_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "cpu_vic_lqos_en"
    },
    [PRIME_EXCL_MASK_EN_ORDER] = {
        .StartBit = PRIME_EXCL_MASK_EN_START,
        .EndBit = PRIME_EXCL_MASK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "prime_excl_mask_en"
    },
    [PRIME_HOME_MASK_EN_ORDER] = {
        .StartBit = PRIME_HOME_MASK_EN_START,
        .EndBit = PRIME_HOME_MASK_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl1_mtx,
        .Name = "prime_home_mask_en"
    },
    [PIME_TIMEOUT_NUM_ORDER] = {
        .StartBit = PIME_TIMEOUT_NUM_START,
        .EndBit = PIME_TIMEOUT_NUM_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_PRIME_NUM_CONFIG1,
        .Sup = 65535,
        .Glb = 0,
        .temp_mtx = &l3t_pnumconf1_mtx,
        .Name = "pime_timeout_num"
    },
    [DVMSNP_OUTSTANDING_ORDER] = {
        .StartBit = DVMSNP_OUTSTANDING_START,
        .EndBit = DVMSNP_OUTSTANDING_END,
        .Base = TB_MN_BASE,
        .Offset = MN_DYNAMIC_CTRL,
        .Sup = 15,
        .Glb = 0,
        .temp_mtx = &mn_dctrl_mtx,
        .Name = "dvmsnp_outstanding"
    },
    [DVMREQ_OUTSTANDING_ORDER] = {
        .StartBit = DVMREQ_OUTSTANDING_START,
        .EndBit = DVMREQ_OUTSTANDING_END,
        .Base = TB_MN_BASE,
        .Offset = MN_DYNAMIC_CTRL,
        .Sup = 31,
        .Glb = 0,
        .temp_mtx = &mn_dctrl_mtx,
        .Name = "dvmreq_outstanding"
    },
    [DVMSNP_PERF_EN_ORDER] = {
        .StartBit = DVMSNP_PERF_EN_START,
        .EndBit = DVMSNP_PERF_EN_END,
        .Base = TB_MN_BASE,
        .Offset = MN_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &mn_dctrl_mtx,
        .Name = "dvmsnp_perf_en"
    },
    [DVMREQ_PERF_EN_ORDER] = {
        .StartBit = DVMREQ_PERF_EN_START,
        .EndBit = DVMREQ_PERF_EN_END,
        .Base = TB_MN_BASE,
        .Offset = MN_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &mn_dctrl_mtx,
        .Name = "dvmreq_perf_en"
    },
    [REG_READONCESNP_DIS_ORDER] = {
        .StartBit = REG_READONCESNP_DIS_START,
        .EndBit = REG_READONCESNP_DIS_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_readoncesnp_dis"
    },
    [REG_CC_EXTER_STASH_ORDER] = {
        .StartBit = REG_CC_EXTER_STASH_START,
        .EndBit = REG_CC_EXTER_STASH_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_exter_stash"
    },
    [REG_CC_WRITEBACKI_SPILL_FULL_ORDER] = {
        .StartBit = REG_CC_WRITEBACKI_SPILL_FULL_START,
        .EndBit = REG_CC_WRITEBACKI_SPILL_FULL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_writebacki_spill_full"
    },
    [REG_CC_WRITEEVICTI_SPILL_FULL_ORDER] = {
        .StartBit = REG_CC_WRITEEVICTI_SPILL_FULL_START,
        .EndBit = REG_CC_WRITEEVICTI_SPILL_FULL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_writeevicti_spill_full"
    },
    [REG_CC_STASHONCE_FULL_ORDER] = {
        .StartBit = REG_CC_STASHONCE_FULL_START,
        .EndBit = REG_CC_STASHONCE_FULL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_stashonce_full"
    },
    [REG_CC_ATOMICSTASHL2_ORDER] = {
        .StartBit = REG_CC_ATOMICSTASHL2_START,
        .EndBit = REG_CC_ATOMICSTASHL2_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_atomicstashl2"
    },
    [REG_CC_ATOMICSTASHL3_ORDER] = {
        .StartBit = REG_CC_ATOMICSTASHL3_START,
        .EndBit = REG_CC_ATOMICSTASHL3_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_atomicstashl3"
    },
    [REG_CC_ATOMICSTASHCLR_ORDER] = {
        .StartBit = REG_CC_ATOMICSTASHCLR_START,
        .EndBit = REG_CC_ATOMICSTASHCLR_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_atomicstashclr"
    },
    [REG_CC_CMO_SNPME_ORDER] = {
        .StartBit = REG_CC_CMO_SNPME_START,
        .EndBit = REG_CC_CMO_SNPME_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_cmo_snpme"
    },
    [REG_CC_MAKEE_CHANGE_ORDER] = {
        .StartBit = REG_CC_MAKEE_CHANGE_START,
        .EndBit = REG_CC_MAKEE_CHANGE_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_makee_change"
    },
    [REG_CC_IOC_HITSCA_DIS_ORDER] = {
        .StartBit = REG_CC_IOC_HITSCA_DIS_START,
        .EndBit = REG_CC_IOC_HITSCA_DIS_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_ioc_hitsca_dis"
    },
    [REG_CC_PASSDIRTY_ORDER] = {
        .StartBit = REG_CC_PASSDIRTY_START,
        .EndBit = REG_CC_PASSDIRTY_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_passdirty"
    },
    [REG_CC_SNPDROP_ORDER] = {
        .StartBit = REG_CC_SNPDROP_START,
        .EndBit = REG_CC_SNPDROP_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_snpdrop"
    },
    [REG_CC_SPILL_ORDER] = {
        .StartBit = REG_CC_SPILL_START,
        .EndBit = REG_CC_SPILL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_ccctrl_mtx,
        .Name = "reg_cc_spill"
    },
    [REG_PRECISIONSNP_DIS_ORDER] = {
        .StartBit = REG_PRECISIONSNP_DIS_START,
        .EndBit = REG_PRECISIONSNP_DIS_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_precisionsnp_dis"
    },
    [REG_NOTONLY_EXCL_ORDER] = {
        .StartBit = REG_NOTONLY_EXCL_START,
        .EndBit = REG_NOTONLY_EXCL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_notonly_excl"
    },
    [REG_MISS_ALLINDEX_ORDER] = {
        .StartBit = REG_MISS_ALLINDEX_START,
        .EndBit = REG_MISS_ALLINDEX_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_miss_allindex"
    },
    [REG_MISS_CBACKTH_ORDER] = {
        .StartBit = REG_MISS_CBACKTH_START,
        .EndBit = REG_MISS_CBACKTH_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_miss_cbackth"
    },
    [REG_MISS_NORMALTH_ORDER] = {
        .StartBit = REG_MISS_NORMALTH_START,
        .EndBit = REG_MISS_NORMALTH_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_miss_normalth"
    },
    [REG_MISS_TOSDIR_ORDER] = {
        .StartBit = REG_MISS_TOSDIR_START,
        .EndBit = REG_MISS_TOSDIR_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_miss_tosdir"
    },
    [REG_ENTRY_EXCEPT_ORDER] = {
        .StartBit = REG_ENTRY_EXCEPT_START,
        .EndBit = REG_ENTRY_EXCEPT_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DIR_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_dirctrl_mtx,
        .Name = "reg_entry_except"
    },
    [STRICT_ORDER_ORDER] = {
        .StartBit = STRICT_ORDER_START,
        .EndBit = STRICT_ORDER_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "strict_order"
    },
    [EVICT_GREEN_ORDER] = {
        .StartBit = EVICT_GREEN_START,
        .EndBit = EVICT_GREEN_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "evict_green"
    },
    [BLOCK_RETRY_ORDER] = {
        .StartBit = BLOCK_RETRY_START,
        .EndBit = BLOCK_RETRY_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "block_retry"
    },
    [BUFFER_PRIO_ORDER] = {
        .StartBit = BUFFER_PRIO_START,
        .EndBit = BUFFER_PRIO_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "buffer_prio"
    },
    [HALF_WR_RDDDR_DELAY_ORDER] = {
        .StartBit = HALF_WR_RDDDR_DELAY_START,
        .EndBit = HALF_WR_RDDDR_DELAY_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "half_wr_rdddr_delay"
    },
    [WBACK_CNFL_RDHALF_ORDER] = {
        .StartBit = WBACK_CNFL_RDHALF_START,
        .EndBit = WBACK_CNFL_RDHALF_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "wback_cnfl_rdhalf"
    },
    [REG_FUNCDIS_PENDPRECISION_ORDER] = {
        .StartBit = REG_FUNCDIS_PENDPRECISION_START,
        .EndBit = REG_FUNCDIS_PENDPRECISION_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_pendprecision"
    },
    [REG_FUNCDIS_COMBRDDDR_ORDER] = {
        .StartBit = REG_FUNCDIS_COMBRDDDR_START,
        .EndBit = REG_FUNCDIS_COMBRDDDR_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_combrdddr"
    },
    [REG_FUNDIS_SCRAMBLE_ORDER] = {
        .StartBit = REG_FUNDIS_SCRAMBLE_START,
        .EndBit = REG_FUNDIS_SCRAMBLE_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_scramble"
    },
    [REG_FUNCDIS_STASHIDPG_ORDER] = {
        .StartBit = REG_FUNCDIS_STASHIDPG_START,
        .EndBit = REG_FUNCDIS_STASHIDPG_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_stashidpg"
    },
    [REG_FUNCDIS_RDATATIME_ORDER] = {
        .StartBit = REG_FUNCDIS_RDATATIME_START,
        .EndBit = REG_FUNCDIS_RDATATIME_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_rdatatime"
    },
    [REG_FUNCDIS_DMCUTL_ORDER] = {
        .StartBit = REG_FUNCDIS_DMCUTL_START,
        .EndBit = REG_FUNCDIS_DMCUTL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_dmcutl"
    },
    [REG_FUNCDIS_CANCELEXCEPT_ORDER] = {
        .StartBit = REG_FUNCDIS_CANCELEXCEPT_START,
        .EndBit = REG_FUNCDIS_CANCELEXCEPT_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_cancelexcept"
    },
    [REG_FUNCDIS_CCIXCBUPDATE_ORDER] = {
        .StartBit = REG_FUNCDIS_CCIXCBUPDATE_START,
        .EndBit = REG_FUNCDIS_CCIXCBUPDATE_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_ccixcbupdate"
    },
    [REG_FUNCDIS_UPDATEOPEN_ORDER] = {
        .StartBit = REG_FUNCDIS_UPDATEOPEN_START,
        .EndBit = REG_FUNCDIS_UPDATEOPEN_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_FUNC_DIS,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_funcdis_mtx,
        .Name = "reg_funcdis_updateopen"
    },
    [REG_PREFETCHTGT_OUTSTANDING_ORDER] = {
        .StartBit = REG_PREFETCHTGT_OUTSTANDING_START,
        .EndBit = REG_PREFETCHTGT_OUTSTANDING_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DDR_LEVEL,
        .Sup = 127,
        .Glb = 0,
        .temp_mtx = &hha_ddrlevel_mtx,
        .Name = "reg_prefetchtgt_outstanding"
    },
    [REG_PREFETCHTGT_LEVEL_ORDER] = {
        .StartBit = REG_PREFETCHTGT_LEVEL_START,
        .EndBit = REG_PREFETCHTGT_LEVEL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DDR_LEVEL,
        .Sup = 127,
        .Glb = 0,
        .temp_mtx = &hha_ddrlevel_mtx,
        .Name = "reg_prefetchtgt_level"
    },
    [REG_SPEC_RD_LEVEL_ORDER] = {
        .StartBit = REG_SPEC_RD_LEVEL_START,
        .EndBit = REG_SPEC_RD_LEVEL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DDR_LEVEL,
        .Sup = 127,
        .Glb = 0,
        .temp_mtx = &hha_ddrlevel_mtx,
        .Name = "reg_spec_rd_level"
    },
    [REG_DROP_LEVEL_ORDER] = {
        .StartBit = REG_DROP_LEVEL_START,
        .EndBit = REG_DROP_LEVEL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_DDR_LEVEL,
        .Sup = 127,
        .Glb = 0,
        .temp_mtx = &hha_ddrlevel_mtx,
        .Name = "reg_drop_level"
    }
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
static unsigned nr_skt = 1, totem_num = 1;
int initial_cpu_info(void)
{
    u32 midr = read_cpuid_id();
    unsigned cvariant = 0x1, core_per_skt = 48;
    int max_cpu = nr_cpu_ids;
    u32 implementor = read_cpuid_implementor();
    if (implementor != 0x48) {
        printk(KERN_ALERT "This is not HiSilicon CPU core;\n");
        return -ENOMEM;
    }

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
    return 1;
}

int get_val(FuncStruct Str)
{
    return read_reg((void *)(Str.Address), Str.StartBit, Str.EndBit);
}

void set_val(FuncStruct Str)
{
    unsigned int die_idx = 0, skt_idx = 0, die_nr = 2;
    for (skt_idx = 0; skt_idx < nr_skt; skt_idx++) {
        for (die_idx = 0; die_idx < die_nr + 1; die_idx++) {
            unsigned long base = skt_idx * skt_offset, base_remap = 0;
            if ((totem_num == 1) && (die_idx == 1))
                continue;

            if (die_idx == 1)
                base += TOTEM_OFFSET;

            if (die_idx < die_nr)
                base_remap = (unsigned long)ioremap(base + Str.Base, REG_RANGE);
            else if (Str.Base == TB_AA_BASE) // To get the NIBUS configuration
                base_remap = (unsigned long)ioremap(base + NI_AA_BASE, REG_RANGE);
            else
                continue;

            if (!base_remap)
                return;

            write_reg((void *)(base_remap + Str.Offset), Str.Val, Str.StartBit, Str.EndBit);
            iounmap((volatile void*)base_remap);
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

void reset_default_cfg(int *old_cfg_int)
{
    int funcnum = 0;
    for (funcnum = 0; funcnum < FUNC_NUM; funcnum++) {
        Funcs[funcnum].Val = old_cfg_int[funcnum];
        set_val(Funcs[funcnum]);
    }
    return;
}

int get_default_cfg(int *arr)
{
    int ret = -1, funcnum = 0;
    unsigned long base = 0;

    for (funcnum = 0; funcnum < FUNC_NUM; funcnum++) {
        base = (unsigned long)ioremap(Funcs[funcnum].Base, REG_RANGE);
        if (!base)
            return -ENOMEM;

        Funcs[funcnum].Address = base + Funcs[funcnum].Offset;
        ret = get_val(Funcs[funcnum]);
        iounmap((volatile void*)base);
        arr[funcnum] = ret;
        if (ret < 0)
            return -ENOMEM;
    }
    return 1;
}

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

FuncStruct *get_func(struct device_attribute* attr)
{
    int funcnum = 0;
    if (attr == NULL)
        return NULL;

    for (funcnum = 0; funcnum < FUNC_NUM; funcnum++) {
        if (strcmp(Funcs[funcnum].Name, (*attr).attr.name) == 0)
            return &Funcs[funcnum];
    }
    return NULL;
}

/*
* Read register value: bitstart == -1
* Read register bitn: bitstart = bitend = n
* Read register consecutive bits: set bitstart and bitend
*/
inline unsigned int read_reg(void* addr, int bitstart, int bitend)
{
    unsigned int val = 0;
    int bitwide = 0;
    unsigned int bitmask = 0;
    if (addr == NULL ||
        bitend < bitstart ||
        bitstart > 31 ||
        bitend > 31)
        return 0;
    val = *(volatile unsigned int *)(addr);
    if (bitstart < 0 || bitend < 0)
        return val;

    bitwide = bitend - bitstart + 1;
    bitmask = (bitwide == 32) ? 0xffffffff : (1 << bitwide) - 1;
    return (val >> bitstart) & bitmask;
}

/*
* Write register bitn: bitstart = bitend = n
* Write register consecutive bits: set bitstart and bitend
* Attention: "setval" no need to shift, just pass native value,
like set 0110B to 23:20 is "write_reg(addr, 0x6, 20, 23)"
*/
inline void write_reg(void* addr, unsigned setval, unsigned bitstart, unsigned bitend)
{
    unsigned int val = 0;
    int bitwide = 0;
    unsigned int bitmask = 0;
    if (addr == NULL ||
        bitend < bitstart ||
        bitstart > 31 ||
        bitend > 31 ||
        bitstart < 0 ||
        bitend < 0)
        return;
    val = *(volatile unsigned int *)(addr);
    bitwide = bitend - bitstart + 1;
    bitmask = (bitwide == 32) ? 0xffffffff : (1 << bitwide) - 1;
    setval &= bitmask;
    val &= ~(bitmask << bitstart);
    setval <<= bitstart;
    *(volatile unsigned int *)(addr) = val | setval;
    return;
}

MODULE_LICENSE("GPL");
