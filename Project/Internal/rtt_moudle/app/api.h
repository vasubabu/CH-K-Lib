#ifndef __API_H__
#define __API_H__

#include "rtt_api.h"

extern api_t *api;

#define rt_kprintf              api->rtthread->rt_kprintf
#define rt_thread_delay         api->rtthread->rt_thread_delay
#define rt_malloc               api->rtthread->rt_malloc
#define rt_tick_get             api->rtthread->rt_tick_get
#define rt_device_find          api->rtthread->rt_device_find
#define rt_thread_create        api->rtthread->rt_thread_create
#define rt_device_register      api->rtthread->rt_device_register
#define rt_thread_startup       api->rtthread->rt_thread_startup
#define rt_device_init          api->rtthread->rt_device_init
#define rt_device_control       api->rtthread->rt_device_control




#endif





