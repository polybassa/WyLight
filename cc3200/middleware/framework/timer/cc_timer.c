//*****************************************************************************
//
// Copyright (C) 2014 Texas Instruments Incorporated - http://www.ti.com/ 
// 
// 
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//    Redistributions of source code must retain the above copyright 
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the 
//    documentation and/or other materials provided with the   
//    distribution.
//
//    Neither the name of Texas Instruments Incorporated nor the names of
//    its contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//*****************************************************************************


#include "cc_timer.h"

#define U32NS_U16MS(nsec)  (nsec / 1000000)
#define U16MS_U32NS(msec)  (msec * 1000000)

/* Model the HW Timer information */
struct hwt_info {

        struct hw_timer_ops  *ops;       /* Methods to operate HW Timer */
        cc_hndl               hndl;      /* Reference  to real HW Timer */

        struct sw_timer      *used_list; // change to work_list? 
        bool                  hw_64bits;
        
        u32                   source;    /* Source that drives HW Timer */
};

static struct hwt_info hwt_objs[MAX_HWT_PLUG];

static sys_irq_enbl enbl_irqc;
static sys_irq_dsbl dsbl_irqc; 

/*-----------------------------------------------------------------------------
 * Utility functions to access either 32bits or 64bits HW Timer on platform
 *---------------------------------------------------------------------------*/
static struct hwt_info *get_hwt(u32 source)
{
        if((HW_REALTIME_CLK == source) ||
           (HW_MONOTONE_CTR == source)) {
                return hwt_objs + source;
        }

        return NULL;
}

static i32 hwt_start(struct hwt_info *hwt, struct u64_val *expires)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        return  hwt->hw_64bits? 
                hwt_ops->start64(hwt->hndl, expires, HW_TIMER_MONOTONE) :
                hwt_ops->start32(hwt->hndl, expires->lo_32, HW_TIMER_MONOTONE);
            
}

static i32 hwt_update(struct hwt_info *hwt, struct u64_val *expires)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        return  hwt->hw_64bits?
                hwt_ops->update_exp64(hwt->hndl, expires) :
                hwt_ops->update_exp32(hwt->hndl, expires->lo_32);
}

static i32 hwt_stop(struct hwt_info *hwt)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        return hwt_ops->stop(hwt->hndl);
}

static i32 hwt_is_running(struct hwt_info *hwt)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        return hwt_ops->is_running(hwt->hndl);
}
#if 0
static i32 hwt_get_remaining(struct hwt_info *hwt, struct u64_val *remaining)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        return  hwt->hw_64bits? 
                hwt_ops->get_remaining64(hwt->hndl, remaining) :
                hwt_ops->get_remaining32(hwt->hndl, &remaining->lo_32);

}
#endif
static i32 hwt_get_current(struct hwt_info *hwt, struct u64_val *current)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        i32 rv = hwt->hw_64bits?
                hwt_ops->get_current64(hwt->hndl, current) :
                hwt_ops->get_current32(hwt->hndl, &current->lo_32);

        current->hi_32 += hwt_ops->get_rollovers(hwt->hndl);

        return rv;
}

static i32 hwt_get_rollovers(struct hwt_info *hwt)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;
        
        return hwt_ops->get_rollovers(hwt->hndl);
}

static i32 hwt_get_frequency(struct hwt_info *hwt)
{
        struct hw_timer_ops *hwt_ops = hwt->ops;

        return hwt_ops->get_frequency(hwt->hndl);
}

static inline bool hwt_is_64bits(struct hwt_info *hwt)
{
        return hwt->hw_64bits;
}

static inline bool hwt_has_abs_time(struct hwt_info *hwt)
{
        return (HW_REALTIME_CLK == hwt->source)? true : false;
}

/*-----------------------------------------------------------------------------
 * SW Timer structure and utility functions
 *----------------------------------------------------------------------------*/
struct sw_timer {

#define SW_TIMER_PERIODIC   0x00000001
#define SW_TIMER_HW_SCHED   0x00000002
#define SW_TIMER_ALLOCATE   0x00000004

