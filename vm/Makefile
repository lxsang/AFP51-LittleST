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

SYS_PRIV = 	source/spriv/spriv.o \
			source/spriv/devmapping.o \
			source/spriv/timepriv.o 
VMDEP = source/memory_v1.o \
		source/names.o \
		source/news.o \
		source/interp.o \
		source/primitive.o \
		source/filein.o \
		source/lex.o \
		source/parser.o \
		source/unixio.o \
		source/free_list.o\
		$(SYS_PRIV) \
		source/tty.o 

ST_BOOTSTRAP =  basic.st \
	 			mag.st \
				collect.st \
				file.st \
				mult.st \
				tty.st \
				../optional/extern.st\
				../optional/imng.st \
				../optional/DeviceMapper.st \
				../optional/Date.st

main: $(PLUGINSDEP) $(VMDEP)  $(PLUGINS)

%.o: %.c
		$(CC) $(CFLAGS) -I$(PLUGINS_BASE) -c $< -o $@


%.$(EXT): %.o
		$(CC) $(CFLAGS) $(PLUGINLIBS) $(VMLIBS) -shared -o $(BUILDIRD)/$(basename $@).$(EXT) \
		$(PLUGINSDEP) $(VMDEP) $(basename $@).o


clean:
		-rm -f *.o source/*.o  source/spriv/*.o *.dylib $(BUILDIRD)/$(PLUGINS) 


.PRECIOUS: %.o

buildimage:
	$(CC) $(CFLAGS) -c source/initial.c -o source/initial.o
	$(CC) $(CFLAGS) -c source/st.c -o source/st.o
	$(CC) $(CFLAGS) $(VMDEP) source/st.o -o lst3 $(VMLIBS)
	$(CC) $(CFLAGS) $(VMDEP) source/initial.o -o build_image $(VMLIBS)
	@cd bootstrap && ../build_image $(ST_BOOTSTRAP)
	@mv bootstrap/systemImage ./image
	cp -i image $(BUILDIRD)/image.im