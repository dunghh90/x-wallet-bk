ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=librrhapi_cpri.so.1.0
TARGETLINK=librrhapi_cpri.so

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm
LIBDIR=$(TOP)/../ramdisk/target/lib

DEV_HOME=../../../../../..
BPF_LIB_DIR=$(DEV_HOME)/ramdisk/target/lib
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


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/api/inc \
-I$(TOP)/pf/sys

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../*.c)
OBJS=$(SRC:.c=.o)


#------------------------------------------------------
# rules
#------------------------------------------------------
#all: $(TARGET)
#$(TARGET): $(OBJS)
#	$(AR) rc $(TARGET) $(OBJS)
#%.o: %.c
#	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES)  $(BPF_LIBS) $(LIB) $(BPFINC) -o $@

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(SYSROOT) -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@

install:
	mv -f ../*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ../*.o ../*.d $(TARGET) *~

endif

ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=librrhapi_cpri.so.1.0
TARGETLINK=librrhapi_cpri.so

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm
LIBDIR=$(TOP)/../ramdisk/target/lib

DEV_HOME=../../../../../..
BPF_LIB_DIR=$(DEV_HOME)/ramdisk/target/lib
BPF_LIB_DIR_QEMU=$(DEV_HOME)/ramdisk/target/usr/local/lib64
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
						-lbpf_com \


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/api/inc \
-I$(TOP)/pf/sys

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../*.c)
OBJS=$(SRC:.c=.o)


#------------------------------------------------------
# rules
#------------------------------------------------------
#all: $(TARGET)
#$(TARGET): $(OBJS)
#	$(AR) rc $(TARGET) $(OBJS)
#%.o: %.c
#	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES)  $(BPF_LIBS) $(LIB) $(BPFINC) -o $@

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(SYSROOT) -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@

install:
	mv -f ../*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ../*.o ../*.d $(TARGET) *~

endif