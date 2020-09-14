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
# Create: 2020-09-08
# Author: Liuke (liuke20@gitee)

echo "L3T_DYNAMIC_CTRL: reads_upgrade_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/reads_upgrade_en
cat /sys/class/misc/prefetch/reads_upgrade_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/reads_upgrade_en
cat /sys/class/misc/prefetch/reads_upgrade_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: rdmerge_pipe_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/rdmerge_pipe_en
cat /sys/class/misc/prefetch/rdmerge_pipe_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/rdmerge_pipe_en
cat /sys/class/misc/prefetch/rdmerge_pipe_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: spill_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/spill_en
cat /sys/class/misc/prefetch/spill_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/spill_en
cat /sys/class/misc/prefetch/spill_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: spill_shared_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/spill_shared_en
cat /sys/class/misc/prefetch/spill_shared_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/spill_shared_en
cat /sys/class/misc/prefetch/spill_shared_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: spill_instr_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/spill_instr_en
cat /sys/class/misc/prefetch/spill_instr_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/spill_instr_en
cat /sys/class/misc/prefetch/spill_instr_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: sqrdmerge_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/sqrdmerge_en
cat /sys/class/misc/prefetch/sqrdmerge_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/sqrdmerge_en
cat /sys/class/misc/prefetch/sqrdmerge_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: prefetch_drop_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/prefetch_drop_en
cat /sys/class/misc/prefetch/prefetch_drop_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/prefetch_drop_en
cat /sys/class/misc/prefetch/prefetch_drop_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: datapull_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/datapull_en
cat /sys/class/misc/prefetch/datapull_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/datapull_en
cat /sys/class/misc/prefetch/datapull_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: mkinvld_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/mkinvld_en
cat /sys/class/misc/prefetch/mkinvld_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/mkinvld_en
cat /sys/class/misc/prefetch/mkinvld_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: ramthr_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/ramthr_en
cat /sys/class/misc/prefetch/ramthr_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/ramthr_en
cat /sys/class/misc/prefetch/ramthr_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: rsperr_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/rsperr_en
cat /sys/class/misc/prefetch/rsperr_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/rsperr_en
cat /sys/class/misc/prefetch/rsperr_en | grep register\(1\)

echo "L3T_DYNAMIC_CTRL: force_cq_clk_en set test, (exp:0~1)"
echo 0 > /sys/class/misc/prefetch/force_cq_clk_en
cat /sys/class/misc/prefetch/force_cq_clk_en | grep register\(1\)
echo 1 > /sys/class/misc/prefetch/force_cq_clk_en
cat /sys/class/misc/prefetch/force_cq_clk_en | grep register\(1\)
