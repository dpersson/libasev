CC 	=	gcc
CFLAGS	=	-O2 -fPIE -fstack-protector --param=ssp-buffer-size=4 \
	-Wall -W -Wshadow -Wformat-security \
	-D_FORTIFY_SOURCE=2 \

LINK	=	-Wl,-s
LDFLAGS	=	-fPIE -pie -Wl,-z,relro -Wl,-z,now

OBJS	=	asev.o utility.o sandbox.o picoev_epoll.o sysutil.o \
			str.o http_parser.o

.c.o:
	$(CC) -c $*.c $(CFLAGS)

bin/asev: $(OBJS)
	mkdir -p bin
	$(CC) -o asev $(OBJS) $(LINK) $(LDFLAGS)

clean:
	rm -f *.o *.swp asev
