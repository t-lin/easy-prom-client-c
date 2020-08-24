#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "promClient.h"

#define NUM_ITER 10
#define MY_RAND_MAX 1000.0

// Generates random value between [0, MY_RAND_MAX]
double generateRandVal() {
    return (double)rand() / ((double)RAND_MAX / MY_RAND_MAX);
}

int main() {
    srand(time(NULL)); // Pseudo-random seed

    printf("Before starting prom server...\n");
    StartPromServer(":12345", "/metrics");
    printf("After starting prom server...\n");

    // Create a gauge for temperature
    void* testGauge = NewGauge("test_gauge", "Test gauge's help");

    void* testGaugeVec = NewGaugeVec("testGaugeVec", "Test gauge vec", 2, "label1", "label2");
    void* testGauge2 = GaugeWithLabelValues(testGaugeVec, 2, "label-val-1", "label-val-2");

    double temp = 0;
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        SetGauge(testGauge, temp);
        SetGauge(testGauge2, temp);
        sleep(1);
    }

    void* testGauge3 = GaugeWithLabelValues(testGaugeVec, 2, "label-val-I", "label-val-II");
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        SetGauge(testGauge3, temp);
        sleep(1);
    }

    return 0;
}
