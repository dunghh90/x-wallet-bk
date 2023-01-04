ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=rrh_pf_fhm

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm -ldl -lrt -lpthread \
		$(TOP)/pf/pf_fhm/ft_wdt/wdt/Makefile/librrhthd_wdt.a \
		$(TOP)/pf/pf_fhm/ft_hdl/hdl/makefile/librrhthd_hdl.a \
		$(TOP)/pf/pf_fhm/ft_eth/eth/Makefile/librrhthd_eth.a \
		$(TOP)/pf/pf_fhm/ft_log/log/makefile/librrhthd_log.a \
		$(TOP)/pf/ft_rfp/dpd/makefile/librrhthd_dpd.a \
		$(TOP)/pf/pf_fhm/ft_rsv/rsv/makefile/librrhthd_rsv.a \
		$(TOP)/pf/pf_fhm/ft_cpr/cpr/makefile/librrhthd_cpr.a \
		$(TOP)/pf/pf_fhm/ft_cpr/nego/makefile/librrhthd_nego.a \
		$(TOP)/pf/pf_fhm/ft_fmg/fmg/makefile/librrhthd_fmg.a \
		$(TOP)/pf/pf_fhm/ft_frw/frw/makefile/librrhthd_frw.a \
		$(TOP)/pf/pf_fhm/ft_frw/frwsla/makefile/librrhthd_frwsla.a \
		$(TOP)/pf/pf_fhm/ft_frw/frwre/makefile/librrhthd_frwre.a \

ifeq ($(BUILD_MODE_DEF), 0)
DEV_HOME=../../../../../../
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
						-lDBG_pf_dpda \
						-lDBG_pf_rsv \
						-lDBG_pf_cpr \
						-lcmn_com
						#-lrfic_pmc
endif

ifeq ($(BUILD_MODE_DEF), 1)
DEV_HOME=../../../../../../
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
						-lDBG_pf_dpda \
						-lDBG_pf_cpr \
						-lDBG_pf_rsv \
						-lcmn_com \
						-ldl \
						-lm \
						-lstdc++
						#-lrfic_pmc


endif



CMN_LIBDIR=$(TOP)/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/cmn/cmu/inc/ \
-I$(TOP)/pf/pf_fhm/ft_hdl/hdl/inc \
-I$(TOP)/pf/pf_fhm/ft_wdt/wdt/inc \
-I$(TOP)/pf/ft_rfp/dpd/inc \
-I$(TOP)/pf/pf_fhm/ft_eth/eth/inc \
-I$(TOP)/pf/pf_fhm/ft_log/log/inc \
-I$(TOP)/pf/pf_fhm/ft_rsv/rsv/inc \
-I$(TOP)/pf/pf_fhm/ft_cpr/cpr/inc \
-I$(TOP)/pf/pf_fhm/ft_cpr/nego/inc \
-I$(TOP)/pf/pf_fhm/ft_frw/frw/inc \
-I$(TOP)/pf/pf_fhm/ft_frw/frwsla/inc \
-I$(TOP)/pf/pf_fhm/ft_frw/frwre/inc \
-I$(TOP)/pf/pf_fhm/ft_fmg/fmg/inc \
-I$(TOP)/pf/pf_fhm/ft_main/main/inc \
-I$(TOP)/pf/ft_rfp/dpd/inc/

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
OBJS=$(SRC:.c=.o)
CMNOBJ= \
	$(TOP)/cmn/com/src/f_cmn_com_abort.o \
	$(TOP)/cmn/com/src/f_cmn_com_rom.o \
	$(TOP)/cmn/com/src/f_cmn_com_assert.o \
	$(TOP)/pf/ft_com/com/src/fldc4.o \
	$(TOP)/pf/ft_com/com/src/f_com_AlmCtl.o \
	$(TOP)/pf/ft_com/com/src/f_com_abortlog.o \
	$(TOP)/pf/ft_com/com/src/f_com_getSysTim.o \
	$(TOP)/pf/ft_com/com/src/f_com_msgCom.o \
	$(TOP)/pf/ft_com/com/src/f_com_ram.o \
	$(TOP)/pf/ft_com/com/src/f_com_rom.o \
	$(TOP)/pf/ft_com/com/src/f_com_SVCtl.o \
	$(TOP)/pf/ft_com/com/src/f_com_taskDelay.o \
	$(TOP)/pf/ft_com/com/src/f_com_FA.o \
	$(TOP)/pf/ft_com/com/src/f_com_iniSysParaTbl.o


#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(TARGET) $(OBJS) $(CMNOBJ) $(BPF_LIBS) $(LIB) $(CMN_LIBS) -lDBG_pf_rsv -lbpf_facc -ldl -lm
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c	 $< $(INCLUDES) -o $@

clean:
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

endif
ifeq ($(BUILD_QEMU_DEF), 1)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=rrh_pf_fhm

