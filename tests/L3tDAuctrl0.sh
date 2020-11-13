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
# Description: This file is for testing bits setting and reading of register
#              L3T_DYNAMIC_AUCTRL0.

register='L3T_DYNAMIC_AUCTRL0'
bitname_array=('rdnosnp_nca_shut_en' 'wrfull_create_en' \
'cleanunique_data_en' 'lock_share_req_en' 'ddr_compress_opt_en' \
'atomic_monitor_en' 'snpsleep_en' 'prefetchtgt_en')
checkflag=1

for bit in ${bitname_array[*]}
do
    echo "${register}: ${bit} set test, expected 0~1"
    for i in {0..1}
    do 
        echo $i > /sys/class/misc/prefetch/${bit}
        cat /sys/class/misc/prefetch/${bit} | grep register\(1\)
        if [ "register\(1\): ${i}.\n" != "$(cat /sys/class/misc/prefetch/${bit} | grep register\(1\))" ]
        then
            checkflag=0
        fi
    done
done

count=0
bit_arr=(1 1 1 1 1 \
1 1 1 1 1 \
1 1 1 1 1 1)
echo "------------${register} combined test--------------"
for a in {0..1}; do bit_arr[0]=$a
for b in {0..1}; do bit_arr[1]=$b
for c in {0..1}; do bit_arr[2]=$c
for d in {0..1}; do bit_arr[3]=$d
for e in {0..1}; do bit_arr[4]=$e
for f in {0..1}; do bit_arr[5]=$f
for g in {0..1}; do bit_arr[6]=$g
for h in {0..1}; do bit_arr[7]=$h
    echo "Current register bits value combination: ${count} ------------"
    for i in {0..7}
    do
        echo ${bit_arr[$i]} > /sys/class/misc/prefetch/${bitname_array[$i]}
        echo "${bitname_array[$i]}: $(cat /sys/class/misc/prefetch/${bitname_array[$i]} | grep register\(1\))"
    done
    let count++
done;done;done;done;done;done;done;done
echo "----------${register} combined test done------------"
