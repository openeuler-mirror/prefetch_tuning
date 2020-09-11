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
