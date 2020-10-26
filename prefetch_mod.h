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
 *         Wangwuzhe (wangwuzhe@gitee)
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
    PREFETCH_ULT_DDR_ORDER,
    PREFETCH_ULT_DDR_EN_ORDER,
    PREFETCH_ULT_L3T_ORDER,
    PREFETCH_UTL_L3T_EN_ORDER,
    PREFETCH_START_LEVEL_ORDER,
    RDMERGE_UPGRADE_EN_ORDER,
    DDR_COMPRESS_OPT_EN_ORDER,
    SNPSLEEP_EN_ORDER,
    PREFETCHTGT_EN_ORDER,
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
    REG_NOSNP_ATOMIC_BYPASS_EN_ORDER,
    REG_RO_ALLOC_SHUT_EN_ORDER,
    REG_WRFULL_HIT_SHUT_EN_ORDER,
    REQ_CONFLICT_EN_ORDER,
    LOWER_POWER_EN_ORDER,
    DATACLEAN_SHUT_EN_ORDER,
    ARB_FLUSH_SHUT_EN_ORDER,
    PGNT_ARB_EXAT_SHUT_EN_ORDER,
    FAST_EXTER_SHUT_EN_ORDER,
    FAST_DATA_SHUT_EN_ORDER,
    PEND_DATA_SHUT_EN_ORDER,
    RAMFWD_SHUT_EN_ORDER,
    RAMTHR_MERGE_EN_ORDER,
    EXT_EN_ORDER,
    READS_UPGRADE_EN_ORDER,
    RDMERGE_PIPE_EN_ORDER,
    SPILL_EN_ORDER,
    SPILL_SHARE_EN_ORDER,
    SPILL_INSTR_EN_ORDER,
    SQRDMERGE_EN_ORDER,
    PREFETCH_DROP_EN_ORDER,
    DATAPULL_EN_ORDER,
    MKINVLD_EN_ORDER,
    RAMTHR_EN_ORDER,
    RSPERR_EN_ORDER,
    FORCE_CQ_CLK_EN_ORDER,
    RDNOSNP_NCA_SHUT_EN_ORDER,
    WRFULL_CREATE_EN_ORDER,
    CLEANUNIQUE_DATA_EN_ORDER,
    LOCK_SHARE_REQ_EN_ORDER,
    ATOMIC_MONITOR_EN_ORDER,
    PREFETCH_CLR_LEVEL_ORDER,
    REG_CTRL_SPILLPREFETCH_ORDER,
    REG_CTRL_MPAMEN_ORDER,
    REG_CTRL_MPAMQOS_ORDER,
    REG_CTRL_POISON_ORDER,
    REG_CTRL_COMPRESS_SPEC_ORDER,
    REG_CTRL_COMPRESS_ORDER,
    REG_CTRL_DATA_RESIDE_ORDER,
    REG_CTRL_WRITEEVICT_DROP_ORDER,
    REG_CTRL_EXCL_CLEAR_DIS_ORDER,
    REG_CTRL_EXCL_EVENTEN_ORDER,
    REG_CTRL_ECCEN_ORDER,
    REG_CTRL_LINKDOWN_ORDER,
    FUNC_NUM
};

