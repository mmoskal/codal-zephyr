#include <zephyr.h>
#include <board.h>
#include <device.h>
#include <gpio.h>

#include "CodalComponent.h"
#include "CodalFiber.h"
#include "MessageBus.h"

#include "ZPin.h"
#include "ZSPI.h"
#include "ST7735.h"

using namespace codal;

MessageBus devMessageBus;

#define PINA(n) (0x00 | (n))
#define PINB(n) (0x10 | (n))
#define PINC(n) (0x20 | (n))

#define PIN(name, id) name(DEVICE_ID_IO_P0 + id, id, PIN_CAPABILITY_ALL)

class IO
{
public:
    ZPin led0, led1, led2;
    ZPin tftcs, tftsck, tftrst, tftsda, tftdc;
    ZSPI screenSPI;
    ST7735 screen;
    IO()
        : PIN(led0, PINC(5)), PIN(led1, PINC(8)), PIN(led2, PINC(9)), // leds
          PIN(tftcs, PINB(12)), PIN(tftsck, PINB(13)), PIN(tftrst, PINB(14)), PIN(tftsda, PINB(15)),
          PIN(tftdc, PINB(4)), //
          screenSPI(&tftsda, NULL, &tftsck),
          screen(screenSPI, tftcs, tftdc)
    {
    }
};

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

    codal_init();

    auto io = new IO();

    io->led0.setDigitalValue(1);
    io->screenSPI.setFrequency(8 * 1000 * 1000);
    io->screen.init();
    io->screen.configure(0x00, 0x000605);

    io->led0.setDigitalValue(0);

    while (1)
    {
        io->led0.setDigitalValue(cnt % 3 == 0);
        io->led1.setDigitalValue(cnt % 3 == 1);
        io->led2.setDigitalValue(cnt % 3 == 2);

        cnt++;

        fiber_sleep(100);
    }
}
