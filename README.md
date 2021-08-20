# Easy Prometheus Client for C and C++
A super simplified (i.e. dumbed down) C and C++ client for Prometheus. It's essentially just an abstraction layer for the golang Prometheus client, which is the most updated (thus, it makes sense to utilize it under-the-hood).

Obviously, this method sacrifices flexibility/customizability for simplicity. See `test.c` and `test.cpp` for usage examples.

**Everything here should be considered a work in progress. Currently only supports creating Gauge, Counter, and Summary metrics.**

**Tested in Ubuntu 20.04 with gcc/g++ 9.3 and golang 1.16.7**

Simply run `make` to compile the Go library, C test code, and C++ test code. The static library archive and accompanying header file that's created is then used by `promClient.h`, which is the only thing the user's program needs to import. For just the library archive and header files, run `make lib`.


## Why does this exist?
I needed a C/C++ based Prometheus library that also exports the standard set of application and usage metrics supported by the official clients. The other libraries (at the time of writing) doesn't support them yet.

## Known Limitations
**Avoid globals:** Currently there's an issue when instantiating global metrics (e.g. Counters, Gauges), which may result in the program hanging before `main` is even invoked. Example:

```C
// Global instantiation of metrics will hang
void* testGauge = NewGauge("test_gauge", "Test gauge's help"); // Hangs

int main() {
    printf("Hello world!\n"); // Will never print
    ...
```

If a global is needed, declare a pointer first, then instantiate it after `main` starts and assign it to the global pointer. Examples:
```C
// C Example
void* testGauge = NULL;

int main() {
    testGauge = NewGauge("test_gauge", "Test gauge's help");
    ...
```

```C++
// C++ Example
Gauge testGauge; // Under-the-hood, just creates a nullptr to the metric

int main() {
    testGauge = Gauge("test_gauge", "Test gauge's help"); // Actual metric created
    ...
```

