CFLAGS += -I. -Wall -O3
LDFLAGS += -L. -lpromclient -pthread
EXENAME = test
ARNAME = promclient

all: c$(EXENAME) cpp$(EXENAME)

debug: CFLAGS += -g
debug: all

c$(EXENAME): test.c libpromclient.a promClient.h
	gcc $(CFLAGS) -std=c99 $< $(LDFLAGS) -o $@

cpp$(EXENAME): test.cpp libpromclient.a promClient.h
	g++ $(CFLAGS) -std=c++14 $< $(LDFLAGS) -o $@

lib: lib$(ARNAME).a

lib$(ARNAME).a: promClient.go
	go build -buildmode=c-archive -o $@ $<

clean:
	rm -f lib$(ARNAME).a lib$(ARNAME).h c$(EXENAME) cpp$(EXENAME)
