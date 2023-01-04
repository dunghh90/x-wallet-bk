ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for dpd debug library
#------------------------------------------------------
# Target: libDBG_pf_cpr
#------------------------------------------------------
TARGET=libDBG_pf_cpr.so.1.0
TARGETLINK=libDBG_pf_cpr.so

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
# LIB=-lbpf_com
LIB=-lm -lrt

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
						-lbpf_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib

#------------------------------------------------------
# PF Includes
#------------------------------------------------------
PFINC=\
-I$(TOP)/src/sys/inc \
-I$(TOP)/src/sys/inc_mw \
-I$(TOP)/src/pf/sys \
-I$(TOP)/src/pf/pf_fhm/ft_cpr/cpr/inc \
-I$(TOP)/src/cmn/dcm/inc \
-I$(TOP)/src/pf/ft_com/com/inc \
-I$(TOP)/src/sys/inc_arch \
-I$(TOP)/src/cmn/com/inc \
-I$(TOP)/src/mw/inc \
-I$(TOP)/src/api/inc


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
	mv $(TARGET) $(LIBDIR)
	mv $(TARGETLINK) $(LIBDIR)
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
# Target: libDBG_pf_cpr
#------------------------------------------------------
TARGET=libDBG_pf_cpr.so.1.0
TARGETLINK=libDBG_pf_cpr.so

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
# LIB=-lbpf_com
LIB=-lm -lrt

BPF_LIB_DIR=$(TOP)/ramdisk/target/lib
BPF_LIB_DIR_QEMU=$(TOP)/ramdisk/target/usr/local/lib64
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
LIBDIR=$(TOP)/ramdisk/target/lib

#------------------------------------------------------
# PF Includes
#------------------------------------------------------
PFINC=\
-I$(TOP)/src/sys/inc \
-I$(TOP)/src/sys/inc_mw \
-I$(TOP)/src/pf/sys \
-I$(TOP)/src/pf/pf_fhm/ft_cpr/cpr/inc \
-I$(TOP)/src/cmn/dcm/inc \
-I$(TOP)/src/pf/ft_com/com/inc \
-I$(TOP)/src/sys/inc_arch \
-I$(TOP)/src/cmn/com/inc \
-I$(TOP)/src/mw/inc \
-I$(TOP)/src/api/inc


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
	mv $(TARGET) $(LIBDIR)
	mv $(TARGETLINK) $(LIBDIR)
.c.o:
	$(CC) $(OPT) $(CFLAGS) -c $< $(INC) $(PFINC)

clean:
	rm -f $(LIBDIR)/$(TARGETLINK)
	rm -f $(LIBDIR)/$(TARGET)
	rm -f *.o *.d

endif