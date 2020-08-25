#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <vector>
#include <string>

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
    Gauge testGauge = Gauge("test_gauge", "Test gauge's help");

    vector<string> labels = {"label1", "label2"};
    vector<string> labelVals = {"label-val-1", "label-val-2"};
    GaugeVec testGaugeVec = GaugeVec("testGaugeVec", "Test gauge vec", labels);
    Gauge testGauge2 = testGaugeVec.WithLabelValues(labelVals);

    // Test setting gauges created by NewGauge and GaugeVec.WithLabelValues
    double temp = 0;
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        testGauge.Set(temp);
        testGauge2.Set(temp);
        sleep(1);
    }

    // Test setting a second gauge created by GaugeVec.WithLabelValues
    vector<string> labelVals2 = {"label-val-I", "label-val-II"};
    Gauge testGauge3 = testGaugeVec.WithLabelValues(labelVals2);
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        testGauge3.Set(temp);
        sleep(1);
    }

    // Test adding and subtracting gauges
    for (int i = 0; i < NUM_ITER; i++) {
        if (i % 2 == 0) {
            printf("%d: Adding %lf to gauge\n", i + 1, MY_RAND_MAX);
            testGauge.Add(MY_RAND_MAX);
        } else {
            printf("%d: Subtracting %lf from gauge\n", i + 1, MY_RAND_MAX);
            testGauge.Sub(MY_RAND_MAX);
        }
        sleep(1);
    }

    // Test adding counters created by NewCounter and CounerVec.WithLabelValues
    Counter testCounter = Counter("test_counter", "Test counter's help");

    CounterVec testCounterVec = CounterVec("testCounterVec", "Test counter vec", labels);
    Counter testCounter2 = testCounterVec.WithLabelValues(labelVals);

    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting counters to %lf\n", i + 1, temp);
        testCounter.Add(temp);
        testCounter2.Add(temp);
        sleep(1);
    }

    return 0;
}
