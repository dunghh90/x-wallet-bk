ifeq ($(BUILD_QEMU_DEF),0)
#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: l2 server
#------------------------------------------------------
TARGET=rrh_l2

include ../../../Makefile.in

#------------------------------------------------------
# parameter
#------------------------------------------------------
#CC=cc
#CC=/opt/ubq/devkit/arm/armv7/bin/arm-armv7-linux-gnueabi-gcc
OPT+= $(CFLAGS)

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
	-lrrhapi_com -lrrhapi_svp -lrrhapi_l2 -lcmn_com
	#-lDBG_l2_lpb

CMN_LIBDIR=$(TOP)/src/cmn/lib
ifeq ($(BUILD_MODE_DEF), 0)
ifdef CROSS_COMPILE
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg
else
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com
endif
endif
ifeq ($(BUILD_MODE_DEF), 1)
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com -lbpf_mblm -ldl
endif
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
	-I../main/inc \
	-I../lpb/inc \
	-I../txs/inc \
	-I../rxs/inc \
	-I../pol/inc \
	-I../dbg/inc

#------------------------------------------------------
# Source Files
#------------------------------------------------------
SRC=\
	$(TOP)/src/cmn/dcm/src/tm_cm_Breq.c \
	$(TOP)/src/cmn/dcm/src/m_cm_Delay.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Assert.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MWait.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPadc.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MAbort.c \
	$(TOP)/src/cmn/com/src/f_com_aplCom.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_log.c \
	$(TOP)/src/cmn/dcm/src/m_cm_l3log.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_rom.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_abort.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_assert.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_cpri_state.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ErrCntRegSet.c \
	../main/src/l2_main_abort.c \
	../main/src/l2_main_main.c \
	../main/src/l2_main_ram.c \
	../main/src/l2_main_rom.c \
	../main/src/l2_main_systemInit.c \
	../main/src/l2_thrd_main.c \
	../com/src/l2_com_endian_util.c \
	../com/src/l2_com_function.c \
	../com/src/l2_com_ram.c \
	../com/src/l2_com_rom.c \
	../lpb/src/m_lb_c_if.c \
	../lpb/src/m_lb_c_p.c \
	../lpb/src/m_lb_disc10kbf.c \
	../lpb/src/m_lb_dstrbtl3msg.c \
	../lpb/src/m_lb_log.c \
	../lpb/src/m_lb_ram.c \
	../lpb/src/m_lb_rom.c \
	../lpb/src/m_lb_RunHisMngInfGet.c \
	../lpb/src/m_lb_RunHisTmrInfGet.c \
	../lpb/src/m_lb_subrunhis.c \
	../lpb/src/tm_lb_1_nop.c \
	../lpb/src/tm_lb_1_p.c \
	../lpb/src/tm_lb_1_psub.c \
	../lpb/src/tm_lb_1_RelNG.c \
	../lpb/src/tm_lb_1_SetupNG.c \
	../lpb/src/tm_lb_abort.c \
	../lpb/src/tm_lb_big_to_little.c \
	../lpb/src/tm_lb_checknr.c \
	../lpb/src/tm_lb_l2state_sub_ntc.c \
	../lpb/src/tm_lb_lappr.c \
	../lpb/src/tm_lb_little_to_big.c \
	../lpb/src/tm_lb_main.c \
	../lpb/src/tm_lb_nop.c \
	../lpb/src/tm_lb_rell2link_rsp.c \
	../lpb/src/tm_lb_RunHisInterEvGet.c \
	../lpb/src/tm_lb_RunHisSet.c \
	../lpb/src/tm_lb_set_L2Error.c \
	../lpb/src/tm_lb_set_stateF.c \
	../lpb/src/tm_lb_setupl2link_rsp.c \
	../lpb/src/tm_lb_stpalltmr.c \
	../lpb/src/tm_lb_trans.c \
	../txs/src/tm_tx_abort.c \
	../txs/src/tm_tx_anlz.c \
	../txs/src/tm_tx_init.c \
	../txs/src/tm_tx_locbnbuf.c \
	../txs/src/tm_tx_main.c \
	../txs/src/tm_tx_nop.c \
	../txs/src/tm_tx_ram.c \
	../txs/src/tm_tx_rom.c \
	../txs/src/tm_tx_selectbank.c \
	../txs/src/tm_tx_send.c \
	../rxs/src/tm_rx_abort.c \
	../rxs/src/tm_rx_anlz.c \
	../rxs/src/tm_rx_cm_fnc.c \
	../rxs/src/tm_rx_init.c \
	../rxs/src/tm_rx_locbnbuf.c \
	../rxs/src/tm_rx_main.c \
	../rxs/src/tm_rx_nop.c \
	../rxs/src/tm_rx_ram.c \
	../rxs/src/tm_rx_readlinks.c \
	../rxs/src/tm_rx_recv.c \
	../rxs/src/tm_rx_rom.c \
	../rxs/src/tm_rx_rxlinkmng.c \
	../rxs/src/tm_rx_simrecv.c \
	../pol/src/tm_pl_abort.c \
	../pol/src/tm_pl_anlz.c \
	../pol/src/tm_pl_init.c \
	../pol/src/tm_pl_loop.c \
	../pol/src/tm_pl_main.c \
	../pol/src/tm_pl_nop.c \
	../pol/src/tm_pl_ram.c \
	../pol/src/tm_pl_rom.c \
	../pol/src/tm_pl_rxpo.c \
	../pol/src/tm_pl_txpo.c \
	../dbg/src/l2_dbg_abort.c \
	../dbg/src/l2_dbg_Anlz.c \
	../dbg/src/l2_dbg_cpriMsgSub.c \
	../dbg/src/l2_dbg_cpriMsgUnsub.c \
	../dbg/src/l2_dbg_Init.c \
	../dbg/src/l2_dbg_l2StateSub.c \
	../dbg/src/l2_dbg_l2StateUnsub.c \
	../dbg/src/l2_dbg_main.c \
	../dbg/src/l2_dbg_monStart.c \
	../dbg/src/l2_dbg_monStop.c \
	../dbg/src/l2_dbg_ram.c \
	../dbg/src/l2_dbg_rom.c \
	../dbg/src/l2_dbg_simuStart.c \
	../dbg/src/l2_dbg_simuStop.c \
	../dbg/src/l2_dbg_statisticsInd.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_shm.c

