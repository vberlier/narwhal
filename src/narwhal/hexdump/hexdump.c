#include "narwhal/hexdump/hexdump.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "narwhal/test/test.h"

char *narwhal_hexdump(const uint8_t *buffer, size_t size, size_t bytes_per_row)
{
    size_t dump_size;
    char *dump;
    FILE *stream = open_memstream(&dump, &dump_size);

    size_t offset = 0;

    while (offset < size)
    {
        fprintf(stream, "%06lX  ", offset);

        for (size_t i = 0; i < bytes_per_row; i++)
        {
            if (offset + i < size)
            {
                fprintf(stream, "%02X ", buffer[offset + i]);
            }
            else
            {
                fprintf(stream, "-- ");
            }
        }

        fprintf(stream, " ");

        for (size_t i = 0; i < bytes_per_row; i++)
        {
            if (offset + i < size)
            {
                uint8_t byte = buffer[offset + i];
                fprintf(stream, "%c", isprint(byte) ? byte : '.');
            }
            else
            {
                fprintf(stream, " ");
            }
        }

        offset += bytes_per_row;

        if (offset < size)
        {
            fprintf(stream, "\n");
        }
    }

    fclose(stream);

    auto_free(dump);

    return dump;
}

size_t narwhal_optimal_bytes_per_row(size_t element_size, size_t target, size_t range)
{
    size_t min = target - range;
    size_t max = target + range;

    if (element_size < min)
    {
        return (size_t)((double)target / (double)element_size + 0.5) * element_size;
    }

    if (element_size > max)
    {
        if (element_size % target == 0)
        {
            return target;
        }

        size_t div_min = target;
        size_t div_max = target;

        while (div_min > min || div_max < max)
        {
            if (div_min > min)
            {
                div_min--;

                if (element_size % div_min == 0)
                {
                    return div_min;
                }
            }

            if (div_max < max)
            {
                div_max++;

                if (element_size % div_max == 0)
                {
                    return div_max;
                }
            }
        }

        return target;
    }

    return element_size;
}
