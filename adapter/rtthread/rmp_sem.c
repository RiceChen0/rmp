#include "rmp_def.h"
#include "rtthread.h"

rmp_sem_id rmp_sem_create(uint32_t value)
{
    rt_sem_t sem = NULL;

    sem = rt_sem_create("rmp", value, RT_IPC_FLAG_FIFO);
    return (rmp_sem_id)sem;
}

rmp_status rmp_sem_lock(rmp_sem_id sem, bool block)
{
    rt_err_t ret;
    if(block == true)
        ret = rt_sem_take((rt_sem_t)sem, RT_WAITING_FOREVER);
    else
        ret = rt_sem_trytake((rt_sem_t)sem);

    if(ret != RT_EOK)
        return RMP_ERROR;

    return RMP_OK;
}

rmp_status rmp_sem_unlock(rmp_sem_id sem)
{
    rt_sem_release((rt_sem_t)sem);
    return RMP_OK;
}

void rmp_sem_delete(rmp_sem_id sem)
{
    rt_sem_delete((rt_sem_t)sem);
}
