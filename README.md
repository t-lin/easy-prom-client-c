# Easy Prometheus Client for C and C++
A super simplified (i.e. dumbed down) C and C++ client for Prometheus. It's essentially just an abstraction layer for the golang Prometheus client, which is the most updated (thus, it makes sense to utilize it under-the-hood).

Obviously, this method sacrifices flexibility/customizability for simplicity.

**Everything here should be considered a work in progress. Currently only supports creating Gauges and Counters.**

**Tested in Ubuntu 16.04 and 18.04**

Simply run `make` to compile the Go library, C test code, and C++ test code. The static library archive and accompanying header file that's created is then used by `promClient.h`, which is the only thing the user's program needs to import. For just the library archive and header files, run `make lib`.
