#------------------------------------------------------
# Makefile for log library
#------------------------------------------------------
# Target: libbpf_log
#------------------------------------------------------
TARGET=libbpf_cmd.so.1.0
TARGETLINK=libbpf_cmd.so
include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../../..
OPT=-shared -fPIC -O2 -Wall -fstack-protector-all -MMD -D_FORTIFY_SOURCE=2 $(SYSROOT)

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lrt

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib

#------------------------------------------------------
# BPF Includes
#------------------------------------------------------
BPFINC=-I$(TOP)/src/sys/inc_mw \
-I$(TOP)/src/mw/inc \
-I$(TOP)/src/sys/inc \
-I$(TOP)/src/sys/inc_arch \
-I$(TOP)/src/api/inc \
-I../include \
-I$(TOP)/src/mw/oal/RM/svrm/include


#---
# Source Files
#---

SRC=$(shell ls *.c)
OBJ=$(SRC:.c=.o)


#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(OPT) -o $(TARGET) $(OBJ) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/
.c.o:
	$(CC) $(OPT) -c $< $(INC) $(BPFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d
