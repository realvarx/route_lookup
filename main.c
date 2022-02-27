#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if(argc != 3) {
        fprintf(stderr, "Incorrect number of arguments\n\n");
        fprintf(stderr, "usage: %s <FIB file> <Input packet file>\n", argv[0]);
		exit(EXIT_FAILURE);
    }

    int errorID;

    if(errorID = initializeIO(argv[1], argv[1]) != 0) {
        printIOExplanationError(errorID);
    }

    

    return 0;
}
