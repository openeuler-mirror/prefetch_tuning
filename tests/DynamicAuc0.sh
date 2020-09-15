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
# Create: 2020-09-09
# Author: Liuke (liuke20@gitee)

echo "L3T_DYNAMIC_AUC0: wrfull_create_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/wrfull_create_en
cat /sys/class/misc/prefetch/wrfull_create_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/wrfull_create_en
cat /sys/class/misc/prefetch/wrfull_create_en | grep register\(1\)

echo "L3T_DYNAMIC_AUC0: cleanunique_data_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/cleanunique_data_en
cat /sys/class/misc/prefetch/cleanunique_data_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/cleanunique_data_en
cat /sys/class/misc/prefetch/cleanunique_data_en | grep register\(1\)

echo "L3T_DYNAMIC_AUC0: lock_share_req_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/lock_share_req_en
cat /sys/class/misc/prefetch/lock_share_req_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/lock_share_req_en
cat /sys/class/misc/prefetch/lock_share_req_en | grep register\(1\)

echo "L3T_DYNAMIC_AUC0: atomic_monitor_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/atomic_monitor_en
cat /sys/class/misc/prefetch/atomic_monitor_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/atomic_monitor_en
cat /sys/class/misc/prefetch/atomic_monitor_en | grep register\(1\)
