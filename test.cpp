#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <vector>
#include <string>
#include <unordered_map>

#include "promClient.h"

#define NUM_ITER 10
#define MY_RAND_MAX 1000.0

using namespace std;
using namespace EasyProm;

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

    testGaugeVec.DeleteLabelValues(labelVals);

    // Test setting a second gauge created by GaugeVec.WithLabelValues
    labelVals[0] = "label-val-I"; labelVals[1] = "label-val-II";
    Gauge testGauge3 = testGaugeVec.WithLabelValues(labelVals);
    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Setting gauge to %lf\n", i + 1, temp);
        testGauge3.Set(temp);
        sleep(1);
    }

    testGaugeVec.DeleteLabelValues(labelVals);

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
    labelVals[0] = "label-val-ONE"; labelVals[1] = "label-val-TWO";
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

    testCounterVec.DeleteLabelValues(labelVals);

    // Test adding summary created by NewSummary and SummaryVec.WithLabelValues
    labelVals[0] = "label-val-UN"; labelVals[1] = "label-val-DEUX";

    // Since there's no "map" data structure in C, we use two arrays to specify
    // the quantiles and their errors.
    unordered_map<double, double> objectives = {{0.5, 0.05}, {0.9, 0.01}, {0.99, 0.001}};
    int nMaxAge = 60; // Seconds
    int nAgeBkts = 5; // nMaxAge is split into nAgeBkts buckets of observations.
                      // In this case, each bucket holds 12 seconds of observations.
                      // When 60 seconds is up, observations in the last bucket is
                      // dropped, and a fresh bucket is created at the front. Hence,
                      // it's a sliding window of buckets.
    Summary testSummary = Summary("test_summary", "Test summary's help",
                                objectives, nMaxAge, nAgeBkts);
    SummaryVec testSummaryVec = SummaryVec("testSummaryVec", "Test summary vec",
                                        labels, objectives, nMaxAge, nAgeBkts);
    Summary testSummary2 = testSummaryVec.WithLabelValues(labelVals);

    for (int i = 0; i < NUM_ITER; i++) {
        temp = generateRandVal();
        printf("%d: Updating summary w/ observation %lf\n", i + 1, temp);
        testSummary.Observe(temp);
        testSummary2.Observe(temp);
        sleep(1);
    }

    testSummaryVec.DeleteLabelValues(labelVals);

    return 0;
}
