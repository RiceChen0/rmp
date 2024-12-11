/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-10     RiceChen     the first version
 */

#include "rmp.h"

rmp_t *rmp_create(uint32_t size, uint32_t count)
{
    rmp_t *mp = NULL;
    uint8_t *element = NULL;

    mp = (rmp_t *)RMP_MALLOC(sizeof(rmp_t));
    if(mp == NULL) {
        return NULL;
    }

    mp->mutex = rmp_mutex_create();
    if(mp->mutex == NULL) {
        goto __EXIT;
    }

    mp->alloc_sem = rmp_sem_create(count);
    if(mp->mutex == NULL) {
        goto __EXIT;
    }

    mp->free_sem = rmp_sem_create(0);
    if(mp->mutex == NULL) {
        goto __EXIT;
    }

    mp->mem = RMP_MALLOC(size * count);
    if(mp->mem == NULL) {
        goto __EXIT;
    }

    mp->size = size * count;
    mp->free_list = NULL;

    for(element = mp->mem; 
        element < ((uint8_t*)mp->mem + mp->size); 
        element += size) {
        rmp_free(mp, element);
    }

    return mp;

__EXIT:
    RMP_FREE(mp);
    mp = NULL;

    return NULL;
}

void rmp_delete(rmp_t *mp)
{
    rmp_mutex_delete(&mp->mutex);
    rmp_sem_delete(&mp->alloc_sem);
    rmp_sem_delete(&mp->free_sem);

    RMP_FREE(mp->mem);
    mp->mem = NULL;

    RMP_FREE(mp);
    mp = NULL;
}

static void *__rmp_alloc(rmp_t *mp, bool block)
{
    RMP_ASSERT(mp);

    rmp_status ret;
    void *new = NULL;

    if(block) {
        rmp_sem_lock(&mp->alloc_sem, true);
    } else {
        ret = rmp_sem_lock(&mp->alloc_sem, false);
        if(ret != RMP_OK) {
            return NULL;
        }
    }

    rmp_mutex_lock(&mp->mutex);
    new = mp->free_list;
    if(new) {
        mp->free_list = *((void **)mp->free_list);
    }
    rmp_mutex_unlock(&mp->mutex);

    rmp_sem_unlock(&mp->free_sem);

    return new;
}

void *rmp_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);

    return __rmp_alloc(mp, true);
}

void *rmp_try_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);
    
    return __rmp_alloc(mp, false);
}

static bool __rmp_in_pool(rmp_t *mp, void *ptr)
{
    return (ptr >= mp->mem) && (uint8_t *)ptr < ((uint8_t*)mp->mem + mp->size);
}

void rmp_free(rmp_t *mp, void *ptr)
{
    RMP_ASSERT(mp);
    RMP_ASSERT(ptr);

    rmp_status ret;

    if(!__rmp_in_pool(mp, ptr)) {
        return;
    }

    ret = rmp_sem_lock(&mp->free_sem, false);
    if(ret != RMP_OK) {
        return;
    }

    rmp_mutex_lock(&mp->mutex);
    memcpy(ptr, &mp->free_list, sizeof(void *));
    mp->free_list = ptr;
    rmp_mutex_unlock(&mp->mutex);

    rmp_sem_unlock(&mp->alloc_sem);
}

uint32_t rmp_available(rmp_t *mp)
{
    RMP_ASSERT(mp);

    uint32_t count = 0;
    void **element = &mp->free_list;

    while (*element) {
        element = (void**)*element;
        count++;
    }
    return count;
}
