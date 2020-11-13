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
# Create: 2020-11-04
# Author: Liuke (liuke20@gitee)
#         Wang Wuzhe (wangwuzhe@gitee)
# Description: This file is for testing bits setting and reading of register
#              L3T_STATIC_CTRL.

register='L3T_STATIC_CTRL'
bitname_array=('reg_nosnp_atomic_bypass_en' 'reg_ro_alloc_shut_en' \
'reg_wrfull_hit_shut_en' 'req_conflict_en' 'lower_power_en' \
'dataclean_shut_en' 'arb_flush_shut_en' 'pgnt_arb_exat_shut_en' \
'fast_exter_shut_en' 'fast_data_shut_en' 'pend_data_shut_en' \
'ramfwd_shut_en')
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
    echo "Current register bits value combination: ${count} ------------"
    for i in {0..11}
    do
        echo ${bit_arr[$i]} > /sys/class/misc/prefetch/${bitname_array[$i]}
        echo "${bitname_array[$i]}: $(cat /sys/class/misc/prefetch/${bitname_array[$i]} | grep register\(1\))"
    done
    let count++
done;done;done;done;done;done;done
done;done;done;done;done
echo "----------${register} combined test done------------"

