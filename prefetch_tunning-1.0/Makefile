# SPDX-License-Identifier: GPL-2.0-only
# Copyright(c) 2019 Huawei Technologies Co., Ltd. 2019. All rights reserved.

BUILD ?= /lib/modules/`uname -r`/build
obj-m := prefetch_tunning.o
EXTRA_CFLAGS += -Wall -Werror

.PHONY : all
all:
	$(MAKE) -C $(BUILD) M=$(PWD) modules

.PHONY : clean
clean:
	$(MAKE) -C $(BUILD) M=$(PWD) clean
