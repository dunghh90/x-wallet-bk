ifeq ($(BUILD_QEMU_DEF), 0)
#------------------------------------------------------
# Makefile for APL process
#------------------------------------------------------
# Target: xxxxxx
#------------------------------------------------------
TARGET=DU_pf

include ../../../../Makefile.in

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
LIB=-lm -ldl -lrt -lpthread \
		$(TOP)/pf/ft_hdl/hdl/makefile/librrhthd_hdl.a \
		$(TOP)/pf/ft_dhcp/dhcp/Makefile/librrhthd_dhcp.a \
		$(TOP)/pf/ft_sfp/sfp/Makefile/librrhthd_sfp.a \
		$(TOP)/pf/ft_rfp/cca/Makefile/libdu_cca.a \
		$(TOP)/pf/ft_rfp/rfa/Makefile/libdu_rfa.a \
		$(TOP)/pf/ft_eqs/eqs/Makefile/libdu_eqs.a \
		$(TOP)/pf/ft_wdt/wdt/Makefile/libdu_wdt.a \
		$(TOP)/pf/ft_fhs/fhs/Makefile/librrhthd_fhs.a \
		$(TOP)/pf/ft_eom/eom/Makefile/librrhthd_eom.a \
		$(TOP)/pf/ft_pfm/pfm/Makefile/librrhthd_pfm.a \
		$(TOP)/pf/ft_nma/nma/makefile/librrhthd_nma.a \
		$(TOP)/pf/ft_lgc/lgc/makefile/librrhthd_lgc.a \
		$(TOP)/pf/ft_flm/fdl/Makefile/libdu_fdl.a \
		$(TOP)/pf/ft_flm/ful/Makefile/libdu_ful.a \
		$(TOP)/pf/ft_nma/nmas/makefile/librrhthd_nmas.a \
		$(TOP)/pf/ft_eio/eio/Makefile/libdu_eio.a \
		$(TOP)/pf/ft_mti/mti/Makefile/libdu_mti.a \
		$(TOP)/pf/ft_aih/aih/Makefile/libdu_aih.a\
		$(TOP)/pf/ft_aih/bsp/makefile/librrhthd_aibsp.a \
		-lm

#		$(TOP)/pf/ft_ptp/zlptp/objARMv8-LE/zlApiLib.out \
#		$(TOP)/pf/ft_ptp/ptp/Makefile/libdu_ptp.a \

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
						-lbpf_facc \
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
						-lbpf_facc \
						-lmpsw_srv


endif

CMN_LIBDIR=$(TOP)/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmu_mpproxy -lcmn_dbg


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_dhcp/dhcp/inc \
-I$(TOP)/pf/ft_hdl/hdl/inc \
-I$(TOP)/pf/ft_sfp/sfp/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/cmn/cmu/inc/ \
-I$(TOP)/pf/ft_main/main/inc \
-I$(TOP)/pf/ft_ptp/ptp/inc \
-I$(TOP)/pf/ft_rfp/cca/inc \
-I$(TOP)/pf/ft_rfp/rfa/inc \
-I$(TOP)/pf/ft_eqs/eqs/inc \
-I$(TOP)/pf/ft_wdt/wdt/inc \
-I$(TOP)/pf/ft_eom/eom/inc \
-I$(TOP)/pf/ft_pfm/pfm/inc \
-I$(TOP)/pf/ft_fhs/fhs/inc \
-I$(TOP)/pf/ft_nma/nma/inc \
-I$(TOP)/pf/ft_lgc/lgc/inc \
-I$(TOP)/pf/ft_flm/fdl/inc \
-I$(TOP)/pf/ft_flm/ful/inc \
-I$(TOP)/pf/ft_nma/nmas/inc \
-I$(TOP)/pf/ft_eio/eio/inc \
-I$(TOP)/pf/ft_mti/mti/inc \
-I$(TOP)/pf/ft_aih/bsp/inc \
-I$(TOP)/pf/ft_aih/aih/inc \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/mp/server/interface/function

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
	$(TOP)/pf/ft_com/com/src/f_com_iniSysParaTbl.o \
	$(TOP)/pf/ft_com/com/src/f_com_alarmHitory.o \
	$(TOP)/pf/ft_com/com/src/f_com_dpda_getAlmHistLog.o \
	$(TOP)/pf/ft_com/com/src/f_com_MSI.o \
	$(TOP)/pf/ft_com/com/src/f_com_searchable_vlan_id.o \
	$(TOP)/pf/ft_com/com/src/f_com_mtCtl.o

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(TARGET) $(OBJS) $(CMNOBJ) $(LIB) $(BPF_LIBS)  $(CMN_LIBS)  -ldl -lmpsw_srv
#	$(AR) rc $(TARGET) $(OBJS)
#	$(CC) $(OPT) -o $(TARGET) $(OBJS) $(BPF_LIBS) $(LIB) $(BPFINC)
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
TARGET=DU_pf

