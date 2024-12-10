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

    mp->free_list = NULL;

    for(element = mp->mem; 
        element < ((uint8_t*)mp->mem + (size * count)); 
        element += size) {
        rmp_free(mp, element);
    }

    return mp;

__EXIT:
    RMP_FREE(mp);
    mp = NULL;

    return NULL;
}

void *rmp_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);

    void *free = mp->free_list;
    if(free) {
        mp->free_list = *((void **)mp->free_list);
    }

    return free;
}

void *rmp_try_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);
    
    return rmp_alloc(mp);
}

void rmp_free(rmp_t *mp, void *ptr)
{
    RMP_ASSERT(mp);
    RMP_ASSERT(ptr);

    memcpy(ptr, &mp->free_list, sizeof(void *));
    mp->free_list = ptr;
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
