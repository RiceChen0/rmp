#include "rtthread.h"
#include "rmp.h"

int rmp_init(void)
{
#ifdef RMP_USING_DYNAMIC
    rmp *mp = NULL;

    mp = rmp_create(16, 4);
    if (mp == NULL)
    {
        rt_kprintf("rmp create failed\n");
        return -1;
    }

    void *mem1 = rmp_try_alloc(mp);
    if (mem1 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem1);
    else
        rt_kprintf("mem: NULL\n");

    void *mem2 = rmp_try_alloc(mp);
    if (mem2 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem2);
    else
        rt_kprintf("mem: NULL\n");

    void *mem3 = rmp_try_alloc(mp);
    if (mem3 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem3);
    else
        rt_kprintf("mem: NULL\n");

    void *mem4 = rmp_try_alloc(mp);
    if (mem4 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem4);
    else
        rt_kprintf("mem: NULL\n");

    void *mem5 = rmp_try_alloc(mp);
    if (mem5 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem5);
    else
        rt_kprintf("mem: NULL\n");

    rmp_free(&mp, mem4);

    void *mem6 = rmp_try_alloc(mp);
    if (mem6 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem6);
    else
        rt_kprintf("mem: NULL\n");
#else
    uint8_t buff[16 * 4];
    rmp_t mp;
    
    rmp_init(&mp, buff, 16, 4);

    void *mem1 = rmp_try_alloc(&mp);
    if (mem1 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem1);
    else
        rt_kprintf("mem: NULL\n");

    void *mem2 = rmp_try_alloc(&mp);
    if (mem2 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem2);
    else
        rt_kprintf("mem: NULL\n");

    void *mem3 = rmp_try_alloc(&mp);
    if (mem3 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem3);
    else
        rt_kprintf("mem: NULL\n");

    void *mem4 = rmp_try_alloc(&mp);
    if (mem4 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem4);
    else
        rt_kprintf("mem: NULL\n");

    void *mem5 = rmp_try_alloc(&mp);
    if (mem5 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem5);
    else
        rt_kprintf("mem: NULL\n");

    rmp_free(&mp, mem4);

    void *mem6 = rmp_try_alloc(&mp);
    if (mem6 != NULL)
        rt_kprintf("mem: 0x%08X\n", (int)mem6);
    else
        rt_kprintf("mem: NULL\n");
#endif

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rmp_init);



