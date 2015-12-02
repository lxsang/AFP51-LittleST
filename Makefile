CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    SERVER_BASE=/root/workspace/ant-http
    BUILDIRD=./build
endif
ifeq ($(UNAME_S),Darwin)
	BUILDIRD=./build
	SERVER_BASE=/Users/mrsang/Documents/ushare/cwp/ant-http
endif
SERVER=	$(SERVER_BASE)/plugin_manager.o \
		$(SERVER_BASE)/ini.o\
		$(SERVER_BASE)/http_server.o\
		$(SERVER_BASE)/plugins/dictionary.o\
		$(SERVER_BASE)/plugins/utils.o
SERVERLIB=-lpthread -ldl

main: httpd 


httpd:$(SERVER)
	$(CC) $(CFLAGS) $(SERVERLIB) $(SERVER)   -o $(BUILDIRD)/httpd $(SERVER_BASE)/httpd.c

%.o: %.c
	$(CC) $(CFLAGS) -I$(SERVER_BASE) -c $< -o $@


clean: 
	rm -f *.o build/httpd
.PRECIOUS: %.o