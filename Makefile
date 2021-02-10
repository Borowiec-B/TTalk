include common.mk

BINDIR		:= $(ROOTDIR)bin/
SRCDIR		:= $(ROOTDIR)src/

EXE_NAME := TTalk
TGT		 := $(BINDIR)$(EXE_NAME)
SRC		 := $(shell find $(SRCDIR) -name '*.c' -type f)
OBJ		 := $(patsubst $(SRCDIR)%.c, $(OBJDIR)%.o, $(SRC))

INCLUDES := -I$(INCLUDEDIR) -I$(SRCDIR)
CFLAGS	 := -std=gnu17 -Wall $(INCLUDES)
LDFLAGS	 +=

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

