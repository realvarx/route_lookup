#include "io.h"
#include "ipv4_engine.h"
#include <stdint.h>
#include <math.h>

// https://stackoverflow.com/questions/1810083/c-pointers-pointing-to-an-array-of-fixed-size

int fillTables(unsigned short (*table)[0x1000000], int *index)
{
    ipv4_addr *prefix;
    int *prefixLength;
    int *outInterface;
    int fillNumber;
    int err;
    ipv4_addr *prev = 0;
    while (err = readFIBLine(prefix, prefixLength, outInterface) == 0) {
        // add the route
        if (*prefixLength <= 24) {
            fillNumber = pow(2, 24 - *prefixLength);    // 2^(24-prefix)
            for(int i = index; i<fillNumber; i++) {
                *table[((uint32_t)prefix >> 8) + i] = outInterface;  // https://stackoverflow.com/questions/19153363/what-does-hibyte-value-8-meaning
            }
        } else {
            fillNumber = pow(2, 32 - *prefixLength);    // 2^(24-prefix)
        }   
    }
}