enum RegOffsetList {
    L3T_DYNAMIC_CTRL = 0x400,
    L3T_STATIC_CTRL	= 0x000,
    L3T_DYNAMIC_AUCTRL0 = 0x404,
    L3T_DYNAMIC_AUCTRL1 = 0x40c,
    L3T_PREFETCH = 0x410,
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
    REG_CTRL_SPILLPREFETCH_START = 22,
    REG_CTRL_SPILLPREFETCH_END = 22,
    REG_CTRL_MPAMEN_START = 21,
    REG_CTRL_MPAMEN_END = 21,
    REG_CTRL_MPAMQOS_START = 20,
    REG_CTRL_MPAMQOS_END = 20,
    REG_CTRL_POISON_START = 19,
    REG_CTRL_POISON_END = 19,
    REG_CTRL_COMPRESS_SPEC_START = 10,
    REG_CTRL_COMPRESS_SPEC_END = 10,
    REG_CTRL_COMPRESS_START = 9,
    REG_CTRL_COMPRESS_END = 9,
    REG_CTRL_DATA_RESIDE_START = 8,
    REG_CTRL_DATA_RESIDE_END = 8,
    REG_CTRL_WRITEEVICT_DROP_START = 7,
    REG_CTRL_WRITEEVICT_DROP_END = 7,
    REG_CTRL_PREFETCH_DROP_START = 6,
    REG_CTRL_PREFETCH_DROP_END = 6,
    REG_CTRL_DMCASSIGN_START = 5,
    REG_CTRL_DMCASSIGN_END = 5,
    REG_CTRL_RDATABYP_START = 4,
    REG_CTRL_RDATABYP_END = 4,
    REG_CTRL_EXCL_CLEAR_DIS_START = 3,
    REG_CTRL_EXCL_CLEAR_DIS_END = 3,
    REG_CTRL_EXCL_EVENTEN_START = 2,
    REG_CTRL_EXCL_EVENTEN_END = 2,
    REG_CTRL_ECCEN_START = 1,
    REG_CTRL_ECCEN_END = 1,
    REG_CTRL_LINKDOWN_START = 0,
    REG_CTRL_LINKDOWN_END = 0
};

enum L3tStaticCtrlReg {
    REG_NOSNP_ATOMIC_BYPASS_EN_START = 31,
    REG_NOSNP_ATOMIC_BYPASS_EN_END = 31,
    REG_RO_ALLOC_SHUT_EN_START = 30,
    REG_RO_ALLOC_SHUT_EN_END = 30,
    REG_WRFULL_HIT_SHUT_EN_START = 29,
    REG_WRFULL_HIT_SHUT_EN_END = 29,
    REQ_CONFLICT_EN_START = 28,
    REQ_CONFLICT_EN_END = 28,
    LOWER_POWER_EN_START = 26,
    LOWER_POWER_EN_END = 26,
    DATACLEAN_SHUT_EN_START = 25,
    DATACLEAN_SHUT_EN_END = 25,
    ARB_FLUSH_SHUT_EN_START = 24,
    ARB_FLUSH_SHUT_EN_END = 24,
    PGNT_ARB_EXAT_SHUT_EN_START = 23,
    PGNT_ARB_EXAT_SHUT_EN_END = 23,
    FAST_EXTER_SHUT_EN_START = 19,
    FAST_EXTER_SHUT_EN_END = 19,
    FAST_DATA_SHUT_EN_START = 18,
    FAST_DATA_SHUT_EN_END = 18,
    PEND_DATA_SHUT_EN_START = 17,
    PEND_DATA_SHUT_EN_END = 17,
    RAMSWAP_START = 16,
    RAMSWAP_END = 16,
    RAMFWD_SHUT_EN_START = 15,
    RAMFWD_SHUT_EN_END = 15,
    RAMTHR_MERGE_EN_START = 14,
    RAMTHR_MERGE_EN_END = 14,
    EXT_EN_START = 11,
    EXT_EN_END = 11
};

enum L3tDynamicAuctrl0Reg {
    RDNOSNP_NCA_SHUT_EN_START = 18,
    RDNOSNP_NCA_SHUT_EN_END = 18,
    WRFULL_CREATE_EN_START = 13,
    WRFULL_CREATE_EN_END = 13,
    CLEANUNIQUE_DATA_EN_START = 12,
    CLEANUNIQUE_DATA_EN_END = 12,
    LOCK_SHARE_REQ_EN_START = 10,
    LOCK_SHARE_REQ_EN_END = 10,
    DDR_COMPRESS_OPT_EN_START = 7,
    DDR_COMPRESS_OPT_EN_END = 7,
    ATOMIC_MONITOR_EN_START = 3,
    ATOMIC_MONITOR_EN_END = 3,
    SNPSLEEP_EN_START = 1,
    SNPSLEEP_EN_END = 1,
    PREFETCHTGT_EN_START = 0,
    PREFETCHTGT_EN_END = 0
};

