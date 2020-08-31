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
 *       into a shared library. Hence, I see no benefit to separate the
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
void StartPromHandler(const char* promEndpoint, const char* metricsPath) {
    GoString gsPromEnd = cStr2GoStr(promEndpoint);
    GoString gsMetricsPath = cStr2GoStr(metricsPath);
    goStartPromHandler(gsPromEnd, gsMetricsPath);

    return;
}

/* ========== GAUGE WRAPPER FUNCTIONS ========== */
inline void* NewGauge(const char* name, const char* help) {
    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewGauge(gsName, gsHelp);
}

// nLabels: The number of labels in 'labels'
// labels: Array of c-string labels
void* NewGaugeVec(const char* name, const char* help, int nLabels, const char** labels) {
    GoString gsLabels[nLabels];
    for (int i = 0; i < nLabels; i++) {
        gsLabels[i] = cStr2GoStr(labels[i]);
    }

    GoSlice gLabelSlice = {(void*)gsLabels, (GoInt)nLabels, (GoInt)nLabels};

    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewGaugeVec(gsName, gsHelp, gLabelSlice);
}

// nLabels: The number of label values in 'labelVals'
// labelVals: Array of c-string label values
void* GaugeWithLabelValues(void* pGaugeVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return (void*)goGaugeWithLabelValues((GoUintptr)pGaugeVec, gLabValSlice);
}

void GaugeDeleteLabelValues(void* pGaugeVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return goGaugeDeleteLabelValues((GoUintptr)pGaugeVec, gLabValSlice);
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

    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewCounterVec(gsName, gsHelp, gLabelSlice);
}

// nLabels: The number of label values in 'labelVals'
// labelVals: Array of c-string label values
void* CounterWithLabelValues(void* pCounterVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return (void*)goCounterWithLabelValues((GoUintptr)pCounterVec, gLabValSlice);
}

void CounterDeleteLabelValues(void* pCounterVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return goCounterDeleteLabelValues((GoUintptr)pCounterVec, gLabValSlice);
}

inline void CounterAdd(void* pCounter, double val) {
    goCounterAdd((GoUintptr)pCounter, (GoFloat64)val);

    return;
}

/* ========== SUMMARY WRAPPER FUNCTIONS ========== */
void* NewSummary(const char* name, const char* help,
        int nQuantiles, const double* quantiles, const double* errors,
        int maxAge, int nAgeBkts) {
    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    GoSlice gQuantSlice = {(void*)quantiles, (GoInt)nQuantiles, (GoInt)nQuantiles};
    GoSlice gErrSlice = {(void*)errors, (GoInt)nQuantiles, (GoInt)nQuantiles};

    return (void*)goNewSummary(gsName, gsHelp, gQuantSlice, gErrSlice, maxAge, nAgeBkts);
}

// nLabels: The number of labels in 'labels'
// labels: Array of c-string labels
void* NewSummaryVec(const char* name, const char* help, int nLabels, const char** labels,
        int nQuantiles, const double* quantiles, const double* errors,
        int maxAge, int nAgeBkts) {
    GoString gsLabels[nLabels];
    for (int i = 0; i < nLabels; i++) {
        gsLabels[i] = cStr2GoStr(labels[i]);
    }

    GoSlice gLabelSlice = {(void*)gsLabels, (GoInt)nLabels, (GoInt)nLabels};

    // TODO: Check to ensure name has no dashes
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    GoSlice gQuantSlice = {(void*)quantiles, (GoInt)nQuantiles, (GoInt)nQuantiles};
    GoSlice gErrSlice = {(void*)errors, (GoInt)nQuantiles, (GoInt)nQuantiles};

    return (void*)goNewSummaryVec(gsName, gsHelp, gLabelSlice, gQuantSlice, gErrSlice, maxAge, nAgeBkts);
}

// nLabels: The number of label values in 'labelVals'
// labelVals: Array of c-string label values
void* SummaryWithLabelValues(void* pSummaryVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return (void*)goSummaryWithLabelValues((GoUintptr)pSummaryVec, gLabValSlice);
}

void SummaryDeleteLabelValues(void* pSummaryVec, int nLabelVals, const char** labelVals) {
    GoString gsLabelVals[nLabelVals];
    for (int i = 0; i < nLabelVals; i++) {
        gsLabelVals[i] = cStr2GoStr(labelVals[i]);
    }

    GoSlice gLabValSlice = {(void*)gsLabelVals, (GoInt)nLabelVals, (GoInt)nLabelVals};

    return goSummaryDeleteLabelValues((GoUintptr)pSummaryVec, gLabValSlice);
}

inline void SummaryObserve(void* pSummary, double val) {
    goSummaryObserve((GoUintptr)pSummary, (GoFloat64)val);

    return;
}

#ifdef __cplusplus
#include <string>
#include <vector>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

/* ========== C++ CLASSES ========== */
// Simply implement them as wrappers around the C functions
// TODO: Make base Metric class and derive everything else from it?

