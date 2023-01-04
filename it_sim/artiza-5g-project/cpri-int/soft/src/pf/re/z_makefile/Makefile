ifeq ($(BUILD_QEMU_DEF),0)
#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: rrh_re
#------------------------------------------------------
TARGET=rrh_re

include ../../../Makefile.in

#------------------------------------------------------
# parameter
#------------------------------------------------------
#CC=cc
#CC=/opt/ubq/devkit/arm/armv7/bin/arm-armv7-linux-gnueabi-gcc
OPT+= $(CFLAGS)
#OPT+= $(CFLAGS) -DOPT_16B_IT
#OPT+= $(CFLAGS) -DOPT_16B_PERFORM_TEST

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
LIBS=-lm -ldl -lrt -lpthread \
	-lbpf_com -lbpf_devc -lbpf_facc \
	-lbpf_mblm -lbpf_string -lbpf_hrtm -lbpf_svrm -lbpf_itcm -lbpf_ipcm -lbpf_log -lbpf_plog \
	-lrrhapi_com -lrrhapi_svp -lrrhapi_l2  -lcmn_com -lbpf_mblm
	#-lDBG_l2_lpb


CMN_LIBDIR=$(TOP)/src/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib
OBJDIR=$../obj/


#------------------------------------------------------
#  Includes
#------------------------------------------------------
INCLUDES= \
	-I$(TOP)/src/sys/inc \
	-I$(TOP)/src/cmn/dcm/inc \
	-I$(TOP)/src/sys/inc_mw \
	-I$(TOP)/src/sys/inc_arch \
	-I$(TOP)/src/api/inc \
	-I$(TOP)/src/cmn/com/inc \
	-I$(TOP)/src/cmn/cmu/inc \
	-I$(TOP)/src/cmn/debug/inc \
	-I$(TOP)/src/pf/sys \
	-I../inc \
	-I../com/inc \
	-I../main/inc \
	-I../rec/inc

#------------------------------------------------------
# Source Files
#------------------------------------------------------
SRC=\
	$(shell ls $(TOP)/src/cmn/com/src/*.c)\
	$(shell ls ../com/src/*.c)\
	$(TOP)/src/cmn/dcm/src/tm_cm_Assert.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Breq.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MAbort.c \
	$(TOP)/src/cmn/dcm/src/m_cm_L3MsgSend.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MWait.c \
	../main/src/re_main_main.c \
	../main/src/re_main_rom.c \
	../main/src/re_thrd_main.c \
	../main/src/re_main_ram.c \
	../main/src/re_main_systemInit.c \
	$(shell ls ../rec/src/*.c)

OBJS=$(SRC:.c=.o)
#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(LDFLAGS) -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIBS) $(CMN_LIBS) -ldl
%.o: %.c
	$(CC) $(OPT) -c  $< $(INCLUDES) -o $@
#	$(CC) -c $(cflag) $< -o $@ -L$(LIBDIR)
clean:
	rm -rf ../main/src/*.o ../main/src/*.d
	rm -rf ../rec/src/*.o ../rec/src/*.d
	rm -rf ../com/src/*.o ../com/src/*.d

endif

ifeq ($(BUILD_QEMU_DEF),1)
#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: rrh_re
#------------------------------------------------------
TARGET=rrh_re

include ../../../Makefile.in

#------------------------------------------------------
# parameter
#------------------------------------------------------
#CC=cc
#CC=/opt/ubq/devkit/arm/armv7/bin/arm-armv7-linux-gnueabi-gcc
OPT+= $(CFLAGS)
#OPT+= $(CFLAGS) -DOPT_16B_IT
#OPT+= $(CFLAGS) -DOPT_16B_PERFORM_TEST

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
LIBS=-lm -ldl -lrt -lpthread \
	-lbpf_com -lbpf_devc_qemu -lbpf_devc -lbpf_facc \
	-lbpf_mblm -lbpf_string -lbpf_hrtm -lbpf_svrm -lbpf_itcm -lbpf_ipcm -lbpf_log -lbpf_plog \
	-lrrhapi_com -lrrhapi_svp -lrrhapi_l2  -lcmn_com -lbpf_mblm
	#-lDBG_l2_lpb


CMN_LIBDIR=$(TOP)/src/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -L$(LIB_DIR_QEMU) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib
LIB_DIR_QEMU	=	$(TOP)/ramdisk/target/usr/local/lib64
OBJDIR=$../obj/


#------------------------------------------------------
#  Includes
#------------------------------------------------------
INCLUDES= \
	-I$(TOP)/src/sys/inc \
	-I$(TOP)/src/cmn/dcm/inc \
	-I$(TOP)/src/sys/inc_mw \
	-I$(TOP)/src/sys/inc_arch \
	-I$(TOP)/src/api/inc \
	-I$(TOP)/src/cmn/com/inc \
	-I$(TOP)/src/cmn/cmu/inc \
	-I$(TOP)/src/cmn/debug/inc \
	-I$(TOP)/src/pf/sys \
	-I../inc \
	-I../com/inc \
	-I../main/inc \
	-I../rec/inc

#------------------------------------------------------
# Source Files
#------------------------------------------------------
SRC=\
	$(shell ls $(TOP)/src/cmn/com/src/*.c)\
	$(shell ls ../com/src/*.c)\
	$(TOP)/src/cmn/dcm/src/tm_cm_Assert.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Breq.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MAbort.c \
	$(TOP)/src/cmn/dcm/src/m_cm_L3MsgSend.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MWait.c \
	../main/src/re_main_main.c \
	../main/src/re_main_rom.c \
	../main/src/re_thrd_main.c \
	../main/src/re_main_ram.c \
	../main/src/re_main_systemInit.c \
	$(shell ls ../rec/src/*.c)

OBJS=$(SRC:.c=.o)
#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(LDFLAGS) -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIBS) $(CMN_LIBS) -ldl
%.o: %.c
	$(CC) $(OPT) -c  $< $(INCLUDES) -o $@
#	$(CC) -c $(cflag) $< -o $@ -L$(LIBDIR)
clean:
	rm -rf ../main/src/*.o ../main/src/*.d
	rm -rf ../rec/src/*.o ../rec/src/*.d
	rm -rf ../com/src/*.o ../com/src/*.d

endif