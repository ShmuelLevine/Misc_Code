# Standard things

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


# Subdirectories, in random order

#dir	:= $(d)/test
#include		$(dir)/Rules.mk


# Local variables


OBJS_$(d)	:= $(d)/File.Wrapper.Sample.o

DEPS_$(d)	:= $(OBJS_$(d):%=%.d)

CLEAN 		:= $(CLEAN) $(d)/../File.Wrapper.Test $(OBJS_$(d))  $(DEPS_$(d))

# Local rules

$(OBJS_$(d)):	CF_TGT := -I$(d)


$(d)/../File.Wrapper.Test:	LL_TGT := -lpthread libgtest.a

$(d)/../File.Wrapper.Test:	$(OBJS_$(d))
				$(LINK)


TGT_BIN := $(TGT_BIN) $(d)/../File.Wrapper.Test

# Standard things

-include	$(DEPS_$(d))

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

