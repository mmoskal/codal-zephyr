/*
The MIT License (MIT)

Copyright (c) 2017 Lancaster University.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "CodalConfig.h"
#include "ZSPI.h"
#include "ErrorNo.h"
#include "CodalDmesg.h"
#include "codal-core/inc/driver-models/Timer.h"
#include "board_pinmux.h"

namespace codal
{

#define ZERO(f) memset(&f, 0, sizeof(f))
/**
 * Constructor.
 */
ZSPI::ZSPI(Pin &mosi, Pin &miso, Pin &sclk)
    : codal::SPI()
{
    pinmux_setup_spi((int)mosi.name, (int)miso.name, (int)sclk.name, &dev);
    ZERO(config);
    ZERO(rxBuf);
    ZERO(txBuf);
    rxBufSet.buffers = &rxBuf;
    rxBufSet.count = 1;
    txBufSet.buffers = &txBuf;
    txBufSet.count = 1;
    setFrequency(1000000);
    setMode(0, 8);
}

/** Set the frequency of the SPI interface
 *
 * @param frequency The bus frequency in hertz
 */
int ZSPI::setFrequency(uint32_t frequency)
{
    config.frequency = frequency;
    return DEVICE_OK;
}

/** Set the mode of the SPI interface
 *
 * @param mode Clock polarity and phase mode (0 - 3)
 * @param bits Number of bits per SPI frame (4 - 16)
 *
 * @code
 * mode | POL PHA
 * -----+--------
 *   0  |  0   0
 *   1  |  0   1
 *   2  |  1   0
 *   3  |  1   1
 * @endcode
 */
int ZSPI::setMode(int mode, int bits)
{
    config.operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8) | SPI_LINES_SINGLE;

    if (mode & 1)
        config.operation |= SPI_MODE_CPHA;
    if (mode & 2)
        config.operation |= SPI_MODE_CPOL;

    return DEVICE_OK;
}

/**
 * Writes the given byte to the SPI bus.
 *
 * The CPU will wait until the transmission is complete.
 *
 * @param data The data to write.
 * @return Response from the SPI slave or DEVICE_SPI_ERROR if the the write request failed.
 */
int ZSPI::write(int data)
{
    rxCh = 0;
    txCh = data;
    if (transfer(&txCh, 1, &rxCh, 1) < 0)
        return DEVICE_SPI_ERROR;
    return rxCh;
}

/**
 * Writes and reads from the SPI bus concurrently. Waits (possibly un-scheduled) for transfer to
 * finish.
 *
 * Either buffer can be NULL.
 */
int ZSPI::transfer(const uint8_t *txBuffer, uint32_t txSize, uint8_t *rxBuffer, uint32_t rxSize)
{
    rxBuf.buf = rxBuffer;
    rxBuf.len = rxSize;
    txBuf.buf = (uint8_t*)txBuffer;
    txBuf.len = txSize;
    if (spi_transceive(dev, &config,  txSize ? &txBufSet : NULL, rxSize ? &rxBufSet : NULL) < 0)
        return DEVICE_SPI_ERROR;
}

} // namespace codal
