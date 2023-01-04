ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

ifeq ($(BUILD_MODE_DEF), 0)
DEV_HOME=../../../../../
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
						-lbpf_plog \
						-lbpf_com \
						-lDBGcmd \
						-lcmn_com

endif

ifeq ($(BUILD_MODE_DEF), 1)
DEV_HOME=../../../../../
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
						-lbpf_plog \
						-lbpf_com \
						-lcmn_com

endif

#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_init/mw/inc \
-I$(TOP)/cmn/debug/inc \

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c) \
	$(TOP)/pf/ft_com/com/src/f_com_rom.c

OBJS=$(SRC:.c=.o)

# INIT DIR
INIT_ROOT=$(TOP)/pf/ft_init/mw

# INIT INC DIR
INIT_INC_DIR=$(INIT_ROOT)/inc

# INIT SRC DIR
INIT_SRC_DIR=$(INIT_ROOT)/src
INIT_LM		=	$(INIT_ROOT)/DU_mw
TARGET		=	$(INIT_LM)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(OBJS)
%.o: %.c
	$(CC)  $(CFLAGS) -c  $< $(INCLUDES) -o $@

all: $(TARGET)
$(INIT_LM):	$(INIT_OBJS) $(INIT_INCS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(INIT_LM) $(OBJS) $(BPF_LIBS)

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c  $< $(INCLUDES) -o $@

clean:
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

doxygen:
	doxygen ../../doxygen/Doxyfile_init

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
include ../../../../Makefile.in
#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library	C9 modify lbpf_devc -> lbpf_devc_qemu
#------------------------------------------------------
LIB=-lm

ifeq ($(BUILD_MODE_DEF), 0)
DEV_HOME=../../../../../
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
						-lbpf_devc \
						-lbpf_log \
						-lbpf_plog \
						-lbpf_com \
						-lDBGcmd \
						-lcmn_com

endif
ifeq ($(BUILD_MODE_DEF), 1)
DEV_HOME=../../../../../
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
						-lbpf_devc \
						-lbpf_log \
						-lbpf_plog \
						-lbpf_com \
						-lcmn_com

endif
#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_init/mw/inc \
-I$(TOP)/cmn/debug/inc \

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c) \
	$(TOP)/pf/ft_com/com/src/f_com_rom.c

OBJS=$(SRC:.c=.o)

# INIT DIR
INIT_ROOT=$(TOP)/pf/ft_init/mw

# INIT INC DIR
INIT_INC_DIR=$(INIT_ROOT)/inc

# INIT SRC DIR
INIT_SRC_DIR=$(INIT_ROOT)/src
INIT_LM		=	$(INIT_ROOT)/DU_mw
TARGET		=	$(INIT_LM)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(OBJS)
%.o: %.c
	$(CC)  $(CFLAGS) -c  $< $(INCLUDES) -o $@

all: $(TARGET)
$(INIT_LM):	$(INIT_OBJS) $(INIT_INCS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(INIT_LM) $(OBJS) $(BPF_LIBS) -ldl

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c  $< $(INCLUDES) -o $@

clean:
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

doxygen:
	doxygen ../../doxygen/Doxyfile_init
endif