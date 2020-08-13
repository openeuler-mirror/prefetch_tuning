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

echo "skt2_ta_cavec set test (exp 0~255) :"
for i in {0..255}
do
    echo $i > /sys/class/misc/prefetch/skt2_ta_cavec
    cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
done

echo "skt2_tb_cavec set test (exp 0~255) :"
for i in {0..255}
do
    echo $i > /sys/class/misc/prefetch/skt2_tb_cavec
    cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
done

echo "skt3_ta_cavec set test (exp 0~255) :"
for i in {0..255}
do
    echo $i > /sys/class/misc/prefetch/skt3_ta_cavec
    cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
done

echo "skt3_tb_cavec set test (exp 0~255) :"
for i in {0..255}
do
    echo $i > /sys/class/misc/prefetch/skt3_tb_cavec
    cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)
done

echo "set skt2_ta_cavec to 0: (exp 0, 255, 255, 255)"
echo 0 > /sys/class/misc/prefetch/skt2_ta_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt2_ta_cavec to 128: (exp 128, 255, 255, 255)"
echo 128 > /sys/class/misc/prefetch/skt2_ta_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt2_tb_cavec to 0: (exp 128, 0, 255, 255)"
echo 0 > /sys/class/misc/prefetch/skt2_tb_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt2_tb_cavec to 128: (exp 128, 128, 255, 255)"
echo 128 > /sys/class/misc/prefetch/skt2_tb_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt3_ta_cavec to 0: (exp 128, 128, 0, 255)"
echo 0 > /sys/class/misc/prefetch/skt3_ta_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt3_tb_cavec to 128: (exp 128, 128, 128, 255)"
echo 128 > /sys/class/misc/prefetch/skt3_ta_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt3_tb_cavec to 0: (exp 128, 128, 128, 0)"
echo 0 > /sys/class/misc/prefetch/skt3_tb_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)

echo "set skt3_tb_cavec to 128: (exp 128, 128, 128, 128)"
echo 128 > /sys/class/misc/prefetch/skt3_tb_cavec
cat /sys/class/misc/prefetch/skt2_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt2_tb_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_ta_cavec | grep register\(1\)
cat /sys/class/misc/prefetch/skt3_tb_cavec | grep register\(1\)