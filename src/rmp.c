/*
 * Change Logs:
 * Date           Author       Notes
 * 2024-12-10     RiceChen     the first version
 */

#include "rmp.h"

static void *_rmp_alloc(rmp_t *mp);
static void _rmp_free(rmp_t *mp, void *ptr);

static void _rmp_init(rmp_t *mp, void *mem, uint32_t size, uint32_t count)
{
    RMP_ASSERT(mp && mem);

    uint8_t *element = NULL;

    mp->size = size * count;
    mp->free_list = NULL;

    for(element = mem; 
        element < ((uint8_t*)mem + mp->size); 
        element += size) {
        _rmp_free(mp, element);
    }
}

static void *_rmp_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);

    void *new = NULL;

    new = mp->free_list;
    if(new) {
        mp->free_list = *((void **)mp->free_list);
    }

    return new;
}

static void _rmp_free(rmp_t *mp, void *ptr)
{
    memcpy(ptr, &mp->free_list, sizeof(void *));
    mp->free_list = ptr;
}

#ifdef RMP_USING_DYNAMIC
rmp_t *rmp_create(uint32_t size, uint32_t count)
{
    rmp_t *mp = NULL;
    
    mp = (rmp_t *)RMP_MALLOC(sizeof(rmp_t));
    if(mp == NULL) {
        return NULL;
    }

    mp->mutex = rmp_mutex_create();
    if(mp->mutex == NULL) {
        goto __EXIT;
    }

    mp->alloc_sem = rmp_sem_create(count);
    if(mp->alloc_sem == NULL) {
        goto __EXIT;
    }

    mp->free_sem = rmp_sem_create(0);
    if(mp->free_sem == NULL) {
        goto __EXIT;
    }

    mp->mem = RMP_MALLOC(size * count);
    if(mp->mem == NULL) {
        goto __EXIT;
    }

    _rmp_init(mp, mp->mem, size, count);

    return mp;

__EXIT:
    RMP_FREE(mp);
    mp = NULL;

    return NULL;
}

void rmp_delete(rmp_t *mp)
{
    RMP_ASSERT(mp);

    rmp_mutex_delete(mp->mutex);
    rmp_sem_delete(mp->alloc_sem);
    rmp_sem_delete(mp->free_sem);

    RMP_FREE(mp->mem);
    mp->mem = NULL;

    RMP_FREE(mp);
    mp = NULL;
}
#endif

void rmp_init(rmp_t *mp, void *mem, uint32_t size, uint32_t count)
{
    mp->mutex = rmp_mutex_create();
    mp->alloc_sem = rmp_sem_create(count);
    mp->free_sem = rmp_sem_create(0);

    mp->mem = mem;
    _rmp_init(mp, mp->mem, size, count);
}

void *rmp_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);

    void *new = NULL;

    rmp_sem_lock(mp->alloc_sem, true);

    rmp_mutex_lock(mp->mutex);
    new = _rmp_alloc(mp);
    rmp_mutex_unlock(mp->mutex);

    rmp_sem_unlock(mp->free_sem);

    return new;
}

void *rmp_try_alloc(rmp_t *mp)
{
    RMP_ASSERT(mp);

    rmp_status ret;
    void *new = NULL;

    ret = rmp_sem_lock(mp->alloc_sem, false);
    if(ret != RMP_OK) {
        return NULL;
    }

    rmp_mutex_lock(mp->mutex);
    new = _rmp_alloc(mp);
    rmp_mutex_unlock(mp->mutex);

    rmp_sem_unlock(mp->free_sem);

    return new;
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

    ret = rmp_sem_lock(mp->free_sem, false);
    if(ret != RMP_OK) {
        return;
    }

    rmp_mutex_lock(mp->mutex);
    _rmp_free(mp, ptr);
    rmp_mutex_unlock(mp->mutex);

    rmp_sem_unlock(mp->alloc_sem);
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
