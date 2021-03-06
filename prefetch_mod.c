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
#include <asm/io.h>
#include "prefetch_mod.h"

#ifndef is_affinity_mask_valid
#define is_affinity_mask_valid(val) 1
#endif

static cpumask_var_t prefetch_cpumask_value;
static cfg_t __percpu *cur_cfg;
static cfg_t __percpu *old_cfg;
static int old_cfg_int[FUNC_NUM] = {0};
static DEFINE_MUTEX(prefetch_mtx);

static ssize_t read_unique_store(struct device* dev,
                                 struct device_attribute* attr, const char* buf, size_t count);
static ssize_t read_unique_show(struct device* dev,
                                struct device_attribute* attr, char* buf);
static ssize_t prefetch_store(struct device* dev,
                              struct device_attribute* attr, const char* buf, size_t count);
static ssize_t prefetch_show(struct device* dev,
                             struct device_attribute* attr, char* buf);
static ssize_t prefetch_mask_store(struct device* dev,
                                   struct device_attribute* attr, const char* buf, size_t count);
static ssize_t prefetch_mask_show(struct device* dev,
                                  struct device_attribute* attr, char* buf);
static ssize_t val_store(struct device* dev,
                         struct device_attribute* attr, const char* buf, size_t count);
static ssize_t val_show(struct device* dev,
                        struct device_attribute* attr, char* buf);

/* Create device attribute files */
static DEVICE_ATTR(policy, S_IRUGO|S_IWUSR,
                   prefetch_show, prefetch_store);

static DEVICE_ATTR(read_unique, S_IRUGO|S_IWUSR,
                   read_unique_show, read_unique_store);

static DEVICE_ATTR(cpumask, S_IRUGO|S_IWUSR,
                   prefetch_mask_show, prefetch_mask_store);

