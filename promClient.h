#ifndef _PROM_CLIENT_H_
#define _PROM_CLIENT_H_

#include "libpromclient.h"

/* ========== HELPER FUNCTIONS ========== */
GoString cStr2GoStr(const char *in) {
    GoString tmp = {in, (ptrdiff_t)strlen(in)};
    return tmp;
}


/* ========== WRAPPER FUNCTIONS FOR GO CODE ========== */
void StartPromServer(const char *promEndpoint, const char *metricsPath) {
    GoString gsPromEnd = cStr2GoStr(promEndpoint);
    GoString gsMetricsPath = cStr2GoStr(metricsPath);
    goStartPromServer(gsPromEnd, gsMetricsPath);

    return;
}

#endif
