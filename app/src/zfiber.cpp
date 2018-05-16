#include "CodalFiber.h"

using namespace codal;

void codal::schedule()
{
    k_yield();
}

int codal::fiber_scheduler_running()
{
    return 1;
}

void codal::scheduler_init(EventModel &_messageBus)
{
    // switch to cooperative mode
    k_thread_priority_set(k_current_get(), FIBER_PRIORITY);
}

void codal::launch_new_fiber_param(void (*ep)(void *), void (*cp)(void *), void *pm)
{
    // Execute the thread's entrypoint.
    ep(pm);

    // Execute the thread's completion routine.
    cp(pm);

    // If we get here, then the completion routine didn't recycle the fiber... so do it anyway. :-)
    release_fiber(pm);
}

void codal::release_fiber(void *)
{
    release_fiber();
}

void codal::release_fiber(void)
{
    k_thread_abort(k_current_get());
    target_panic(999); // unreachable
}

static void free_current()
{
    // this is called from k_thread_abort() with irqs locked, and Zephyr isn't using malloc()
    // so we should be fine with Zephyr using the current thread for a little while longer
    struct k_thread *curr = k_current_get();
    free(curr);
}

Fiber *__create_fiber(uint32_t ep, uint32_t cp, uint32_t pm)
{
    if (ep == 0 || cp == 0)
        return NULL;

    struct k_thread *thr =
        (struct k_thread *)malloc(sizeof(struct k_thread) + STACK_ALIGN + FIBER_STACK_SIZE);
    uintptr_t stack = (uintptr_t)(thr + 1);
    while (stack & (STACK_ALIGN - 1))
        stack++;

    thr->fn_abort = free_current;

    k_thread_create(thr, (k_thread_stack_t *)stack, FIBER_STACK_SIZE,
                    (k_thread_entry_t)launch_new_fiber_param, (void *)ep, (void *)cp, (void *)pm,
                    FIBER_PRIORITY, 0, K_NO_WAIT);

    return (Fiber *)42;
}

Fiber *codal::create_fiber(void (*entry_fn)(void), void (*completion_fn)(void))
{
    return __create_fiber((uint32_t)entry_fn, (uint32_t)completion_fn, 0);
}

Fiber *codal::create_fiber(void (*entry_fn)(void *), void *param, void (*completion_fn)(void *))
{
    return __create_fiber((uint32_t)entry_fn, (uint32_t)completion_fn, (uint32_t)param);
}

int codal::invoke(void (*entry_fn)(void *), void *param)
{
    if (entry_fn == NULL)
        return DEVICE_INVALID_PARAMETER;
    create_fiber(entry_fn, param);
    return DEVICE_OK;
}

int codal::invoke(void (*entry_fn)(void))
{
    return invoke((void (*)(void *))entry_fn, NULL);
}
