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
						-lcmn_com \
						-lrrhapi_mpn \
						-lbpf_cmd \
						-lmpsw_srv

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
						-lcmn_com \
						-lrrhapi_mpn \
						-lbpf_cmd \
						-lmpsw_srv

endif


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_init/init/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/mp/server/interface/function

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c) \
	$(TOP)/pf/ft_com/com/src/f_com_alarmHitory.c \
	$(TOP)/pf/ft_com/com/src/f_com_dpda_getAlmHistLog.c \
	$(TOP)/pf/ft_com/com/src/f_com_FA.c \
	$(TOP)/pf/ft_com/com/src/f_com_getSysTim.c \
	$(TOP)/pf/ft_com/com/src/f_com_ram.c \
	$(TOP)/pf/ft_com/com/src/f_com_rom.c \
	$(TOP)/pf/ft_com/com/src/f_com_logSaveFlash.c \
	$(TOP)/pf/ft_com/com/src/fldc4.c \
	$(TOP)/pf/ft_com/com/src/f_com_SVCtl.c \
	$(TOP)/pf/ft_com/com/src/f_com_abortlog.c

OBJS=$(SRC:.c=.o)

# INIT DIR
INIT_ROOT=$(TOP)/pf/ft_init/init

# INIT INC DIR
INIT_INC_DIR=$(INIT_ROOT)/inc

# INIT SRC DIR
INIT_SRC_DIR=$(INIT_ROOT)/src
INIT_LM		=	$(INIT_ROOT)/DU_init
TARGET		=	$(INIT_LM)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(OBJS)
%.o: %.c
	$(CC)  $(CFLAGS) -c  $< $(INCLUDES) -o $@

all: $(TARGET)
$(INIT_LM):	$(INIT_OBJS) $(INIT_INCS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(INIT_LM) $(OBJS) $(BPF_LIBS) -pthread

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
						-lcmn_com \
						-lrrhapi_mpn \
						-lbpf_cmd \
						-lDBGcmd \
						-lmpsw_srv


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_init/init/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/mp/server/interface/function \
-I$(TOP)/mw/oal/RU/ipcm/include \
-I$(TOP)/mw/inc

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c) \
	$(TOP)/pf/ft_com/com/src/f_com_alarmHitory.c \
	$(TOP)/pf/ft_com/com/src/f_com_dpda_getAlmHistLog.c \
	$(TOP)/pf/ft_com/com/src/f_com_FA.c \
	$(TOP)/pf/ft_com/com/src/f_com_getSysTim.c \
	$(TOP)/pf/ft_com/com/src/f_com_ram.c \
	$(TOP)/pf/ft_com/com/src/f_com_rom.c \
	$(TOP)/pf/ft_com/com/src/f_com_logSaveFlash.c \
	$(TOP)/pf/ft_com/com/src/fldc4.c \
	$(TOP)/pf/ft_com/com/src/f_com_SVCtl.c \
	$(TOP)/pf/ft_com/com/src/f_com_abortlog.c

OBJS=$(SRC:.c=.o)

# INIT DIR
INIT_ROOT=$(TOP)/pf/ft_init/init

# INIT INC DIR
INIT_INC_DIR=$(INIT_ROOT)/inc

# INIT SRC DIR
INIT_SRC_DIR=$(INIT_ROOT)/src
INIT_LM		=	$(INIT_ROOT)/DU_init
TARGET		=	$(INIT_LM)

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(OBJS)
%.o: %.c
	$(CC)  $(CFLAGS) -c  $< $(INCLUDES) -o $@

all: $(TARGET)
$(INIT_LM):	$(INIT_OBJS) $(INIT_INCS)
	$(CC) -g $(LDFLAGS) $(SYSROOT) -o $(INIT_LM) $(OBJS) $(BPF_LIBS) -pthread -ldl

%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c  $< $(INCLUDES) -o $@

clean:
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

doxygen:
	doxygen ../../doxygen/Doxyfile_init
endif