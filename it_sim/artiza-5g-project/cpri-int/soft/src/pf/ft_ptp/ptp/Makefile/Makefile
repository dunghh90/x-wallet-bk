#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: cmn/dcm
#------------------------------------------------------
TARGET=libdu_ptp.a

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..
OPT=
CFLAGS+= -DAPR_INCLUDED -DAPR_ONLY -DLINUX_3 -DOS_LINUX -DPROGRAM_TXT_ONTHEFLY -DZLS3038X_INCLUDED -DAPR_INCLUDED -DAPR_ONLY -DZLS30390_INCLUDED -DZLS30771_INCLUDED -D_DEBUG

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

#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/api/inc \
-I$(TOP)/mw/inc \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/mp/public \
-I$(TOP)/pf/ft_ptp/zlptp/zlApi/include \
-I$(TOP)/pf/ft_ptp/zlptp/zlApr/include \
-I$(TOP)/pf/ft_ptp/zlptp/zlDpll/include \
-I$(TOP)/pf/ft_ptp/zlptp/zlUserPorting/include \
-I$(TOP)/pf/ft_ptp/zlptp/zlPtp/include \
-I$(TOP)/pf/ft_ptp/zlptp/zlUserUtils/include \
-I$(TOP)/pf/ft_ptp/zlptp/zlUserExamples/include \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_ptp/ptp/inc

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
OBJS=$(SRC:.c=.o)

#------------------------------------------------------
# rules
#------------------------------------------------------

all: $(TARGET)
$(TARGET): $(OBJS)
	$(AR) rc $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c $< $(INCLUDES) -o $@
	
install:
	mv -f ../src/*.o ../lib
	mv -f ./*.a ../lib
	
clean:
	rm -f ../lib/*.o $(TARGET) *~
	rm -f ../src/*.o ../src/*.d $(TARGET) *~
