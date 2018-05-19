#ifndef __INC_BOARD_PINMUX_H
#define __INC_BOARD_PINMUX_H

#ifdef __cplusplus
extern "C"
{
#endif

    int pinmux_setup_pwm(int pin, struct device **dev, int *channel);
    int pinmux_setup_spi(int mosi, int miso, int sck, struct device **dev);

#if defined(__cplusplus)
}
#endif

#endif /* __INC_BOARD_PINMUX_H */
