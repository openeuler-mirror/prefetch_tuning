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
 *         Liuke20 (liuke20@gitee)
 */
#ifndef __PREFETCH_TUNING__
#define __PREFETCH_TUNING__

enum {
    DISABLE = 0,
    ENABLE
};

enum FunctionOrderList {
    IOCAPACITY_LIMIT_ORDER = 0,
    TAG_REP_ALG_ORDER,
    SQMERGE_ORDER,
    RDMERGE_ORDER,
    PREFETCH_DROP_HHA_ORDER,
    RAMSWAP_ORDER,
    PRIME_DROP_MASK_ORDER,
    SEQUENCE_OPT_ORDER,
    BANKINTLV_ORDER,
    PREFETCH_ULT_DDR_ORDER,
    PREFETCH_ULT_DDR_EN_ORDER,
    PREFETCH_ULT_L3T_ORDER,
    PREFETCH_UTL_L3T_EN_ORDER,
    PREFETCH_START_LEVEL_ORDER,
    REG_TOTEM_DUAL_ORDER,
    REG_CANUM_SKTVEC_ORDER,
    REG_SKT1_TB_CAVEC_ORDER,
    REG_SKT1_TA_CAVEC_ORDER,
    REG_SKT0_TB_CAVEC_ORDER,
    REG_SKT0_TA_CAVEC_ORDER,
    REG_SKT3_TB_CAVEC_ORDER,
    REG_SKT3_TA_CAVEC_ORDER,
    REG_SKT2_TB_CAVEC_ORDER,
    REG_SKT2_TA_CAVEC_ORDER,
    RDMERGE_UPGRADE_EN_ORDER,
    DDR_COMPRESS_OPT_EN_ORDER,
    SNPSLEEP_EN_ORDER,
    PREFETCHTGT_EN_ORDER,
    BANKINTL_STAGGER_ORDER,
    CPU_PF_LQOS_EN_ORDER,
    REFILLSIZE_COM_ADA_EN_ORDER,
    REFILLSIZE_PRE_ADA_EN_ORDER,
    PREFETCH_OVERIDE_LEVEL_ORDER,
    PREFETCH_VAGUE_EN_ORDER,
    PREFETCH_CORE_EN_ORDER,
    PREFETCH_MATCH_EN_ORDER,
    REG_CTRL_PREFETCH_DROP_ORDER,
    REG_CTRL_DMCASSIGN_ORDER,
    REG_CTRL_RDATABYP_ORDER,
    REG_DIR_REPLACE_ALG_ORDER,
    PREFETCH_COMB_ORDER,
    REG_FUNCDIS_COMB_ORDER,
    DDR_INTLV_SKT_ORDER,
    DDR_INTLV_DIE_ORDER,
    FUNC_NUM
};

enum RegOffsetList {
    L3T_DYNAMIC_CTRL = 0x400,
    L3T_STATIC_CTRL	= 0x000,
    L3T_DYNAMIC_AUCTRL0 = 0x404,
    L3T_DYNAMIC_AUCTRL1 = 0x40c,
    L3T_PREFETCH = 0x410,
    HHA_TOTEMNUM = 0x020,
    HHA_CANUM_L = 0x024,
    HHA_CANUM_H = 0x028,
    HHA_CTRL = 0x000,
    HHA_DIR_CTRL = 0x00c,
    HHA_FUNC_DIS = 0x010,
    AA_MSD1_CTRL = 0x384
};

enum ComMsd1Ctrl {
    DDR_INTLV_SKT_START = 29,
    DDR_INTLV_SKT_END = 30,
    DDR_INTLV_DIE_START = 27,
    DDR_INTLV_DIE_END = 27,
};

enum HhaFuncDisReg {
    PREFETCH_COMB_START = 21,
    PREFETCH_COMB_END = 21,
    PREFETCH_FUNCDIS_COMB_START = 2,
    PREFETCH_FUNCDIS_COMB_END = 2
};

enum HhaDirCtrlReg {
    REG_DIR_REPLACE_ALG_START = 0,
    REG_DIR_REPLACE_ALG_END = 1
};

enum HhaCtrlReg {
    REG_CTRL_PREFETCH_DROP_START = 6,
    REG_CTRL_PREFETCH_DROP_END = 6,
    REG_CTRL_DMCASSIGN_START = 5,
    REG_CTRL_DMCASSIGN_END = 5,
    REG_CTRL_RDATABYP_START = 4,
    REG_CTRL_RDATABYP_END = 4
};

enum L3tStaticCtrlReg {
    RAMSWAP_START = 16,
    RAMSWAP_END = 16
};

enum L3tDynamicAuctrl0Reg {
    DDR_COMPRESS_OPT_EN_START = 7,
    DDR_COMPRESS_OPT_EN_END = 7,
    SNPSLEEP_EN_START = 1,
    SNPSLEEP_EN_END = 1,
    PREFETCHTGT_EN_START = 0,
    PREFETCHTGT_EN_END = 0
};

