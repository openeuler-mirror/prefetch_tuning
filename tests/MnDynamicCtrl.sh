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
# Create: 2020-10-28
# Author: Wang Wuzhe (wangwuzhe@gitee)
# Description: This file is for testing bits setting and reading of register
#              MN_DYNAMIC_CTRL.

echo "MN_DYNAMIC_CTRL: dvmsnp_outstanding set test, (exp: 0 ~ 5)"
for i in {0..5}
do
    echo $i > /sys/class/misc/prefetch/dvmsnp_outstanding
    cat /sys/class/misc/prefetch/dvmsnp_outstanding | grep register\(1\)
done

echo "MN_DYNAMIC_CTRL: dvmreq_outstanding set test, (exp: 0 ~ 9)"
for i in {0..9}
do
    echo $i > /sys/class/misc/prefetch/dvmreq_outstanding
    cat /sys/class/misc/prefetch/dvmreq_outstanding | grep register\(1\)
done

echo "MN_DYNAMIC_CTRL: dvmsnp_perf_en set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/dvmsnp_perf_en
    cat /sys/class/misc/prefetch/dvmsnp_perf_en | grep register\(1\)
done

echo "MN_DYNAMIC_CTRL: dvmreq_perf_en set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/dvmreq_perf_en
    cat /sys/class/misc/prefetch/dvmreq_perf_en | grep register\(1\)
done

echo "MN_DYNAMIC_CTRL combined test"
for i in {0..15}
do
    echo "-------------------dvmsnp_outstanding"
    echo $i > /sys/class/misc/prefetch/dvmsnp_outstanding
    cat /sys/class/misc/prefetch/dvmsnp_outstanding | grep register\(1\)
    for j in {0..31}
    do
        echo "----------dvmreq_outstanding"
        echo $j > /sys/class/misc/prefetch/dvmreq_outstanding
        cat /sys/class/misc/prefetch/dvmreq_outstanding | grep register\(1\)
        for k in {0..1}
        do
            echo "------dvmsnp_perf_en"
            echo $k > /sys/class/misc/prefetch/dvmsnp_perf_en
            cat /sys/class/misc/prefetch/dvmsnp_perf_en | grep register\(1\)
            for n in {0..1}
            do
                echo "----dvmreq_perf_en"
                echo $n > /sys/class/misc/prefetch/dvmreq_perf_en
                cat /sys/class/misc/prefetch/dvmreq_perf_en | grep register\(1\)
            done
        done
    done
done