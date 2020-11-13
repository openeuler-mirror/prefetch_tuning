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
#              L3T_PREFETCH.

register='L3T_PREFETCH'
bitname_array=('prefetch_clr_level' 'prefetch_overide_level' \
'prefetch_utl_ddr' 'prefetch_utl_ddr_en' 'prefetch_utl_l3t' \
'prefetch_utl_l3t_en' 'prefetch_vague_en' 'prefetch_core_en' \
'prefetch_match_en' 'prefetch_start_level')
bit_sup=(255 15 3 1 3 1 1 15 1 31)
checkflag=1
bit_cnt=0

for bit in ${bitname_array[*]}
do
    echo "${register}: ${bit} set test, expected 0 ~ ${bit_sup[$bit_cnt]}"
    for i in $(seq 0 ${bit_sup[$bit_cnt]})
    do 
        echo $i > /sys/class/misc/prefetch/${bit}
        cat /sys/class/misc/prefetch/${bit} | grep register\(1\)
        if [ "register\(1\): ${i}.\n" != "$(cat /sys/class/misc/prefetch/${bit} | grep register\(1\))" ]
        then
            checkflag=0
        fi
    done
    let bit_cnt++
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
for a0 in $(seq 0 ${bit_sup[0]}); do bit_arr[0]=$a0
for a1 in $(seq 0 ${bit_sup[1]}); do bit_arr[1]=$a1
for a2 in $(seq 0 ${bit_sup[2]}); do bit_arr[2]=$a2
for a3 in $(seq 0 ${bit_sup[3]}); do bit_arr[3]=$a3
for a4 in $(seq 0 ${bit_sup[4]}); do bit_arr[4]=$a4
for a5 in $(seq 0 ${bit_sup[5]}); do bit_arr[5]=$a5
for a6 in $(seq 0 ${bit_sup[6]}); do bit_arr[6]=$a6
for a7 in $(seq 0 ${bit_sup[7]}); do bit_arr[7]=$a7
for a8 in $(seq 0 ${bit_sup[8]}); do bit_arr[8]=$a8
for a9 in $(seq 0 ${bit_sup[9]}); do bit_arr[9]=$a9
    if [ `expr $count % 10000` == 0 ]; then 
        echo "Current register bits value combination: ${count} ------------"
    fi
    
    for i in {0..9}
    do
        echo ${bit_arr[$i]} > /sys/class/misc/prefetch/${bitname_array[$i]}
        #echo "${bitname_array[$i]}: $(cat /sys/class/misc/prefetch/${bitname_array[$i]} | grep register\(1\))"
    done
    let count++
done;done;done;done;done;done;done
done;done;done
echo "----------${register} combined test done------------"


