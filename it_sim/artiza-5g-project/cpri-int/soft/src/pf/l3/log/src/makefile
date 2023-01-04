ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: radioTrDcServer
#------------------------------------------------------
TARGET=docomo_L3

include ../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../..
OPT=

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

DEV_HOME=../../../..
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
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/api/inc \
-I$(TOP)/l3/inc \
-I$(TOP)/l3/log/inc \
-I$(TOP)/cmn/debug/inc

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls *.c)
OBJS=$(SRC:.c=.o)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c  $< $(INCLUDES) -o $@

clean:
	rm -f ./*.o

doxygen:
	doxygen ../doxygen/Doxyfile_l3_log

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: radioTrDcServer
#------------------------------------------------------
TARGET=docomo_L3

include ../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../..
OPT=

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

DEV_HOME=../../../..
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
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/api/inc \
-I$(TOP)/l3/inc \
-I$(TOP)/l3/log/inc \
-I$(TOP)/cmn/debug/inc

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls *.c)
OBJS=$(SRC:.c=.o)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c  $< $(INCLUDES) -o $@

clean:
	rm -f ./*.o

doxygen:
	doxygen ../doxygen/Doxyfile_l3_log

endif
