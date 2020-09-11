#!/bin/bash

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/sqmerge"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/sqmerge"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_drop_hha"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_drop_hha"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prime_drop_mask"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prime_drop_mask"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/sequence_opt"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/sequence_opt"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_ult_ddr"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_ult_ddr"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 2 > /sys/class/misc/prefetch/prefetch_ult_ddr"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 3 > /sys/class/misc/prefetch/prefetch_ult_ddr"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_utl_l3t"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_utl_l3t"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 2 > /sys/class/misc/prefetch/prefetch_utl_l3t"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 3 > /sys/class/misc/prefetch/prefetch_utl_l3t"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_utl_l3t_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_utl_l3t_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_start_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 3 > /sys/class/misc/prefetch/prefetch_start_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 8 > /sys/class/misc/prefetch/prefetch_start_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 31 > /sys/class/misc/prefetch/prefetch_start_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/reg_totem_dual"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/rdmerge_upgrade_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/rdmerge_upgrade_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/ddr_compress_opt_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/ddr_compress_opt_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/snpsleep_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/snpsleep_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetchtgt_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetchtgt_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/cpu_pf_lqos_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/cpu_pf_lqos_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/refillsize_com_ada_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/refillsize_com_ada_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/refillsize_pre_ada_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/refillsize_pre_ada_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_overide_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 3 > /sys/class/misc/prefetch/prefetch_overide_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 7 > /sys/class/misc/prefetch/prefetch_overide_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 11 > /sys/class/misc/prefetch/prefetch_overide_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 15 > /sys/class/misc/prefetch/prefetch_overide_level"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_vague_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_vague_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_match_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_match_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/reg_ctrl_prefetch_drop"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/reg_ctrl_prefetch_drop"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/reg_ctrl_dmcassign"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/reg_ctrl_dmcassign"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/prefetch_core_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/prefetch_core_en"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/reg_ctrl_rdatabyp"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/reg_ctrl_rdatabyp"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/reg_funcdis_comb"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/reg_funcdis_comb"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/reg_dir_replace_alg"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/reg_dir_replace_alg"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 2 > /sys/class/misc/prefetch/reg_dir_replace_alg"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 3 > /sys/class/misc/prefetch/reg_dir_replace_alg"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 0 > /sys/class/misc/prefetch/read_unique"
sh rm.sh
sh rs.sh

ETS_SSHCMD "echo 1 > /sys/class/misc/prefetch/read_unique"
sh rm.sh
sh rs.sh
