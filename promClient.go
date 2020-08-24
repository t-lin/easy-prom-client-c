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
	gaugeHandles    = make(map[unsafe.Pointer]prometheus.Gauge)
	gaugeVecHandles = make(map[unsafe.Pointer]*prometheus.GaugeVec)
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

//export goNewGaugeVec
func goNewGaugeVec(name, help string, labels []string) uintptr {
	// NewGaugeVec seems to keep a pointer to the labels slice.
	// Since the labels slice was created in C, its pointers may not be
	// valid after this call. Thus, perform deep copy of labels slice.
	labelsCopy := make([]string, len(labels))
	copy(labelsCopy, labels)
	gaugeVec := promauto.NewGaugeVec(
		prometheus.GaugeOpts{
			Name: name,
			Help: help,
		},
		labelsCopy,
	)

	gaugeVecHandles[unsafe.Pointer(gaugeVec)] = gaugeVec

	return uintptr(unsafe.Pointer(gaugeVec))
}

//export goGaugeWithLabelValues
func goGaugeWithLabelValues(uPtrGaugeVec uintptr, labelVals []string) uintptr {
	gaugeVec := gaugeVecHandles[unsafe.Pointer(uPtrGaugeVec)]
	gauge := gaugeVec.WithLabelValues(labelVals...)

	gaugeHandles[unsafe.Pointer(&gauge)] = gauge

	return uintptr(unsafe.Pointer(&gauge))
}

//export goSetGauge
func goSetGauge(uPtrGauge uintptr, val float64) {
	gauge := gaugeHandles[unsafe.Pointer(uPtrGauge)]
	gauge.Set(val)
}

func main() {}
