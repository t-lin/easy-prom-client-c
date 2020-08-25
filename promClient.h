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

/* NOTE: Yes, you're seeing this right. Implementation code in a header file!
 *       For now, this is a small project/library, and it's not being built
 *       into a shared library. Hence, I see no benefit from separating the
 *       implementation from the header declarations, other than to appease
 *       orthodoxy. If seeing this code triggers you, close the window.
 */

#ifndef _PROM_CLIENT_H_
#define _PROM_CLIENT_H_

#include <assert.h>
#include <string.h>

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
void* NewGaugeVec(const char* name, const char* help, int nLabels, const char** labels) {
    GoString gsLabels[nLabels];
    for (int i = 0; i < nLabels; i++) {
        gsLabels[i] = cStr2GoStr(labels[i]);
    }

    GoSlice gLabelSlice = {(void*)gsLabels, (GoInt)nLabels, (GoInt)nLabels};

    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewGaugeVec(gsName, gsHelp, gLabelSlice);
}

// nLabels: The number of label values that follow. Each value must be a c-string.
void* GaugeWithLabelValues(void* pGaugeVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

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

// nLabels: The number of labels in 'labels'
// labels: Array of c-string labels
void* NewCounterVec(const char* name, const char* help, int nLabels, const char** labels) {
    GoString gsLabels[nLabels];
    for (int i = 0; i < nLabels; i++) {
        gsLabels[i] = cStr2GoStr(labels[i]);
    }

    GoSlice gLabelSlice = {(void*)gsLabels, (GoInt)nLabels, (GoInt)nLabels};

    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewCounterVec(gsName, gsHelp, gLabelSlice);
}

// nLabels: The number of label values that follow. Each value must be a c-string.
void* CounterWithLabelValues(void* pCounterVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return (void*)goCounterWithLabelValues((GoUintptr)pCounterVec, gLabValSlice);
}

inline void CounterAdd(void* pCounter, double val) {
    goCounterAdd((GoUintptr)pCounter, (GoFloat64)val);

    return;
}

#ifdef __cplusplus
#include <string>
#include <vector>

using std::string;
using std::vector;

/* ========== C++ CLASSES ========== */
// Simply implement them as wrappers around the C functions
// TODO: Make base Metric class and derive everything else from it

class Gauge {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object
        string _name;
        string _help;

    public:
        Gauge(string name, string help) : _name(name), _help(help) {
            _metric = NewGauge(_name.c_str(), _help.c_str());
        }

        Gauge(string name, string help, void* pGauge) : _name(name), _help(help) {
            assert(pGauge != nullptr);
            _metric = pGauge;
        }

        ~Gauge() {}

        void Set(double val) {
            GaugeSet(_metric, val);
        }

        void Add(double val) {
            GaugeAdd(_metric, val);
        }

        void Sub(double val) {
            GaugeSub(_metric, val);
        }

};

class GaugeVec {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object
        string _name;
        string _help;
        vector<string> _labels;

    public:
        GaugeVec(string name, string help, vector<string> labels)
            : _name(name), _help(help), _labels(labels) {

            const char* cStrLabels[_labels.size()];
            for (unsigned int i = 0; i < _labels.size(); i++) {
                cStrLabels[i] = _labels[i].c_str();
            }
            _metric = NewGaugeVec(_name.c_str(), _help.c_str(), _labels.size(), cStrLabels);
        }

        ~GaugeVec() {}

        Gauge WithLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            void* pGauge = GaugeWithLabelValues(_metric, labelVals.size(), cStrLabelVals);
            return Gauge(_name, _help, pGauge);
        }
};
#endif

#endif
