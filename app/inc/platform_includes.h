#ifndef PLATFORM_INCLUDES
#define PLATFORM_INCLUDES

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include <linker/linker-defs.h>

#define PROCESSOR_WORD_TYPE uint32_t
#define DEVICE_STACK_BASE __kernel_ram_end
#define DEVICE_STACK_SIZE 0

#endif