enum L3tDynamicCtrlReg {
    READS_UPGRADE_EN_START =31,
    READS_UPGRADE_EN_END =31,
    RDMERGE_PIPE_EN_START = 27,
    RDMERGE_PIPE_EN_END = 27,
    SPILL_EN_START = 26,
    SPILL_EN_END = 26,
    SPILL_SHARE_EN_START = 25,
    SPILL_SHARE_EN_END = 25,
    SPILL_INSTR_EN_START = 24,
    SPILL_INSTR_EN_END = 24,
    SQRDMERGE_EN_START = 19,
    SQRDMERGE_EN_END = 19,
    PREFETCH_DROP_EN_START = 18,
    PREFETCH_DROP_EN_END = 18,
    DATAPULL_EN_START = 17,
    DATAPULL_EN_END = 17,
    MKINVLD_EN_START = 16,
    MKINVLD_EN_END = 16,
    RAMTHR_EN_START = 15,
    RAMTHR_EN_END = 15,
    RSPERR_EN_START = 14,
    RSPERR_EN_END = 14,
    IOCAPACITY_LIMIT_START = 13,
    IOCAPACITY_LIMIT_END = 13,
    FORCE_CQ_CLK_EN_START = 11,
    FORCE_CQ_CLK_EN_END = 11,
    SQMERGE_START = 10,
    SQMERGE_END = 10,
    RDMERGE_START = 9,
    RDMERGE_END = 9,
    RDMERGE_UPGRADE_EN_START = 8,
    RDMERGE_UPGRADE_EN_END = 8,
    PREFETCH_DROP_HHA_START = 4,
    PREFETCH_DROP_HHA_END = 4,
    TAG_REP_ALG_START = 0,
    TAG_REP_ALG_END = 1
};

enum L3tDynamicAuctrl1Reg {
    CPU_PF_LQOS_EN_START = 11,
    CPU_PF_LQOS_EN_END = 11,
    PRIME_DROP_MASK_START = 5,
    PRIME_DROP_MASK_END = 5,
    REFILLSIZE_COM_ADA_EN_START = 3,
    REFILLSIZE_COM_ADA_EN_END = 3,
    REFILLSIZE_PRE_ADA_EN_START = 2,
    REFILLSIZE_PRE_ADA_EN_END = 2,
    SEQUENCE_OPT_START = 1,
    SEQUENCE_OPT_END = 1    
};

enum L3tPrefetchReg {
    PREFETCH_CLR_LEVEL_START = 24,
    PREFETCH_CLR_LEVEL_END = 31,
    PREFETCH_OVERIDE_LEVEL_START = 20,
    PREFETCH_OVERIDE_LEVEL_END = 23,
    PREFETCH_ULT_DDR_START = 17,
    PREFETCH_ULT_DDR_END = 18,
    PREFETCH_ULT_DDR_EN_START = 16,
    PREFETCH_ULT_DDR_EN_END = 16,
    PREFETCH_ULT_L3T_START = 14,
    PREFETCH_ULT_L3T_END = 15,
    PREFETCH_UTL_L3T_EN_START = 13,
    PREFETCH_UTL_L3T_EN_END = 13,
    PREFETCH_VAGUE_EN_START = 12,
    PREFETCH_VAGUE_EN_END = 12,
    PREFETCH_CORE_EN_START = 8,
    PREFETCH_CORE_EN_END = 11,
    PREFETCH_MATCH_EN_START = 7,
    PREFETCH_MATCH_EN_END = 7,
    PREFETCH_START_LEVEL_START = 0,
    PREFETCH_START_LEVEL_END = 4,
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
