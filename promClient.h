#ifndef _PROM_CLIENT_H_
#define _PROM_CLIENT_H_

#include "libpromclient.h"

/* ========== HELPER FUNCTIONS ========== */
GoString cStr2GoStr(const char* in) {
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

void* NewGauge(const char* name, const char* help) {
    GoString gsName = cStr2GoStr(name);
    GoString gsHelp = cStr2GoStr(help);

    return (void*)goNewGauge(gsName, gsHelp);
}

void SetGauge(void* pGauge, double val) {
    goSetGauge((GoUintptr)pGauge, (GoFloat64)val);

    return;
}

#endif
