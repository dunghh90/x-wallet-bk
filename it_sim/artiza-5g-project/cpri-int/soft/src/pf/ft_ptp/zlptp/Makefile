# The complete list of commands that this file can be used to make is :
#
#   ALL         = build everything (default)
#   CLEAN       = remove derived objects (byproducts of the make process)
#
# Environment variable LIBRARIES can be used to specify additional libraries to be
# used during the linker stage
#
# From the command line, the make file can be invoked with the following macros set
# to control the build process:
#
# This makefile builds each of the individual components of the api and then
# links them all together into a single library in the top level directory
.SILENT :
export

BOARD = NONE

include rules.mak

CFLAGS_DEF += -DZLS3038X_INCLUDED -DAPR_INCLUDED -DAPR_ONLY -DZLS30390_INCLUDED -DZLS30771_INCLUDED -DEXAMPLE_PTP_SOCKET_TEST -DZL_INCLUDE_ETH2_SOCKET

ifeq "$(OBJ_DIR)" ""
  OBJ_DIR = obj
endif

# The final target file to build
API_LIB = $(OBJ_DIR)/zlApiLib.out

# List of api components to build. The following is assumed:
#   that the target library name to build is <component_name>Lib.o
#   and that the target library will be built in OBJ_DIR

SRC_DIRS = $(wildcard zl*/src)


RQD_DIRS = $(patsubst %/src, %/$(OBJ_DIR), $(SRC_DIRS))
# Check for the zlPtp directory.
ifneq "$(wildcard zlPtp/)" ""
    ifeq "$(wildcard zlPtp/src)" ""
     RQD_DIRS += zlPtp/$(OBJ_DIR)    # Support for Obj-Only
     EXCLUDE_DIRS = $(foreach i, $(ALL_OBJ), zlPtp/$(i))
    endif
endif

ALL_OBJ = $(wildcard obj*)
ALL_OBJ_DIRS = $(foreach dir,$(ALL_OBJ),$(patsubst %/include, %/$(dir), $(wildcard zl*/include)))

APR_CLEAN = $(foreach i,$(ALL_OBJ),$(patsubst zlApr/src/%.c, zlApr/$(i)/%.o, $(wildcard zlApr/src/*.c)))
EXCLUDE_DIRS += $(foreach i, $(ALL_OBJ), zlApr/$(i))

COMPONENTS = $(sort $(patsubst %/$(OBJ_DIR), %, $(RQD_DIRS)) zlApr)

# Don't build the zlApiLib.o object if there are no .c files in that directory.
ifeq "$(wildcard zlApi/src/*.c)" ""
   COMPONENTS := $(filter-out zlApi, $(COMPONENTS))
endif

COMPONENT_LIBS = $(foreach cpt, $(COMPONENTS), $(cpt)/$(OBJ_DIR)/$(cpt)Lib.o)


TOP=../../..
CMNOBJ= \
	$(TOP)/cmn/com/src/f_cmn_com_abort.o \
	$(TOP)/cmn/com/src/f_cmn_com_rom.o \
	$(TOP)/cmn/com/src/f_cmn_com_assert.o \


# By default build the final library
# ALL: $(COMPONENT_LIBS) $(API_LIB)
ALL: $(COMPONENT_LIBS) $(API_LIB) $(CMNOBJ)


$(API_LIB): $(COMPONENT_LIBS) $(LIBRARIES)
	@echo "==================================="
	@echo "Building library $(API_LIB)"
	@echo CFLAGS=$(CFLAGS) > $(dir $@)/FLAGS.txt
	@echo LDFLAGS=$(LDFLAGS) >> $(dir $@)/FLAGS.txt
	$(LD) $(LDFLAGS) $^ $(LIB_INT64) -o $@
	@echo "============================================================"

.PHONY : ALL MAKE_LIBS

# For windows 'mkdir' need to substitute '/' for '\'
$(OBJ_DIR) $(RQD_DIRS):
	@echo Creating directory $@
ifeq ($(OS_TYPE),VXWORKS)
  ifeq ($(TOOLS),LNX_VXW-69)
	mkdir $@
  else
	mkdir $(subst /,\,$@)
  endif
endif
ifeq ($(OS_TYPE),LINUX)
	mkdir $@
endif

# Rule to force all the components to be remade
$(COMPONENT_LIBS): $(OBJ_DIR) $(RQD_DIRS) MAKE_LIBS
	@echo ===================================
	@echo Checking $(@F)
	@echo CFLAGS = $(CFLAGS)
	$(MAKE) -C $(dir $@) -f../../build.mak TARGET=$(notdir $@)


# make functions to clean up the derived objects
.PHONY : CLEAN

CLEAN :
	@echo Removing all generated object files
	rm -r -f $(foreach dir,$(ALL_OBJ),$(wildcard $(dir)/*))
	rm -f $(foreach i,$(filter-out $(EXCLUDE_DIRS), $(ALL_OBJ_DIRS)),$(wildcard $(i)/*.*))
	-rm -f $(APR_CLEAN)
