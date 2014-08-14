
# Standard stuff

.SUFFIXES:
.SUFFIXES:	.c .o

all:		targets


# Tracking of subdirectories for simplifying includes: It is intended that Rules.mk
# files in subdirectories will append this list, which will then be added to the
# compiler flags
INCL_DIRS := -I../include/gtest/include -I../include/gmock/include

# Subdirectories, in random order

dir := File.Wrapper
include		$(dir)/Rules.mk


# Dependecies
#include $(DEPS)

# General directory independent rules

%.o:		%.cc
		$(COMP)

%:		%.o
	$(LINK)

%.a:	%.o
	$(ARCHIVE)

%:		%.cc
		$(COMPLINK)

%.pch:	%.h
	$(PRECOMP)

%.pch:	%.hpp
	$(PRECOMP)

#%.d:	%.cc
#	./depend.sh `dirname $*.cc` $*.cc > $@

# The variables TGT_*, CLEAN and CMD_INST* may be added to by the Makefile
# fragments in the various subdirectories.

$(TGT_LIB) : $(TGT_SO)

.PHONY:		targets
targets:	$(TGT_BIN) $(TGT_SBIN) $(TGT_ETC) $(TGT_LIB) $(TGT_SO)

.DEFAULT: 	tests
.PHONY : 	tests

tests: 		$(TGT_TEST_OBJ) $(TGT_SO) 
		$(TEST_BUILD)

CLEAN	:= $(CLEAN) test/tests

.PHONY:		clean
clean:
		rm -f $(CLEAN)

.PHONY:		install
install:	targets 
		$(INST) $(TGT_BIN) -m 755 -d $(DIR_BIN)
		$(CMD_INSTBIN)
		$(INST) $(TGT_SBIN) -m 750 -d $(DIR_SBIN)
		$(CMD_INSTSBIN)
ifeq ($(wildcard $(DIR_ETC)/*),)
		$(INST) $(TGT_ETC) -m 644 -d $(DIR_ETC)
		$(CMD_INSTETC)
else
		@echo Configuration directory $(DIR_ETC) already present -- skipping
endif
		$(INST) $(TGT_LIB) -m 750 -d $(DIR_LIB)
		$(CMD_INSTLIB)


# Prevent make from removing any build targets, including intermediate ones

.SECONDARY:	$(CLEAN)

