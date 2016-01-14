CC=gcc
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    PLUGINS_BASE=/root/workspace/ant-http/plugins
	STROS_BASE=/root/workspace/STRos
    BUILDIRD=../build/plugins/
	PF_FLAG=-D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
	BUILDIRD=../build/plugins/
	PLUGINS_BASE=/Users/mrsang/Documents/ushare/cwp/ant-http/plugins
	STROS_BASE=/Users/mrsang/Documents/ushare/cwp/STRos
	PF_FLAG=-D_GNU_SOURCE
endif
CFLAGS=-W -Wall -g -std=c99 -D DEBUG $(PF_FLAG)
EXT=dylib
PLUGINS=ffvm.$(EXT)
UTILS = 	$(PLUGINS_BASE)/dictionary.o \
			$(PLUGINS_BASE)/list.o \
			$(PLUGINS_BASE)/utils.o
PLUGINSDEP = 	$(PLUGINS_BASE)/plugin.o \
				$(PLUGINS_BASE)/dbhelper.o \
				$(UTILS)
PLUGINLIBS = -lsqlite3 
VMLIBS = -lm -lpthread

SYS_PRIV = 	source/spriv/spriv.o \
			source/spriv/pointerpriv.o \
			source/spriv/devmapping.o \
			source/spriv/timepriv.o \
			source/spriv/rospriv.o 
STROS_OBJ = $(STROS_BASE)/request.o\
 			$(STROS_BASE)/3rd/ini/ini.o\
			$(STROS_BASE)/3rd/xml/sxmlc.o\
			$(STROS_BASE)/response.o\
			$(STROS_BASE)/rpc.o\
			$(STROS_BASE)/stros.o\
			$(STROS_BASE)/api.o 
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
		$(STROS_OBJ) \
		source/tty.o 
			
ST_BOOTSTRAP =  basic.st \
	 			mag.st \
				collect.st \
				file.st \
				mult.st \
				tty.st \
				../optional/extern.st\
				../optional/imng.st \
				../optional/Pointer.st \
				../optional/DeviceMapper.st \
				../optional/Date.st \
				../optional/ROSNode.st \
				../optional/ROSTopic.st \
				../optional/APF51Mapper.st \
				../optional/ODMapper.st \
				../optional/SCMapper.st \
				../optional/FRDBMapper.st \
				../optional/RWTest.st

main: $(PLUGINSDEP) $(VMDEP)  $(PLUGINS) 

%.o: %.c
		$(CC) $(CFLAGS) -I$(PLUGINS_BASE) -I$(STROS_BASE) -c $< -o $@


%.$(EXT): %.o
		$(CC) $(CFLAGS) $(PLUGINLIBS) $(VMLIBS) -shared -o $(BUILDIRD)/$(basename $@).$(EXT) \
		$(PLUGINSDEP) $(VMDEP) $(basename $@).o


clean:
		-rm -f *.o source/*.o  source/spriv/*.o *.dylib $(BUILDIRD)/$(PLUGINS) 


.PRECIOUS: %.o

buildimage:
	$(CC) $(CFLAGS) -c source/initial.c -o source/initial.o
	$(CC) $(CFLAGS) -c source/st.c -o source/st.o
	$(CC) $(CFLAGS) $(VMDEP) $(UTILS) source/st.o -o lst3 $(VMLIBS)
	$(CC) $(CFLAGS) $(VMDEP) $(UTILS) source/initial.o -o build_image $(VMLIBS)
	@cd bootstrap && ../build_image $(ST_BOOTSTRAP)
	@mv bootstrap/systemImage ./image
	cp -i image $(BUILDIRD)/image.im
	
full: clean main buildimage