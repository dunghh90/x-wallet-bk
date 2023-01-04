ifeq ($(BUILD_QEMU_DEF),0)
#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: l3 rrh
#------------------------------------------------------
TARGET=rrh_l3

include ../../../Makefile.in

#------------------------------------------------------
# parameter
#------------------------------------------------------
ifeq ($(BUILD_MODE_DEF), 0)
#CC=cc
endif
ifeq ($(BUILD_MODE_DEF), 1)
CC=cc
endif

#CC=/opt/ubq/devkit/arm/armv7/bin/arm-armv7-linux-gnueabi-gcc
OPT+= $(CFLAGS)
#OPT+= $(CFLAGS) -DOPT_16B_IT

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
	-lrrhapi_l2 -lrrhapi_com -lrrhapi_cpri -lrrhapi_fmng -lrrhapi_svp -lrrhapi_log -lm

CMN_LIBDIR=$(TOP)/src/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_com -lbpf_mblm -ldl

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib
OBJDIR=../obj/


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
	-I$(TOP)/src/pf/ft_com/com/inc \
	-I../inc \
	-I../main/inc \
	-I../cpr/inc \
	-I../dlm/inc \
	-I../rct/inc \
	-I../log/inc

#------------------------------------------------------
# Source Files
#------------------------------------------------------
SRC=\
	$(TOP)/src/cmn/dcm/src/tm_cm_Breq.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ActiveChk.c \
	$(TOP)/src/cmn/dcm/src/m_cm_AxC.c \
	$(TOP)/src/cmn/dcm/src/m_cm_CardSarch.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ChkCardSta.c \
	$(TOP)/src/cmn/dcm/src/m_cm_chksig.c \
	$(TOP)/src/cmn/dcm/src/m_cm_Delay.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ErrCntRegSet.c \
	$(TOP)/src/cmn/dcm/src/m_cm_Hex.c \
	$(TOP)/src/cmn/dcm/src/m_cm_L3MsgSend.c \
	$(TOP)/src/cmn/dcm/src/m_cm_SysParaDSet.c \
	$(TOP)/src/cmn/dcm/src/m_cm_writeLEDReg.c \
	$(TOP)/src/cmn/dcm/src/m_cm_l3log.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Assert.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_IsSpace.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Itoa.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MAbort.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPadc.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPadi.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPads.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MWait.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_CheckSumCalc.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_CheckSumCalcCpri.c \
	../main/src/l3_main_main.c \
	../main/src/l3_main_ram.c \
	../main/src/l3_main_rom.c \
	../main/src/l3_main_systemInit.c \
	../main/src/l3_thrd_main.c \
	../com/src/l3_com_ram.c \
	../com/src/l3_com_rom.c \
	../com/src/l3_com_ushortsort_asc.c \
	../cpr/src/m_cr_Anlz.c \
	../cpr/src/m_cr_cm_RunHisSet.c \
	../cpr/src/m_cr_ComLinkSettingL3Rsp.c \
	../cpr/src/m_cr_CpriMsgRcvProc.c \
	../cpr/src/m_cr_CpriStaPriorityChk.c \
	../cpr/src/m_cr_CpriStateChgProc.c \
	../cpr/src/m_cr_CpriStateChgProcRE.c \
	../cpr/src/m_cr_DlinkSettingReq.c \
	../cpr/src/m_cr_DlinkStopReq.c \
	../cpr/src/m_cr_HcReq.c \
	../cpr/src/m_cr_HcTimeoutNtc.c \
	../cpr/src/m_cr_HcTimeoutNtcRSP.c \
	../cpr/src/m_cr_HcTimeoutNtcSND.c \
	../cpr/src/m_cr_HcRsp.c \
	../cpr/src/m_cr_Init.c \
	../cpr/src/m_cr_L2DeactReq.c \
	../cpr/src/m_cr_L2LinkStateProc.c \
	../cpr/src/m_cr_L2LinkStateProcRE.c \
	../cpr/src/m_cr_L3HcSta.c \
	../cpr/src/m_cr_L3StaNtc.c \
	../cpr/src/m_cr_main.c \
	../cpr/src/m_cr_Nop.c \
	../cpr/src/m_cr_ram.c \
	../cpr/src/m_cr_rom.c \
	../cpr/src/m_cr_CpriLinkPrepareRes.c \
	../cpr/src/m_cr_CpriLinkPrepareReq.c \
	../cpr/src/m_cr_CpriRestartNtc.c \
	../cpr/src/m_cr_CpriStartRecNotice.c \
	../cpr/src/m_cr_CpriStartReNotice.c \
	../cpr/src/t_cr_CpriMonitorEndProc.c \
	../cpr/src/t_cr_CpriMonitorStrProc.c \
	../cpr/src/t_cr_CpriSimStartProc.c \
	../cpr/src/t_cr_CpriSimStopProc.c \
	../cpr/src/t_cr_forceStopProc.c \
	../cpr/src/m_cr_CpriConnectNtc.c \
	../cpr/src/m_cr_CpriDisConnectNtc.c \
	../cpr/src/m_cr_CpriStateChgNotice.c \
	../cpr/src/m_cr_Cpri2ndRstReq.c \
	../cpr/src/m_cr_CpriStopReNotice.c \
	$(shell ls ../dlm/src/*.c) \
	$(shell ls $(TOP)/src/cmn/com/src/*.c)\
	$(shell ls ../log/src/*.c)\
	../com/src/l3_com_msgCom.c \
	$(shell ls ../rct/src/*.c)

OBJS=$(SRC:.c=.o)
#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(LDFLAGS) -o $(TARGET) $(OBJS) -L$(LIBDIR) -lrrhapi_com $(LIBS) $(CMN_LIBS)
%.o: %.c
	$(CC) $(OPT) -c  $< $(INCLUDES) -L$(LIBDIR) -lrrhapi_com -o $@
#	$(CC) -c $(cflag) $< -o $@ -L$(LIBDIR)
clean:
	rm -f ../main/src/*.o ../main/src/*.d
	rm -f ../com/src/*.o ../com/src/*.d
	rm -f ../cpr/src/*.o ../cpr/src/*.d
	rm -f ../dlm/src/*.o ../dlm/src/*.d
	rm -f ../rct/src/*.o ../rct/src/*.d
	rm -f ../log/src/*.o ../log/src/*.d

endif

ifeq ($(BUILD_QEMU_DEF),1)
#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: l3 rrh
#------------------------------------------------------
TARGET=rrh_l3

include ../../../Makefile.in

#------------------------------------------------------
# parameter
#------------------------------------------------------
ifeq ($(BUILD_MODE_DEF), 0)
#CC=cc
endif
ifeq ($(BUILD_MODE_DEF), 1)
CC=cc
endif

#CC=/opt/ubq/devkit/arm/armv7/bin/arm-armv7-linux-gnueabi-gcc
OPT+= $(CFLAGS)
#OPT+= $(CFLAGS) -DOPT_16B_IT

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
	-lrrhapi_l2 -lrrhapi_com -lrrhapi_cpri -lrrhapi_fmng -lrrhapi_svp -lrrhapi_log -lm

CMN_LIBDIR=$(TOP)/src/cmn/lib
CMN_LIBS = -L$(CMN_LIBDIR) -L$(LIBDIR_QEMU) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_com -lbpf_mblm -ldl

#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib
LIBDIR_QEMU=$(TOP)/ramdisk/target/usr/local/lib64
OBJDIR=../obj/


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
	-I$(TOP)/src/pf/ft_com/com/inc \
	-I../inc \
	-I../main/inc \
	-I../cpr/inc \
	-I../dlm/inc \
	-I../rct/inc \
	-I../log/inc

#------------------------------------------------------
# Source Files
#------------------------------------------------------
SRC=\
	$(TOP)/src/cmn/dcm/src/tm_cm_Breq.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ActiveChk.c \
	$(TOP)/src/cmn/dcm/src/m_cm_AxC.c \
	$(TOP)/src/cmn/dcm/src/m_cm_CardSarch.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ChkCardSta.c \
	$(TOP)/src/cmn/dcm/src/m_cm_chksig.c \
	$(TOP)/src/cmn/dcm/src/m_cm_Delay.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ErrCntRegSet.c \
	$(TOP)/src/cmn/dcm/src/m_cm_Hex.c \
	$(TOP)/src/cmn/dcm/src/m_cm_L3MsgSend.c \
	$(TOP)/src/cmn/dcm/src/m_cm_SysParaDSet.c \
	$(TOP)/src/cmn/dcm/src/m_cm_writeLEDReg.c \
	$(TOP)/src/cmn/dcm/src/m_cm_l3log.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Assert.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_IsSpace.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Itoa.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MAbort.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPadc.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPadi.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPads.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MWait.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_CheckSumCalc.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_CheckSumCalcCpri.c \
	../main/src/l3_main_main.c \
	../main/src/l3_main_ram.c \
	../main/src/l3_main_rom.c \
	../main/src/l3_main_systemInit.c \
	../main/src/l3_thrd_main.c \
	../com/src/l3_com_ram.c \
	../com/src/l3_com_rom.c \
	../com/src/l3_com_ushortsort_asc.c \
	../cpr/src/m_cr_Anlz.c \
	../cpr/src/m_cr_cm_RunHisSet.c \
	../cpr/src/m_cr_ComLinkSettingL3Rsp.c \
	../cpr/src/m_cr_CpriMsgRcvProc.c \
	../cpr/src/m_cr_CpriStaPriorityChk.c \
	../cpr/src/m_cr_CpriStateChgProc.c \
	../cpr/src/m_cr_CpriStateChgProcRE.c \
	../cpr/src/m_cr_DlinkSettingReq.c \
	../cpr/src/m_cr_DlinkStopReq.c \
	../cpr/src/m_cr_HcReq.c \
	../cpr/src/m_cr_HcTimeoutNtc.c \
	../cpr/src/m_cr_HcTimeoutNtcRSP.c \
	../cpr/src/m_cr_HcTimeoutNtcSND.c \
	../cpr/src/m_cr_HcRsp.c \
	../cpr/src/m_cr_Init.c \
	../cpr/src/m_cr_L2DeactReq.c \
	../cpr/src/m_cr_L2LinkStateProc.c \
	../cpr/src/m_cr_L2LinkStateProcRE.c \
	../cpr/src/m_cr_L3HcSta.c \
	../cpr/src/m_cr_L3StaNtc.c \
	../cpr/src/m_cr_main.c \
	../cpr/src/m_cr_Nop.c \
	../cpr/src/m_cr_ram.c \
	../cpr/src/m_cr_rom.c \
	../cpr/src/m_cr_CpriLinkPrepareRes.c \
	../cpr/src/m_cr_CpriLinkPrepareReq.c \
	../cpr/src/m_cr_CpriRestartNtc.c \
	../cpr/src/m_cr_CpriStartRecNotice.c \
	../cpr/src/m_cr_CpriStartReNotice.c \
	../cpr/src/t_cr_CpriMonitorEndProc.c \
	../cpr/src/t_cr_CpriMonitorStrProc.c \
	../cpr/src/t_cr_CpriSimStartProc.c \
	../cpr/src/t_cr_CpriSimStopProc.c \
	../cpr/src/t_cr_forceStopProc.c \
	../cpr/src/m_cr_CpriConnectNtc.c \
	../cpr/src/m_cr_CpriDisConnectNtc.c \
	../cpr/src/m_cr_CpriStateChgNotice.c \
	../cpr/src/m_cr_Cpri2ndRstReq.c \
	../cpr/src/m_cr_CpriStopReNotice.c \
	$(shell ls ../dlm/src/*.c) \
	$(shell ls $(TOP)/src/cmn/com/src/*.c)\
	$(shell ls ../log/src/*.c)\
	../com/src/l3_com_msgCom.c \
	$(shell ls ../rct/src/*.c)

OBJS=$(SRC:.c=.o)
#------------------------------------------------------
# rules
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(LDFLAGS) -o $(TARGET) $(OBJS) -L$(LIBDIR) -lrrhapi_com $(LIBS) $(CMN_LIBS)
%.o: %.c
	$(CC) $(OPT) -c  $< $(INCLUDES) -L$(LIBDIR) -lrrhapi_com -o $@
#	$(CC) -c $(cflag) $< -o $@ -L$(LIBDIR)
clean:
	rm -f ../main/src/*.o ../main/src/*.d
	rm -f ../com/src/*.o ../com/src/*.d
	rm -f ../cpr/src/*.o ../cpr/src/*.d
	rm -f ../dlm/src/*.o ../dlm/src/*.d
	rm -f ../rct/src/*.o ../rct/src/*.d
	rm -f ../log/src/*.o ../log/src/*.d

endif