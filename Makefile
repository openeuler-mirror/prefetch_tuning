# SPDX-License-Identifier: GPL-2.0-only
# Copyright(c) 2019 Huawei Technologies Co., Ltd. 2019. All rights reserved.

BUILD ?= /lib/modules/`uname -r`/build
obj-m := prefetch_tune.o
prefetch_tune-objs := prefetch_reg.o prefetch_tuning.o
EXTRA_CFLAGS += -Wall -Werror -I./

.PHONY : all
all:
	$(MAKE) -C $(BUILD) M=$(PWD) modules

.PHONY : clean
clean:
	$(MAKE) -C $(BUILD) M=$(PWD) clean
