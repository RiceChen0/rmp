/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-10     RiceChen     the first version
 */

#ifndef __RMP_H__
#define __RMP_H__

#include <stdint.h>

/**
 * @brief Memory pool
 */
typedef struct {
    void  *free_list;           /**< memory pool head of the free list */
    uint32_t size;              /**< size of single list element */
} rmp_t;

#endif
