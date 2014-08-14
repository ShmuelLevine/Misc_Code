# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Subdirectories, in random order

#dir	:= $(d)/test
#include		$(dir)/Rules.mk


# Local variables


OBJS_$(d)	:= $(d)/Test.Parser.o

PCHS_$(d)	:= $(d)/parser_includes.h
PCHS_OUT_$(d)	:= $(PCHS_$(d):%=%.pch)

DEPS_$(d)	:= $(OBJS_$(d):%=%.d)

CLEAN := $(CLEAN) $(OBJS_$(d)) $(DEPS_$(d)) $(PCHS_OUT_$(d))


# Local rules

$(OBJS_$(d)):   libs/parser/libparser.a

$(OBJS_$(d)):	CF_TGT := -I$(d) -include $(d)/parser_includes.h
$(OBJS_$(d)):	LL_TGT := $(TGT_LIB)
$(OBJS_$(d)):	$(TGT_LIB)

$(PCHS_$(d):%=%.pch): 	$(PCHS_$(d))
			$(PRECOMP)

$(OBJS_$(d)) : $(PCHS_OUT_$(d))


TGT_TEST_OBJ := $(TGT_TEST_OBJ) $(OBJS_$(d))

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

