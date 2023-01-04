#------------------------------------------------------
# Makefile for common debug library
#------------------------------------------------------
# Target: libcmn_dbg
#------------------------------------------------------
TARGET=libcmn_dbg.a

include ../../Makefile.in
ifeq ($(BUILD_MODE_DEF), 0)
#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../..
OPT=-shared -fPIC

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/../ramdisk/target/lib
LIB=

#------------------------------------------------------
# Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/api/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/l3/inc \
-I$(TOP)/pf/l3/dlm/inc

#---
# Source Files
#---
SRC=$(shell ls src/*.c)
OBJS=$(SRC:.c=.o)

#------------------------------------------------------
# Rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(AR) rc $(TARGET) $(OBJS)
	mv $(TARGET) $(LIBDIR)

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@

clean:
	rm -f $(LIBDIR)/$(TARGET)
	rm -f src/*.o src/*.d
endif

ifeq ($(BUILD_MODE_DEF), 1)
#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../..
OPT=-shared -fPIC

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/../ramdisk/target/lib
LIB=

#------------------------------------------------------
# Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/api/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/l3/inc \
-I$(TOP)/pf/l3/dlm/inc

#---
# Source Files
#---
SRC=$(shell ls src/*.c)
OBJS=$(SRC:.c=.o)

#------------------------------------------------------
# Rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(AR) rc $(TARGET) $(OBJS)
	mv $(TARGET) $(LIBDIR)

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -ldl -c $< $(INCLUDES) -o $@

clean:
	rm -f $(LIBDIR)/$(TARGET)
	rm -f src/*.o src/*.d
endif