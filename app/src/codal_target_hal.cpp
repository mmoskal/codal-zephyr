#include "codal_target_hal.h"
#include "CodalDmesg.h"

#include <misc/reboot.h>
#include <misc/printk.h>

void target_enable_irq()
{
    irq_unlock(0);
}

void target_disable_irq()
{
    irq_lock();
}

void target_wait_for_event()
{
    k_yield();
}

void target_wait(uint32_t milliseconds)
{
    k_busy_wait(milliseconds * 1000);
}

void target_reset()
{
    sys_reboot(0);
}

void target_panic(int statusCode)
{
    target_disable_irq();

    printk("*** CODAL PANIC : [%d]", statusCode);

#if DEVICE_DMESG_BUFFER_SIZE > 0
    DMESG("*** CODAL PANIC : [%d]", statusCode);
#endif

    for (;;)
        ;
}

PROCESSOR_WORD_TYPE codal_heap_start = (PROCESSOR_WORD_TYPE)(&_end);
