package main

import "C"

import (
	"net/http"

	"github.com/prometheus/client_golang/prometheus/promhttp"
)

//export goStartPromServer
func goStartPromServer(promEndpoint, metricsPath string) {
	http.Handle(metricsPath, promhttp.Handler())
	go http.ListenAndServe(promEndpoint, nil)
}

func main() {}
