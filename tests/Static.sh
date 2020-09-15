#!/bin/bash
# SPDX-License-Identifier: GPL-2.0
# * Copyright(c) 2019 Huawei Technologies Co., Ltd
# *
# * This program is free software; you can redistribute it and/or modify it
# * under the terms and conditions of the GNU General Public License,
# * version 2, as published by the Free Software Foundation.
# *
# * This program is distributed in the hope it will be useful, but WITHOUT
# * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# * for more details.
# Create: 2020-09-07
# Author: Liuke (liuke20@gitee)

echo "L3T_STATIC: reg_nosnp_atomic_bypass_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/reg_nosnp_atomic_bypass_en
cat /sys/class/misc/prefetch/reg_nosnp_atomic_bypass_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/reg_nosnp_atomic_bypass_en
cat /sys/class/misc/prefetch/reg_nosnp_atomic_bypass_en | grep register\(1\)

echo "L3T_STATIC: reg_ro_alloc_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/reg_ro_alloc_shut_en
cat /sys/class/misc/prefetch/reg_ro_alloc_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/reg_ro_alloc_shut_en
cat /sys/class/misc/prefetch/reg_ro_alloc_shut_en | grep register\(1\)

echo "L3T_STATIC: reg_wrfull_hit_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/reg_wrfull_hit_shut_en
cat /sys/class/misc/prefetch/reg_wrfull_hit_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/reg_wrfull_hit_shut_en
cat /sys/class/misc/prefetch/reg_wrfull_hit_shut_en | grep register\(1\)

echo "L3T_STATIC: req_conflict_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/req_conflict_en
cat /sys/class/misc/prefetch/req_conflict_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/req_conflict_en
cat /sys/class/misc/prefetch/req_conflict_en | grep register\(1\)

echo "L3T_STATIC: lower_power_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/lower_power_en
cat /sys/class/misc/prefetch/lower_power_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/lower_power_en
cat /sys/class/misc/prefetch/lower_power_en | grep register\(1\)

echo "L3T_STATIC: dataclean_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/dataclean_shut_en
cat /sys/class/misc/prefetch/dataclean_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/dataclean_shut_en
cat /sys/class/misc/prefetch/dataclean_shut_en | grep register\(1\)

echo "L3T_STATIC: arb_flush_shut_en_n set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/arb_flush_shut_en_n
cat /sys/class/misc/prefetch/arb_flush_shut_en_n | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/arb_flush_shut_en_n
cat /sys/class/misc/prefetch/arb_flush_shut_en_n | grep register\(1\)

echo "L3T_STATIC: pgnt_arb_exat_shut_en_n set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/pgnt_arb_exat_shut_en_n
cat /sys/class/misc/prefetch/pgnt_arb_exat_shut_en_n | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/pgnt_arb_exat_shut_en_n
cat /sys/class/misc/prefetch/pgnt_arb_exat_shut_en_n | grep register\(1\)

echo "L3T_STATIC: fast_exter_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/fast_exter_shut_en
cat /sys/class/misc/prefetch/fast_exter_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/fast_exter_shut_en
cat /sys/class/misc/prefetch/fast_exter_shut_en | grep register\(1\)

echo "L3T_STATIC: fast_data_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/fast_data_shut_en
cat /sys/class/misc/prefetch/fast_data_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/fast_data_shut_en
cat /sys/class/misc/prefetch/fast_data_shut_en | grep register\(1\)

echo "L3T_STATIC: pend_data_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/pend_data_shut_en
cat /sys/class/misc/prefetch/pend_data_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/pend_data_shut_en
cat /sys/class/misc/prefetch/pend_data_shut_en | grep register\(1\)

echo "L3T_STATIC: ramfwd_shut_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/ramfwd_shut_en
cat /sys/class/misc/prefetch/ramfwd_shut_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/ramfwd_shut_en
cat /sys/class/misc/prefetch/ramfwd_shut_en | grep register\(1\)

echo "L3T_STATIC: ramthr_merge_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/ramthr_merge_en
cat /sys/class/misc/prefetch/ramthr_merge_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/ramthr_merge_en
cat /sys/class/misc/prefetch/ramthr_merge_en | grep register\(1\)

echo "L3T_STATIC: ext_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/ext_en
cat /sys/class/misc/prefetch/ext_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/ext_en
cat /sys/class/misc/prefetch/ext_en | grep register\(1\)
