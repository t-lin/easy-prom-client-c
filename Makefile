CFLAGS += -I. -Wall -std=c99 -O3
LDFLAGS += -L. -lpromclient -pthread
EXENAME = test
ARNAME = promclient

all: $(EXENAME)

debug: CFLAGS += -g
debug: all

$(EXENAME): test.c libpromclient.a promClient.h
	gcc $(CFLAGS) $< $(LDFLAGS) -o $@

lib: lib$(ARNAME).a

lib$(ARNAME).a: promClient.go
	go build -buildmode=c-archive -o $@ $<

clean:
	rm -f lib$(ARNAME).a lib$(ARNAME).h $(EXENAME)
