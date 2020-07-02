#include <linux/module.h>
#include <linux/cpu.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/cpumask.h>
#include <linux/miscdevice.h>
#include "prefetch_mod.h"

#ifdef CONFIG_ARCH_HISI
#define PREFETCH_POLICY_MAX 15
static cfg_t prefetch_cfg[] = {
	[0] = {.cpuprefctrl_el1 = 0x112f8127f,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[1] = {.cpuprefctrl_el1 = 0x112f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[2] = {.cpuprefctrl_el1 = 0x112f81254,
			.adps_lld_ddr_el1 = 0x4d34a200,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[3] = {.cpuprefctrl_el1 = 0xb52f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[4] = {.cpuprefctrl_el1 = 0x112f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[5] = {.cpuprefctrl_el1 = 0x3012f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[6] = {.cpuprefctrl_el1 = 0x3012f81254,
			.adps_lld_ddr_el1 = 0x4d142000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[7] = {.cpuprefctrl_el1 = 0x3012f81254,
			.adps_lld_ddr_el1 = 0x4d142000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x4d145100},
	[8] = {.cpuprefctrl_el1 = 0x4112f81254,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[9] = {.cpuprefctrl_el1 = 0x112f81260,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[10] = {.cpuprefctrl_el1 = 0x112f81260,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x658e5700},
	[11] = {.cpuprefctrl_el1 = 0x3412f81254,
			.adps_lld_ddr_el1 = 0x4d142000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[12] = {.cpuprefctrl_el1 = 0x3412F81260,
			.adps_lld_ddr_el1 = 0x4d12000,
			.adpp_l1v_mop_el1 = 0x29080082a880,
			.adps_lld_l3_el1 = 0x65965700},
	[13] = {.cpuprefctrl_el1 = 0x112f81240,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[14] = {.cpuprefctrl_el1 = 0x112f81240,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x69154332a840,
			.adps_lld_l3_el1 = 0x65965700},
	[15] = {.cpuprefctrl_el1 = 0x80110f81380,
			.adps_lld_ddr_el1 = 0x6554a000,
			.adpp_l1v_mop_el1 = 0x29154332a840,
			.adps_lld_l3_el1 = 0x65965700},
};

void set_prefetch(void* dummy)
{
    cfg_t *cfg = NULL;
    int policy = *(int *)dummy;
    unsigned long read_uniq = read_sysreg(S3_1_c15_c6_4);
    if (policy < 0 || policy > PREFETCH_POLICY_MAX)
        return;
    cfg = &prefetch_cfg[policy];
    read_uniq &= CACHE_READUNIQ_CTRL;
    write_sysreg(cfg->cpuprefctrl_el1 | read_uniq, S3_1_c15_c6_4);
    write_sysreg(cfg->adps_lld_ddr_el1, S3_1_c15_c7_1);
    write_sysreg(cfg->adpp_l1v_mop_el1, S3_1_c15_c6_5);
    write_sysreg(cfg->adps_lld_l3_el1, S3_1_c15_c7_0);
    return;
}

void get_prefetch(void* dummy)
{
    cfg_t *pcfg = this_cpu_ptr((cfg_t __percpu *)dummy);
    pcfg->cpuprefctrl_el1 = read_sysreg(S3_1_c15_c6_4);
    pcfg->adps_lld_ddr_el1 = read_sysreg(S3_1_c15_c7_1);
    pcfg->adpp_l1v_mop_el1 = read_sysreg(S3_1_c15_c6_5);
    pcfg->adps_lld_l3_el1 = read_sysreg(S3_1_c15_c7_0);
    
    /* Ignore READUNIQ bit */
    pcfg->cpuprefctrl_el1 &= ~(CACHE_READUNIQ_CTRL);
    return;
}

void read_unique_set(void *dummy)
{
    int *value = (int *)dummy;
    if (*value == ENABLE)
        sysreg_clear_set(S3_1_c15_c6_4, 0, CACHE_READUNIQ_CTRL);
    else if (*value == DISABLE)
        sysreg_clear_set(S3_1_c15_c6_4, CACHE_READUNIQ_CTRL, 0);
    
    return;
}

void read_unique_get(void *dummy)
{
    int *value = this_cpu_ptr((int __percpu *)dummy);
    u64 reg_value = read_sysreg(S3_1_c15_c6_4);
    *value = (reg_value >> CACHE_READUNIQ_OFFSET) & 0x1;
    return;
}
#else
#define PREFETCH_POLICY_MAX 0
static cfg_t prefetch_cfg[] = {};

void set_prefetch(void* dummy)
{
    return;
}

void get_prefetch(void* dummy)
{
    return;
}

void read_unique_set(void *dummy)
{
    return;
}

void read_unique_get(void *dummy)
{
    return;
}
#endif

int prefetch_policy_num(void)
{
    return ARRAY_SIZE(prefetch_cfg);
}

cfg_t *prefetch_policy(int policy)
{
    if (policy < 0 || policy > PREFETCH_POLICY_MAX)
        return NULL;
    return &prefetch_cfg[policy];
}

void reset_prefetch(void* dummy)
{
    cfg_t *pcfg = this_cpu_ptr((cfg_t __percpu *)dummy);
	set_prefetch(pcfg);
}

MODULE_LICENSE("GPL");