namespace EasyProm {
class Gauge {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object

    public:
        Gauge() {}

        Gauge(string name, string help) {
            _metric = NewGauge(name.c_str(), help.c_str());
        }

        // Mainly used by GaugeVec, regular users likely wouldn't use this
        // constructor. If the user has a raw pointer to the Go Gauge object,
        // they can use this constructor to wrap it into a C++ object.
        Gauge(void* pGauge) {
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

    public:
        GaugeVec() {}

        GaugeVec(string name, string help, vector<string> labels) {
            const char* cStrLabels[labels.size()];
            for (unsigned int i = 0; i < labels.size(); i++) {
                cStrLabels[i] = labels[i].c_str();
            }
            _metric = NewGaugeVec(name.c_str(), help.c_str(), labels.size(), cStrLabels);
        }

        ~GaugeVec() {}

        Gauge WithLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            void* pGauge = GaugeWithLabelValues(_metric, labelVals.size(), cStrLabelVals);
            return Gauge(pGauge);
        }

        void DeleteLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            GaugeDeleteLabelValues(_metric, labelVals.size(), cStrLabelVals);
        }
};

class Counter {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object

    public:
        Counter() {}

        Counter(string name, string help) {
            _metric = NewCounter(name.c_str(), help.c_str());
        }

        // Mainly used by CounterVec, regular users likely wouldn't use this
        // constructor. If the user has a raw pointer to the Go Counter object,
        // they can use this constructor to wrap it into a C++ object.
        Counter(void* pCounter) {
            assert(pCounter != nullptr);
            _metric = pCounter;
        }

        ~Counter() {}

        void Add(double val) {
            CounterAdd(_metric, val);
        }
};

class CounterVec {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object

    public:
        CounterVec() {}

        CounterVec(string name, string help, vector<string> labels) {
            const char* cStrLabels[labels.size()];
            for (unsigned int i = 0; i < labels.size(); i++) {
                cStrLabels[i] = labels[i].c_str();
            }
            _metric = NewCounterVec(name.c_str(), help.c_str(), labels.size(), cStrLabels);
        }

        ~CounterVec() {}

        Counter WithLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            void* pCounter = CounterWithLabelValues(_metric, labelVals.size(), cStrLabelVals);
            return Counter(pCounter);
        }

        void DeleteLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            CounterDeleteLabelValues(_metric, labelVals.size(), cStrLabelVals);
        }
};

class Summary {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object

    public:
        Summary() {}

        Summary(string name, string help, unordered_map<double, double> objectives,
                int maxAge = 60, int nAgeBkts = 5) {
            int nQuantiles = objectives.size();
            double quantiles[nQuantiles];
            double errors[nQuantiles];
            auto objIter = objectives.begin();
            for (int i = 0; i < nQuantiles; i++, objIter++) {
                quantiles[i] = objIter->first;
                errors[i] = objIter->second;
            }

            _metric = NewSummary(name.c_str(), help.c_str(), nQuantiles,
                                    quantiles, errors, maxAge, nAgeBkts);
        }

        // Mainly used by SummaryVec, regular users likely wouldn't use this
        // constructor. If the user has a raw pointer to the Go Summary object,
        // they can use this constructor to wrap it into a C++ object.
        Summary(void* pSummary) {
            assert(pSummary != nullptr);
            _metric = pSummary;
        }

        ~Summary() {}

        void Observe(double val) {
            SummaryObserve(_metric, val);
        }
};

class SummaryVec {
    private:
        void* _metric = nullptr; // "Pointer" to go-land object

    public:
        SummaryVec() {}

        SummaryVec(string name, string help, vector<string> labels,
                unordered_map<double, double> objectives, int maxAge = 60, int nAgeBkts = 5) {

            const char* cStrLabels[labels.size()];
            for (unsigned int i = 0; i < labels.size(); i++) {
                cStrLabels[i] = labels[i].c_str();
            }

            int nQuantiles = objectives.size();
            double quantiles[nQuantiles];
            double errors[nQuantiles];
            auto objIter = objectives.begin();
            for (int i = 0; i < nQuantiles; i++, objIter++) {
                quantiles[i] = objIter->first;
                errors[i] = objIter->second;
            }

            _metric = NewSummaryVec(name.c_str(), help.c_str(), labels.size(), cStrLabels,
                                    nQuantiles, quantiles, errors, maxAge, nAgeBkts);
        }

        ~SummaryVec() {}

        Summary WithLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            void* pSummary = SummaryWithLabelValues(_metric, labelVals.size(), cStrLabelVals);
            return Summary(pSummary);
        }

        void DeleteLabelValues(vector<string> labelVals) {
            const char* cStrLabelVals[labelVals.size()];
            for (unsigned int i = 0; i < labelVals.size(); i++) {
                cStrLabelVals[i] = labelVals[i].c_str();
            }

            SummaryDeleteLabelValues(_metric, labelVals.size(), cStrLabelVals);
        }
};
} // End namespace EasyProm
#endif

#endif
