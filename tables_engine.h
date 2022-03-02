

typedef struct table_controller {
    unsigned short table1[0x1000000];
    unsigned short *table2;
    int currentIndex;
    int extension;
} table_controller_t;