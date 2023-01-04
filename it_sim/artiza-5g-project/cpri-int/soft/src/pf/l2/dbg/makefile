ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=libDBG_l2_lpb.so.1.0
TARGETLINK=libDBG_l2_lpb.so

include ../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm
LIBDIR=$(TOP)/ramdisk/target/lib

DEV_HOME=../../..
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
-I$(TOP)/src/sys/inc \
-I$(TOP)/src/sys/inc_mw \
-I$(TOP)/src/sys/inc_arch \
-I$(TOP)/src/api/inc \
-I$(TOP)/src/cmn/dcm/inc \
-I$(TOP)/src/cmn/com/inc \
-I$(TOP)/src/cmn/cmu/inc \
-I$(TOP)/src/cmn/debug/inc \
-I$(TOP)/src/l2/inc \
-I$(TOP)/src/l2/main/inc \
-I$(TOP)/src/l2/pol/inc \
-I$(TOP)/src/l2/txs/inc \
-I$(TOP)/src/l2/rxs/inc \
-I$(TOP)/src/l2/lpb/inc \
-I$(TOP)/src/l2/txs/inc \
-I$(TOP)/src/l2/dbg/inc

BPFINC=-I$(TOP)/src/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ./*.c)
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
	$(CC) $(OPT)  -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	#cp $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -rdynamic -c $< $(INCLUDES) -o $@

install:
	mv -f ../*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ./*.o ./*.d ./src/*.d $(TARGET) *~

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=libDBG_l2_lpb.so.1.0
TARGETLINK=libDBG_l2_lpb.so

include ../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../..
OPT=-shared -fPIC

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm
LIBDIR=$(TOP)/ramdisk/target/lib

DEV_HOME=../../..
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
-I$(TOP)/src/sys/inc \
-I$(TOP)/src/sys/inc_mw \
-I$(TOP)/src/sys/inc_arch \
-I$(TOP)/src/api/inc \
-I$(TOP)/src/cmn/dcm/inc \
-I$(TOP)/src/cmn/com/inc \
-I$(TOP)/src/cmn/cmu/inc \
-I$(TOP)/src/cmn/debug/inc \
-I$(TOP)/src/l2/inc \
-I$(TOP)/src/l2/main/inc \
-I$(TOP)/src/l2/pol/inc \
-I$(TOP)/src/l2/txs/inc \
-I$(TOP)/src/l2/rxs/inc \
-I$(TOP)/src/l2/lpb/inc \
-I$(TOP)/src/l2/txs/inc \
-I$(TOP)/src/l2/dbg/inc

BPFINC=-I$(TOP)/src/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ./*.c)
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
	$(CC) $(OPT)  -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIB)
	ln -s $(TARGET) $(TARGETLINK)
	#cp $(TARGET) $(TARGETLINK)
	mv $(TARGET) $(LIBDIR)/
	mv $(TARGETLINK) $(LIBDIR)/

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -rdynamic -c $< $(INCLUDES) -o $@

install:
	mv -f ../*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ./*.o ./*.d ./src/*.d $(TARGET) *~

endif