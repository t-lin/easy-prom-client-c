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

    // Create a test gauge
    void* testGauge = NewGauge("test_gauge", "Test gauge's help");

    const char* labels[2] = {"label1", "label2"};
    const char* labelVals[2] = {"label-val-1", "label-val-2"};
    int nLabels = sizeof(labels) / sizeof(labels[0]);

    void* testGaugeVec = NewGaugeVec("testGaugeVec", "Test gauge vec", nLabels, labels);
    void* testGauge2 = GaugeWithLabelValues(testGaugeVec, nLabels, labelVals);

    // Test setting gauges created by NewGauge and GaugeVec.WithLabelValues
    double temp = 0;
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        GaugeSet(testGauge, temp);
        GaugeSet(testGauge2, temp);
        sleep(1);
    }

    // Test setting a second gauge created by GaugeVec.WithLabelValues
    const char* labelVals2[2] = {"label-val-I", "label-val-II"};
    void* testGauge3 = GaugeWithLabelValues(testGaugeVec, nLabels, labelVals2);
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        GaugeSet(testGauge3, temp);
        sleep(1);
    }

    // Test adding and subtracting gauges
    for (int i = 0; i < NUM_ITER; i++) {
        if (i % 2 == 0) {
            printf("%d: Adding %lf to gauge\n", i + 1, MY_RAND_MAX);
            GaugeAdd(testGauge, MY_RAND_MAX);
        } else {
            printf("%d: Subtracting %lf from gauge\n", i + 1, MY_RAND_MAX);
            GaugeSub(testGauge, MY_RAND_MAX);
        }
        sleep(1);
    }

    // Test adding counters created by NewCounter and CounerVec.WithLabelValues
    void* testCounter = NewCounter("test_counter", "Test counter's help");

    void* testCounterVec = NewCounterVec("testCounterVec", "Test counter vec", nLabels, labels);
    void* testCounter2 = CounterWithLabelValues(testCounterVec, nLabels, labelVals);

    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting counters to %lf\n", i + 1, temp);
        CounterAdd(testCounter, temp);
        CounterAdd(testCounter2, temp);
        sleep(1);
    }

    // Test adding counters created by NewCounter and CounerVec.WithLabelValues
    // Since there's no "map" data structure in C, we use two arrays to specify
    // the quantiles and their errors.
    int nQuantiles = 3;
    double quantiles[3] = {0.5, 0.9, 0.99};
    double errors[3] = {0.05, 0.01, 0.001};
    int nMaxAge = 60; // Seconds
    int nAgeBkts = 5; // nMaxAge is split into nAgeBkts buckets of observations.
                      // In this case, each bucket holds 12 seconds of observations.
                      // When 60 seconds is up, observations in the last bucket is
                      // dropped, and a fresh bucket is created at the front. Hence,
                      // it's a sliding window of buckets.
    void* testSummary = NewSummary("test_summary", "Test summary's help",
            nQuantiles, quantiles, errors, nMaxAge, nAgeBkts);

    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Updating summary w/ observation %lf\n", i + 1, temp);
        SummaryObserve(testSummary, temp);
        sleep(1);
    }

    return 0;
}