        u32                   flags;       /* Flags to manage SW Timer state */

        struct u64_val        hwt_expires; /* Expiry value: unit is HW count */
        struct u64_val        sw_interval; /* SWT Timeout interval: HW count */ 
        
        cc_cb_fn              timeout_cb;  /* Invoke @ timeout: usr callback */
        cc_hndl               cb_param;

        struct hwt_info      *hwt_obj;     /* Associated Real-world HW Timer */

        struct sw_timer      *next;
};

static inline bool is_periodic(struct sw_timer *swt)
{
        return (swt->flags & SW_TIMER_PERIODIC)? true : false;
}

static inline void set_periodic(struct sw_timer *swt, bool periodic)
{
        if(periodic)
                swt->flags |=  SW_TIMER_PERIODIC;
        else
                swt->flags &= ~SW_TIMER_PERIODIC;
}

static inline bool is_scheduled(struct sw_timer *swt)
{
        return (swt->flags & SW_TIMER_HW_SCHED)? true : false;
}

static inline void set_scheduled(struct sw_timer *swt, bool sched)
{
        if(sched) 
                swt->flags |=  SW_TIMER_HW_SCHED;
        else
                swt->flags &= ~SW_TIMER_HW_SCHED;
}
#if 0
static inline void set_user_status(struct sw_timer *swt, bool used)
{
        if(used)
                swt->flags |=  SW_TIMER_ALLOCATE;
        else
                swt->flags &= ~SW_TIMER_ALLOCATE;
}
#endif
static inline void set_alloc_status(struct sw_timer *swt, bool alloc)
{
        if(alloc)
                swt->flags = SW_TIMER_ALLOCATE;
        else
                swt->flags = 0;
}

static inline bool is_alloc_only(struct sw_timer *swt)
{
        return (swt->flags == SW_TIMER_ALLOCATE)? true : false;
}

static inline bool has_started(struct sw_timer *swt)
{
        return  (swt->flags &  SW_TIMER_ALLOCATE) && 
                (swt->flags != SW_TIMER_ALLOCATE)? true : false;
}

/*-----------------------------------------------------------------------------
 * 64bit Math utility functions
 *----------------------------------------------------------------------------*/

/* Returns: 1 for val1 > val2; 0 for val1 = val2; -1 for val1 < val2 */
static i32 cmp_u32(u32 val1, u32 val2)
{
        i32 rv = -1;

        if(val1 == val2)
                rv = 0;
        else if(val1 > val2)
                rv = 1;

        return rv;
}

/* Returns: 1 for val1 > val2; 0 for val1 = val2; -1 for val1 < val2 */
static i32 cmp_u64(struct u64_val *u64_val1, struct u64_val *u64_val2)
{
        i32 rv = -1;

        if(u64_val1->hi_32 == u64_val2->hi_32)
                rv = cmp_u32(u64_val1->lo_32, u64_val2->lo_32);
        else if(u64_val1->hi_32 > u64_val2->hi_32)
                rv = 1;

        return rv;
}

#define NSEC_VAL_FOR_1SEC   1000000000

static i32 calc_ticks_interval_u32(u64 addendum, struct u64_val *sw_interval)
{
        u64 tmp64 = addendum + sw_interval->lo_32;

        if(0xFFFFFFFF < tmp64)
                return -1;  /* Exceeds 32bit value */
        
        sw_interval->lo_32 = tmp64 & 0xFFFFFFFF;
        sw_interval->hi_32 = 0;

        return 0;
}

static i32 calc_ticks_interval_u64(u64 addendum, struct u64_val *sw_interval)
{
        u64 tmp64 = addendum + sw_interval->lo_32;
        
        sw_interval->lo_32 =  tmp64 & 0xFFFFFFFF;
        sw_interval->hi_32 = (tmp64 >> 32) & 0xFFFFFFFF;
        
        return 0;
}

