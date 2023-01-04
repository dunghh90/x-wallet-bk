#------------------------------------------------------
# Makefile for devc library for QEMU
#------------------------------------------------------
# Target: libbpf_devc
#------------------------------------------------------
TARGET=libbpf_devc_qemu.so.1.0
TARGETLINK=libbpf_devc_qemu.so

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lbpf_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib
LIBDIR_QEMU=$(TOP)/ramdisk/target/usr/local/lib64

#------------------------------------------------------
# BPF Includes
#------------------------------------------------------
BPFINC=-I$(TOP)/src/sys/inc -I$(TOP)/src/sys/inc_arch -I$(TOP)/src/sys/inc_mw -I$(TOP)/src/mw/inc -I../include

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
	$(CC) $(OPT) $(SYSROOT) -o $(TARGET) $(OBJ) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR_QEMU)/
	mv $(TARGETLINK) $(LIBDIR_QEMU)/
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INC) $(BPFINC)

clean:
	rm -f $(LIBDIR_QEMU)/$(TARGETLINK)
	rm -f $(LIBDIR_QEMU)/$(TARGET)
	rm -f *.o *.d
