/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-10     RiceChen     the first version
 */

#ifndef __RMP_H__
#define __RMP_H__

#include <stdint.h>
#include "rmp_def.h"

/**
 * @brief Memory pool
 */
typedef struct {
    void *mem;
    uint32_t size;                      /**< size of single list element */
    void  *free_list;                   /**< memory pool head of the free list */
    rmp_sem_id alloc_sem;
    rmp_sem_id free_sem;
    rmp_mutex_id mutex;
} rmp_t;

typedef struct rmp_element {
    struct rmp_element *next;      /**< Pointer to the next element */
} rmp_element_t;

#ifdef RMP_USING_DYNAMIC
rmp_t *rmp_create(uint32_t size, uint32_t count);

void rmp_delete(rmp_t *mp);
#endif

void rmp_init(rmp_t *mp, void *mem, uint32_t size, uint32_t count);

void *rmp_alloc(rmp_t *mp);

void *rmp_try_alloc(rmp_t *mp);

void rmp_free(rmp_t *mp, void *ptr);

uint32_t rmp_available(rmp_t *mp);

#endif
