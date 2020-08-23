a.out: test.c libpromclient.a promClient.h
	gcc -Wall -std=c99 -I. $< -L. -lpromclient -pthread -o $@

libpromclient.a: promClient.go
	go build -buildmode=c-archive -o $@ $<

clean:
	rm libpromclient.a libpromclient.h a.out
