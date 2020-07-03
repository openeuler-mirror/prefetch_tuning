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
# Create: 2020-07-02
# Author: Liqiang (liqiang9102@gitee)

GREP=cpu\(1\)

echo "Policy set test, (exp:0~15):"
for i in {0..15}
do
        echo $i > /sys/class/misc/prefetch/policy
        cat /sys/class/misc/prefetch/policy | grep $GREP
done

echo "Set read_unique to 0, (exp:0,15):"
echo 0 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP
echo "Set policy to 8, (exp:0,8):"
echo 8 > /sys/class/misc/prefetch/policy
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set read_unique to 1, (exp:1,8):"
echo 1 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set policy to 10, (exp:1,10):"
echo 10 > /sys/class/misc/prefetch/policy
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP


echo "Set policy to 5, (exp:1, 5):"
echo 5 > /sys/class/misc/prefetch/policy
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set read_unique to 0, (0,5):"
echo 0 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set read_unique to 1, (1,5):"
echo 1 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP
