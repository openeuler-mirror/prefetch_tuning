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
# Create: 2020-08-06
# Author: Liuke (liuke20@gitee)
#         Wang Wuzhe (wangwuzhe@gitee)

echo "reg_ctrl_spillprefetch set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_spillprefetch
    cat /sys/class/misc/prefetch/reg_ctrl_spillprefetch | grep register\(1\)
done

echo "reg_ctrl_mpamen set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_mpamen
    cat /sys/class/misc/prefetch/reg_ctrl_mpamen | grep register\(1\)
done

echo "reg_ctrl_mpamqos set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_mpamqos
    cat /sys/class/misc/prefetch/reg_ctrl_mpamqos | grep register\(1\)
done

echo "reg_ctrl_poison set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_poison
    cat /sys/class/misc/prefetch/reg_ctrl_poison | grep register\(1\)
done

echo "reg_ctrl_compress_spec set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_compress_spec
    cat /sys/class/misc/prefetch/reg_ctrl_compress_spec | grep register\(1\)
done

echo "reg_ctrl_data_reside set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_data_reside
    cat /sys/class/misc/prefetch/reg_ctrl_data_reside | grep register\(1\)
done

echo "reg_ctrl_writeevict_drop set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_writeevict_drop
    cat /sys/class/misc/prefetch/reg_ctrl_writeevict_drop | grep register\(1\)
done

echo "reg_ctrl_prefetch_drop set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_prefetch_drop
    cat /sys/class/misc/prefetch/reg_ctrl_prefetch_drop | grep register\(1\)
done

echo "reg_ctrl_dmcassign set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_dmcassign
    cat /sys/class/misc/prefetch/reg_ctrl_dmcassign | grep register\(1\)
done

echo "reg_ctrl_rdatabyp set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_rdatabyp
    cat /sys/class/misc/prefetch/reg_ctrl_rdatabyp | grep register\(1\)
done

echo "reg_ctrl_excl_clear_dis set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_excl_clear_dis
    cat /sys/class/misc/prefetch/reg_ctrl_excl_clear_dis | grep register\(1\)
done

echo "reg_ctrl_excl_eventen set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_excl_eventen
    cat /sys/class/misc/prefetch/reg_ctrl_excl_eventen | grep register\(1\)
done

echo "reg_ctrl_eccen set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_ctrl_eccen
    cat /sys/class/misc/prefetch/reg_ctrl_eccen | grep register\(1\)
done

echo "set reg_ctrl_prefetch_drop to be 0: exp(0, 1, 1)"
echo 0 > /sys/class/misc/prefetch/reg_ctrl_prefetch_drop
cat /sys/class/misc/prefetch/reg_ctrl_prefetch_drop | grep register\(1\)
cat /sys/class/misc/prefetch/reg_ctrl_dmcassign | grep register\(1\)
cat /sys/class/misc/prefetch/reg_ctrl_rdatabyp | grep register\(1\)

echo "set reg_ctrl_dmcassign to be 0: exp(0, 0, 1)"
echo 0 > /sys/class/misc/prefetch/reg_ctrl_dmcassign
cat /sys/class/misc/prefetch/reg_ctrl_prefetch_drop | grep register\(1\)
cat /sys/class/misc/prefetch/reg_ctrl_dmcassign | grep register\(1\)
cat /sys/class/misc/prefetch/reg_ctrl_rdatabyp | grep register\(1\)

echo "set reg_ctrl_rdatabyp to be 0: exp(0, 0, 0)"
echo 0 > /sys/class/misc/prefetch/reg_ctrl_rdatabyp
cat /sys/class/misc/prefetch/reg_ctrl_prefetch_drop | grep register\(1\)
cat /sys/class/misc/prefetch/reg_ctrl_dmcassign | grep register\(1\)
cat /sys/class/misc/prefetch/reg_ctrl_rdatabyp | grep register\(1\)

echo "reg_dir_replace_alg set text (exp 0, 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_dir_replace_alg
    cat /sys/class/misc/prefetch/reg_dir_replace_alg | grep register\(1\)
done
