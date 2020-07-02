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

echo "Set read_unique to 0, (exp:close,15):"
echo 0 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP
echo "Set policy to 8, (exp:close,8):"
echo 8 > /sys/class/misc/prefetch/policy
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set read_unique to 1, (exp:open,8):"
echo 1 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set policy to 10, (exp:open,10):"
echo 10 > /sys/class/misc/prefetch/policy
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP


echo "Set policy to 5, (exp:open, 5):"
echo 5 > /sys/class/misc/prefetch/policy
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set read_unique to 0, (close,5):"
echo 0 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP

echo "Set read_unique to 1, (open,5):"
echo 1 > /sys/class/misc/prefetch/read_unique
cat /sys/class/misc/prefetch/read_unique | grep $GREP
cat /sys/class/misc/prefetch/policy | grep $GREP
