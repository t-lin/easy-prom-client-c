/* Copyright 2020 Thomas Lin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _PROM_CLIENT_H_
#define _PROM_CLIENT_H_

#include <stdarg.h>

#include "libpromclient.h"

/* ========== HELPER FUNCTIONS ========== */
inline GoString cStr2GoStr(const char* in) {
    GoString tmp = {in, (ptrdiff_t)strlen(in)};
    return tmp;
}


/* ========== WRAPPER FUNCTIONS FOR GO CODE ========== */
void StartPromServer(const char* promEndpoint, const char* metricsPath) {
    GoString gsPromEnd = cStr2GoStr(promEndpoint);
    GoString gsMetricsPath = cStr2GoStr(metricsPath);
    goStartPromServer(gsPromEnd, gsMetricsPath);

    return;
}

/* ========== GAUGE WRAPPER FUNCTIONS ========== */
inline void* NewGauge(const char* name, const char* help) {
    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewGauge(gsName, gsHelp);
}

// nLabels: The number of labels that follow. Each label must be a c-string.
void* NewGaugeVec(const char* name, const char* help, int nLabels, ...) {
    va_list vlLabels;
    va_start(vlLabels, nLabels);

    char* tmp = NULL;
    GoString gsLabels[nLabels];
    for (int i = 0; i < nLabels; i++) {
        tmp = va_arg(vlLabels, char*);
        gsLabels[i] = cStr2GoStr(tmp);
    }
    va_end(vlLabels);

    GoSlice gLabelSlice = {(void*)gsLabels, (GoInt)nLabels, (GoInt)nLabels};

    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewGaugeVec(gsName, gsHelp, gLabelSlice);
}

// nLabels: The number of label values that follow. Each value must be a c-string.
void* GaugeWithLabelValues(void* pGaugeVec, int nLabelVals, ...) {
    va_list vlLabelVals;
    va_start(vlLabelVals, nLabelVals);

    char* tmp = NULL;
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        tmp = va_arg(vlLabelVals, char*);
        gsLabelVals[i] = cStr2GoStr(tmp);
    }
    va_end(vlLabelVals);

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return (void*)goGaugeWithLabelValues((GoUintptr)pGaugeVec, gLabValSlice);
}

inline void GaugeSet(void* pGauge, double val) {
    goGaugeSet((GoUintptr)pGauge, (GoFloat64)val);

    return;
}

inline void GaugeAdd(void* pGauge, double val) {
    goGaugeAdd((GoUintptr)pGauge, (GoFloat64)val);

    return;
}

inline void GaugeSub(void* pGauge, double val) {
    goGaugeSub((GoUintptr)pGauge, (GoFloat64)val);

    return;
}

/* ========== COUNTER WRAPPER FUNCTIONS ========== */
inline void* NewCounter(const char* name, const char* help) {
    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewCounter(gsName, gsHelp);
}

// nLabels: The number of labels that follow. Each label must be a c-string.
void* NewCounterVec(const char* name, const char* help, int nLabels, ...) {
    va_list vlLabels;
    va_start(vlLabels, nLabels);

    char* tmp = NULL;
    GoString gsLabels[nLabels];
    for (int i = 0; i < nLabels; i++) {
        tmp = va_arg(vlLabels, char*);
        gsLabels[i] = cStr2GoStr(tmp);
    }
    va_end(vlLabels);

    GoSlice gLabelSlice = {(void*)gsLabels, (GoInt)nLabels, (GoInt)nLabels};

    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewCounterVec(gsName, gsHelp, gLabelSlice);
}

// nLabels: The number of label values that follow. Each value must be a c-string.
void* CounterWithLabelValues(void* pCounterVec, int nLabelVals, ...) {
    va_list vlLabelVals;
    va_start(vlLabelVals, nLabelVals);

    char* tmp = NULL;
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        tmp = va_arg(vlLabelVals, char*);
        gsLabelVals[i] = cStr2GoStr(tmp);
    }
    va_end(vlLabelVals);

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return (void*)goCounterWithLabelValues((GoUintptr)pCounterVec, gLabValSlice);
}

inline void CounterAdd(void* pCounter, double val) {
    goCounterAdd((GoUintptr)pCounter, (GoFloat64)val);

    return;
}

#endif