enum L3tDynamicCtrlReg {
    TAG_REP_ALG_START = 0,
    TAG_REP_ALG_END = 1,
    PREFETCH_DROP_HHA_START = 4,
    PREFETCH_DROP_HHA_END = 4,
    RDMERGE_UPGRADE_EN_START = 8,
    RDMERGE_UPGRADE_EN_END = 8,
    RDMERGE_START = 9,
    RDMERGE_END = 9,
    SQMERGE_START = 10,
    SQMERGE_END = 10,
    IOCAPACITY_LIMIT_START = 13,
    IOCAPACITY_LIMIT_END = 13
};

enum L3tDynamicAuctrl1Reg {
    BANKINTLV_START = 0,
    BANKINTLV_END = 0,
    SEQUENCE_OPT_START = 1,
    SEQUENCE_OPT_END = 1,
    REFILLSIZE_PRE_ADA_EN_START = 2,
    REFILLSIZE_PRE_ADA_EN_END = 2,
    REFILLSIZE_COM_ADA_EN_START = 3,
    REFILLSIZE_COM_ADA_EN_END = 3,
    PRIME_DROP_MASK_START = 5,
    PRIME_DROP_MASK_END = 5,
    CPU_PF_LQOS_EN_START = 11,
    CPU_PF_LQOS_EN_END = 11,
    BANKINTL_STAGGER_START = 19,
    BANKINTL_STAGGER_END = 19
};

enum L3tPrefetchReg {
    PREFETCH_START_LEVEL_START = 0,
    PREFETCH_START_LEVEL_END = 4,
    PREFETCH_MATCH_EN_START = 7,
    PREFETCH_MATCH_EN_END = 7,
    PREFETCH_CORE_EN_START = 8,
    PREFETCH_CORE_EN_END = 11,
    PREFETCH_VAGUE_EN_START = 12,
    PREFETCH_VAGUE_EN_END = 12,
    PREFETCH_UTL_L3T_EN_START = 13,
    PREFETCH_UTL_L3T_EN_END = 13,
    PREFETCH_ULT_L3T_START = 14,
    PREFETCH_ULT_L3T_END = 15,
    PREFETCH_ULT_DDR_EN_START = 16,
    PREFETCH_ULT_DDR_EN_END = 16,
    PREFETCH_ULT_DDR_START = 17,
    PREFETCH_ULT_DDR_END = 18,
    PREFETCH_OVERIDE_LEVEL_START = 20,
    PREFETCH_OVERIDE_LEVEL_END = 23
};

enum HhaTotemnumReg {
    REG_CANUM_SKTVEC_START = 0,
    REG_CANUM_SKTVEC_END = 3,
    REG_TOTEM_DUAL_START = 4,
    REG_TOTEM_DUAL_END = 4
};

enum HhaCanumLReg {
    REG_SKT0_TA_CAVEC_START = 0,
    REG_SKT0_TA_CAVEC_END = 7,
    REG_SKT0_TB_CAVEC_START = 8,
    REG_SKT0_TB_CAVEC_END = 15,
    REG_SKT1_TA_CAVEC_START = 16,
    REG_SKT1_TA_CAVEC_END = 23,
    REG_SKT1_TB_CAVEC_START = 24,
    REG_SKT1_TB_CAVEC_END = 31
};

enum HhaCanumHReg {
    REG_SKT2_TA_CAVEC_START = 0,
    REG_SKT2_TA_CAVEC_END = 7,
    REG_SKT2_TB_CAVEC_START = 8,
    REG_SKT2_TB_CAVEC_END = 15,
    REG_SKT3_TA_CAVEC_START = 16,
    REG_SKT3_TA_CAVEC_END = 23,
    REG_SKT3_TB_CAVEC_START = 24,
    REG_SKT3_TB_CAVEC_END = 31
};

#define CACHE_READUNIQ_CTRL (1L << 40)
#define TB_L3T0_BASE		0x90180000
#define TOTEM_OFFSET		0x8000000
#define TB_AA_BASE		    0x90300000
#define TB_HHA0_BASE		0x90120000
#define NI_AA_BASE		    0x200140000ULL
#define REG_RANGE	        0x5000
#define MAX_STR             32


typedef struct {
    unsigned long Base;
    unsigned long Offset;
    unsigned long Address;
    int Val;
    int StartBit;
    int EndBit;
    int Glb;
    int Sup;
    char Name[MAX_STR];
    struct mutex* temp_mtx;
} FuncStruct;

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
extern int initial_cpu_info(void);
extern void set_val(FuncStruct Str);
extern int get_val(FuncStruct Str);
extern int get_default_cfg(int* arr);
extern FuncStruct *get_func(struct device_attribute* attr);

extern unsigned get_totem_num(void);
extern unsigned get_nr_skt(void);
extern unsigned long get_skt_offset(void);
extern int prefetch_policy_num(void);
extern cfg_t *prefetch_policy(int policy);
extern void reset_default_cfg(int *old_cfg_int);

#endif