static 
i32 calc_ticks_interval(struct hwt_info *hwt, struct u64_time *time_u64, 
                        struct u64_val *sw_interval)
                       
{
        u64 tmp64 = 0;
        u32 hwt_freq = hwt_get_frequency(hwt);
        
        if(time_u64->nsec > (NSEC_VAL_FOR_1SEC - 1))
                return -1; /* Value exceeds a sec */

        tmp64 = time_u64->nsec * hwt_freq;
        sw_interval->lo_32 = tmp64 / NSEC_VAL_FOR_1SEC;

        return  hwt_is_64bits(hwt)? 
                calc_ticks_interval_u64(time_u64->secs * hwt_freq, sw_interval) :
                calc_ticks_interval_u32(time_u64->secs * hwt_freq, sw_interval);
}

#if 0
static void calc_hwt_expiry_u32(struct u64_val *hwt_current, 
                                struct u64_val *sw_interval, 
                                struct u64_val *hwt_expires)
{
        /* 32bit overflow check: v1 + v2 > 0xFFFFFFFF => v1 > 0xFFFFFFFF - v2 */
        if(hwt_current->lo_32 > ~sw_interval->lo_32) {
                /* exceeds by => v1 + v2 - (0xFFFFFFFF + 1) = v1 - (~v2 + 1)  */
                u32 exceeds = hwt_current->lo_32 - ~sw_interval->lo_32 - 1;
                hwt_expires->lo_32  = exceeds;
                hwt_expires->hi_32 += 1;                        /* Carry flag */
        } else {
                /* Simple 32bits addition without any carry flag and overflow */
                hwt_expires->lo_32  = hwt_current->lo_32 +  sw_interval->lo_32;
        }
        
        return;
}
#else
static void calc_hwt_expiry_u32(struct u64_val *hwt_current,
                                struct u64_val *sw_interval,
                                struct u64_val *hwt_expires)
{
        /* 32bit overflow check: v1 + v2 > 0xFFFFFFFF => v1 > 0xFFFFFFFF - v2 */
        if(hwt_current->lo_32 + sw_interval->lo_32 >= 1000000000) {
                /* exceeds by => v1 + v2 - (0xFFFFFFFF + 1) = v1 - (~v2 + 1)  */
                u32 exceeds = hwt_current->lo_32 + sw_interval->lo_32 - 1000000000;
                hwt_expires->lo_32  = exceeds;
                hwt_expires->hi_32 += 1;                        /* Carry flag */
        } else {
                /* Simple 32bits addition without any carry flag and overflow */
                hwt_expires->lo_32  = hwt_current->lo_32 +  sw_interval->lo_32;
        }

        return;
}
#endif
static void calc_hwt_expiry_u64(struct u64_val *hwt_current, 
                                struct u64_val *sw_interval, 
                                struct u64_val *hwt_expires)
{
        calc_hwt_expiry_u32(hwt_current, sw_interval, hwt_expires);

        hwt_expires->hi_32 += hwt_current->hi_32 + sw_interval->hi_32;

        return;
}

static 
void calc_hwt_expiry(struct hwt_info *hwt, struct u64_val *hwt_current, 
                     struct u64_val *sw_interval, struct u64_val *hwt_expires)
{
        hwt_expires->hi_32 = hwt_get_rollovers(hwt);

        hwt_is_64bits(hwt)? 
                calc_hwt_expiry_u64(hwt_current, sw_interval, hwt_expires) :
                calc_hwt_expiry_u32(hwt_current, sw_interval, hwt_expires);
        return;
}

/*-----------------------------------------------------------------------------
 * Core SW Timer servics
 *----------------------------------------------------------------------------*/
