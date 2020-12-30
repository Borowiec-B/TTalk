include common.mk

BINDIR		:= $(ROOTDIR)bin/
OBJDIR		:= $(ROOTDIR)obj/
SRCDIR		:= $(ROOTDIR)src/

EXE_NAME := TTalk
TGT		 := $(BINDIR)$(EXE_NAME)
SRC		 := $(shell find $(SRCDIR) -name '*.c' -type f)
OBJ		 := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))

CFLAGS	 := -std=c18 -Wall $(INCLUDES)
LDFLAGS	 :=

.DEFAULT_GOAL := all


all : $(TGT)

$(TGT) : $(OBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o "$@" $(LDFLAGS)

$(OBJ) : $(OBJDIR)%.o : $(SRCDIR)%.c | $(OBJDIR)
	@# This is not going to work with spaces in paths.
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) "$<" -o "$@"

$(BINDIR) $(OBJDIR) :
	mkdir -p "$@"

