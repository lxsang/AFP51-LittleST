CC=gcc
CFLAGS=-W -Wall -g -std=c99 -D DEBUG
BUILDIRD=.
EXT=dylib
PLUGINS=ffvm.$(EXT)
PLUGINSDEP = 	../plugin.o \
				../dbhelper.o \
				../dictionary.o \
				../utils.o
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
		$(CC) $(CFLAGS) -c $< -o $@


%.$(EXT): %.o
		$(CC) $(CFLAGS) $(PLUGINLIBS) $(VMLIBS) -shared -o $(BUILDIRD)/$(basename $@).$(EXT) \
		$(PLUGINSDEP) $(VMDEP) $(basename $@).o
		cp -i $(PLUGINS) ../../build/plugins/


clean:
		-rm -f *.o source/*.o *.dylib image

pclean:clean
		-rm ../*.o

.PRECIOUS: %.o

buildimage:
	$(CC) $(CFLAGS) -c source/initial.c -o source/initial.o
	$(CC) $(CFLAGS) -c source/st.c -o source/st.o
	$(CC) $(CFLAGS) $(VMDEP) source/st.o -o lst3 $(VMLIBS)
	$(CC) $(CFLAGS) $(VMDEP) source/initial.o -o build_image $(VMLIBS)
	@cd bootstrap && ../build_image basic.st mag.st collect.st file.st mult.st ../optional/imng.st tty.st
	@mv bootstrap/systemImage ./image
	cp -i image ../../build/plugins/