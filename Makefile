### Build flags for all targets
#

ROOT_DIR :=	/home/shmuel/src/fx/src
LIB_DIR :=	$(ROOT_DIR)/libs
GMOCK_DIR :=	$(ROOT_DIR)/../include/gmock
GTEST_DIR :=	$(ROOT_DIR)/../include/gtest
LIB_OUT := $(ROOT_DIR)/../lib

INCLUDE :=      -I$(GMOCK_DIR)/include \
                -I$(GTEST_DIR)/include 

#CF_ALL          = -g -Wall -std=c++11 -I/home/shmuel/src/fx/include/ -I/home/shmuel/src/fx/include/Eigen -I/home/shmuel/src/fx/src/libs/core/timeseries
CF_ALL		= -g -Wall -Wno-deprecated -std=c++11 $(INCLUDE)  -O0 -fPIC
LF_ALL          = -L/usr/local/lib -L$(LIB_OUT)
LL_ALL          = -lpthread

#LL_SO		= $(patsubst lib, , $(basename, $(notdir $(TGT_SO))))
LL_SO 		= $(patsubst lib%, -l%, $(basename $(notdir $(TGT_SO))))

### Build tools
#
ifeq ($(CXX),clang++)
	CF_ALL +=  -ferror-limit=1000
	LF_SO = -shared -Wl,-soname,$@
endif

ifeq ($(CXX),icpc)
	LF_SO = -Xlinker -soname=$@
endif

#CXX              = distcc clang++
CCXX              = ./ccd-gcc

# Intel's xiar will call ar internally
AR		?= xiar
COMP            = $(CCXX) $(CF_ALL) $(CF_TGT) -o $@ -c $<
PRECOMP         = $(CCXX) $(CF_ALL) $(CF_TGT) -o $@ -x c++-header  $<
#PRECOMP         = $(CCXX) $(CF_ALL) $(CF_TGT) -o $@ -x c++-header --relocatable-pch $<
LINK            = $(CCXX) $(LF_ALL) $(LF_TGT) -o $@ $^ $(LL_TGT) $(LL_ALL)
COMPLINK        = $(CCXX) $(CF_ALL) $(CF_TGT) $(LF_ALL) $(LF_TGT) -o $@ $< $(LL_TGT) $(LL_ALL)
ARCHIVE         = $(AR) rvsc $@ $^
MAKE_SO		= $(CXX) $(CF_ALL) $(CF_TGT) -o $@ $^ $(LF_SO)
TEST_BUILD      = $(CCXX) $(LF_ALL) $(LF_TGT) -o $@ $(filter %.o, $^) $(LL_TGT) $(LL_ALL) $(ROOT_DIR)/../lib/libgtest_main.a $(ROOT_DIR)/../lib/libgtest.a -lpthread $(LL_SO)

### Standard parts
#
include Rules.mk
