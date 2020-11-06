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
# Create: 2020-10-30
# Author: Wang Wuzhe (wangwuzhe@gitee)
# Description: This file is for testing bits setting and reading of register
#              HHA_DDR_LEVEL.

echo "HHA_DDR_LEVEL: reg_prefetchtgt_outstanding set test, (exp: 0 ~ 127)"
for i in {0..127}
do
    echo $i > /sys/class/misc/prefetch/reg_prefetchtgt_outstanding
    cat /sys/class/misc/prefetch/reg_prefetchtgt_outstanding | grep register\(1\)
done

echo "HHA_DDR_LEVEL: reg_prefetchtgt_level set test, (exp: 0 ~ 127)"
for i in {0..127}
do
    echo $i > /sys/class/misc/prefetch/reg_prefetchtgt_level
    cat /sys/class/misc/prefetch/reg_prefetchtgt_level | grep register\(1\)
done

echo "HHA_DDR_LEVEL: reg_spec_rd_level set test, (exp: 0 ~ 127)"
for i in {0..127}
do
    echo $i > /sys/class/misc/prefetch/reg_spec_rd_level
    cat /sys/class/misc/prefetch/reg_spec_rd_level | grep register\(1\)
done

echo "HHA_DDR_LEVEL: reg_drop_level set test, (exp: 0 ~ 127)"
for i in {0..127}
do
    echo $i > /sys/class/misc/prefetch/reg_drop_level
    cat /sys/class/misc/prefetch/reg_drop_level | grep register\(1\)
done

echo "HHA_DDR_LEVEL combined test"
for i in {0..127}
do
    echo $i > /sys/class/misc/prefetch/reg_prefetchtgt_outstanding
    for j in {0..127}
    do
        echo $j > /sys/class/misc/prefetch/reg_prefetchtgt_level
        for k in {0..127}
        do
            echo $k > /sys/class/misc/prefetch/reg_spec_rd_level
            for n in {0..127}
            do
                echo $n > /sys/class/misc/prefetch/reg_drop_level
            done
        done
        echo "---Round 2: ${j} done"
    done
    echo "---------Round 1: ${i} done"  
done
echo "--------------HHA_DDR_LEVEL combined test done----------------"