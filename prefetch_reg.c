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
static DEFINE_MUTEX(l3t_dauctrl_mtx);
static DEFINE_MUTEX(l3t_dauctr0_mtx);
static DEFINE_MUTEX(l3t_prefetch_mtx);
static DEFINE_MUTEX(hha_totemnum_mtx);
static DEFINE_MUTEX(hha_canuml_mtx);
static DEFINE_MUTEX(hha_canumh_mtx);
static DEFINE_MUTEX(l3t_sctrl_mtx);
static DEFINE_MUTEX(hha_ctrl_mtx);
static DEFINE_MUTEX(hha_dirctrl_mtx);
static DEFINE_MUTEX(hha_funcdis_mtx);
static DEFINE_MUTEX(com_msd1ctrl_mtx);

unsigned int read_reg(void *addr, int bitstart, int bitend);
void write_reg(void *addr, unsigned setval, unsigned bitstart, unsigned bitend);
#define PREFETCH_POLICY_MAX 15

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

static FuncStruct Funcs[] = {
    [IOCAPACITY_LIMIT_ORDER] = {
        .StartBit = IOCAPACITY_LIMIT_START,
        .EndBit = IOCAPACITY_LIMIT_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "iocapacity_limit"
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
    [SQMERGE_ORDER] = {
        .StartBit = SQMERGE_START,
        .EndBit = SQMERGE_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "sqmerge"
    },
    [RDMERGE_ORDER] = {
        .StartBit = RDMERGE_START,
        .EndBit = RDMERGE_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "rdmerge"
    },
    [PREFETCH_DROP_HHA_ORDER] = {
        .StartBit = PREFETCH_DROP_HHA_START,
        .EndBit = PREFETCH_DROP_HHA_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dctrl_mtx,
        .Name = "prefetch_drop_hha"
    },
    [RAMSWAP_ORDER] = {
        .StartBit = RAMSWAP_START,
        .EndBit = RAMSWAP_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_STATIC_CTRL,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_sctrl_mtx,
        .Name = "ramswap"
    },
    [PRIME_DROP_MASK_ORDER] = {
        .StartBit = PRIME_DROP_MASK_START,
        .EndBit = PRIME_DROP_MASK_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl_mtx,
        .Name = "prime_drop_mask"
    },
    [SEQUENCE_OPT_ORDER] = {
        .StartBit = SEQUENCE_OPT_START,
        .EndBit = SEQUENCE_OPT_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl_mtx,
        .Name = "sequence_opt"
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
    [REG_TOTEM_DUAL_ORDER] = {
        .StartBit = REG_TOTEM_DUAL_START,
        .EndBit = REG_TOTEM_DUAL_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_TOTEMNUM,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &hha_totemnum_mtx,
        .Name = "totem_dual"
    },
    [REG_CANUM_SKTVEC_ORDER] = {
        .StartBit = REG_CANUM_SKTVEC_START,
        .EndBit = REG_CANUM_SKTVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_TOTEMNUM,
        .Sup = 15,
        .Glb = 0,
        .temp_mtx = &hha_totemnum_mtx,
        .Name = "canum_sktvec"
    },
    [REG_SKT1_TB_CAVEC_ORDER] = {
        .StartBit = REG_SKT1_TB_CAVEC_START,
        .EndBit = REG_SKT1_TB_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_L,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canuml_mtx,
        .Name = "skt1_tb_cavec"
    },
    [REG_SKT1_TA_CAVEC_ORDER] = {
        .StartBit = REG_SKT1_TA_CAVEC_START,
        .EndBit = REG_SKT1_TA_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_L,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canuml_mtx,
        .Name = "skt1_ta_cavec"
    },
    [REG_SKT0_TB_CAVEC_ORDER] = {
        .StartBit = REG_SKT0_TB_CAVEC_START,
        .EndBit = REG_SKT0_TB_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_L,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canuml_mtx,
        .Name = "skt0_tb_cavec"
    },
    [REG_SKT0_TA_CAVEC_ORDER] = {
        .StartBit = REG_SKT0_TA_CAVEC_START,
        .EndBit = REG_SKT0_TA_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_L,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canuml_mtx,
        .Name = "skt0_ta_cavec"
    },
    [REG_SKT3_TB_CAVEC_ORDER] = {
        .StartBit = REG_SKT3_TB_CAVEC_START,
        .EndBit = REG_SKT3_TB_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_H,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canumh_mtx,
        .Name = "skt3_tb_cavec"
    },
    [REG_SKT3_TA_CAVEC_ORDER] = {
        .StartBit = REG_SKT3_TA_CAVEC_START,
        .EndBit = REG_SKT3_TA_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_H,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canumh_mtx,
        .Name = "skt3_ta_cavec"
    },
    [REG_SKT2_TB_CAVEC_ORDER] = {
        .StartBit = REG_SKT2_TB_CAVEC_START,
        .EndBit = REG_SKT2_TB_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_H,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canumh_mtx,
        .Name = "skt2_tb_cavec"
    },
    [REG_SKT3_TA_CAVEC_END] = {
        .StartBit = REG_SKT2_TA_CAVEC_START,
        .EndBit = REG_SKT2_TA_CAVEC_END,
        .Base = TB_HHA0_BASE,
        .Offset = HHA_CANUM_H,
        .Sup = 255,
        .Glb = 0,
        .temp_mtx = &hha_canumh_mtx,
        .Name = "skt2_ta_cavec"
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
        .StartBit = PRIME_DROP_MASK_START,
        .EndBit = PRIME_DROP_MASK_END,
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
        .temp_mtx = &l3t_dauctrl_mtx,
        .Name = "cpu_pf_lqos_en"
    },
    [REFILLSIZE_COM_ADA_EN_ORDER] = {
        .StartBit = REFILLSIZE_COM_ADA_EN_START,
        .EndBit = REFILLSIZE_COM_ADA_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl_mtx,
        .Name = "refillsize_com_ada_en"
    },
    [REFILLSIZE_PRE_ADA_EN_ORDER] = {
        .StartBit = REFILLSIZE_PRE_ADA_EN_START,
        .EndBit = REFILLSIZE_PRE_ADA_EN_END,
        .Base = TB_L3T0_BASE,
        .Offset = L3T_DYNAMIC_AUCTRL1,
        .Sup = 1,
        .Glb = 0,
        .temp_mtx = &l3t_dauctrl_mtx,
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