include ../../../../Makefile.in
#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library	C9 modify lbpf_devc -> lbpf_devc_qemu
#------------------------------------------------------
LIB= -ldl -lrt -lc -lpthread \
		$(TOP)/pf/ft_hdl/hdl/makefile/librrhthd_hdl.a \
		$(TOP)/pf/ft_dhcp/dhcp/Makefile/librrhthd_dhcp.a \
		$(TOP)/pf/ft_sfp/sfp/Makefile/librrhthd_sfp.a \
		$(TOP)/pf/ft_rfp/cca/Makefile/libdu_cca.a \
		$(TOP)/pf/ft_rfp/rfa/Makefile/libdu_rfa.a \
		$(TOP)/pf/ft_eqs/eqs/Makefile/libdu_eqs.a \
		$(TOP)/pf/ft_wdt/wdt/Makefile/libdu_wdt.a \
		$(TOP)/pf/ft_fhs/fhs/Makefile/librrhthd_fhs.a \
		$(TOP)/pf/ft_eom/eom/Makefile/librrhthd_eom.a \
		$(TOP)/pf/ft_pfm/pfm/Makefile/librrhthd_pfm.a \
		$(TOP)/pf/ft_nma/nma/makefile/librrhthd_nma.a \
		$(TOP)/pf/ft_lgc/lgc/makefile/librrhthd_lgc.a \
		$(TOP)/pf/ft_flm/fdl/Makefile/libdu_fdl.a \
		$(TOP)/pf/ft_flm/ful/Makefile/libdu_ful.a \
		$(TOP)/pf/ft_nma/nmas/makefile/librrhthd_nmas.a \
		$(TOP)/pf/ft_eio/eio/Makefile/libdu_eio.a \
		$(TOP)/pf/ft_mti/mti/Makefile/libdu_mti.a \
		$(TOP)/pf/ft_aih/aih/Makefile/libdu_aih.a\
		$(TOP)/pf/ft_aih/bsp/makefile/librrhthd_aibsp.a\
		-lm
#$(TOP)/pf/ft_ptp/zlptp/objARMv8-LE/zlApiLib.out
#		$(TOP)/pf/ft_ptp/ptp/Makefile/libdu_ptp.a \

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
						-lcmn_com \
						-lrrhapi_mpn \
						-lbpf_cmd \
						-lbpf_facc \
						-lmpsw_srv

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
						-lcmn_com \
						-lrrhapi_mpn \
						-lbpf_cmd \
						-lbpf_facc \
						-lmpsw_srv \
						-lDBGcmd

endif

CMN_LIBDIR=$(TOP)/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmu_mpproxy -lcmn_dbg


#------------------------------------------------------
# PF Includes
#------------------------------------------------------
INCLUDES=\
-I$(TOP)/sys/inc \
-I$(TOP)/sys/inc_arch \
-I$(TOP)/sys/inc_mw \
-I$(TOP)/api/inc \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/pf/sys \
-I$(TOP)/pf/ft_com/com/inc \
-I$(TOP)/pf/ft_dhcp/dhcp/inc \
-I$(TOP)/pf/ft_hdl/hdl/inc \
-I$(TOP)/pf/ft_sfp/sfp/inc \
-I$(TOP)/cmn/com/inc \
-I$(TOP)/cmn/dcm/inc \
-I$(TOP)/cmn/debug/inc \
-I$(TOP)/cmn/cmu/inc/ \
-I$(TOP)/pf/ft_main/main/inc \
-I$(TOP)/pf/ft_ptp/ptp/inc \
-I$(TOP)/pf/ft_rfp/cca/inc \
-I$(TOP)/pf/ft_rfp/rfa/inc \
-I$(TOP)/pf/ft_eqs/eqs/inc \
-I$(TOP)/pf/ft_wdt/wdt/inc \
-I$(TOP)/pf/ft_eom/eom/inc \
-I$(TOP)/pf/ft_pfm/pfm/inc \
-I$(TOP)/pf/ft_fhs/fhs/inc \
-I$(TOP)/pf/ft_nma/nma/inc \
-I$(TOP)/pf/ft_lgc/lgc/inc \
-I$(TOP)/pf/ft_flm/fdl/inc \
-I$(TOP)/pf/ft_flm/ful/inc \
-I$(TOP)/pf/ft_nma/nmas/inc \
-I$(TOP)/pf/ft_eio/eio/inc \
-I$(TOP)/pf/ft_mti/mti/inc \
-I$(TOP)/pf/ft_aih/bsp/inc \
-I$(TOP)/pf/ft_aih/aih/inc \
-I$(TOP)/mp/public \
-I$(TOP)/mp/server/interface/msg \
-I$(TOP)/mp/server/interface/function

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
	$(TOP)/pf/ft_com/com/src/f_com_iniSysParaTbl.o \
	$(TOP)/pf/ft_com/com/src/f_com_alarmHitory.o \
	$(TOP)/pf/ft_com/com/src/f_com_dpda_getAlmHistLog.o \
	$(TOP)/pf/ft_com/com/src/f_com_MSI.o \
	$(TOP)/pf/ft_com/com/src/f_com_searchable_vlan_id.o \
	$(TOP)/pf/ft_com/com/src/f_com_mtCtl.o

#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $(SYSROOT) -o $(TARGET) -DSIM_DEBUG $(OBJS) $(CMNOBJ) $(LIB) $(BPF_LIBS)  $(CMN_LIBS)  -ldl -lmpsw_srv -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
#	$(AR) rc $(TARGET) $(OBJS)
#	$(CC) $(OPT) -o $(TARGET) $(OBJS) $(BPF_LIBS) $(LIB) $(BPFINC)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -c	 $< $(INCLUDES) -o $@

clean:
	rm -f ../src/*.o ../src/*.d $(TARGET) *~
endif