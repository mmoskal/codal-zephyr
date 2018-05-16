#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

#include "CodalComponent.h"
#include "CodalFiber.h"
#include "MessageBus.h"

using namespace codal;

#define LED_PORT "GPIOC"

#define LED0 5
#define LED1 8
#define LED2 9

MessageBus devMessageBus;

void codal_init()
{
    scheduler_init(devMessageBus);

    for (int i = 0; i < DEVICE_COMPONENT_COUNT; i++)
    {
        if (CodalComponent::components[i])
            CodalComponent::components[i]->init();
    }
}

void main(void)
{
    int cnt = 0;
    struct device *dev;

    codal_init();

    dev = device_get_binding(LED_PORT);

    gpio_pin_configure(dev, LED0, GPIO_DIR_OUT);
    gpio_pin_configure(dev, LED1, GPIO_DIR_OUT);
    gpio_pin_configure(dev, LED2, GPIO_DIR_OUT);

    while (1)
    {
        gpio_pin_write(dev, LED0, cnt % 3 == 0);
        gpio_pin_write(dev, LED1, cnt % 3 == 1);
        gpio_pin_write(dev, LED2, cnt % 3 == 2);
        cnt++;
        k_sleep(100);
    }
}
