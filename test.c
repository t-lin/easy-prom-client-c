#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "promClient.h"

#define NUM_BUFS 100

int main() {

    printf("Before starting prom server...\n");

    StartPromServer(":12345", "/metrics");

    printf("After starting prom server...\n");

    char *bufs[NUM_BUFS] = {NULL};
    for (int i = 0; i < NUM_BUFS; i++) {
        printf("%d: Allocated 1 MB...\n", i + 1);
        bufs[i] = malloc(1024 * 1024);
        sleep(1);
    }

    for (int i = 0; i < NUM_BUFS; i++) {
        if (bufs[i] != NULL) {
            free(bufs[i]);
        }
    }

    return 0;
}
