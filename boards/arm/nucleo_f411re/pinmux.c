#include <kernel.h>
#include <device.h>
#include <init.h>
#include <pinmux.h>
#include <sys_io.h>

#include <pinmux/stm32/pinmux_stm32.h>

#include "stm32f4xx_hal.h"
#include "board_pinmux.h"

#define LINE_PWM 0
#define LINE_SDA 1
#define LINE_SCL 2
#define LINE_SCK 3
#define LINE_MISO 4
#define LINE_MOSI 5
#define LINE_SSEL 6
#define LINE_RX 7
#define LINE_TX 8
#define LINE_CTS 9
#define LINE_RTS 10
#define LINE_ADC 11

typedef enum
{
    STM_PIN_INPUT = 0,
    STM_PIN_OUTPUT = 1,
    STM_PIN_ALTERNATE = 2,
    STM_PIN_ANALOG = 3,
    STM_PIN_OD_BITS = 4, // different than mbed
} StmPinFunction;

#define STM_MODE_INPUT (STM_PIN_INPUT)
#define STM_MODE_OUTPUT_PP (STM_PIN_OUTPUT)
#define STM_MODE_OUTPUT_OD (STM_PIN_OUTPUT | STM_PIN_OD_BITS)
#define STM_MODE_AF_PP (STM_PIN_ALTERNATE)
#define STM_MODE_AF_OD (STM_PIN_ALTERNATE | STM_PIN_OD_BITS)
#define STM_MODE_ANALOG (STM_PIN_ANALOG)
#define STM_MODE_ANALOG_ADC_CONTROL (STM_PIN_ANALOG | STM_PIN_ANALOG_CONTROL_BIT)

#define PIN_CFG(pin, periph, line, mode, pull, af)                                                 \
    PIN_CFG_EXT(pin, periph, line, mode, pull, af, 0, 0)

