ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=librrhthd_dpd.a

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..
OPT=
CFLAGS+= -DAPR_INCLUDED -DAPR_ONLY -DLINUX_3 -DOS_LINUX -DPROGRAM_TXT_ONTHEFLY -DZLS3038X_INCLUDED -DAPR_INCLUDED -DAPR_ONLY -DZLS30390_INCLUDED -DZLS30771_INCLUDED

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

DEV_HOME=../../../../..
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
						-lrfic_pmc \


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_rfp/dpd/inc \
-I$(TOP)/pf/ft_rfp/rfic/inc \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/mw/inc \
-I$(TOP)/api/inc

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
OBJS=$(SRC:.c=.o) \
$(TOP)/pf/ft_com/com/src/f_com_rom.o \
$(TOP)/pf/ft_com/com/src/f_com_getSysTim.o \
$(TOP)/pf/ft_com/com/src/f_com_AlmCtl.o

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
	$(AR) rc $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@

install:
	mv -f ../src/*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ../lib/*.o ../src/*.o ../src/*.d $(TOP)/pf/ft_com/com/src/f_com_rom.o $(TARGET) *~

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=librrhthd_dpd.a

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..
OPT=
CFLAGS+= -DAPR_INCLUDED -DAPR_ONLY -DLINUX_3 -DOS_LINUX -DPROGRAM_TXT_ONTHEFLY -DZLS3038X_INCLUDED -DAPR_INCLUDED -DAPR_ONLY -DZLS30390_INCLUDED -DZLS30771_INCLUDED

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

DEV_HOME=../../../../..
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
						-lrfic_pmc \


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_rfp/dpd/inc \
-I$(TOP)/pf/ft_rfp/rfic/inc \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/mw/inc \
-I$(TOP)/api/inc

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
OBJS=$(SRC:.c=.o) \
$(TOP)/pf/ft_com/com/src/f_com_rom.o \
$(TOP)/pf/ft_com/com/src/f_com_getSysTim.o \
$(TOP)/pf/ft_com/com/src/f_com_AlmCtl.o

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
	$(AR) rc $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@

install:
	mv -f ../src/*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ../lib/*.o ../src/*.o ../src/*.d $(TOP)/pf/ft_com/com/src/f_com_rom.o $(TARGET) *~

endif