static void insert_ordered_expiry(struct sw_timer *elem, struct sw_timer **list)
{
        struct sw_timer *node = *list, *prev = NULL;

        if(NULL == node) {  /* First user request for the  HW timer */
                *list = elem;
                goto sort_insert_elem_exit1;
        }

        while(node) {        /* There is atleast one active request */
                if(0 > cmp_u64(&elem->hwt_expires, &node->hwt_expires)) {
                        /* 'elem' expires earlier than this 'node'  */
                        elem->next = node; 
                        
                        if(NULL == prev) {
                                *list  = elem; /* New first element */
                                set_scheduled(node, false);
                        }
                        else
                                prev->next = elem; /* Intermediary  */

                        /* 'elem' has been placed in 'list', quit.. */
                        goto sort_insert_elem_exit1;
                }

                prev = node;
                node = node->next;
        }

        if(NULL == node) 
                prev->next = elem; /* Farthest in time for schedule */

 sort_insert_elem_exit1:
        return;
}

static i32 sched_timer_if_new(struct sw_timer *head)
{
        i32 rv = 0;
        struct hwt_info *hwt = head->hwt_obj;

        if(true == is_scheduled(head)) 
                goto sched_timer_if_new_exit;
        
        /* If HW timer running, then update it othewise start it */        
        rv = hwt_is_running(hwt)?
                hwt_update(hwt, &head->hwt_expires) : 
                hwt_start(hwt, &head->hwt_expires);
        
        if(0 == rv)
                set_scheduled(head, true);
        
 sched_timer_if_new_exit:
        return rv;
}

static i32 setup_timer_exec(struct sw_timer *swt, u32 options) 
{
        /* Step 1: Use absolute expiry time, if provided. Otherwise .....
           Step 2: Calculate absolute expiry time in terms of HW counter.
           
           TODO: Seems that providing both flags and time_u64 may be redundant
        */

        struct hwt_info *hwt = swt->hwt_obj;

        if(options & OPT_TIME_ABS_VALUE) {
                /* Step 1: Use absolute time, if it has been provided */
                swt->hwt_expires.hi_32 = swt->sw_interval.hi_32;
                swt->hwt_expires.lo_32 = swt->sw_interval.lo_32;
        } else {
                
                struct u64_val hwt_current;
                
                if(-1 == hwt_get_current(hwt, &hwt_current))
                        return -1;
                
                /* Step 2. Calculate HW expiry time.... */
                calc_hwt_expiry(hwt, &hwt_current, &swt->sw_interval,
                                &swt->hwt_expires);
        }
        
        insert_ordered_expiry(swt, &hwt->used_list);

        sched_timer_if_new(hwt->used_list);

        return 0;
}

static i32 setup_timer_interval(struct sw_timer *swt, struct u64_time *time_u64)
{
        struct hwt_info *hwt = swt->hwt_obj;

        if(false == hwt_has_abs_time(hwt)) {
                /* Convert time specified by user into ticks for HW Timer */
                if(-1 == calc_ticks_interval(hwt, time_u64, &swt->sw_interval))
                        return -1;
        } else {
                /* If HWT has support for real-time, work with user info */
                swt->sw_interval.hi_32 = time_u64->secs;
                swt->sw_interval.lo_32 = time_u64->nsec;
        }
        
        return 0;
}

static i32 timer_start(struct sw_timer *swt, struct u64_time *time_u64,
                       u32 options)
{
        if(false == is_alloc_only(swt))
                return -1; /* Not only allocated but started as well */
        
        if((-1 == setup_timer_interval(swt, time_u64))  ||
           (-1 == setup_timer_exec(swt, options)))
                return -1;

        set_periodic(swt, options & OPT_TIMER_PERIODIC ? true : false);

        return 0;
}

static bool has_valid_opts(struct hwt_info *hwt, u32 options)
{
        /* Implementtion: A periodic timer can't work with absolute time */
        if((OPT_TIMER_PERIODIC & options) && (OPT_TIME_ABS_VALUE & options))
                return false; 

        /* A request for absolute time has to be supported on HW */
        if((options & OPT_TIME_ABS_VALUE) && !hwt_has_abs_time(hwt))
                return false;

        return true;
}

