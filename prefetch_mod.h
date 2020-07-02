#ifndef __PREFETCH_TUNING__
#define __PREFETCH_TUNING__

enum {
    DISABLE = 0,
    ENABLE
};

#define CACHE_READUNIQ_OFFSET 40
#define CACHE_READUNIQ_CTRL (1L << CACHE_READUNIQ_OFFSET)

#ifdef CONFIG_ARCH_HISI
typedef struct {
    long cpuprefctrl_el1;
    long adps_lld_ddr_el1;
    long adpp_l1v_mop_el1;
    long adps_lld_l3_el1;
} cfg_t;
#else
typedef long cfg_t;
#endif

extern void set_prefetch(void* dummy);
extern void get_prefetch(void* dummy);
extern void read_unique_set(void *dummy);
extern void read_unique_get(void *dummy);
extern void reset_prefetch(void* dummy);

extern int prefetch_policy_num(void);
extern cfg_t *prefetch_policy(int policy);

#endif
