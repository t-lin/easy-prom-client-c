package main

import "C"

import (
	"net/http"
	"unsafe"

	"github.com/prometheus/client_golang/prometheus"
	"github.com/prometheus/client_golang/prometheus/promauto"
	"github.com/prometheus/client_golang/prometheus/promhttp"
)

var (
	// Since we're allocating in "Go-land" and passing "pointers" back to
	// "C-land", there's the risk that the Go GC will reap the Go objects.
	// To avoid this, we keep a handle in Go-land to all objects created.
	// Note that the "pointers" we pass back are raw unsigned integers,
	// essentially a unique ID to the object (same concept as pointer).
	// Use a separate function for explicit deletion of objects.
	gaugeHandles = make(map[unsafe.Pointer]prometheus.Gauge)
)

//export goStartPromServer
func goStartPromServer(promEndpoint, metricsPath string) {
	http.Handle(metricsPath, promhttp.Handler())
	go http.ListenAndServe(promEndpoint, nil)
}

//export goNewGauge
func goNewGauge(name, help string) uintptr {
	gauge := promauto.NewGauge(prometheus.GaugeOpts{
		Name: name,
		Help: help,
	})

	gaugeHandles[unsafe.Pointer(&gauge)] = gauge

	return uintptr(unsafe.Pointer(&gauge))
}

//export goSetGauge
func goSetGauge(uPtr uintptr, val float64) {
	pGauge := gaugeHandles[unsafe.Pointer(uPtr)]
	pGauge.Set(val)
}

func main() {}
