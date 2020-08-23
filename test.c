#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "libpromclient.h"

#define NUM_BUFS 100

GoString cStr2GoStr(const char *in) {
    GoString tmp = {in, (ptrdiff_t)strlen(in)};
    return tmp;
}

int main() {

    printf("Before starting prom server...\n");

    GoString gsPromEnd = cStr2GoStr(":12345");
    GoString gsMetricsPath = cStr2GoStr("/metrics");
    StartPromServer(gsPromEnd, gsMetricsPath);

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
