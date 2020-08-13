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
# Create: 2020-07-21
# Author: Liuke (liuke20@gitee)

<<<<<<< HEAD
=======
echo "L3T_PREFETCH: prefetch_overide_level set test, (exp:0~15)"
for i in {0..15}
do
    echo $i > /sys/class/misc/prefetch/prefetch_overide_level
    cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
done

>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
echo "L3T_PREFETCH: prefetch_utl_ddr set test, (exp:0~3)"
for i in {0..3}
do
    echo $i > /sys/class/misc/prefetch/prefetch_utl_ddr
    cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
done

echo "L3T_PREFETCH: prefetch_utl_ddr_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prefetch_utl_ddr_en
    cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
done

echo "L3T_PREFETCH: prefetch_utl_l3t set test, (exp:0~3)"
for i in {0..3}
do
    echo $i > /sys/class/misc/prefetch/prefetch_utl_l3t
    cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
done

echo "L3T_PREFETCH: prefetch_utl_l3t_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prefetch_utl_l3t_en
    cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
done

<<<<<<< HEAD
=======
echo "L3T_PREFETCH: prefetch_vague_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prefetch_vague_en
    cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
done

echo "L3T_PREFETCH: prefetch_core_en set test, (exp:0~15)"
for i in {0..15}
do
    echo $i > /sys/class/misc/prefetch/prefetch_core_en
    cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
done

echo "L3T_PREFETCH: prefetch_match_en set test, (exp:0,1)"
for i in {0..1}
do
    echo $i > /sys/class/misc/prefetch/prefetch_match_en
    cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
done

>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
echo "L3T_PREFETCH: prefetch_start_level set test, (exp:0~15)"
for i in {0..15}
do
    echo $i > /sys/class/misc/prefetch/prefetch_start_level
    cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)
done

<<<<<<< HEAD
echo "L3T_PREFETCH: set prefetch_utl_ddr to 0, (exp:0,1,3,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_ddr
=======
echo "L3T_PREFETCH: set prefetch_overide_level to 10, (exp:10,3,1,3,1,1,15,1,15)"
echo 10 > /sys/class/misc/prefetch/prefetch_overide_level
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_ddr to 2, (exp:2,1,3,1,15)"
echo 2 > /sys/class/misc/prefetch/prefetch_utl_ddr
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_ddr to 0, (exp:10,0,1,3,1,1,15,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_ddr
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_ddr_en to 0, (exp:2,0,3,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_ddr_en
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_ddr_en to 0, (exp:10,0,0,3,1,1,15,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_ddr_en
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_l3t to 0, (exp:2,0,0,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_l3t
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_l3t to 0, (exp:10,0,0,0,1,1,15,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_l3t
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_l3t to 2, (exp:2,0,2,1,15)"
echo 2 > /sys/class/misc/prefetch/prefetch_utl_l3t
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_l3t_en to 0, (exp:10,0,0,0,0,1,15,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_l3t_en
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_utl_l3t_en to 0, (exp:2,0,2,0,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_utl_l3t_en
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_vague_en to 0, (exp:10,0,0,0,0,0,15,1,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_vague_en
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_start_level to 0, (exp:2,0,2,0,0)"
echo 0 > /sys/class/misc/prefetch/prefetch_start_level
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_core_en to 10, (exp:10,0,0,0,0,0,10,1,15)"
echo 10 > /sys/class/misc/prefetch/prefetch_core_en
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_start_level to 4, (exp:2,0,2,0,4)"
echo 4 > /sys/class/misc/prefetch/prefetch_start_level
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_match_en to 10, (exp:10,0,0,0,0,0,10,0,15)"
echo 0 > /sys/class/misc/prefetch/prefetch_match_en
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_start_level to 10, (exp:2,0,2,0,10)"
echo 10 > /sys/class/misc/prefetch/prefetch_start_level
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_start_level to 10, (exp:10,0,0,0,0,0,10,0,10)"
echo 10 > /sys/class/misc/prefetch/prefetch_start_level
cat /sys/class/misc/prefetch/prefetch_overide_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
<<<<<<< HEAD
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)

echo "L3T_PREFETCH: set prefetch_start_level to 15, (exp:2,0,2,0,15)"
echo 15 > /sys/class/misc/prefetch/prefetch_start_level
cat /sys/class/misc/prefetch/prefetch_utl_ddr | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_ddr_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_utl_l3t_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)
=======
cat /sys/class/misc/prefetch/prefetch_vague_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_core_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_match_en | grep register\(1\)
cat /sys/class/misc/prefetch/prefetch_start_level | grep register\(1\)
>>>>>>> 6acb3957d70036f30447fb21f5d76af392855196
