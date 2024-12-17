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
    void *mem;                          /* memory pool */
    uint32_t mem_size;                  /* size of memory pool */
    void  *free_list;                   /* memory pool head of the free list */
    rmp_sem_id alloc_sem;               /* can alloc semaphore */
    rmp_sem_id free_sem;                /* can free semaphore */
    rmp_mutex_id mutex;                 /* mutex of shared resource */
} rmp_t;

#ifdef RMP_USING_DYNAMIC
/**
 * @brief create memory pool
 * 
 * @param [in] size  single block size
 * @param [in] count block count
 * 
 * @return rmp_t: create mempool success
 *         NULL: create mempool failed
 */
rmp_t *rmp_create(uint32_t size, uint32_t count);

/**
 * @brief delete memory pool
 * 
 * @param [in] mp memory pool object
 */
void rmp_delete(rmp_t *mp);
#endif
/**
 * @brief init memory pool
 * 
 * @param [in] mp    memory pool object
 * @param [in] mem   point of memory pool
 * @param [in] size  single block size
 * @param [in] count block count
 */
void rmp_init(rmp_t *mp, void *mem, uint32_t size, uint32_t count);

/**
 * @brief deinit memory pool
 * 
 * @param [in] mp memory pool object
 */
void rmp_deinit(rmp_t *mp);

/**
 * @brief alloc memory block
 * @note blocking alloc memory blocks
 * 
 * @param [in] mp memory pool object
 * 
 * @return success: point of memory block
 *         fail: NULL
 */
void *rmp_alloc(rmp_t *mp);

/**
 * @brief try alloc memory block
 * @note nonblocking alloc memory blocks
 * 
 * @param [in] mp memory pool object
 * 
 * @return success: point of memory block
 *         fail: NULL
 */
void *rmp_try_alloc(rmp_t *mp);

/**
 * @brief free memory block
 * 
 * @param [in] mp memory pool object
 * @param [in] ptr point of memory block
 */
void rmp_free(rmp_t *mp, void *ptr);

/**
 * @brief number of available memory blocks
 * 
 * @param [in] mp memory pool object
 * 
 * @return number of available
 */
uint32_t rmp_available(rmp_t *mp);

#endif
