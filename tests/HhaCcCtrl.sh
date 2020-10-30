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
# Create: 2020-10-30
# Author: Wang Wuzhe (wangwuzhe@gitee)
# Description: This file is for testing bits setting and reading of register
#              HHA_CC_CTRL.

echo "HHA_CC_CTRL: reg_readoncesnp_dis set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_readoncesnp_dis
    cat /sys/class/misc/prefetch/reg_readoncesnp_dis | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_exter_stash set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_exter_stash
    cat /sys/class/misc/prefetch/reg_cc_exter_stash | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_writebacki_spill_full set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_writebacki_spill_full
    cat /sys/class/misc/prefetch/reg_cc_writebacki_spill_full | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_writeevicti_spill_full set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_writeevicti_spill_full
    cat /sys/class/misc/prefetch/reg_cc_writeevicti_spill_full | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_stashonce_full set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_stashonce_full
    cat /sys/class/misc/prefetch/reg_cc_stashonce_full | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_atomicstashl2 set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_atomicstashl2
    cat /sys/class/misc/prefetch/reg_cc_atomicstashl2 | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_atomicstashl3 set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_atomicstashl3
    cat /sys/class/misc/prefetch/reg_cc_atomicstashl3 | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_atomicstashclr set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_atomicstashclr
    cat /sys/class/misc/prefetch/reg_cc_atomicstashclr | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_cmo_snpme set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_cmo_snpme
    cat /sys/class/misc/prefetch/reg_cc_cmo_snpme | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_makee_change set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_makee_change
    cat /sys/class/misc/prefetch/reg_cc_makee_change | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_ioc_hitsca_dis set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_ioc_hitsca_dis
    cat /sys/class/misc/prefetch/reg_cc_ioc_hitsca_dis | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_passdirty set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_passdirty
    cat /sys/class/misc/prefetch/reg_cc_passdirty | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_snpdrop set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_snpdrop
    cat /sys/class/misc/prefetch/reg_cc_snpdrop | grep register\(1\)
done

echo "HHA_CC_CTRL: reg_cc_spill set test, (exp: 0 ~ 1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/reg_cc_spill
    cat /sys/class/misc/prefetch/reg_cc_spill | grep register\(1\)
done