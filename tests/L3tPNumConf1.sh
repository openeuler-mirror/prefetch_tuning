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
#              L3T_PRIME_NUM_CONFIG1.

echo "L3T_PRIME_NUM_CONFIG1: pime_timeout_num set test, (exp: 60000 ~ 65535)"
for i in {60000..65535}
do
    echo $i > /sys/class/misc/prefetch/pime_timeout_num
    cat /sys/class/misc/prefetch/pime_timeout_num | grep register\(1\)
done