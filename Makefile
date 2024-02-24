# Build xidle.

prefix = /usr
exec_prefix = ${prefix}
libdir = ${exec_prefix}/lib

# ${loadablesdir} is where the example loadable builtins and data files
# are installed (make install target in Makefile.in)
loadablesdir = ${libdir}/bash

DESTDIR =
LIBS = -lX11 -lXext -lXss
SHOBJ_LIBS = $(SHOBJ_LIBS) -lX11 -lXext -lXss

# include Makefile.inc for all boilerplate definitions
include $(DESTDIR)$(loadablesdir)/Makefile.inc

CFLAGS = -g -O2
LOCAL_CFLAGS = 
DEFS = -DHAVE_CONFIG_H
LOCAL_DEFS = -DSHELL
CCFLAGS = $(DEFS) $(LOCAL_DEFS) $(LOCAL_CFLAGS) $(CFLAGS)

all:	xidle
	strip xidle

xidle:	xidle.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) $(CCFLAGS) -o $@ xidle.o $(SHOBJ_LIBS) $(LIBS)

xidle.o: xidle.c

clean: clean_exec clean_obj

clean_exec:
	[ -e xidle ] && rm xidle

clean_obj:
	[ -e xidle.o ] && rm xidle.o

install:
	[ -e xidle ] && sudo mv xidle /usr/lib/bash/xidle
	[ -e /usr/lib/bash/xidle ] && sudo chmod a+x /usr/lib/bash/xidle
