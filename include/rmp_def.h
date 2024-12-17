/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-10     RiceChen     the first version
 */

#ifndef __RMP_DEF_H__
#define __RMP_DEF_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#ifdef __RTTHREAD__
#include <rtthread.h>
#endif

#ifdef RMP_USING_DYNAMIC

/**
 * memory API
*/
#ifndef RMP_MALLOC
    #define RMP_MALLOC          malloc
#endif

#ifndef RMP_FREE
    #define RMP_FREE            free
#endif

#endif

#ifndef RMP_ASSERT
    #define RMP_ASSERT          assert
#endif

typedef enum {
    RMP_OK = 0,                 /**< There is no error */
    RMP_ERROR,                  /**< A generic error happens */
    RMP_INVAL,                  /**< Invalid argument */
} rmp_status;

#ifdef RMP_USING_RTOS

/**
 * Mutex API
 */
typedef void *rmp_mutex_id;

rmp_mutex_id rmp_mutex_create(void);
rmp_status rmp_mutex_lock(rmp_mutex_id mutex);
rmp_status rmp_mutex_unlock(rmp_mutex_id mutex);
void rmp_mutex_delete(rmp_mutex_id mutex);

/**
 * Sem API
 */
typedef void *rmp_sem_id;

rmp_sem_id rmp_sem_create(uint32_t value);
rmp_status rmp_sem_lock(rmp_sem_id sem, bool block);
rmp_status rmp_sem_unlock(rmp_sem_id sem);
void rmp_sem_delete(rmp_sem_id sem);

#endif

#endif
