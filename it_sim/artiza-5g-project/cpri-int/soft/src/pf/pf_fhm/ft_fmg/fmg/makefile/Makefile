ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: cmn/dcm
#------------------------------------------------------
TARGET=librrhthd_fmg.a

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..
OPT=

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

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
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/pf_fhm/ft_fmg/fmg/inc

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
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
	$(AR) rc $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) $(SYSROOT) -c $< $(INCLUDES) -o $@

install:
	mv -f ../src/*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ../lib/*.o $(TARGET) *~
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: cmn/dcm
#------------------------------------------------------
TARGET=librrhthd_fmg.a

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..
OPT=

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm

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
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/pf_fhm/ft_fmg/fmg/inc

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
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
	$(AR) rc $(TARGET) $(OBJS)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) $(SYSROOT) -c $< $(INCLUDES) -lbpf_facc -o $@

install:
	mv -f ../src/*.o ../lib
	mv -f ./*.a ../lib

clean:
	rm -f ../lib/*.o $(TARGET) *~
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

endif