#-----------------------------------------------------
# Makefile for svrm library
#------------------------------------------------------
# Target: libbpf_svrm
#------------------------------------------------------
TARGET=libbpf_svrm.so.1.0
TARGETLINK=libbpf_svrm.so

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../../..
OPT=-shared -fPIC

include ../../../../../Makefile.in

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lbpf_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib

#------------------------------------------------------
# BPF Includes
#------------------------------------------------------
BPFINC=-I$(TOP)/src/sys/inc -I$(TOP)/src/sys/inc_arch -I$(TOP)/src/sys/inc_mw -I$(TOP)/src/mw/inc -I../include -I$(TOP)/src/mw/oal/RU/ipcm/include

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
	$(CC) $(OPT) $(SYSROOT) -pthread  -o  $(TARGET) $(OBJ) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INC) $(BPFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d
