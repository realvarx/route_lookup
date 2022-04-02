#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables_engine.h"
#include "io.h"
#include "utils.h"
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>

#include <arpa/inet.h>

table_controller_t tableController;

uint32_t *prefix;
int *prefixLength;     
int *outInterface;


int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Incorrect number of arguments\n\n");
        exit(EXIT_FAILURE);
    }
    int errorID;
    
    if ((errorID = initializeIO(argv[1], argv[2])) != 0) {
        printIOExplanationError(errorID);
    }

    createTable(&tableController);
    
    prefix = calloc(1, sizeof(int));
    prefixLength = calloc(1, sizeof(int));
    outInterface = calloc(1, sizeof(int));

    
    while ((errorID = readFIBLine(prefix, prefixLength, outInterface)) == 0) { // while no error or EOF
        // printf("%s\n",inet_ntoa(*(struct in_addr *)prefix));
        fillTable(&tableController, prefix, prefixLength, outInterface);
    }

    uint32_t *lookUpPrefix = calloc(1, sizeof(int));
    int *accessedTables = calloc(1, sizeof(int));

    int processedCounter = 0;
    int accessesCounter = 0;
    double timeCounter = 0.0;

    struct timespec start, end; // https://www.geeksforgeeks.org/measure-execution-time-with-high-precision-in-c-c/

    while ((errorID = readInputPacketFileLine(lookUpPrefix)) == 0) {
        
        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        int interface = lookUpInterface(&tableController, lookUpPrefix, accessedTables);
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);

        double *searchingTime = calloc(1, sizeof(double));  
        printOutputLine(*lookUpPrefix, interface, &start, &end, searchingTime, *accessedTables);
        processedCounter++;
        accessesCounter += *accessedTables;
        timeCounter += *searchingTime;
    }
        // printf("Accesses counter %d\n", accessesCounter);

    printSummary(processedCounter, (double) ((double) accessesCounter/(double) processedCounter), (double) (timeCounter/processedCounter));
    
    return 0;
}