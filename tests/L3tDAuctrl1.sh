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
#         Wang Wuzhe (wangwuzhe@gitee)
# Description: This file is for testing bits setting and reading of register
#              L3T_DYNAMIC_AUCTRL1.

register='L3T_DYNAMIC_AUCTRL1'
bitname_array=('sequence_shape_en' 'mpam_portion_en' \
'mpam_capacity_en' 'eccchk_en' 'refill_1024_relax_en' \
'lookup_thr_en' 'snpunique_stash_en' 'prime_timeout_mask_en' \
'prime_sleep_mask_en' 'prime_extend_mask_en' 'force_intl_allocate_fail' \
'cpu_write_unique_stream_en' 'cpu_pf_lqos_en' 'cpu_vic_lqos_en' \
'prime_excl_mask_en' 'prime_drop_mask_en' 'prime_home_mask_en' \
'refillsize_com_ada_en' 'refillsize_pre_ada_en' 'sequence_opt_en')
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
bit_arr=(
1 1 1 1 1 \
1 1 1 1 1 \
1 1 1 1 1 \
1 1 1 1 1 \
1 1 1 1 1 \
1 1 1 1 1 \
1 1 1 1 1 \
1 1\
)
echo "------------${register} combined test--------------"
for a0 in {0..1}; do bit_arr[0]=$a0
for a1 in {0..1}; do bit_arr[1]=$a1
for a2 in {0..1}; do bit_arr[2]=$a2
for a3 in {0..1}; do bit_arr[3]=$a3
for a4 in {0..1}; do bit_arr[4]=$a4
for a5 in {0..1}; do bit_arr[5]=$a5
for a6 in {0..1}; do bit_arr[6]=$a6
for a7 in {0..1}; do bit_arr[7]=$a7
for a8 in {0..1}; do bit_arr[8]=$a8
for a9 in {0..1}; do bit_arr[9]=$a9
for a10 in {0..1}; do bit_arr[10]=$a10
for a11 in {0..1}; do bit_arr[11]=$a11
for a12 in {0..1}; do bit_arr[12]=$a12
for a13 in {0..1}; do bit_arr[13]=$a13
for a14 in {0..1}; do bit_arr[14]=$a14
for a15 in {0..1}; do bit_arr[15]=$a15
for a16 in {0..1}; do bit_arr[16]=$a16
for a17 in {0..1}; do bit_arr[17]=$a17
for a18 in {0..1}; do bit_arr[18]=$a18
for a19 in {0..1}; do bit_arr[19]=$a19
    echo "Current register bits value combination: ${count} ------------"
    for i in {0..19}
    do
        echo ${bit_arr[$i]} > /sys/class/misc/prefetch/${bitname_array[$i]}
        echo "${bitname_array[$i]}: $(cat /sys/class/misc/prefetch/${bitname_array[$i]} | grep register\(1\))"
    done
    let count++
done;done;done;done;done;done;done
done;done;done;done;done;done;done
done;done;done;done;done;done
echo "----------${register} combined test done------------"
