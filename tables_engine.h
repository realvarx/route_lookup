#ifndef TABLES_ENGINE
#define TABLES_ENGINE "tables_engine.h"

#include <stdint.h>
#include <math.h>

#define STATIC_TABLE_LEN 16777216

typedef struct table_controller {
    unsigned short staticTable[0x1000000]; // static table
    unsigned short *dynamicTable;          // dynamic table
    int dynamicExtensions;
} table_controller_t;

int fillTable(table_controller_t *tableController, uint32_t *prefix, 
                int *prefixLength, int *outInterface);

int lookUpInterface(table_controller_t *tableController, 
                uint32_t *lookUpPrefix, int *accessedTables);

void createTable(table_controller_t *tableController);

#endif