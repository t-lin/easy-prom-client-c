#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "promClient.h"

#define NUM_ITER 100
#define TEMP_MAX 100.0

// Generates random temperature between [0, TEMP_MAX]
double generateRandTemp() {
    return (double)rand() / ((double)RAND_MAX / TEMP_MAX);
}

int main() {
    srand(time(NULL)); // Pseudo-random seed

    printf("Before starting prom server...\n");
    StartPromServer(":12345", "/metrics");
    printf("After starting prom server...\n");

    // Create a gauge for temperature
    void* tempGauge = NewGauge("temperature", "Test temperature gauge");

    double temp = 0;
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandTemp();
        printf("%d: Setting temperature to %lf\n", i + 1, temp);
        SetGauge(tempGauge, temp);
        sleep(1);
    }

    return 0;
}