#define PIN_CFG_EXT(pin, periph, line, mode, pull, af, chan, inv)                                  \
    {                                                                                              \
        {STM32_PIN_##pin, ((mode) << STM32_MODER_SHIFT) | ((pull) << STM32_PUPDR_SHIFT) | (af)},   \
            #periph, LINE_##line, chan, inv                                                        \
    }

struct ext_pin_config
{
    struct pin_config cfg;
    const char *device_name;
    u8_t line;
    u8_t channel;
    u8_t inverted;
};

#undef ADC
#define ADC(pin, ch) PIN_CFG_EXT(pin, ADC1, ADC, STM_MODE_ANALOG, GPIO_NOPULL, 0, ch, 0)

#define PWM(pin, pwm, af, ch, inv)                                                                 \
    PIN_CFG_EXT(pin, pwm, PWM, STM_MODE_AF_PP, GPIO_PULLUP, af, ch, inv)

const struct ext_pin_config pins[] = {
    ADC(PA0, 0),  // ADC1_IN0DC
    ADC(PA1, 1),  // ADC1_IN1
    ADC(PA2, 2),  // ADC1_IN2
    ADC(PA3, 3),  // ADC1_IN3
    ADC(PA4, 4),  // ADC1_IN4
    ADC(PA5, 5),  // ADC1_IN5
    ADC(PA6, 6),  // ADC1_IN6
    ADC(PA7, 7),  // ADC1_IN7
    ADC(PB0, 8),  // ADC1_IN8
    ADC(PB1, 9),  // ADC1_IN9
    ADC(PC0, 10), // ADC1_IN10
    ADC(PC1, 11), // ADC1_IN11
    ADC(PC2, 12), // ADC1_IN12
    ADC(PC3, 13), // ADC1_IN13
    ADC(PC4, 14), // ADC1_IN14
    ADC(PC5, 15), // ADC1_IN15

    PIN_CFG(PB3, I2C_2, SDA, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF9_I2C2),
    PIN_CFG(PB4, I2C_3, SDA, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF9_I2C3),
    PIN_CFG(PB7, I2C_1, SDA, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1),
    PIN_CFG(PB9, I2C_1, SDA, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1), // ARDUINO
    PIN_CFG(PC9, I2C_3, SDA, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3),

    PIN_CFG(PA8, I2C_3, SCL, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C3),
    PIN_CFG(PB6, I2C_1, SCL, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1),
    PIN_CFG(PB8, I2C_1, SCL, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C1), // ARDUINO
    PIN_CFG(PB10, I2C_2, SCL, STM_MODE_AF_OD, GPIO_NOPULL, GPIO_AF4_I2C2),

    // the PWM list should be ordered, so that multiple PWM pins using the same TIM* instance are
    // minimized (it isn't yet)

    PWM(PA0, PWM_2, GPIO_AF1_TIM2, 1, 0),  // TIM2_CH1
    PWM(PA0, PWM_5, GPIO_AF2_TIM5, 1, 0),  // TIM5_CH1
    PWM(PA1, PWM_2, GPIO_AF1_TIM2, 2, 0),  // TIM2_CH2
    PWM(PA1, PWM_5, GPIO_AF2_TIM5, 2, 0),  // TIM5_CH2
    PWM(PA2, PWM_2, GPIO_AF1_TIM2, 3, 0),  // TIM2_CH3
    PWM(PA2, PWM_5, GPIO_AF2_TIM5, 3, 0),  // TIM5_CH3
    PWM(PA2, PWM_9, GPIO_AF3_TIM9, 1, 0),  // TIM9_CH1
    PWM(PA3, PWM_2, GPIO_AF1_TIM2, 4, 0),  // TIM2_CH4
    PWM(PA3, PWM_5, GPIO_AF2_TIM5, 4, 0),  // TIM5_CH4
    PWM(PA3, PWM_9, GPIO_AF3_TIM9, 2, 0),  // TIM9_CH2
    PWM(PA5, PWM_2, GPIO_AF1_TIM2, 1, 0),  // TIM2_CH1
    PWM(PA6, PWM_3, GPIO_AF2_TIM3, 1, 0),  // TIM3_CH1
    PWM(PA7, PWM_1, GPIO_AF1_TIM1, 1, 1),  // TIM1_CH1N - ARDUINO
    PWM(PA7, PWM_3, GPIO_AF2_TIM3, 2, 0),  // TIM3_CH2 - ARDUINO
    PWM(PA8, PWM_1, GPIO_AF1_TIM1, 1, 0),  // TIM1_CH1
    PWM(PA9, PWM_1, GPIO_AF1_TIM1, 2, 0),  // TIM1_CH2
    PWM(PA10, PWM_1, GPIO_AF1_TIM1, 3, 0), // TIM1_CH3
    PWM(PA11, PWM_1, GPIO_AF1_TIM1, 4, 0), // TIM1_CH4
    PWM(PA15, PWM_2, GPIO_AF1_TIM2, 1, 0), // TIM2_CH1

    PWM(PB0, PWM_1, GPIO_AF1_TIM1, 2, 1),   // TIM1_CH2N
    PWM(PB0, PWM_3, GPIO_AF2_TIM3, 3, 0),   // TIM3_CH3
    PWM(PB1, PWM_1, GPIO_AF1_TIM1, 3, 1),   // TIM1_CH3N
    PWM(PB1, PWM_3, GPIO_AF2_TIM3, 4, 0),   // TIM3_CH4
    PWM(PB3, PWM_2, GPIO_AF1_TIM2, 2, 0),   // TIM2_CH2 - ARDUINO
    PWM(PB4, PWM_3, GPIO_AF2_TIM3, 1, 0),   // TIM3_CH1 - ARDUINO
    PWM(PB5, PWM_3, GPIO_AF2_TIM3, 2, 0),   // TIM3_CH2
    PWM(PB6, PWM_4, GPIO_AF2_TIM4, 1, 0),   // TIM4_CH1 - ARDUINO
    PWM(PB7, PWM_4, GPIO_AF2_TIM4, 2, 0),   // TIM4_CH2
    PWM(PB8, PWM_4, GPIO_AF2_TIM4, 3, 0),   // TIM4_CH3
    PWM(PB8, PWM_10, GPIO_AF3_TIM10, 1, 0), // TIM10_CH1
    PWM(PB9, PWM_4, GPIO_AF2_TIM4, 4, 0),   // TIM4_CH4
    PWM(PB9, PWM_11, GPIO_AF3_TIM11, 1, 0), // TIM11_CH1
    PWM(PB10, PWM_2, GPIO_AF1_TIM2, 3, 0),  // TIM2_CH3 - ARDUINO
    PWM(PB13, PWM_1, GPIO_AF1_TIM1, 1, 1),  // TIM1_CH1N
    PWM(PB14, PWM_1, GPIO_AF1_TIM1, 2, 1),  // TIM1_CH2N
    PWM(PB15, PWM_1, GPIO_AF1_TIM1, 3, 1),  // TIM1_CH3N

    PWM(PC6, PWM_3, GPIO_AF2_TIM3, 1, 0), // TIM3_CH1
    PWM(PC7, PWM_3, GPIO_AF2_TIM3, 2, 0), // TIM3_CH2 - ARDUINO
    PWM(PC8, PWM_3, GPIO_AF2_TIM3, 3, 0), // TIM3_CH3
    PWM(PC9, PWM_3, GPIO_AF2_TIM3, 4, 0), // TIM3_CH4

    PIN_CFG(PA2, UART2, TX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2),
    PIN_CFG(PA9, UART1, TX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1),
    PIN_CFG(PA11, UART6, TX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6),
    PIN_CFG(PB6, UART1, TX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1),
    PIN_CFG(PC6, UART6, TX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6),

    PIN_CFG(PA3, UART2, RX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2),
    PIN_CFG(PA10, UART1, RX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1),
    PIN_CFG(PA12, UART6, RX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6),
    PIN_CFG(PB7, UART1, RX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1),
    PIN_CFG(PC7, UART6, RX, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF8_USART6),

    PIN_CFG(PA1, UART2, RTS, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2),
    PIN_CFG(PA12, UART1, RTS, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1),

    PIN_CFG(PA0, UART2, CTS, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART2),
    PIN_CFG(PA11, UART1, CTS, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF7_USART1),

    PIN_CFG(PA7, SPI_1, MOSI, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1), // ARDUINO
    PIN_CFG(PB5, SPI_1, MOSI, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1),
    // PIN_CFG(PB5, SPI_3, MOSI, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3),
    PIN_CFG(PB15, SPI_2, MOSI, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2),
    PIN_CFG(PC3, SPI_2, MOSI, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2),
    PIN_CFG(PC12, SPI_3, MOSI, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3),

    PIN_CFG(PA6, SPI_1, MISO, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1), // ARDUINO
    PIN_CFG(PB4, SPI_1, MISO, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1),
    // PIN_CFG(PB4, SPI_3, MISO, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3),
    PIN_CFG(PB14, SPI_2, MISO, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2),
    PIN_CFG(PC2, SPI_2, MISO, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2),
    PIN_CFG(PC11, SPI_3, MISO, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3),

    PIN_CFG(PA5, SPI_1, SCK, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1), // ARDUINO
    PIN_CFG(PB3, SPI_1, SCK, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI1),
    // PIN_CFG(PB3, SPI_3, SCK, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3),
    PIN_CFG(PB10, SPI_2, SCK, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2),
    PIN_CFG(PB13, SPI_2, SCK, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF5_SPI2),
    PIN_CFG(PC10, SPI_3, SCK, STM_MODE_AF_PP, GPIO_PULLUP, GPIO_AF6_SPI3),

    PIN_CFG(PA4, SPI_1, SSEL, STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI1),
    PIN_CFG(PA4, SPI_3, SSEL, STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3),
    PIN_CFG(PA15, SPI_1, SSEL, STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI1),
    PIN_CFG(PA15, SPI_3, SSEL, STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF6_SPI3),
    PIN_CFG(PB9, SPI_2, SSEL, STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI2),
    PIN_CFG(PB12, SPI_2, SSEL, STM_MODE_AF_PP, GPIO_NOPULL, GPIO_AF5_SPI2),

    {{0, 0}, 0, 0, 0, 0}};

static int pinmux_setup(int pin, struct device **dev, int *channel, int line)
{
    for (const struct ext_pin_config *p = pins; p->device_name; p++)
        if (p->cfg.pin_num == pin && p->line == line)
        {
            struct device *curr = device_get_binding(p->device_name);
            if (!curr)
                return -EINVAL;
            if (*dev && *dev != curr)
                return -EINVAL;
            *dev = curr;
            if (channel)
                *channel = p->inverted ? -p->channel : p->channel;
            stm32_setup_pins(&p->cfg, 1);
            return 0;
        }
    return -EINVAL;
}

int pinmux_setup_pwm(int pin, struct device **dev, int *channel)
{
    *dev = NULL;
    return pinmux_setup(pin, dev, channel, LINE_PWM);
}

int pinmux_setup_spi(int mosi, int miso, int sck, struct device **dev)
{
    *dev = NULL;
    return                                          //
        pinmux_setup(mosi, dev, NULL, LINE_MOSI) || //
        pinmux_setup(miso, dev, NULL, LINE_MISO) || //
        pinmux_setup(sck, dev, NULL, LINE_SCK);
}

// TODO i2c uart

/* pin assignments for NUCLEO-F411RE board */
static const struct pin_config pinconf[] = {
#ifdef CONFIG_UART_STM32_PORT_1
    {STM32_PIN_PB6, STM32F4_PINMUX_FUNC_PB6_USART1_TX},
    {STM32_PIN_PB7, STM32F4_PINMUX_FUNC_PB7_USART1_RX},
#endif /* CONFIG_UART_STM32_PORT_1 */
#ifdef CONFIG_UART_STM32_PORT_2
    {STM32_PIN_PA2, STM32F4_PINMUX_FUNC_PA2_USART2_TX},
    {STM32_PIN_PA3, STM32F4_PINMUX_FUNC_PA3_USART2_RX},
#endif /* CONFIG_UART_STM32_PORT_2 */
};

static int pinmux_stm32_init(struct device *port)
{
    ARG_UNUSED(port);

    stm32_setup_pins(pinconf, ARRAY_SIZE(pinconf));

    return 0;
}

SYS_INIT(pinmux_stm32_init, PRE_KERNEL_1, CONFIG_PINMUX_STM32_DEVICE_INITIALIZATION_PRIORITY);
