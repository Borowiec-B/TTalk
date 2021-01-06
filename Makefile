include common.mk

BINDIR		:= $(ROOTDIR)bin/
OBJDIR		:= $(ROOTDIR)obj/
SRCDIR		:= $(ROOTDIR)src/

EXE_NAME := TTalk
TGT		 := $(BINDIR)$(EXE_NAME)
SRC		 := $(shell find $(SRCDIR) -name '*.c' -type f)
OBJ		 := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))

INCLUDES := -I$(INCLUDEDIR) -I$(SRCDIR)
CFLAGS	 := $$(pkg-config --cflags gtk4) -std=c18 -Wall $(INCLUDES)
LDFLAGS	 += $$(pkg-config --libs gtk4)

.DEFAULT_GOAL := all
.PHONY : clean 


all : $(TGT)

$(TGT) : $(OBJ) | $(BINDIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ) : $(OBJDIR)%.o : $(SRCDIR)%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BINDIR) $(OBJDIR) :
	mkdir -p $@


clean :
	-rm -r $(OBJDIR) $(BINDIR) 2>/dev/null

