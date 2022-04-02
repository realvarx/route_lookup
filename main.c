#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables_engine.h"
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>

unsigned short table1[0x1000000];

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