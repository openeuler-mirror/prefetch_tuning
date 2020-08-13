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

echo "prefetch_comb set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prefetch_comb
    cat /sys/class/misc/prefetch/prefetch_comb | grep register\(1\)
done

echo "reg_funcdis_comb set test (exp 0,1) :"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_funcdis_comb
    cat /sys/class/misc/prefetch/reg_funcdis_comb | grep register\(1\)
done

echo "set prefetch_comb to be 0: exp(0,1)"
echo 0 > /sys/class/misc/prefetch/prefetch_comb
cat /sys/class/misc/prefetch/prefetch_comb | grep register\(1\)
cat /sys/class/misc/prefetch/reg_funcdis_comb | grep register\(1\)

echo "set reg_funcdis_comb to be 0: exp(0,0)"
echo 0 > /sys/class/misc/prefetch/reg_funcdis_comb
cat /sys/class/misc/prefetch/prefetch_comb | grep register\(1\)
cat /sys/class/misc/prefetch/reg_funcdis_comb | grep register\(1\)