OBJS=$(SRC:.c=.o)
#------------------------------------------------------
# rules //C9 add -DOPT_RRH_ARCH_ZYNQ
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(LDFLAGS) -DOPT_RRH_ARCH_ZYNQ -o $(TARGET) $(OBJS) -L$(LIBDIR) $(LIBS) $(CMN_LIBS)
%.o: %.c
	$(CC) $(OPT) -c -DOPT_RRH_ARCH_ZYNQ $< $(INCLUDES) -o $@
#	$(CC) -c $(cflag) $< -o $@ -L$(LIBDIR)
clean:
	rm -rf ../rxs/src/*.o ../rxs/src/*.d
	rm -rf ../txs/src/*.o ../txs/src/*.d
	rm -rf ../pol/src/*.o ../pol/src/*.d
	rm -rf ../lpb/src/*.o ../lpb/src/*.d
	rm -rf ../main/src/*.o ../main/src/*.d
	rm -rf ../com/src/*.o ../com/src/*.d
	rm -rf ../dbg/src/*.o ../dbg/src/*.d ../dbg/*.d ../dbg/*.o

endif

ifeq ($(BUILD_QEMU_DEF),1)
#------------------------------------------------------
# Makefile for common library
#------------------------------------------------------
# Target: l2 server
#------------------------------------------------------
TARGET=rrh_l2

include ../../../Makefile.in

#------------------------------------------------------
# parameter
#------------------------------------------------------
#CC=cc
#CC=/opt/ubq/devkit/arm/armv7/bin/arm-armv7-linux-gnueabi-gcc
OPT+= $(CFLAGS)

#------------------------------------------------------
# build top
#------------------------------------------------------
TOP=../../../..

#------------------------------------------------------
# Library
#------------------------------------------------------
# LIBS=-lm -ldl -lrt -lpthread \
# 	-lbpf_com -lbpf_devc_qemu -lbpf_devc -lbpf_facc \
# 	-lbpf_mblm -lbpf_string -lbpf_hrtm -lbpf_svrm -lbpf_itcm -lbpf_ipcm -lbpf_log -lbpf_plog \
# 	-lrrhapi_com -lrrhapi_svp -lrrhapi_l2 -lcmn_com -ldl
# 	#-lDBG_l2_lpb
#C9 test
LIBS=-lm -ldl -lrt -lpthread \
	-lbpf_com -lbpf_devc_qemu -lbpf_devc -lDBGcmd -lbpf_facc \
	-lbpf_mblm -lbpf_string -lbpf_hrtm -lbpf_svrm -lbpf_itcm -lbpf_ipcm -lbpf_log -lbpf_plog \
	-lrrhapi_com -lrrhapi_svp -lrrhapi_l2 -lcmn_com -ldl
	#-lDBG_l2_lpb
CMN_LIBDIR=$(TOP)/src/cmn/lib
ifeq ($(BUILD_MODE_DEF), 0)
ifdef CROSS_COMPILE
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg
else
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send -lcmn_dbg -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com
endif
endif
ifeq ($(BUILD_MODE_DEF), 1)
CMN_LIBS = -L$(CMN_LIBDIR) -lcmu_recv -lcmu_send  -lcmn_com -lcmn_dbg -lDBGcmd -lrrhapi_cpri -lrrhapi_l2 -lrrhapi_com -lbpf_mblm -ldl
endif
#------------------------------------------------------
# Default Parameters
#------------------------------------------------------
LIBDIR=$(TOP)/ramdisk/target/lib
LIBDIR_QEMU=$(TOP)/ramdisk/target/usr/local/lib64
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
	-I../main/inc \
	-I../lpb/inc \
	-I../txs/inc \
	-I../rxs/inc \
	-I../pol/inc \
	-I../dbg/inc

#------------------------------------------------------
# Source Files
#------------------------------------------------------
SRC=\
	$(TOP)/src/cmn/dcm/src/tm_cm_Breq.c \
	$(TOP)/src/cmn/dcm/src/m_cm_Delay.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_Assert.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MWait.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MemPadc.c \
	$(TOP)/src/cmn/dcm/src/tm_cm_MAbort.c \
	$(TOP)/src/cmn/com/src/f_com_aplCom.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_log.c \
	$(TOP)/src/cmn/dcm/src/m_cm_l3log.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_rom.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_abort.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_assert.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_cpri_state.c \
	$(TOP)/src/cmn/dcm/src/m_cm_ErrCntRegSet.c \
	../main/src/l2_main_abort.c \
	../main/src/l2_main_main.c \
	../main/src/l2_main_ram.c \
	../main/src/l2_main_rom.c \
	../main/src/l2_main_systemInit.c \
	../main/src/l2_thrd_main.c \
	../com/src/l2_com_endian_util.c \
	../com/src/l2_com_function.c \
	../com/src/l2_com_ram.c \
	../com/src/l2_com_rom.c \
	../lpb/src/m_lb_c_if.c \
	../lpb/src/m_lb_c_p.c \
	../lpb/src/m_lb_disc10kbf.c \
	../lpb/src/m_lb_dstrbtl3msg.c \
	../lpb/src/m_lb_log.c \
	../lpb/src/m_lb_ram.c \
	../lpb/src/m_lb_rom.c \
	../lpb/src/m_lb_RunHisMngInfGet.c \
	../lpb/src/m_lb_RunHisTmrInfGet.c \
	../lpb/src/m_lb_subrunhis.c \
	../lpb/src/tm_lb_1_nop.c \
	../lpb/src/tm_lb_1_p.c \
	../lpb/src/tm_lb_1_psub.c \
	../lpb/src/tm_lb_1_RelNG.c \
	../lpb/src/tm_lb_1_SetupNG.c \
	../lpb/src/tm_lb_abort.c \
	../lpb/src/tm_lb_big_to_little.c \
	../lpb/src/tm_lb_checknr.c \
	../lpb/src/tm_lb_l2state_sub_ntc.c \
	../lpb/src/tm_lb_lappr.c \
	../lpb/src/tm_lb_little_to_big.c \
	../lpb/src/tm_lb_main.c \
	../lpb/src/tm_lb_nop.c \
	../lpb/src/tm_lb_rell2link_rsp.c \
	../lpb/src/tm_lb_RunHisInterEvGet.c \
	../lpb/src/tm_lb_RunHisSet.c \
	../lpb/src/tm_lb_set_L2Error.c \
	../lpb/src/tm_lb_set_stateF.c \
	../lpb/src/tm_lb_setupl2link_rsp.c \
	../lpb/src/tm_lb_stpalltmr.c \
	../lpb/src/tm_lb_trans.c \
	../txs/src/tm_tx_abort.c \
	../txs/src/tm_tx_anlz.c \
	../txs/src/tm_tx_init.c \
	../txs/src/tm_tx_locbnbuf.c \
	../txs/src/tm_tx_main.c \
	../txs/src/tm_tx_nop.c \
	../txs/src/tm_tx_ram.c \
	../txs/src/tm_tx_rom.c \
	../txs/src/tm_tx_selectbank.c \
	../txs/src/tm_tx_send.c \
	../rxs/src/tm_rx_abort.c \
	../rxs/src/tm_rx_anlz.c \
	../rxs/src/tm_rx_cm_fnc.c \
	../rxs/src/tm_rx_init.c \
	../rxs/src/tm_rx_locbnbuf.c \
	../rxs/src/tm_rx_main.c \
	../rxs/src/tm_rx_nop.c \
	../rxs/src/tm_rx_ram.c \
	../rxs/src/tm_rx_readlinks.c \
	../rxs/src/tm_rx_recv.c \
	../rxs/src/tm_rx_rom.c \
	../rxs/src/tm_rx_rxlinkmng.c \
	../rxs/src/tm_rx_simrecv.c \
	../pol/src/tm_pl_abort.c \
	../pol/src/tm_pl_anlz.c \
	../pol/src/tm_pl_init.c \
	../pol/src/tm_pl_loop.c \
	../pol/src/tm_pl_main.c \
	../pol/src/tm_pl_nop.c \
	../pol/src/tm_pl_ram.c \
	../pol/src/tm_pl_rom.c \
	../pol/src/tm_pl_rxpo.c \
	../pol/src/tm_pl_txpo.c \
	../dbg/src/l2_dbg_abort.c \
	../dbg/src/l2_dbg_Anlz.c \
	../dbg/src/l2_dbg_cpriMsgSub.c \
	../dbg/src/l2_dbg_cpriMsgUnsub.c \
	../dbg/src/l2_dbg_Init.c \
	../dbg/src/l2_dbg_l2StateSub.c \
	../dbg/src/l2_dbg_l2StateUnsub.c \
	../dbg/src/l2_dbg_main.c \
	../dbg/src/l2_dbg_monStart.c \
	../dbg/src/l2_dbg_monStop.c \
	../dbg/src/l2_dbg_ram.c \
	../dbg/src/l2_dbg_rom.c \
	../dbg/src/l2_dbg_simuStart.c \
	../dbg/src/l2_dbg_simuStop.c \
	../dbg/src/l2_dbg_statisticsInd.c \
	$(TOP)/src/cmn/com/src/f_cmn_com_shm.c

OBJS=$(SRC:.c=.o)
#------------------------------------------------------
# rules //C9 add -DOPT_RRH_ARCH_ZYNQ
#------------------------------------------------------
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(OPT) $(LDFLAGS) -DOPT_RRH_ARCH_ZYNQ -o $(TARGET) $(OBJS) -L$(LIBDIR) -L$(LIBDIR_QEMU) $(LIBS) $(CMN_LIBS)
%.o: %.c
	$(CC) $(OPT) -c -DOPT_RRH_ARCH_ZYNQ $< $(INCLUDES) -o $@
#	$(CC) -c $(cflag) $< -o $@ -L$(LIBDIR)
clean:
	rm -rf ../rxs/src/*.o ../rxs/src/*.d
	rm -rf ../txs/src/*.o ../txs/src/*.d
	rm -rf ../pol/src/*.o ../pol/src/*.d
	rm -rf ../lpb/src/*.o ../lpb/src/*.d
	rm -rf ../main/src/*.o ../main/src/*.d
	rm -rf ../com/src/*.o ../com/src/*.d
	rm -rf ../dbg/src/*.o ../dbg/src/*.d ../dbg/*.d ../dbg/*.o

endif