# Have $(ROOTDIR) set to project's top-level directory before including this!

LIBDIR	 	:= $(ROOTDIR)lib/
INCLUDEDIR	:= $(ROOTDIR)include/

CC		 ?= gcc
INCLUDES := -I$(INCLUDEDIR) -I$(SRCDIR)
LDFLAGS	 :=