include ../../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm -ldl -lrt -lpthread \
		$(TOP)/pf/pf_fhm/ft_wdt/wdt/Makefile/librrhthd_wdt.a \
		$(TOP)/pf/pf_fhm/ft_hdl/hdl/makefile/librrhthd_hdl.a \
		$(TOP)/pf/pf_fhm/ft_eth/eth/Makefile/librrhthd_eth.a \
		$(TOP)/pf/pf_fhm/ft_log/log/makefile/librrhthd_log.a \
		$(TOP)/pf/ft_rfp/dpd/makefile/librrhthd_dpd.a \
		$(TOP)/pf/pf_fhm/ft_rsv/rsv/makefile/librrhthd_rsv.a \
		$(TOP)/pf/pf_fhm/ft_cpr/cpr/makefile/librrhthd_cpr.a \
		$(TOP)/pf/pf_fhm/ft_cpr/nego/makefile/librrhthd_nego.a \
		$(TOP)/pf/pf_fhm/ft_fmg/fmg/makefile/librrhthd_fmg.a \
		$(TOP)/pf/pf_fhm/ft_frw/frw/makefile/librrhthd_frw.a \
		$(TOP)/pf/pf_fhm/ft_frw/frwsla/makefile/librrhthd_frwsla.a \
		$(TOP)/pf/pf_fhm/ft_frw/frwre/makefile/librrhthd_frwre.a \

ifeq ($(BUILD_MODE_DEF), 0)
DEV_HOME=../../../../../../
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
						-lDBG_pf_dpda \
						-lDBG_pf_rsv \
						-lDBG_pf_cpr \
						-lcmn_com
						#-lrfic_pmc
endif
ifeq ($(BUILD_MODE_DEF), 1)
DEV_HOME=../../../../../../
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
						-lDBG_pf_dpda \
						-lDBG_pf_rsv \
						-lDBG_pf_cpr \
						-lcmn_com \
						-ldl \
						-lm

						#-lrfic_pmc


endif

CMN_LIBDIR=$(TOP)/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/cmn/cmu/inc/ \
-I$(TOP)/pf/pf_fhm/ft_hdl/hdl/inc \
-I$(TOP)/pf/pf_fhm/ft_wdt/wdt/inc \
-I$(TOP)/pf/pf_fhm/ft_rfp/dpd/inc \
-I$(TOP)/pf/pf_fhm/ft_eth/eth/inc \
-I$(TOP)/pf/pf_fhm/ft_log/log/inc \
-I$(TOP)/pf/pf_fhm/ft_rsv/rsv/inc \
-I$(TOP)/pf/pf_fhm/ft_cpr/cpr/inc \
-I$(TOP)/pf/pf_fhm/ft_cpr/nego/inc \
-I$(TOP)/pf/pf_fhm/ft_frw/frw/inc \
-I$(TOP)/pf/pf_fhm/ft_frw/frwsla/inc \
-I$(TOP)/pf/pf_fhm/ft_frw/frwre/inc \
-I$(TOP)/pf/pf_fhm/ft_fmg/fmg/inc \
-I$(TOP)/pf/pf_fhm/ft_main/main/inc\
-I$(TOP)/pf/ft_rfp/dpd/inc/

BPFINC=-I$(TOP)/sys/inc_mw

#---
# Source Files
#---
SRC=$(shell ls ../src/*.c)
OBJS=$(SRC:.c=.o)
CMNOBJ= \
	$(TOP)/cmn/com/src/f_cmn_com_abort.o \
	$(TOP)/cmn/com/src/f_cmn_com_rom.o \
	$(TOP)/cmn/com/src/f_cmn_com_assert.o \
	$(TOP)/pf/ft_com/com/src/fldc4.o \
	$(TOP)/pf/ft_com/com/src/f_com_AlmCtl.o \
	$(TOP)/pf/ft_com/com/src/f_com_abortlog.o \
	$(TOP)/pf/ft_com/com/src/f_com_getSysTim.o \
	$(TOP)/pf/ft_com/com/src/f_com_msgCom.o \
	$(TOP)/pf/ft_com/com/src/f_com_ram.o \
	$(TOP)/pf/ft_com/com/src/f_com_rom.o \
	$(TOP)/pf/ft_com/com/src/f_com_SVCtl.o \
	$(TOP)/pf/ft_com/com/src/f_com_taskDelay.o \
	$(TOP)/pf/ft_com/com/src/f_com_FA.o \
	$(TOP)/pf/ft_com/com/src/f_com_iniSysParaTbl.o


#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(TARGET) $(OBJS) $(CMNOBJ) $(BPF_LIBS) $(LIB) $(CMN_LIBS) -lDBG_pf_rsv -lbpf_facc -ldl -lm
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c	 $< $(INCLUDES) -o $@

clean:
	rm -f ../src/*.o ../src/*.d $(TARGET) *~

endif