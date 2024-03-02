# xidle.
#
# $Author: Omar Sakka $
# $Branch: main $
# $Date: Sat Mar  2 10:42:13 PM GMT 2024 $
# $Revision: 3.24c45f3.20240302 $

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
	rm -f xidle

clean_obj:
	rm -f xidle.o

install:
	sudo mv xidle /usr/lib/bash/xidle
	sudo chmod a+x /usr/lib/bash/xidle
	sudo chown root:root /usr/lib/bash/xidle

update:
	/home/osakka/tidbits/git-hooks/srcvar_substition

commit: update
	git add -A
	git commit -m "This is a non-descriptive commit message"
	git push
