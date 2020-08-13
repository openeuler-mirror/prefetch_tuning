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
# Create: 2020-07-22
# Author: Liuke (liuke20@gitee)

echo "totem_dual set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/totem_dual
    cat /sys/class/misc/prefetch/totem_dual | grep register\(1\)
done

echo "canum_sktvec set test (exp 0~15) :"
for i in {0..15}
do
    echo $i > /sys/class/misc/prefetch/canum_sktvec
    cat /sys/class/misc/prefetch/canum_sktvec | grep register\(1\)
done

echo "set totem_dual to 0 (exp 0, 15) :"
echo 0 > /sys/class/misc/prefetch/totem_dual
cat /sys/class/misc/prefetch/totem_dual | grep register\(1\)
cat /sys/class/misc/prefetch/canum_sktvec | grep register\(1\)

echo "set canum_sktvec to 2 (exp 0, 2) :"
echo 2 > /sys/class/misc/prefetch/canum_sktvec
cat /sys/class/misc/prefetch/totem_dual | grep register\(1\)
cat /sys/class/misc/prefetch/canum_sktvec | grep register\(1\)

echo "set canum_sktvec to 8 (exp 0, 8) :"
echo 8 > /sys/class/misc/prefetch/canum_sktvec
cat /sys/class/misc/prefetch/totem_dual | grep register\(1\)
cat /sys/class/misc/prefetch/canum_sktvec | grep register\(1\)

echo "set canum_sktvec to 11 (exp 0, 11) :"
echo 11 > /sys/class/misc/prefetch/canum_sktvec
cat /sys/class/misc/prefetch/totem_dual | grep register\(1\)
cat /sys/class/misc/prefetch/canum_sktvec | grep register\(1\)