static DEVICE_ATTR(reg_nosnp_atomic_bypass_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ro_alloc_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_wrfull_hit_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(req_conflict_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(lower_power_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(dataclean_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(arb_flush_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(pgnt_arb_exat_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(fast_exter_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(fast_data_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(pend_data_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(ramswap_full_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(ramfwd_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reads_upgrade_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(rdmerge_pipe_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(spill_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(spill_shared_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(spill_instr_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(sqrdmerge_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_drop_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(datapull_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(mkinvld_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(ramthr_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(rsperr_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(iocapacity_limit_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(force_cq_clk_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(sqmerge_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(rdmerge_upgrade_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_drop_hha_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(tag_rep_alg, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(rdnosnp_nca_shut_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(wrfull_create_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(cleanunique_data_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(lock_share_req_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(ddr_compress_opt_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(atomic_monitor_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(snpsleep_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetchtgt_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(sequence_shape_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(mpam_portion_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(mpam_capacity_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(eccchk_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(refill_1024_relax_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(lookup_thr_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(snpunique_stash_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prime_timeout_mask_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prime_sleep_mask_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prime_extend_mask_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(force_intl_allocate_fail, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(cpu_write_unique_stream_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(cpu_pf_lqos_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(cpu_vic_lqos_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prime_excl_mask_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prime_drop_mask_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prime_home_mask_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(refillsize_com_ada_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(refillsize_pre_ada_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(sequence_opt_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_clr_level, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_overide_level, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_utl_ddr, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_utl_ddr_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_utl_l3t, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_utl_l3t_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_vague_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_core_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_match_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_start_level, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(pime_timeout_num, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_spillprefetch, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_mpamen, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_mpamqos, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_poison, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_compress_spec, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_writeevict_drop, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_prefetch_drop, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_dmcassign, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_rdatabyp, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_excl_clear_dis, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_excl_eventen, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_ctrl_eccen, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_readoncesnp_dis, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_exter_stash, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_writebacki_spill_full, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_writeevicti_spill_full, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_stashonce_full, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_atomicstashl2, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_atomicstashl3, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_atomicstashclr, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_cmo_snpme, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_makee_change, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_ioc_hitsca_dis, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_passdirty, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_snpdrop, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_cc_spill, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_precisionsnp_dis, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_notonly_excl, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_miss_allindex, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_miss_cbackth, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_miss_normalth, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_miss_tosdir, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_entry_except, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_dir_replace_alg, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(strict_order, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(prefetch_comb, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(evict_green, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(block_retry, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(buffer_prio, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(half_wr_rdddr_delay, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(wback_cnfl_rdhalf, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_pendprecision, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_combrdddr, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_scramble, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_stashidpg, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_rdatatime, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_dmcutl, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_cancelexcept, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_ccixcbupdate, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_updateopen, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_funcdis_comb, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_prefetchtgt_outstanding, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_prefetchtgt_level, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_spec_rd_level, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(reg_drop_level, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(dvmsnp_outstanding, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(dvmreq_outstanding, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(dvmsnp_perf_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);

static DEVICE_ATTR(dvmreq_perf_en, S_IRUGO|S_IWUSR,
                   val_show, val_store);
static struct attribute *prefetch_attrs[] = {
    &dev_attr_policy.attr,
    &dev_attr_cpumask.attr,
    &dev_attr_read_unique.attr,
    &dev_attr_iocapacity_limit_en.attr,
    &dev_attr_tag_rep_alg.attr,
    &dev_attr_ramswap_full_shut_en.attr,
    &dev_attr_sqmerge_en.attr,
    &dev_attr_prefetch_drop_hha_en.attr,
    &dev_attr_prefetch_utl_ddr.attr,
    &dev_attr_prefetch_utl_ddr_en.attr,
    &dev_attr_prefetch_utl_l3t.attr,
    &dev_attr_prefetch_utl_l3t_en.attr,
    &dev_attr_prefetch_start_level.attr,
    &dev_attr_rdmerge_upgrade_en.attr,
    &dev_attr_prefetch_overide_level.attr,
    &dev_attr_prefetch_vague_en.attr,
    &dev_attr_prefetch_core_en.attr,
    &dev_attr_prefetch_match_en.attr,
    &dev_attr_reg_ctrl_prefetch_drop.attr,
    &dev_attr_reg_ctrl_dmcassign.attr,
    &dev_attr_reg_ctrl_rdatabyp.attr,
    &dev_attr_reg_dir_replace_alg.attr,
    &dev_attr_prefetch_comb.attr,
    &dev_attr_reg_funcdis_comb.attr,
    &dev_attr_reg_nosnp_atomic_bypass_en.attr,
    &dev_attr_reg_ro_alloc_shut_en.attr,
    &dev_attr_reg_wrfull_hit_shut_en.attr,
    &dev_attr_req_conflict_en.attr,
    &dev_attr_lower_power_en.attr,
    &dev_attr_dataclean_shut_en.attr,
    &dev_attr_arb_flush_shut_en.attr,
    &dev_attr_pgnt_arb_exat_shut_en.attr,
    &dev_attr_fast_exter_shut_en.attr,
    &dev_attr_fast_data_shut_en.attr,
    &dev_attr_pend_data_shut_en.attr,
    &dev_attr_ramfwd_shut_en.attr,
    &dev_attr_reads_upgrade_en.attr,
    &dev_attr_rdmerge_pipe_en.attr,
    &dev_attr_spill_en.attr,
    &dev_attr_spill_shared_en.attr,
    &dev_attr_spill_instr_en.attr,
    &dev_attr_sqrdmerge_en.attr,
    &dev_attr_prefetch_drop_en.attr,
    &dev_attr_datapull_en.attr,
    &dev_attr_mkinvld_en.attr,
    &dev_attr_ramthr_en.attr,
    &dev_attr_rsperr_en.attr,
    &dev_attr_force_cq_clk_en.attr,
    &dev_attr_rdnosnp_nca_shut_en.attr,
    &dev_attr_wrfull_create_en.attr,
    &dev_attr_cleanunique_data_en.attr,
    &dev_attr_lock_share_req_en.attr,
    &dev_attr_ddr_compress_opt_en.attr,
    &dev_attr_atomic_monitor_en.attr,
    &dev_attr_snpsleep_en.attr,
    &dev_attr_prefetchtgt_en.attr,
    &dev_attr_prefetch_clr_level.attr,
    &dev_attr_reg_ctrl_spillprefetch.attr,
    &dev_attr_reg_ctrl_mpamen.attr,
    &dev_attr_reg_ctrl_mpamqos.attr,
    &dev_attr_reg_ctrl_poison.attr,
    &dev_attr_reg_ctrl_compress_spec.attr,
    &dev_attr_reg_ctrl_writeevict_drop.attr,
    &dev_attr_reg_ctrl_excl_clear_dis.attr,
    &dev_attr_reg_ctrl_excl_eventen.attr,
    &dev_attr_reg_ctrl_eccen.attr,
    &dev_attr_sequence_shape_en.attr,
    &dev_attr_mpam_portion_en.attr,
    &dev_attr_mpam_capacity_en.attr,
    &dev_attr_eccchk_en.attr,
    &dev_attr_refill_1024_relax_en.attr,
    &dev_attr_lookup_thr_en.attr,
    &dev_attr_snpunique_stash_en.attr,
    &dev_attr_prime_timeout_mask_en.attr,
    &dev_attr_prime_sleep_mask_en.attr,
    &dev_attr_prime_extend_mask_en.attr,
    &dev_attr_force_intl_allocate_fail.attr,
    &dev_attr_cpu_write_unique_stream_en.attr,
    &dev_attr_cpu_pf_lqos_en.attr,
    &dev_attr_cpu_vic_lqos_en.attr,
    &dev_attr_prime_excl_mask_en.attr,
    &dev_attr_prime_drop_mask_en.attr,
    &dev_attr_prime_home_mask_en.attr,
    &dev_attr_refillsize_com_ada_en.attr,
    &dev_attr_refillsize_pre_ada_en.attr,
    &dev_attr_sequence_opt_en.attr,
    &dev_attr_pime_timeout_num.attr,
    &dev_attr_dvmsnp_outstanding.attr,
    &dev_attr_dvmreq_outstanding.attr,
    &dev_attr_dvmsnp_perf_en.attr,
    &dev_attr_dvmreq_perf_en.attr,
    &dev_attr_reg_readoncesnp_dis.attr,
    &dev_attr_reg_cc_exter_stash.attr,
    &dev_attr_reg_cc_writebacki_spill_full.attr,
    &dev_attr_reg_cc_writeevicti_spill_full.attr,
    &dev_attr_reg_cc_stashonce_full.attr,
    &dev_attr_reg_cc_atomicstashl2.attr,
    &dev_attr_reg_cc_atomicstashl3.attr,
    &dev_attr_reg_cc_atomicstashclr.attr,
    &dev_attr_reg_cc_cmo_snpme.attr,
    &dev_attr_reg_cc_makee_change.attr,
    &dev_attr_reg_cc_ioc_hitsca_dis.attr,
    &dev_attr_reg_cc_passdirty.attr,
    &dev_attr_reg_cc_snpdrop.attr,
    &dev_attr_reg_cc_spill.attr,
    &dev_attr_reg_precisionsnp_dis.attr,
    &dev_attr_reg_notonly_excl.attr,
    &dev_attr_reg_miss_allindex.attr,
    &dev_attr_reg_miss_cbackth.attr,
    &dev_attr_reg_miss_normalth.attr,
    &dev_attr_reg_miss_tosdir.attr,
    &dev_attr_reg_entry_except.attr,
    &dev_attr_strict_order.attr,
    &dev_attr_evict_green.attr,
    &dev_attr_block_retry.attr,
    &dev_attr_buffer_prio.attr,
    &dev_attr_half_wr_rdddr_delay.attr,
    &dev_attr_wback_cnfl_rdhalf.attr,
    &dev_attr_reg_funcdis_pendprecision.attr,
    &dev_attr_reg_funcdis_combrdddr.attr,
    &dev_attr_reg_funcdis_scramble.attr,
    &dev_attr_reg_funcdis_stashidpg.attr,
    &dev_attr_reg_funcdis_rdatatime.attr,
    &dev_attr_reg_funcdis_dmcutl.attr,
    &dev_attr_reg_funcdis_cancelexcept.attr,
    &dev_attr_reg_funcdis_ccixcbupdate.attr,
    &dev_attr_reg_funcdis_updateopen.attr,
    &dev_attr_reg_prefetchtgt_outstanding.attr,
    &dev_attr_reg_prefetchtgt_level.attr,
    &dev_attr_reg_spec_rd_level.attr,
    &dev_attr_reg_drop_level.attr,
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
static ssize_t read_unique_store(struct device* dev,
                                 struct device_attribute* attr, const char* buf, size_t count)
{
    ssize_t ret = -1;
    int value = -1;

    ret = kstrtouint(buf, 0, &value);
    if (ret || (value != 0 && value != 1)) {
        pr_err("invalid input!\n");
        return count;
    }

    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, read_unique_set, &value, 1);
    mutex_unlock(&prefetch_mtx);

    return count;
}

static ssize_t read_unique_show(struct device* dev,
                                struct device_attribute* attr, char* buf)
{
    int cpu = -1;
    int count = 0;

    int __percpu *cur = alloc_percpu(int);
    if (!cur) {
        pr_err("alloc_percpu fail\n");
        return -ENOMEM;
    }
    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, read_unique_get, cur, 1);

    for_each_cpu(cpu, prefetch_cpumask_value) {
        int *ptr = per_cpu_ptr(cur, cpu);
        count += scnprintf(buf + count, PAGE_SIZE, "cpu(%d): %d.\n",
                           cpu, (ptr == NULL) ? -1 : *ptr);
    }
    mutex_unlock(&prefetch_mtx);
    free_percpu(cur);
    return count;
}

static ssize_t prefetch_store(struct device* dev,
                              struct device_attribute* attr, const char* buf, size_t count)
{
    ssize_t ret = -1;
    int policy = -1;

    ret = kstrtouint(buf, 0, &policy);
    if (ret) {
        pr_err("invalid input\n");
        return count;
    }

    mutex_lock(&prefetch_mtx);
    if (policy < prefetch_policy_num()) {
        on_each_cpu_mask(prefetch_cpumask_value, set_prefetch, prefetch_policy(policy), 1);
    } else {
        pr_err("policy %d is out of range\n", policy);
    }
    mutex_unlock(&prefetch_mtx);

    return count;
}

static ssize_t prefetch_show(struct device* dev,
                             struct device_attribute* attr, char* buf)
{
    int cpu = -1;
    int policy = -1;
    int count = 0;

    mutex_lock(&prefetch_mtx);
    on_each_cpu_mask(prefetch_cpumask_value, get_prefetch, cur_cfg, 1);
    for_each_cpu(cpu, prefetch_cpumask_value) {
        cfg_t *this_cfg = per_cpu_ptr(cur_cfg, cpu);
        for (policy = prefetch_policy_num() - 1; policy >= 0; policy--) {
            if (!memcmp(prefetch_policy(policy), this_cfg, sizeof(cfg_t)))
                break;
        }
        count += scnprintf(buf + count, PAGE_SIZE, "cpu(%d): %d\n", cpu, policy);
    }
    mutex_unlock(&prefetch_mtx);

    return count;
}

static ssize_t prefetch_mask_store(struct device* dev,
                                   struct device_attribute* attr, const char* buf, size_t count)
{
    int ret = -1;

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
    ssize_t ret = -1;
    mutex_lock(&prefetch_mtx);
    ret = cpumap_print_to_pagebuf(true, buf, prefetch_cpumask_value);
    mutex_unlock(&prefetch_mtx);
    return ret;
}

static ssize_t show_all(struct device* dev, struct device_attribute* attr, char* buf, FuncStruct* temp)
{
    int reg = 1, count = 0;
    unsigned int die_idx = 0, skt_idx = 0, die_nr = 2;
    unsigned long skt_offset = 0x200000000000ULL;
    unsigned nr_skt = 2, totem_num = 1;
    nr_skt = get_nr_skt();
    totem_num = get_totem_num();
    skt_offset = get_skt_offset();
    for (skt_idx = 0; skt_idx < nr_skt; skt_idx++) {
        for (die_idx = 0; die_idx < die_nr + 1; die_idx++) {
            unsigned long base_remap = 0;
            unsigned long base = skt_idx * skt_offset;
            unsigned val = 0;
            if ((totem_num == 1) && (die_idx == 1))
                continue;

            if (die_idx == 1)
                base += TOTEM_OFFSET;

            if (die_idx < die_nr)
                base_remap = (unsigned long)ioremap(base + (*temp).Base, REG_RANGE);
            else if ((*temp).Base == TB_AA_BASE) // To get the NIBUS configuration
                base_remap = (unsigned long)ioremap(base + NI_AA_BASE, REG_RANGE);
            else
                continue;

            if (!base_remap)
                return count;

            (*temp).Address = base_remap + (*temp).Offset;
            val = get_val(*temp);
            count += scnprintf(buf + count, PAGE_SIZE, "register(%d): %d.\n",
                               reg++, val);
            iounmap((volatile void*)base_remap);
        }
    }
    return count;
}

static ssize_t val_store(struct device* dev,
                         struct device_attribute* attr, const char* buf, size_t count)
{
    int ret = -1, val = -1;
    FuncStruct *temp = NULL;

    temp = get_func(attr);
    if (temp == NULL) {
        return count;
    }

    ret = kstrtouint(buf, 0, &val);
    if (ret || val < (*temp).Glb || val > (*temp).Sup) {
        pr_err("invalid input!\n");
        return count;
    }

    (*temp).Val = val;
    mutex_lock((*temp).temp_mtx);
    set_val(*temp);
    mutex_unlock((*temp).temp_mtx);
    return count;
}

static ssize_t val_show(struct device* dev,
                        struct device_attribute* attr, char* buf)
{
    int count = 0;
    FuncStruct *temp = NULL;

    temp = get_func(attr);
    if (temp == NULL) {
        return count;
    }

    mutex_lock((*temp).temp_mtx);
    count = show_all(dev, attr, buf, temp);
    mutex_unlock((*temp).temp_mtx);
    return count;
}

/*
 * prefetch policy, can be 0~15:
 * 0: disable; 1~15: different thresholds for sms,amop algrithom;
 */
static int __init prefetch_init(void)
{
#ifdef CONFIG_ARCH_HISI
    int ret = -1;
    int cpu = -1;
    /* get cpu infomation to identify L3T_DYNAMIC_CTRL registers */
    ret = initial_cpu_info();
    if (ret < 0) {
        pr_err("get cpu information fail\n");
        goto err_init;
    }

    /* get the and store default configurations */
    ret = get_default_cfg(old_cfg_int);
    if (ret < 0) {
        pr_err("get default cfg fail\n");
        goto err_init;
    }

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
err_init:
    return ret;

#else
    return 0;
#endif
}

static void __exit prefetch_exit(void)
{
#ifdef CONFIG_ARCH_HISI
    misc_deregister(&misc);
    on_each_cpu(reset_prefetch, old_cfg, 1);
    free_percpu(old_cfg);
    free_percpu(cur_cfg);
    free_cpumask_var(prefetch_cpumask_value);
    reset_default_cfg(old_cfg_int);
#endif
}

module_init(prefetch_init);
module_exit(prefetch_exit);
MODULE_LICENSE("GPL");
