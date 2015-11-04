CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLUGINS_BASE=/root/workspace/ant-http/plugins
    BUILDIRD=/root/antd/plugins
endif
ifeq ($(UNAME_S),Darwin)
	BUILDIRD=../build/plugins/
	PLUGINS_BASE=/Users/mrsang/Documents/ushare/cwp/ant-http/plugins
endif

EXT=dylib
PLUGINS=ffvm.$(EXT)
PLUGINSDEP = 	$(PLUGINS_BASE)/plugin.o \
				$(PLUGINS_BASE)/dbhelper.o \
				$(PLUGINS_BASE)/dictionary.o \
				$(PLUGINS_BASE)/utils.o
PLUGINLIBS = -lsqlite3 
VMLIBS = -lm
VMDEP = source/memory.o \
		source/names.o \
		source/news.o \
		source/interp.o \
		source/primitive.o \
		source/filein.o \
		source/lex.o \
		source/parser.o \
		source/unixio.o \
		source/tty.o 

main: $(PLUGINSDEP) $(VMDEP)  $(PLUGINS)

%.o: %.c
		$(CC) $(CFLAGS) -I$(PLUGINS_BASE) -c $< -o $@


%.$(EXT): %.o
		$(CC) $(CFLAGS) $(PLUGINLIBS) $(VMLIBS) -shared -o $(BUILDIRD)/$(basename $@).$(EXT) \
		$(PLUGINSDEP) $(VMDEP) $(basename $@).o


clean:
		-rm -f *.o source/*.o *.dylib $(BUILDIRD)/$(PLUGINS) 


.PRECIOUS: %.o

buildimage:
	$(CC) $(CFLAGS) -c source/initial.c -o source/initial.o
	$(CC) $(CFLAGS) -c source/st.c -o source/st.o
	$(CC) $(CFLAGS) $(VMDEP) source/st.o -o lst3 $(VMLIBS)
	$(CC) $(CFLAGS) $(VMDEP) source/initial.o -o build_image $(VMLIBS)
	@cd bootstrap && ../build_image basic.st mag.st collect.st file.st mult.st ../optional/imng.st tty.st
	@mv bootstrap/systemImage ./image
	cp -i image ../../build/plugins/