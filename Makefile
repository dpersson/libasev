CC_OPTS = -Wall -Werror -ggdb3

all: epoll bin/server

clean:
	rm -f *.o bin/*

epoll:
	$(CC) $(CC_OPTS) -c picoev_epoll.c $<

bin/server: server.c
	mkdir bin
	$(CC) $(CC_OPTS) picoev_epoll.o -o $@ $< 