i32 cc_timer_start(cc_hndl hndl, struct u64_time *time_u64, u32 options)
{
		u32 intr_mask;
        struct sw_timer *swt = (struct sw_timer*) hndl;
        struct u64_time time_64_loc = {0,0};
        i32 rv = -1;
        
        if((NULL == swt) || 
           (false == has_valid_opts(swt->hwt_obj, options)))
                goto cc_timer_start_exit1;
        
        intr_mask = dsbl_irqc();
        /* reducing the precision to milliseconds to avoid floating point ops */
        time_64_loc.secs = time_u64->secs;
        time_64_loc.nsec = U16MS_U32NS(U32NS_U16MS(time_u64->nsec));
        rv = timer_start(swt, &time_64_loc, options);
        enbl_irqc(intr_mask);

cc_timer_start_exit1:
        
        return rv;
} 

static i32 remove_elem(struct sw_timer *elem, struct sw_timer **list)
{
        struct sw_timer *node = *list, *prev = NULL;
        i32 rv = -1;

        if(NULL == node)
                return rv;

        while(node) {
                if(elem == node) {
                        if(NULL == prev)
                                *list      = node->next;
                        else
                                prev->next = node->next; 
                        
                        rv = 0;
                        break;  /* Exit while loop */
                }
                
                prev = node;
                node = node->next;
        }

        return rv;
}

static i32 timer_stop(struct sw_timer *swt)
{
        struct hwt_info *hwt = swt->hwt_obj;

        if(0 != remove_elem(swt, &hwt->used_list)) {
                return -1;
        }

        set_scheduled(swt, false);
        set_periodic(swt, false);

        if(NULL != hwt->used_list)
                sched_timer_if_new(hwt->used_list);
        else
                hwt_stop(hwt); /* No pending request, stop HW */

        return 0;
}

i32 cc_timer_stop(cc_hndl hndl)
{
		u32 intr_mask;
        struct sw_timer *swt = (struct sw_timer*) hndl;
        i32 rv = -1;

        intr_mask = dsbl_irqc();
        if(swt && has_started(swt)) {
                rv = timer_stop(swt);
        }
        enbl_irqc(intr_mask);

        return rv;
}

static void handle_expired_timer(struct sw_timer *swt)
{
        set_scheduled(swt, false);

        /* Invoke user's callback routine */
        if(swt->timeout_cb)
                swt->timeout_cb(swt->cb_param);
        
        /* Period timer: Set-up next iteration */
        if(true == is_periodic(swt))
                setup_timer_exec(swt, 0);

        return;
}

/* Called in the interrupt context */
static void process_timer_expiry(struct hwt_info *hwt)
{
        struct sw_timer *head = hwt->used_list;
        
        while(head) { /* Run through list to process all expired timers */
                struct u64_val hwt_current;

                if(0 != hwt_get_current(head->hwt_obj, &hwt_current))
                        goto handle_timer_expiry_exit1;
                
                if(0 > cmp_u64(&hwt_current, &head->hwt_expires))
                        break; /* Timer is yet to reach expiry, so quit */

                remove_elem(head, &hwt->used_list);

                handle_expired_timer(head);

                head = hwt->used_list;
        }
        
        if(head)
                sched_timer_if_new(head);
        else
                hwt_stop(hwt);
        
 handle_timer_expiry_exit1:
        return;
}

/* To be invoked by HAL */
static void timer_isr(cc_hndl cb_param)
{
        struct hwt_info *hwt  = (struct hwt_info*) cb_param;
        
        if(NULL != hwt)
                process_timer_expiry(hwt);

        return;
}

/*-----------------------------------------------------------------------------
 * SW Timer Service Initialization
 *----------------------------------------------------------------------------*/
static struct sw_timer  sw_timers[MAX_SW_TIMER];
static struct sw_timer *free_list = NULL;

