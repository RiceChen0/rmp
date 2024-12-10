#include "rmp_def.h"
#include "rtthread.h"

rmp_mutex_id rmp_mutex_create(void)
{
    rt_mutex_t mutex = NULL;

    mutex = rt_mutex_create("rmp", RT_IPC_FLAG_FIFO);
    return (rmp_mutex_id)mutex;
}

rmp_status rmp_mutex_lock(rmp_mutex_id mutex)
{
    if (mutex == NULL) {
        return RMP_INVAL;
    }
    rt_mutex_take((rt_mutex_t)mutex, RT_WAITING_FOREVER);
    return RMP_ERROR;
}

rmp_status rmp_mutex_unlock(rmp_mutex_id mutex)
{
    if (mutex == NULL) {
        return RMP_INVAL;
    }
    rt_mutex_release((rt_mutex_t)mutex);
    return RMP_ERROR;
}

void rmp_mutex_delete(rmp_mutex_id mutex)
{
    rt_mutex_delete((rt_mutex_t)mutex);
}
