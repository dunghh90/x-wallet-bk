#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: libbpf_com 
#------------------------------------------------------
TARGET=libbpf_com.so.1.0
TARGETLINK=libbpf_com.so

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib

#------------------------------------------------------
# BPF Includes
#------------------------------------------------------
BPFINC= -I$(TOP)/src/sys/inc -I$(TOP)/src/sys/inc_arch -I$(TOP)/src/sys/inc_mw -I$(TOP)/src/mw/inc -I../include

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
#	$(CC) $(OPT) -rdynamic -DBPF_DEBUG_CONSOLE -DBPF_DEBUG -D_GNU_SOURCE  -c $< $(INC) $(BPFINC)
#	$(CC) $(OPT) -rdynamic -DBPF_DEBUG_CONSOLE -DBPF_COM_LOG_ERR -D_GNU_SOURCE  -c $< $(INC) $(BPFINC)
#	$(CC) $(OPT) -rdynamic -DBPF_DEBUG -D_GNU_SOURCE  -c $< $(INC) $(BPFINC)
	$(CC) $(OPT) $(CFLAGS) -rdynamic -DBPF_COM_LOG_ERR -D_GNU_SOURCE  -c  $< $(INC) $(BPFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d
