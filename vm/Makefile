CC=gcc
UNAME_S := $(shell uname -s)
INC_ROS 	:= FALSE
INC_REST	:= TRUE
USE_DB 		:= FALSE
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
ifeq ($(USE_DB),TRUE)
	DB_OBJ = $(PLUGINS_BASE)/dbhelper.o 
	DB_LIB = -lsqlite3 
	DB_FLG = -D USE_DB
endif
ifeq ($(USE_DB),FALSE)
	DB_OBJ=
	DB_FLG=
	DB_LIB =
endif
EXT=dylib
PLUGINS=ffvm.$(EXT)
 
UTILS = 	$(PLUGINS_BASE)/dictionary.o \
			$(PLUGINS_BASE)/list.o \
			$(PLUGINS_BASE)/utils.o \
			$(PLUGINS_BASE)/base64.o \
			$(PLUGINS_BASE)/ws.o \
			$(PLUGINS_BASE)/sha1.o \
			$(PLUGINS_BASE)/plugin.o 
PLUGINSDEP = 	vmproxy.o\
				$(DB_OBJ) \
				$(UTILS)
PLUGINLIBS = $(DB_LIB) 
VMLIBS = -lm -lpthread

SYS_PRIV = 	source/spriv/spriv.o \
			source/spriv/pointerpriv.o \
			source/spriv/devmapping.o \
			source/spriv/timepriv.o 
ifeq ($(INC_ROS),TRUE)
	STROS_OBJ = source/spriv/rospriv.o \
				$(STROS_BASE)/request.o\
 				$(STROS_BASE)/3rd/ini/ini.o\
				$(STROS_BASE)/3rd/xml/sxmlc.o\
				$(STROS_BASE)/response.o\
				$(STROS_BASE)/rpc.o\
				$(STROS_BASE)/stros.o\
				$(STROS_BASE)/api.o
	ROS_FLG = -D INC_ROS
	ROS_CLS = 	../optional/ROSNode.st \
				../optional/ROSTopic.st 
endif
ifeq ($(INC_ROS),FALSE)
	STROS_BASE= $(PLUGINS_BASE)
	STROS_OBJ= 
	ROS_FLG = 
	ROS_CLS = 
endif
ifeq ($(INC_REST),TRUE)
	REST_OBJ = 	source/spriv/restpriv.o 
	REST_FLG = -DINC_REST
	REST_CLS = 	 ../optional/RESTProxy.st
endif
ifeq ($(INC_REST),FALSE)
	REST_OBJ = 
	REST_FLG = 
	REST_CLS  = 
endif
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
		$(REST_OBJ) \
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
				../optional/JSONParser.st\
				$(ROS_CLS) \
				$(REST_CLS) \
				../optional/APF51Mapper.st \
				../optional/ODMapper.st \
				../optional/SCMapper.st \
				../optional/FRDBMapper.st \
				../optional/parser.st \
				../optional/RWTest.st
				
CFLAGS=-W -Wall -g -std=c99 -D DEBUG $(DB_FLG) $(ROS_FLG) $(REST_FLG) $(PF_FLAG)
main: $(PLUGINSDEP) $(VMDEP)  $(PLUGINS) 

%.o: %.c
		$(CC) $(CFLAGS) -I$(PLUGINS_BASE) -I$(STROS_BASE) -c $< -o $@


%.$(EXT): %.o
		$(CC) $(CFLAGS) $(PLUGINLIBS) $(VMLIBS) -shared -o $(BUILDIRD)/$(basename $@).$(EXT) \
		$(PLUGINSDEP) $(VMDEP) $(basename $@).o


clean:
		-rm -f *.o source/*.o  source/spriv/*.o *.dylib $(BUILDIRD)/$(PLUGINS) 
		-rm $(PLUGINS_BASE)/plugin.o


.PRECIOUS: %.o

buildimage:
	$(CC) $(CFLAGS) -c source/initial.c -o source/initial.o
	$(CC) $(CFLAGS) -c source/st.c -o source/st.o
	$(CC) $(CFLAGS) $(VMDEP) $(UTILS) source/st.o -o lst3 $(VMLIBS)
	$(CC) $(CFLAGS) $(VMDEP) $(UTILS) source/initial.o -o build_image $(VMLIBS)
	@cd bootstrap && ../build_image $(ST_BOOTSTRAP)
	@mv bootstrap/systemImage ./image
	-mkdir $(BUILDIRD)/ffvm
	cp -i image $(BUILDIRD)/ffvm/image.im
	
full: clean main buildimage