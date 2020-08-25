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

    // Test setting gauges created by NewGauge and GaugeVec.WithLabelValues
    double temp = 0;
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        SetGauge(testGauge, temp);
        SetGauge(testGauge2, temp);
        sleep(1);
    }

    // Test setting a second gauge created by GaugeVec.WithLabelValues
    void* testGauge3 = GaugeWithLabelValues(testGaugeVec, 2, "label-val-I", "label-val-II");
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        SetGauge(testGauge3, temp);
        sleep(1);
    }

    // Test adding and subtracting gauges
    for (int i = 0; i < NUM_ITER; i++) {
        if (i % 2 == 0) {
            printf("%d: Adding %lf to gauge\n", i + 1, MY_RAND_MAX);
            AddGauge(testGauge, MY_RAND_MAX);
        } else {
            printf("%d: Subtracting %lf from gauge\n", i + 1, MY_RAND_MAX);
            SubGauge(testGauge, MY_RAND_MAX);
        }
        sleep(1);
    }

    // Test adding counters created by NewCounter and CounerVec.WithLabelValues
    void* testCounter = NewCounter("test_counter", "Test counter's help");

    void* testCounterVec = NewCounterVec("testCounterVec", "Test counter vec", 2, "label1", "label2");
    void* testCounter2 = CounterWithLabelValues(testCounterVec, 2, "label-val-1", "label-val-2");

    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        AddCounter(testCounter, temp);
        AddCounter(testCounter2, temp);
        sleep(1);
    }

    return 0;
}
