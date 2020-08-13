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
# Create: 2020-07-13
# Author: Liuke (liuke20@gitee)

echo "L3T_STATIC_CTRL: ramswap set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/ramswap
    cat /sys/class/misc/prefetch/ramswap | grep register\(1\)
done

echo "L3T_DYNAMIC_CTRL: iocapacity_limit set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/iocapacity_limit
    cat /sys/class/misc/prefetch/iocapacity_limit | grep register\(1\)
done

echo "L3T_DYNAMIC_CTRL: sqmerge set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/sqmerge
    cat /sys/class/misc/prefetch/sqmerge | grep register\(1\)
done
'''
echo "L3T_DYNAMIC_CTRL: prefetch_drop_hha set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prefetch_drop_hha
    cat /sys/class/misc/prefetch/prefetch_drop_hha | grep register\(1\)
done
'''
echo "L3T_DYNAMIC_CTRL: tag_rep_alg set test, (exp:0~3)"
for i in {0..3}
do
    echo $i > /sys/class/misc/prefetch/tag_rep_alg
    cat /sys/class/misc/prefetch/tag_rep_alg | grep register\(1\)
done

echo "L3T_DYNAMIC_CTRL: rdmerge_upgrade_en set test, (exp:0~1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/rdmerge_upgrade_en
    cat /sys/class/misc/prefetch/rdmerge_upgrade_en | grep register\(1\)
done

echo "L3T_DYNAMIC_CTRL: set iocapacity_limit to 0, (exp:0, 1, 1, 3, 1)"
echo 0 > /sys/class/misc/prefetch/iocapacity_limit
cat /sys/class/misc/prefetch/iocapacity_limit | grep register\(1\)
cat /sys/class/misc/prefetch/sqmerge | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_drop_hha | grep register\(1\)
cat /sys/class/misc/prefetch/tag_rep_alg | grep register\(1\)
cat /sys/class/misc/prefetch/rdmerge_upgrade_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: set sqmerge to 0, (exp:0, 0, 1, 3, 1)"
echo 0 > /sys/class/misc/prefetch/sqmerge
cat /sys/class/misc/prefetch/iocapacity_limit | grep register\(1\)
cat /sys/class/misc/prefetch/sqmerge | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_drop_hha | grep register\(1\)
cat /sys/class/misc/prefetch/tag_rep_alg | grep register\(1\)
cat /sys/class/misc/prefetch/rdmerge_upgrade_en | grep register\(1\)
'''
echo "L3T_DYNAMIC_CTRL: set prefetch_drop_hha to 0, (exp:0, 0, 0, 3, 1)"
echo 0 > /sys/class/misc/prefetch/prefetch_drop_hha
cat /sys/class/misc/prefetch/iocapacity_limit | grep register\(1\)
cat /sys/class/misc/prefetch/sqmerge | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_drop_hha | grep register\(1\)
cat /sys/class/misc/prefetch/tag_rep_alg | grep register\(1\)
cat /sys/class/misc/prefetch/rdmerge_upgrade_en | grep register\(1\)
'''
echo "L3T_DYNAMIC_CTRL: set tag_rep_alg to 2, (exp:0, 0, 0, 2, 1)"
echo 2 > /sys/class/misc/prefetch/tag_rep_alg
cat /sys/class/misc/prefetch/iocapacity_limit | grep register\(1\)
cat /sys/class/misc/prefetch/sqmerge | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_drop_hha | grep register\(1\)
cat /sys/class/misc/prefetch/tag_rep_alg | grep register\(1\)
cat /sys/class/misc/prefetch/rdmerge_upgrade_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: set rdmerge_upgrade_en to 0, (exp:0, 0, 0, 2, 0)"
echo 0 > /sys/class/misc/prefetch/rdmerge_upgrade_en
cat /sys/class/misc/prefetch/iocapacity_limit | grep register\(1\)
cat /sys/class/misc/prefetch/sqmerge | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_drop_hha | grep register\(1\)
cat /sys/class/misc/prefetch/tag_rep_alg | grep register\(1\)
cat /sys/class/misc/prefetch/rdmerge_upgrade_en | grep register\(1\)