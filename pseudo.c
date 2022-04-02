#include <io.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct table_controller {
    unsigned short staticTable[0x1000000]; // static table
    unsigned short *dynamicTable;          // dynamic table
    int dynamicExtensions;
} table_controller_t;

int currentIndex = 0;

int main(int argc, char const *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Incorrect number of arguments\n\n");
        exit(EXIT_FAILURE);
    }
    int errorID;
    if (errorID = initializeIO(argv[1], argv[1]) != 0) {
        printIOExplanationError(errorID);
    }

    unsigned short staticTable[0x1000000];
    for (int i = 0; i < 0x1000000; i++) staticTable[i] = 0;
    
    // Declare temporal variables
    uint32_t *prefix;
    int *prefixLength;
    int *outInterface;
    table_controller_t *tableController;

    // Init controller
    *tableController->staticTable = staticTable;
    *tableController->dynamicTable = calloc(1, sizeof(unsigned short));
    tableController->dynamicExtensions = 0;

    // memset(tableController->dynamicExtensions, 0, 1);

    while (errorID = readFIBLine(prefix, prefixLength, outInterface) == 0) { // while no error or EOF
        fillTable(tableController, prefix, prefixLength, outInterface);
    }

    uint32_t *lookUpPrefix;
    int *accessedTables;
    struct timespec *start, *end; // https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/

    while (errorID = readInputPacketFileLine(lookUpInterface) == 0) {

        clock_gettime(_SC_MONOTONIC_CLOCK, &start);
        int interface = lookUpInterface(tableController, lookUpPrefix, accessedTables);
        clock_gettime(_SC_MONOTONIC_CLOCK, &end);

        double *searchingTime;
        *searchingTime = (end->tv_sec - start->tv_sec) * 1e9;
        *searchingTime = (*searchingTime + (end->tv_nsec - start->tv_nsec)) * 1e-9;

        printOutputLine(lookUpPrefix, interface, start, end, searchingTime, accessedTables);
        freeAllResources();
    }
    return 0;
}

int fillTable(table_controller_t *tableController, uint32_t *prefix, int *prefixLength, int *outInterface)
{
    int fillNpos = 0;
    if (*prefix <= 24) {
        fillNpos = pow(2, 24 - *prefixLength);
        for (int i = 0; i < fillNpos; i++) {
            // >> displaces 8 positions a binary number to the right, filling with 0s the left part
            // we want the first 24 bits of the 32 total bits of the IP, so we discard the last 8
            tableController->staticTable[*prefix >> 8 + i] = *outInterface;
        }
    } else {
        fillNpos = pow(2, 32 - *prefixLength);
        if (tableController->staticTable[*prefix >> 8] >> 15 == 0) {
            // As we can see in this flag, (the first bit is 0)
            // there isn't any reserved space for this IP in the dynamic table
            // so we must realloc
            tableController->dynamicTable = realloc(tableController->dynamicTable,
                                                    256 * sizeof(short) * (tableController->dynamicExtensions + 1));

            for (int i = 0; i < 256; i++) {
                tableController->dynamicTable[tableController->dynamicExtensions * 256 + i] =
                    tableController->staticTable[*prefix >> 8];
            }

            // If we do an AND operation with the given PREFIX and 0xFF (1111 1111), we
            // save the last 8 bits of that prefix. Those are the bits that will indicate us
            // the index of the dynamic table related with our prefix (as the first 24 out of 32)
            // are in the static table.
            for (int i = 0; i < fillNpos; i++) {
                tableController->dynamicTable[tableController->dynamicExtensions * 256 + (*prefix & 0xFF) + i] = *outInterface;
            }

            // Place a 1 in the first position of the 16 bits, as a flag,
            // followed by the extension number (in the remaining 15 bits)
            tableController->staticTable[*prefix >> 8] = tableController->dynamicExtensions | 0x8000;
            tableController->dynamicExtensions++;
        } else {

            // As there is a 1 in the flag, we just have to access the extension indicated
            // For that, we do an AND operation to ignore the first flag bit out of the total 16
            // And finally, also an AND operation to take just the last 8 bits (out of 32) of the address
            for (int i = 0; i < fillNpos; i++) {
                tableController->dynamicTable[tableController->staticTable[*prefix >> 8 & 0x7FFF] * 256 + (*prefix & 0xFF) + i] = *outInterface;
            }
        }
    }
    return 0;
}

int lookUpInterface(table_controller_t *tableController, uint32_t *lookUpPrefix, int *accessedTables)
{
    int interface = 0;
    if (tableController->staticTable[*lookUpPrefix >> 8] & 0x8000 == 0) {
        interface = tableController->staticTable[*lookUpPrefix >> 8];
        accessedTables = 1;
    } else {
        accessedTables = 2;
        interface = tableController->dynamicTable[((tableController->staticTable[*lookUpPrefix >> 8]) & 0x7FFF) * 256 + (*lookUpPrefix & 0xFF)];
    }
    return interface;
}