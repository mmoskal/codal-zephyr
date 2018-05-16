#include "CodalFiber.h"

using namespace codal;

static EventModel *messageBus = NULL;

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

    messageBus = &_messageBus;

    if (messageBus)
    {
        messageBus->listen(DEVICE_ID_NOTIFY, DEVICE_EVT_ANY, scheduler_event,
                           MESSAGE_BUS_LISTENER_IMMEDIATE);
        messageBus->listen(DEVICE_ID_NOTIFY_ONE, DEVICE_EVT_ANY, scheduler_event,
                           MESSAGE_BUS_LISTENER_IMMEDIATE);
    }
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

struct EventAlert
{
    EventAlert *next;
    k_sem sem;
    u16_t id;
    u16_t value;
};

static EventAlert *alerts;

static void activate(EventAlert *prev, EventAlert *p)
{
    k_sem_give(&p->sem);
    if (prev)
        prev->next = p->next;
    else
        alerts = p->next;
    delete p;
}

void codal::scheduler_event(Event evt)
{
    EventAlert *next, *prev = NULL;
    int notifyOneComplete = 0;

    int key = irq_lock();

    for (auto p = alerts; p; p = next)
    {
        next = p->next;

        if ((evt.source == DEVICE_ID_NOTIFY_ONE && p->id == DEVICE_ID_NOTIFY) &&
            (p->value == DEVICE_EVT_ANY || p->value == evt.value))
        {
            if (!notifyOneComplete)
            {
                notifyOneComplete = 1;
                activate(prev, p);
            }
            else
            {
                prev = p;
            }
        }
        else if ((p->id == DEVICE_ID_ANY || p->id == evt.source) &&
                 (p->value == DEVICE_EVT_ANY || p->value == evt.value))
        {
            activate(prev, p);
        }
        else
        {
            prev = p;
        }
    }

    // Unregister this event, as we've woken up all the fibers with this match.
    if (evt.source != DEVICE_ID_NOTIFY && evt.source != DEVICE_ID_NOTIFY_ONE)
        messageBus->ignore(evt.source, evt.value, scheduler_event);

    irq_unlock(key);
}

void codal::fiber_sleep(unsigned long t)
{
    k_sleep(t);
}

int codal::fiber_wait_for_event(uint16_t id, uint16_t value)
{
    auto e = new EventAlert();
    e->id = id;
    e->value = value;
    k_sem_init(&e->sem, 0, 1);

    int key = irq_lock();
    e->next = alerts;
    alerts = e;
    irq_unlock(key);

    k_sem_take(&e->sem, K_FOREVER);

    return DEVICE_OK;
}
