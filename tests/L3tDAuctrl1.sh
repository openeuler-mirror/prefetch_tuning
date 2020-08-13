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
# Create: 2020-07-21
# Author: Liuke (liuke20@gitee)

echo "L3T_DYNAMIC_AUCTRL1: bankintl_stagger set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/bankintl_stagger
    cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: cpu_pf_lqos_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/cpu_pf_lqos_en
    cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: prime_drop_mask set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prime_drop_mask
    cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: refillsize_com_ada_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/refillsize_com_ada_en
    cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: refillsize_pre_ada_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/refillsize_pre_ada_en
    cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: sequence_opt set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/sequence_opt
    cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: bankintlv set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/bankintlv
    cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)
done

echo "L3T_DYNAMIC_AUCTRL1: set bankintl_stagger to 0, (exp:0,1,1,1,1,1,1)"
echo 0 > /sys/class/misc/prefetch/bankintl_stagger
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)

echo "L3T_DYNAMIC_AUCTRL1: set cpu_pf_lqos_en to 0, (exp:0,0,1,1,1,1,1)"
echo 0 > /sys/class/misc/prefetch/cpu_pf_lqos_en
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)

echo "L3T_DYNAMIC_AUCTRL1: set prime_drop_mask to 0, (exp:0,0,0,1,1,1,1)"
echo 0 > /sys/class/misc/prefetch/prime_drop_mask
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)

echo "L3T_DYNAMIC_AUCTRL1: set refillsize_com_ada_en to 0, (exp:0,0,0,0,1,1,1)"
echo 0 > /sys/class/misc/prefetch/refillsize_com_ada_en
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)

echo "L3T_DYNAMIC_AUCTRL1: set refillsize_pre_ada_en to 0, (exp:0,0,0,0,0,1,1)"
echo 0 > /sys/class/misc/prefetch/refillsize_pre_ada_en
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)

echo "L3T_DYNAMIC_AUCTRL1: set sequence_opt to 0, (exp:0,0,0,0,0,0,1)"
echo 0 > /sys/class/misc/prefetch/sequence_opt
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)

echo "L3T_DYNAMIC_AUCTRL1: set bankintlv to 0, (exp:0,0,0,0,0,0,0)"
echo 0 > /sys/class/misc/prefetch/bankintlv
cat /sys/class/misc/prefetch/bankintl_stagger | grep register\(1\)
cat /sys/class/misc/prefetch/cpu_pf_lqos_en | grep register\(1\)
cat /sys/class/misc/prefetch/prime_drop_mask | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_com_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/refillsize_pre_ada_en | grep register\(1\)
cat /sys/class/misc/prefetch/sequence_opt | grep register\(1\)
cat /sys/class/misc/prefetch/bankintlv | grep register\(1\)