#------------------------------------------------------
# Makefile for facc library
#------------------------------------------------------
# Target: libbpf_facc
#------------------------------------------------------
TARGET=libbpf_facc.so.1.0
TARGETLINK=libbpf_facc.so

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../../..
OPT=-shared -fPIC
#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lbpf_com \
    -lxml2


#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib

#------------------------------------------------------
# BPF Includes
#------------------------------------------------------
INCDIR=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux/usr/include
BPFINC=-I$(TOP)/src/sys/inc -I$(TOP)/src/sys/inc_arch -I$(TOP)/src/sys/inc_mw -I$(TOP)/src/mw/inc -I../include -I$(INCDIR)/libxml2

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
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INC) $(BPFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d
