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
# Author: Wang Wuzhe (wangwuzhe@gitee)
# Description: This file is for testing bits setting and reading of register
#              HHA_DIR_CTRL.

register='HHA_DIR_CTRL'
bitname_array=('reg_precisionsnp_dis' 'reg_notonly_excl' \
'reg_buffer_share_dis' 'reg_miss_allindex' 'reg_miss_cbackth' \
'reg_miss_normalth' 'reg_miss_tosdir' 'reg_entry_except' \
'reg_dir_precision' 'reg_dir_replace_alg')
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

echo "${register}: ${bitname_array[10]} set test, expected 0~3"
for i in {0..3}
do
    echo $i > /sys/class/misc/prefetch/${bitname_array[10]}
    cat /sys/class/misc/prefetch/${bitname_array[10]} | grep register\(1\)
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
for a9 in {0..3}; do bit_arr[9]=$a9
    echo "Current register bits value combination: ${count} ------------"
    for i in {0..9}
    do
        echo ${bit_arr[$i]} > /sys/class/misc/prefetch/${bitname_array[$i]}
        echo "${bitname_array[$i]}: $(cat /sys/class/misc/prefetch/${bitname_array[$i]} | grep register\(1\))"
    done
    let count++
done;done;done;done;done;done;done
done;done;done
echo "----------${register} combined test done------------"

