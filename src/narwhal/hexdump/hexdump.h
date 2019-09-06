#ifndef NARWHAL_HEXDUMP_H
#define NARWHAL_HEXDUMP_H

#include <stdint.h>
#include <stdlib.h>

char *narwhal_hexdump(const uint8_t *buffer, size_t size, size_t bytes_per_row);
size_t narwhal_optimal_bytes_per_row(size_t element_size, size_t target, size_t range);

#endif
