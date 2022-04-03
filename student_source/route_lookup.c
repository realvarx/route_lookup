#include "io.h"
#include "tables_engine.h"
#include "utils.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <arpa/inet.h>

table_controller_t tableController;
unsigned short *dynamicTablePtr;

uint32_t *prefix;
int *prefixLength;
int *outInterface;

uint32_t *lookUpPrefix;
int *accessedTables;

double *searchingTime;

void freeResources();

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Incorrect number of arguments\n\n");
        exit(EXIT_FAILURE);
    }
    int errorID;

    if ((errorID = initializeIO(argv[1], argv[2])) != 0) {
        printIOExplanationError(errorID);
        exit(EXIT_FAILURE);
    }

    createTable(&tableController);

    prefix = calloc(1, sizeof(int));
    prefixLength = calloc(1, sizeof(int));
    outInterface = calloc(1, sizeof(int));

    while ((errorID = readFIBLine(prefix, prefixLength, outInterface)) == 0) { // while no error or EOF
        // printf("%s\n",inet_ntoa(*(struct in_addr *)prefix));
        fillTable(&tableController, prefix, prefixLength, outInterface);
    }

    lookUpPrefix = calloc(1, sizeof(int));
    accessedTables = calloc(1, sizeof(int));

    int processedCounter = 0;
    int accessesCounter = 0;
    double timeCounter = 0.0;

    struct timespec start, end; // https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/
    searchingTime = calloc(1, sizeof(double));

    while ((errorID = readInputPacketFileLine(lookUpPrefix)) == 0) {

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        int interface = lookUpInterface(&tableController, lookUpPrefix, accessedTables);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        printOutputLine(*lookUpPrefix, interface, &start, &end, searchingTime, *accessedTables);
        processedCounter++;
        accessesCounter += *accessedTables;
        timeCounter += *searchingTime;
    }
    // printf("Accesses counter %d\n", accessesCounter);

    printSummary(processedCounter, (double)((double)accessesCounter / (double)processedCounter), (double)(timeCounter / processedCounter));
    freeResources();

    exit(EXIT_SUCCESS);
}

void freeResources()
{
    freeIO();
    free(dynamicTablePtr);
    free(prefix);
    free(prefixLength);
    free(outInterface);
    free(lookUpPrefix);
    free(accessedTables);
    free(searchingTime);
}