cc_hndl cc_timer_create(struct cc_timer_cfg *cfg)
{
		u32 intr_mask;
        struct sw_timer *swt = NULL;
        struct hwt_info *hwt = NULL;

        hwt = get_hwt(cfg->source);
        if((NULL == hwt) || (NULL == hwt->ops))
                goto cc_timer_create_exit1;

        intr_mask = dsbl_irqc();  /* Disable interrupts */

        swt = free_list;
        if(NULL == swt)
                goto cc_timer_create_exit2;

        free_list = swt->next;
        swt->next = NULL;

        set_alloc_status(swt, true);
        swt->hwt_obj    = hwt;
        swt->timeout_cb = cfg->timeout_cb;
        swt->cb_param   = cfg->cb_param;

 cc_timer_create_exit2:
        enbl_irqc(intr_mask); /* Enable  interrupts */

 cc_timer_create_exit1:

        return (cc_hndl) swt;
}

i32 cc_timer_delete(cc_hndl hndl)
{
		u32 intr_mask;
        struct sw_timer *swt = (struct sw_timer*) hndl;

        if(NULL == swt || !is_alloc_only(swt))
                return -1;

        set_alloc_status(swt, false);
        memset(swt, 0, sizeof(swt));
        swt->hwt_obj = NULL;
        swt->next    = NULL;
        
        intr_mask = dsbl_irqc();
        swt->next = free_list;
        free_list = swt;
        enbl_irqc(intr_mask);

        return 0;
}

static i32 sw_timers_init(void)
{
        i32 i = 0;

        memset(sw_timers, 0, sizeof(sw_timers));

        for(i = 0; i < MAX_SW_TIMER; i++) {
                struct sw_timer *swt = sw_timers + i;
                swt->next = free_list;
                free_list = swt;
        }

        return 0;
}

static i32 hw_timers_init(void)
{
        i32 i = 0;
        i32 max_val = MAX_HWT_PLUG - 1;
        
        if((HW_REALTIME_CLK > max_val) || (HW_MONOTONE_CTR > max_val))
                return -1;
        
        for(i = 0; i < MAX_HWT_PLUG; i++) {
                struct hwt_info *hwt = hwt_objs + i;

                hwt->ops       = NULL;
                hwt->hndl      = NULL;
                hwt->used_list = NULL;
                hwt->hw_64bits = false;
                hwt->source    = i;
        }
        
        return 0;
}

i32 cc_timer_module_init(struct cc_timer_setup *timer_setup)
{
        if((NULL == timer_setup->enbl_irqc) || 
                (NULL == timer_setup->dsbl_irqc)) {
                return -1;
        }
        enbl_irqc = timer_setup->enbl_irqc;
        dsbl_irqc = timer_setup->dsbl_irqc;

        return sw_timers_init() || hw_timers_init();
}

i32 cc_timer_register_hwt_ops(u32 source,  cc_hndl hwt_hndl, 
                              struct hw_timer_ops *hwt_ops)
{
        struct hwt_info *hwt = get_hwt(source);
        bool op64;
        
        if((NULL == hwt) || (NULL != hwt->ops) || (NULL == hwt_hndl)) 
                return -1; /* HWT ops already initialized */

#define HAS_HWT_OPS(f1, f2, f3, f4)                                     \
        (hwt_ops->f1 && hwt_ops->f2 && hwt_ops->f3 && hwt_ops->f4)?  true : false
        
        op64 = HAS_HWT_OPS(start64, update_exp64, get_remaining64,
                           get_current64);

        /* Set of ops specific to either 32bit or 64bit must be available */
        if(!(HAS_HWT_OPS(start32, update_exp32, get_remaining32, get_current32) ^ 
             op64))
                return -1;
        
        /* Set of ops common to both 32bit or 64bit must be availalbe */
        if(!HAS_HWT_OPS(stop, is_running, get_rollovers, get_frequency) ||
           !hwt_ops->register_cb)
                return -1;

        /* Install callback function in HW Timer */
        if(-1 == hwt_ops->register_cb(hwt_hndl, timer_isr, hwt))
                return -1;

        hwt->ops       = hwt_ops;
        hwt->hndl      = hwt_hndl;
        hwt->hw_64bits = op64;

        return 0;
}
