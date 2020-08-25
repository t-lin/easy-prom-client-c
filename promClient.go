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
	gaugeHandles      = make(map[unsafe.Pointer]prometheus.Gauge)
	gaugeVecHandles   = make(map[unsafe.Pointer]*prometheus.GaugeVec)
	counterHandles    = make(map[unsafe.Pointer]prometheus.Counter)
	counterVecHandles = make(map[unsafe.Pointer]*prometheus.CounterVec)
)

// ========== HELPER FUNCTIONS ==========
// Function to force a copy of a string's backing array.
// Strings in Go use an immutable underlying backing array to store the
// bytes. When a Go string is copied or assigned, the new string has the
// same pointer to the backing array. It only ever creates a new array if
// the string content is modified.
// Since we're passing strings from "C-land", we can't assume the array is
// immutable (in fact, they are likely *not* immutable unless they're string
// literals). Hence we need this function.
func stringCopy(src string) string {
	// Super hacky way to force allocation of new backing array
	// Credit: https://groups.google.com/g/golang-nuts/c/naMCI9Jt6Qg
	if len(src) == 0 {
		return ""
	}

	return src[0:1] + src[1:]
}

// String slice copying function that utilizes stringCopy()
func stringSliceCopy(dst, src []string) {
	min := 0
	if len(dst) <= len(src) {
		min = len(dst)
	} else {
		min = len(src)
	}

	if min == 0 {
		return
	}

	for i := 0; i < min; i++ {
		dst[i] = stringCopy(src[i])
	}
}

// ========== EXPORTED FUNCTIONS ==========
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
	// Since the labels slice was created in C, its pointers may not be
	// valid after this call. Thus, perform deep copy of labels.
	labelsCopy := make([]string, len(labels))
	stringSliceCopy(labelsCopy, labels)
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
	// Since the labelVals slice was created in C, its pointers may not be
	// valid after this call. Thus, perform deep copy of labelVals.
	labelValsCopy := make([]string, len(labelVals))
	stringSliceCopy(labelValsCopy, labelVals)
	gaugeVec := gaugeVecHandles[unsafe.Pointer(uPtrGaugeVec)]
	gauge := gaugeVec.WithLabelValues(labelValsCopy...)

	gaugeHandles[unsafe.Pointer(&gauge)] = gauge

	return uintptr(unsafe.Pointer(&gauge))
}

//export goGaugeSet
func goGaugeSet(uPtrGauge uintptr, val float64) {
	gauge := gaugeHandles[unsafe.Pointer(uPtrGauge)]
	gauge.Set(val)
}

//export goGaugeAdd
func goGaugeAdd(uPtrGauge uintptr, val float64) {
	gauge := gaugeHandles[unsafe.Pointer(uPtrGauge)]
	gauge.Add(val)
}

//export goGaugeSub
func goGaugeSub(uPtrGauge uintptr, val float64) {
	gauge := gaugeHandles[unsafe.Pointer(uPtrGauge)]
	gauge.Sub(val)
}

//export goNewCounter
func goNewCounter(name, help string) uintptr {
	counter := promauto.NewCounter(prometheus.CounterOpts{
		Name: name,
		Help: help,
	})

	counterHandles[unsafe.Pointer(&counter)] = counter

	return uintptr(unsafe.Pointer(&counter))
}

//export goNewCounterVec
func goNewCounterVec(name, help string, labels []string) uintptr {
	// Since the labels slice was created in C, its pointers may not be
	// valid after this call. Thus, perform deep copy of labels.
	labelsCopy := make([]string, len(labels))
	stringSliceCopy(labelsCopy, labels)
	counterVec := promauto.NewCounterVec(
		prometheus.CounterOpts{
			Name: name,
			Help: help,
		},
		labelsCopy,
	)

	counterVecHandles[unsafe.Pointer(counterVec)] = counterVec

	return uintptr(unsafe.Pointer(counterVec))
}

//export goCounterWithLabelValues
func goCounterWithLabelValues(uPtrCounterVec uintptr, labelVals []string) uintptr {
	// Since the labelVals slice was created in C, its pointers may not be
	// valid after this call. Thus, perform deep copy of labelVals.
	labelValsCopy := make([]string, len(labelVals))
	stringSliceCopy(labelValsCopy, labelVals)
	counterVec := counterVecHandles[unsafe.Pointer(uPtrCounterVec)]
	counter := counterVec.WithLabelValues(labelValsCopy...)

	counterHandles[unsafe.Pointer(&counter)] = counter

	return uintptr(unsafe.Pointer(&counter))
}

//export goCounterAdd
func goCounterAdd(uPtrCounter uintptr, val float64) {
	counter := counterHandles[unsafe.Pointer(uPtrCounter)]
	counter.Add(val)
}

func main() {}
