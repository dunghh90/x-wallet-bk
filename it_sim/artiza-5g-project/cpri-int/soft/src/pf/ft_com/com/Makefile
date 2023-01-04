#------------------------------------------------------
# Makefile for ft_com library
#------------------------------------------------------
# Target: libpf_com
#------------------------------------------------------
TARGET=libpf_com.so.1.0
TARGETLINK=libpf_com.so

include ../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
# LIB=-lbpf_com
BPF_LIB_DIR=$(TOP)/ramdisk/target/lib
BPF_LIBS=-L$(BPF_LIB_DIR) \
						-lbpf_svrm \
						-lbpf_facc \
						-lbpf_hrtm \
						-lbpf_string \
						-lbpf_ipcm \
						-lbpf_itcm \
						-lbpf_mblm \
						-lbpf_devc \
						-lbpf_log \
						-lbpf_com \
						-lDBGcmd

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
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/api/inc \
-I$(TOP)/mw/inc \
-I$(TOP)/mw/oal/RU/ipcm/include \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/mp/server/interface/function

#---
# Source Files
#---
SRC=$(shell ls src/*.c)
OBJS=$(SRC:.c=.o)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(SYSROOT) -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIB) -lDBGcmd
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)
	mv $(TARGETLINK) $(LIBDIR)
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f src/*.o src/*.d
