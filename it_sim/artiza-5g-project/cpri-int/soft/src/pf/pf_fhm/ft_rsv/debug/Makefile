ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for dpd debug library
#------------------------------------------------------
# Target: libbpf_itcm
#------------------------------------------------------
TARGET=libDBG_pf_rsv.so.1.0
TARGETLINK=libDBG_pf_rsv.so

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
# LIB=-lbpf_com
LIB=-lm  -lrt

BPF_LIB_DIR=$(TOP)/../ramdisk/target/lib
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
						-lbpf_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/../ramdisk/target/lib

#------------------------------------------------------
# PF Includes
#------------------------------------------------------
PFINC=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/pf_fhm/ft_rsv/rsv/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/mw/inc \
-I$(TOP)/api/inc

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
	$(CC) $(OPT) $(SYSROOT) -o $(TARGET) $(OBJ) -L$(LIBDIR) $(LIB) $(BPF_LIBS)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INC) $(PFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for dpd debug library
#------------------------------------------------------
# Target: libbpf_itcm
#------------------------------------------------------
TARGET=libDBG_pf_rsv.so.1.0
TARGETLINK=libDBG_pf_rsv.so

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
# LIB=-lbpf_com
LIB=-lm  -lrt

BPF_LIB_DIR=$(TOP)/../ramdisk/target/lib
BPF_LIB_DIR_QEMU=$(TOP)/../ramdisk/target/usr/local/lib64
BPF_LIBS=-L$(BPF_LIB_DIR) -L$(BPF_LIB_DIR_QEMU) \
						-lbpf_svrm \
						-lbpf_facc \
						-lbpf_hrtm \
						-lbpf_string \
						-lbpf_ipcm \
						-lbpf_itcm \
						-lbpf_mblm \
						-lbpf_devc_qemu \
						-lbpf_log \
						-lbpf_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/../ramdisk/target/lib

#------------------------------------------------------
# PF Includes
#------------------------------------------------------
PFINC=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/pf_fhm/ft_rsv/rsv/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/mw/inc \
-I$(TOP)/api/inc

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
	$(CC) $(OPT) $(SYSROOT) -o $(TARGET) $(OBJ) -L$(LIBDIR) $(LIB) $(BPF_LIBS)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INC) $(PFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d

endif