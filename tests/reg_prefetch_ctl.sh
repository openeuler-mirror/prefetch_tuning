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
# Create: 2026-11-24
# Description: This file is for testing bit0 setting and reading of the
#              S3_1_c15_c6_4 register through the reg_prefetch_ctl
#              attribute of the prefetch_tuning_950 module, which acts
#              on all online CPUs captured at module load time.
#              Note: a sysfs read is limited to one page, so cat merges
#              consecutive CPUs with the same value into one line
#              (e.g. "cpu(0-327): 0."); a single CPU stays "cpu(1): 1.".

set -euo pipefail

DEV=/sys/class/misc/prefetch
CTL=${DEV}/reg_prefetch_ctl

fail()
{
    echo "FAIL: $1"
    exit 1
}

[ -e "${CTL}" ] || fail "${CTL} not found (is prefetch_tuning_950 loaded?)"

echo "Set reg_prefetch_ctl to 1, (exp: merged line(s) with value 1, e.g. cpu(0-327): 1.):"
echo 1 > "${CTL}"
cat "${CTL}" | grep ': 1\.' || fail "bit0 of the CPUs is not 1"

echo "Set reg_prefetch_ctl to 0, (exp: merged line(s) with value 0, e.g. cpu(0-327): 0.):"
echo 0 > "${CTL}"
cat "${CTL}" | grep ': 0\.' || fail "bit0 of the CPUs is not 0"

echo "PASS"
