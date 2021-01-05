English | [简体中文](./README.md)

# Prefetch Tuning

This kernel module is based on Kunpeng chip and provides some performance-sensitive chip parameters for adjustment.

## Environment

- Hardware: Based on HiSilicon Kunpeng 920

- OS: EulerOS or openEuler

## Build and install

```
make clean && make
insmod prefetch_tuning.ko
```

To confirm the kernel module is installed successfully:

```
lsmod | grep prefetch_tuning
```

## Adjustable Parameters

There will be some files under `/sys/class/misc/prefetch/` supporting `cat` and `echo` command to get and revise parameters.

| Parameter | Meaning | Adjustment Range |
| --------- | ------- | ---------------- |
| policy | Prefetch policy | 0 ~ 15 |
| read_unique | Whether to allow cross-numa access to cache. | 0 (allow), 1 (forbid) |
| reg_nosnp_atomic_bypass_en | Whether to bypass atomic operations of CPUs. | 0 (disable), 1 (enable) |
| reg_ro_alloc_shut_en | Indicates whether to enable the function of allocating readOnce operations to L3. | 0 (disable), 1(enable) |
| reg_wrfull_hit_shut_en | Indicates whether to disable the function. When 64wu_full hits pipe M, do not send createE to the HA. | 1 (enable), 0 (disable) |
| req_conflict_en | Whether to enable backpressure on the CPU in one beat if requests from the CPU and non-CPU are received at the same beat. | 1 (enable), 0 (disable) |
| lower_power_en | Whether to enable the CQ low-power mode. | 1 (enable), 0 (disable) |
| dataclean_shut_en | Whether to mask the CE bit carried by the writenosnoopfull of the TaiShan kernel. | 0 (not shielded), 1 (shielded) |
| arb_flush_shut_en | When the CQ is idle, the enable status of the ARBIT adjustment scheduling pointer is reset. | 0 (enable), 1(disable) |
| pgnt_arb_exat_shut_en | Whether to enable the scheduling of the pgrant in the try mode. | 0 (enable), 1 (disable) |
| fast_exter_shut_en | Whether to disable the external request data of miss from passing through the fast path. | 1(disable), 0 (enable) |
|fast_data_shut_en | Whether to disable the miss data from passing through the fast path. | 1 (disable), 0 (enable) |
| pend_data_shut_en | Whether to disable the miss data from passing through the pend channel. | 1 (disable), 0 (enable) |
| ramswap_full_shut_en | Full or partial when doing ramswap. | 0 (full), 1 (partial) |
| ramfwd_shut_en | Whether to enable the ramfwd function. | 1 (disable), 0 (enable) |
| reads_upgrade_en | Enable read_shared operation status promotion. | 0 (disable), 1(enable) |
| rdmerge_pipe_en | Whether to allow Sqmerge requests to be hit in Cpipe5. | 1 (enable), 0 (disable) |
| spill_en | Whether the L3T spin function is enabled. | 0 (disable), 1 (enable) |
| spill_shared_en | Whether L3T enables the Spill function in the shared state. | 0 (disable), 1(enable) |
| spill_instr_en | Whether the L3T enables the instruction spear function. | 0 (disable), 1(enable) |
| sqrdmerge_en | Enable RDMERGE acceleration after SQ merge operation. | 0 (disable), 1(enable) |
| prefetch_drop_en | Whether to discard prefetch packets in L3T mode. | 0 (disable), 1(enable) |
| datapull_en | Whether the L3T enables the data pull function. | 0 (disable), 1(enable) |
| mkinvld_en | Whether L3T is enabled to convert makelinvalid to cleaninvalid. | 0 (enable), 1(disable) |
| ramthr_en | Whether to allow L3D to directly return data to the CPU through the thr channel. | 1 (enable), 0 (disable) |
| rsperr_en | Indicates whether to report rsperr. | 1 (enable), 0 (disable) |
| iocapacity_limit_en | Whether to limit the io capacity of cache. | 0 (no limit), 1 (limit) |
| force_cq_clk_en | Whether to enable the cache queue clock forcibly for L3T. | 0 (disable), 1(enable) |
| sqmerge_en | Whether consecutive address access can occupy only one entry in the squeue to accelerate the merge process. | 0 (limit), 1 (merge) |
| rdmerge_upgrade_en | Whether to allow the RS to merge with the preceding ReadE. | 0 (disable), 1 (allow) |
| prefetch_drop_hha_en | Whether to merge a non-prefetch operation with the previous prefetch operation. | 0 (allow), 1 (limit) |
| tag_rep_alg | Choose cache line algorithm. | 0 (random), 1 (drrip), 2 (plru), 3 (random) |
| rdnosnp_nca_shut_en | Whether to mark the readnosnp of the bypass sent by the CPU as NCA. | 0 (yes), 1 (no) |
| wrfull_create_en | Whether to enable the 128-byte writeunique function to obtain the permission but not data from the HHA. | 0 (disable), 1 (enable) |
| cleanunique_data_en | Whether cleanunqunie returns data. | 0 (disable), 1(enable) |
| lock_share_req_en | Whether to enable the register lock in share mode and not to deliver operations to the HHA. | 0 (prohibited), 1 (allowed) |
| ddr_compress_opt_en | Optimization switch of support HHA compression access. | 0 (disable), 1 (enable) |
| atomic_monitor_en | Specifies whether to enable the atomic_monitor function. | 0 (disable), 1 (enable) |
| snpsleep_en | Whether to enable snp sleep. | 0 (disable), 1 (enable) |
| prefetchtgt_en | Whether to enable the prefetchtgt. | 0 (disable), 1 (enable) |
| sequence_shape_en | Enable to push back to the CPU for several cycles when the SQ is about to be full. | 0 (disable), 1 (enable) |
| mpam_portion_en | Enable the function of allocating MPAM based on the way. | 0 (disable), 1 (enable) |
| mpam_capacity_en | Enable the function of allocating MPAM based on capacity statistics. | 0 (disable), 1 (enable) |
| eccchk_en | Enable ECC_CHK. | 0 (disable), 1 (enable) |
| refill_1024_relax_en | Whether to use the 1024-bit size to send requests for access. | 0 (disable), 1 (enable) |
| lookup_thr_en | Whether to enable the through channel during pipeline query. | 0 (disable), 1 (enable) |
| snpunique_stash_en | Support to receive hydra SnpUniqueStash. | 0 (forbid), 1 (support) |
| prime_timeout_mask_en | Enable the count for timeout. | 0 (disable), 1 (enable) |
| prime_sleep_mask_en | Enable the function of releasing a sleep request after a period of time. | 0 (disable), 1 (enable) |
| prime_extend_mask_en | Whether to enable random allocation of a request to extendway. | 0 (disable), 1 (enable) |
| force_intl_allocate_fail | Enable the function of forcibly determining that the assign operation of the intleave type fails. | 0 (disable), 1 (enable) |
| cpu_write_unique_stream_en | Whether to forcibly process the writeunique operation delivered by the CPU as the stream type. | 0 (disable), 1 (enable) |
| cpu_pf_lqos_en | Whether to enable the prefetch operation delivered by the CPU to be forcibly processed as the lqos operation. | 0 (disable), 1 (enable) |
| cpu_vic_lqos_en | Whether to forcibly process the writeunique operation delivered by the CPU as the stream type. | 0 (disable), 1 (enable) |
| prime_excl_mask_en | Whether to enable the random exclusive operation. | 0 (disable), 1 (enable) |
| prime_drop_mask_en | Whether to enable prefetch to retry randomly. | 0 (disable), 1 (enable) |
| prime_home_mask_en | Enable the forcehome processing on internal requests randomly. | 0 (disable), 1 (enable) |
| refillsize_com_ada_en | Whether to enable the auto-sensing of the size of the request sent to the HHA. If the size of the continuously received requests is 128 bytes or 64 bytes, the size of the prefetched request is automatically adjusted. | 0 (disable), 1 (enable adaptive size adjustment) |
| refillsize_pre_ada_en | Whether to enable the adaptation of the size of the request sent to the HHA. If the size of the continuously received request is 128 bytes or 64 bytes, the size of the normal request is automatically adjusted. | 0 (disable), 1 (enable adaptive size adjustment) |
| sequence_opt_en | Whether to change the L3T processing to serial mode when blocked. | 0 (limit), 1 (enable) |
| prefetch_clr_level | Number of requests that fail to find the corresponding prefetch buffer and lower the priority of each buffer to make the existing buffer easier to replace. | 0 ~ 255 |
| prefetch_overide_level | Initial coverage priority for an operation to enter the prefetch buffer. If the value is incorrect, the threshold is decreased by 1. If the value is correct, the threshold is increased by 1. If the value is 0, the prefetch rule needs to be replaced. | 0 ~ 15 |
| prefetch_utl_ddr | The utilization of ddr that leads to the halving the threshold of prefetch. | 0 (less than 1/2), 1 (1/2), 2 (3/4), 3 (almost full) |
| prefetch_utl_ddr_en | Whether to allow the automatic threshold reduction according to the utilization of ddr. | 0 (forbid), 1 (allow). |
| prefetch_utl_l3t | The utilization of l3t that leads to the halving the threshold of prefetch. | 0 (less thean 1/2), 1 (1/2), 2 (3/4), 3 (almost full) |
| prefetch_utl_l3t_en | Whether to allow the automatic threshold reduction according to the utilization of l3t. | 0 (forbid), 1 (allow) |
| prefetch_vague_en | Indicates whether to enable fuzzy match for prefetch. After the function is enabled, the prefetch summarizes the same 16 KB address rule. The four 4 KB address rules are the same and can be used together. |0 (disable), 1 (enable) |
| prefetch_core_en | Whether to enable core prefetch. Every bit 1 setting indicates that the matching core request needs to be prefetched. e.g. 0x1001 represents core 1 and core 4 enable prefetch. **Note: this parameter controls the L3T_PREFETCH register which determines the cpu prefetch policies, so set every bit to 1 to enable prefetch policies which can be configurated by parameters prefixed with 'prefetch' in this module. (default disabled)**| 0 ~ 15 |
| prefetch_match_en | Whether to enable the prefetch operation after the prefetch hit. | 0 (disable), 1 (enable) |
| prefetch_start_level | The number of missing addresses that leads to prefetch. 0 -> 32, 1 -> 2, n-1 -> n | 0 ~ 31 |
| pime_timeout_num | The maximum count of timeout. | 0 ~ 65535 |
| reg_ctrl_spillprefetch | Snoop type configuration of the spill. | 0 (type of request), 1 (prefetch) |
| reg_ctrl_mpamen | Enable HHA MPAM scheduling. | 0 (disable), 1 (enable) |
| reg_ctrl_mpamqos | Enable QoS for modifying the DDR read/write command based on the MPAM monitoring and control bandwidth. | 0 (disable), 1 (enable) |
| reg_ctrl_poison | Enable HHA to return poison. | 0 (disable), 1 (enable) |
| reg_ctrl_compress_spec | Enable the random read of 128-byte data in HHA memory data compression. | 0 (disable), 1 (enable) |
| reg_ctrl_writeevict_drop | Enable the discard of WriteEvictI. | 0 (disable), 1 (enable) |
| reg_ctrl_prefetch_drop | Whether to enable prefetch operation discard. | 0 (disable), 1 (enable) |
| reg_ctrl_dmcassign | DDR access address alignment enable. | 0 (The DDR read operation uses the wrap mode, and the address is 32-byte-aligned. The DDR write operation is always in INCR mode, and the address is aligned with the access boundary), 1 (The DDR read operation is always in INCR mode, and the address is aligned with the access boundary. The DDR write operation is always in INCR mode, and the address is aligned with the access boundary) |
| reg_ctrl_rdatabyp | DDR read data bypass memory enable in the HHA. |0 (disable), 1 (The internal data of the HHA is bypassed, and the DDR read data can be transmitted quickly) |
| reg_ctrl_excl_clear_dis | Whether to disable the function of periodically clearing HHA non-cacheable exclusive monitor. | 0 (enable), 1 (disable) |
| reg_ctrl_excl_eventen | Enable HHA non-cacheable exclusive monitor event. An event can be sent to wake up the CPU when an address is successfully written or corrupted. | 0 (disable), 1 (enable) |
| reg_ctrl_eccen | Enable the memory ECC error correction in the HHA. | 0 (disable), 1 (enable) |
| reg_readoncesnp_dis | Disable NCA Readonce fixed snoop. | 0 (enable), 1 (disable) |
| reg_cc_exter_stash | L3T configuration of extern snoop stash. | 0 (forbid), 1 (allow) |
| reg_cc_writebacki_spill_full | Enable fixed 128-byte data spill of the WritebackI operation. | 0 (disable), 1 (enable) |
| reg_cc_writeevicti_spill_full | Enable fixed 128-byte data spill of the WriteEvictI operation. | 0 (disable), 1 (enable) |
| reg_cc_stashonce_full | Enable fixed 128-byte data stash of the StashOnce operation. | 0 (disable), 1 (enable) |
| reg_cc_atomicstashl2 | Enable L2 stash of atomic operations. | 0 (disable), 1 (enable) |
| reg_cc_atomicstashl3 | Enable L3 stash of atomic operations. | 0 (disable), 1 (enable) |
| reg_cc_atomicstashclr | Clear L3 stash monitor of atomic operations. | 0 (disable), 1 (enable) |
| reg_cc_cmo_snpme | Enable snoop me for CMO operations. | 0 (disable), 1 (enable) |
| reg_cc_makee_change | Enable HHA MakeE conversion to readE when the HHA MakeE is not self-hit. | 0 (disable), 1 (enable) |
| reg_cc_ioc_hitsca_dis | Disable the function of recording CAIDs when the HHA I/O cache hits the exact directory. | 0 (enable), 1 (disable) |
| reg_cc_passdirty | Enable HHA pass dirty. | 0 (disable), 1 (enable) |
| reg_cc_snpdrop | Enable Snoop Drop. | 0 (disable), 1 (enable) |
| reg_cc_spill | Enable local multi-partition sharing. | 0 (disable), 1 (enable) |
| reg_precisionsnp_dis | Disable HHA precise snoop based on shared directories. | 0 (disable), 1 (enable) |
| reg_notonly_excl | Whether to create new entries for exclusive operations in the HHA share directory buffer. | 0 (only for exclusive operations), 1 (for all of operations) |
| reg_miss_allindex | Enable that HHA miss queues are related based on index. | 0 (disable), 1 (enable) |
| reg_miss_cbackth | Enable HHA miss queue copyback request to use second threshold. | 0 (disable), 1 (enable) |
| reg_miss_normalth | Enable HHA miss queue common request to use second threshold. | 0 (disable), 1 (enable) |
| reg_miss_tosdir | Enable HHA only to allow miss alloc to be sent to sdir. | 0 (disable), 1 (enable) |
| reg_entry_except | Exclude the same entry address in HHA. | 0 (disable), 1 (enable) |
| reg_dir_replace_alg | Directory replacement algorithm configuration. | 0 (EDIR random + SDIR random), 1 (EDIR random + SDIR polling), 2 (EDIR PLRU + SDIR random), 3 (EDIR PLRU + SDIR polling) |
| strict_order | Keep the order of HHA operation queue strictly. | 0 (disable), 1 (enable) |
| prefetch_comb | Read operation and prefetchtgt merge enable. | 0 (The read operation can be merged with the fetchtgt operation), 1 (The read operation and the fetchtgt merge operation are not allowed) |
| evict_green | Unblocking configuration of the evict in PQ. | 0 (evict can't be blocked), 1 (evict can be blocked) |
| block_retry | Whether to perform retry configuration directly when the MPAM hardlim flow bandwidth exceeds the configured one such that enters CMD. | 0 (retry directly), 1 (don't retry directly and be scheduled with other flows) |
| buffer_prio | Priority configuration for the ingress queue of the CMD buffer request and PGNT application. | 0 (CMD buffer priors to pgnt), 1 (CMD buffer and pgnt have equivalent priority) |
| half_wr_rdddr_delay | Enables the DDR read delay during 64-byte full write operations after compression. | 0 (disable), 1 (enable) |
| wback_cnfl_rdhalf | DDR size configuration that is reread when the writeback conflict occurs. | 0 (depend on Writeback address and size), 1 (size = 128B) |
| reg_funcdis_pendprecision | Enable precise pend. | 0 (pend is precisely depend on flit), 1 (pend = 1) |
| reg_funcdis_combrdddr | Reread DDR after multiple adjacent narrow write operations are merged. | 0, 1 |
| reg_funcdis_scramble | Ingress queue scrambling. | 0 (disable), 1 (enable) |
| reg_funcdis_stashidpg | Whether to enable the partial good conversion of the Stash TGTID. | 0 (disable), 1 (enable) |
| reg_funcdis_rdatatime | HHA receives DMC read data anti-starvation threshold configuration. | 0 (threshold = 8), 1 (threshold = 4) |
| reg_funcdis_dmcutl | DMC usage source selection. | 0 (from DDRC), 1 (from queue processing utilization ratio inside HHA) |
| reg_funcdis_cancelexcept | The pipeline index check excludes requests that are not actually queried (for example, prefetchtgt). | 0 (enable exclusion), 1 (disable exclusion) |
| reg_funcdis_ccixcbupdate | Whether to update the directory in the CCIX copyback of the multi-CA. | 0 (allow), 1 (forbid) |
| reg_funcdis_updateopen | Block the update dir command in the processing queue based on index. | 0 (disable), 1 (enable) |
| reg_funcdis_comb | Whether to merge write operations whose size is less than 128 bytes. | 0 (enable), 1 (disables the merge function of the write operation) |
| reg_prefetchtgt_outstanding | Outstanding configuration for the HHA to read data from the DDR prefetch. When the read/write operation sent by the HHA to the DDR exceeds the threshold, the prefetchtgt operation is forbidden to read the DDR data and the operation is directly discarded. This configuration and reg_prefetch_outstanding control the prefetch threshold at the same time. | 0 ~ 127 |
| reg_prefetchtgt_level | Threshold for the HHA to read data from the DDR prefetch. When the DDR read/write operations in the HHA processing queue exceed the threshold, the prefetchtgt operation is forbidden to read the DDR data and the operation is directly discarded. This configuration and reg_prefetch_outstanding control the prefetch threshold at the same time. | 0 ~ 127 |
| reg_spec_rd_level | DDR threshold configuration for speculation read. When the DDR read and write commands in the HHA processing queue exceed the threshold, speculative reading of the DDR is prohibited. After the directory is queried, the system determines whether to read the DDR based on the directory query result. Note: The value 0x08 or 0x10 is recommended. | 0 ~ 127 |
| reg_drop_level | Prefetch drop threshold configuration. When the number of DDR read and write commands in the HHA processing queue exceeds the threshold, some prefetch read commands can be discarded. | 0 ~ 127 |
| dvmsnp_outstanding | Outstanding value of the DVMSNP of the MN. Note 1: If dvmsnp_perf_en is enabled, the configured value is valid. The maximum value of outstanding can be 5 when the TaiShan core is used. Otherwise, overflow errors occur. Note 2: The SMMU cannot match the Dvmsnp outstanding value 5. Therefore, you need to set the switch to 3 for Totem and Infinite of 1383. Totem and Nimbus of 1620: Set DVM outstanding to 5. However, do not configure POE for the DVMSNP broadcast node. The POE uses a private page table and does not require DVMSNP. | 0 ~ 15(Note: 0 represents that outstanding level is 1)
| dvmreq_outstanding | Outstanding value of the DVMREQ of the MN. Note 1: If dvmreq_perf_en is enabled, the configured value is valid. The maximum value of outstanding can be 9 when there are four chips. Otherwise, an overflow error occurs. Note 2: In the case of two chips, the maximum outstanding value of totem can be 10, and the maximum outstanding value of nimbus or infinite is 24. This ensures the best performance. Note 3: In the case of a single chip, there is no restriction on the outstanding configuration of the totem. | 0 ~ 31(Note: 0 represents that outstanding level is 1) |
| dvmsnp_perf_en | Whether to enable the outstanding level for the dvmsnp.(Note: After the function is enabled, the dvmsnp outstanding value of the MN is equal to the value of dvmsnp_outstanding.) | 0 (disable), 1 (enable) |
| dvmreq_perf_en | Whether to enable the outstanding level for the dvmreq.(Note: After the function is enabled, the dvmreq outstanding value of the MN is equal to the value of dvmreq_outstanding.) | 0 (disable), 1 (enable) |


> Note: To get more details, please read the specific chip manual.

## Configuration Method

Configurate the specific paramters like this:

```
echo 1 > /sys/class/misc/prefetch/policy
```

To confirm whether the configuration is valid or not, use `cat` like this:

```
cat /sys/class/misc/prefetch/policy
```

## Uninstall

```
rmmod prefetch_tuning
```

The uninstall operation can reset the parameter configurations to default.