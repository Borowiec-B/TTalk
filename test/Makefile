ROOTDIR ?= $(CURDIR)/../
TESTDIR	:= $(CURDIR)/

# common.mk stores some of main target's variables ($(CC), $(INCLUDEDIR), $(LDFLAGS), $(LIBDIR), ...).
include $(ROOTDIR)common.mk

# Binaries and object files are to be output somewhere under build/,
# all resembling source files' hierarchies.
# (e.g. src/A/B/test.c -> build/obj/A/B/test.o -> build/bin/A/B/test)
#
# Then, binaries and results generated from them are to be stored in top-level bin/ and txt/ without any further subdirectories.
# Only the top-level versions should be interacted with.
#
# Example of complete process for a result file:
# src/A/B/test.c -> build/obj/A/B/test.o -> build/bin/A/B/test -> (move/copy to) bin/test -> (write results to) txt/test.txt

TESTBINDIR		:= $(TESTDIR)bin/
TESTSRCDIR		:= $(TESTDIR)src/
TESTTXTDIR		:= $(TESTDIR)txt/

TESTBUILDDIR	:= $(TESTDIR)build/
TESTBUILDBINDIR	:= $(TESTBUILDDIR)bin/
TESTBUILDOBJDIR	:= $(TESTBUILDDIR)obj/

TESTFRAMEWORKDIR := $(LIBDIR)Unity/src/
TESTFRAMEWORKSRC := $(TESTFRAMEWORKDIR)unity.c
TESTFRAMEWORKOBJ := $(TESTBUILDOBJDIR)unity.o

TESTSRC		 := $(shell find $(TESTSRCDIR) -name '*.c' -type f)
TESTBUILDTGT := $(patsubst $(TESTSRCDIR)%.c, $(TESTBUILDBINDIR)%, $(TESTSRC))
TESTTGT		 := $(addprefix $(TESTBINDIR), $(notdir $(TESTBUILDTGT)))
TESTTXT		 := $(addsuffix .txt, $(addprefix $(TESTTXTDIR), $(notdir $(TESTTGT))))

IGNOREDRESULTSCMD := grep --no-filename --no-messages IGNORE $(TESTTXTDIR)*.txt
FAILINGRESULTSCMD := grep --no-filename --no-messages FAIL $(TESTTXTDIR)*.txt
PASSINGRESULTSCMD := grep --no-filename --no-messages PASS $(TESTTXTDIR)*.txt

TESTINCLUDES := -I$(INCLUDEDIR) -I$(TESTSRCDIR) -I$(TESTFRAMEWORKDIR)
TESTCFLAGS	 := -std=c18 -Wall $(TESTINCLUDES)
TESTLDFLAGS	 := $(LDFLAGS)

.DEFAULT_GOAL : all
.PHONY		  : clean run_tests

all : run_tests


run_tests : $(TESTTXT)
	@clear
	@echo "-----------------------IGNORES:-----------------------"
	@echo "$$($(IGNORERESULTSCMD))"
	@echo "-----------------------FAILURES:-----------------------"
	@echo "$$($(FAILINGRESULTSCMD))"
	@echo "-----------------------PASSED:-----------------------"
	@echo "$$($(PASSINGRESULTSCMD))"

$(TESTTXT) : $(TESTTXTDIR)%.txt : $(TESTBINDIR)% | $(TESTTXTDIR)
	-$< 1>$@ 2>&1

# This rule is effectively:
#
# target_binary_name=$(notdir $@)
# binary_path_in_build_dir=$(find $(TESTBUILDBINDIR) -type f -name $binary_name)
# cp $binary_path_in_build_dir $@
$(TESTTGT) : $(TESTBUILDTGT) | $(TESTBINDIR)
	cp $$(find $(TESTBUILDBINDIR) -type f -name $(notdir $@)) $@

$(TESTBUILDTGT) : $(TESTBUILDBINDIR)% : $(TESTBUILDOBJDIR)%.o $(TESTFRAMEWORKOBJ)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(TESTLDFLAGS)

$(TESTBUILDOBJDIR)%.o : $(TESTSRCDIR)%.c
	mkdir -p $(dir $@)
	$(CC) -c $(TESTCFLAGS) $< -o $@


$(TESTFRAMEWORKOBJ) : $(TESTFRAMEWORKSRC)
	mkdir -p $(dir $@)
	$(CC) -c $(TESTCFLAGS) $< -o $@


$(TESTBINDIR) $(TESTSRCDIR) $(TESTTXTDIR) :
	mkdir -p $@


clean :
	-rm -r $(TESTBINDIR) $(TESTBUILDDIR) $(TESTTXTDIR) 2>/dev/null

