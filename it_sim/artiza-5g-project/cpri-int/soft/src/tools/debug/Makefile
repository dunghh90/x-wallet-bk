ifeq ($(BUILD_QEMU_DEF),0)
#
# ZynqDCM
#
include ../../Makefile.in

OPT=-shared -fPIC
INC= -I../../pf/sys \
	-I../../cmn/dcm/inc \
	-I../../cmn/debug/inc \
	-I../../../src/sys/inc_mw/ \
	-I../../../src/sys/inc \
	-I../../sys/inc_arch \
	-I../../api/inc \
	-I./inc \
        -I../../mp/public \
        -I../../mp/server/interface/msg \
	-I../../pf/ft_com/com/src \
	-I../../pf/ft_com/com/inc \
	-I../../pf/ft_rfp/cca/inc \
	-I../../pf/ft_rfp/cca/src \
	-I../../cmn/com/inc \
	-I../../mw/oal/RU/hrtm/include  \
	-I../../mw/oal/HM/devc/include  \
	-I../../mw/oal/RU/ipcm/include \
	-I../../mw/inc \
	-I../../../src/mw/oal/COM/com/include

CFLAGS+= $(INC) -D_GNU_SOURCE
BPF_LIBS= -L$(TARGETLIB_DIR) \
		-lbpf_devc \
		-lbpf_facc \
		-lbpf_ipcm \
		-lbpf_itcm \
		-lbpf_log \
		-lbpf_plog \
		-lbpf_svrm \
		-lbpf_string \
		-lbpf_mblm \
		-lbpf_hrtm \
		-lbpf_com

LDFLAGS+=-lm

LOCAL_BIN_DIR	=	./bin
LOCAL_LIB_DIR	=	./lib
CMD_DBG_DIR		=	./cmd_dbg
TARGETBIN_DIR	=	../../../ramdisk/target/opt/debug
TARGETLIB_DIR	=	../../../ramdisk/target/lib

LMNAME	=	cmd_dbg
LM		=	$(LOCAL_BIN_DIR)/$(LMNAME)
OBJS	=	src/cmd_dbg/f_dbg_cmd_check_parameter.o \
			src/cmd_dbg/f_dbg_cmd_cat_parameter.o \
			src/cmd_dbg/f_dbg_cmd_display_result.o \
			src/cmd_dbg/f_dbg_cmd_main.o \
			src/cmd_dbg/f_dbg_cmd_print_usage.o \
			src/cmd_dbg/f_dbg_cmd_recv_exec_cmd_rep.o  \
			src/cmd_dbg/f_dbg_cmd_rom.o \
			src/cmd_dbg/f_dbg_cmd_send_exec_cmd_req.o \
			src/cmd_dbg/f_dbg_exec_local_cmd.o

DLNAME	=	libDBGcmd.so
DLVER	=	1.0
DLVERNAME=	$(DLNAME).$(DLVER)
DL		=	$(LOCAL_LIB_DIR)/$(DLNAME).$(DLVER)
DLOBJS	=	src/cmd_lm_write.o	\
			src/cmd_rrhdbg.o	\
			src/traw.o		\
			src/trar.o		\
			src/sm.o		\
			src/pm.o		\
			src/cmd_version.o	\
			src/cmd_eeprom_disp.o	\
			src/cmd_eeprom_access.o \
			src/cmd_reg_access.o	\
			src/cmd_spi_access.o	\
			src/cmd_mmi_cpld_access.o \
			src/cmd_alm_hist_log.o 	\
			src/cmd_shm_dmp.o	\
			src/cmd_shm_write.o \
			src/cmd_shmgetmon.o \
			src/cmd_trouble_log_get.o \
			src/cmd_flash_read.o \
			src/cmd_flash_slave_read.o \
			src/resetmask.o \
			src/f_cmd_sfp_info.o \
			src/cmd_sfp_log_disp.o \
			src/dbg_common_lib.o

all:	$(LM) $(DL)

$(LM):	$(OBJS) $(DL)
	$(CC) $(CFLAGS) -o $(LM)	\
		$(OBJS) $(LIBS) -lm -ldl -L$(LOCAL_LIB_DIR) -lDBGcmd $(BPF_LIBS)

$(DL):	$(DLOBJS)
	$(CC) $(OPT) $(SYSROOT) -o $(DL)  $(DLOBJS)
	cd $(LOCAL_LIB_DIR) ; ln -fs $(DLVERNAME) $(DLNAME)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -rdynamic -c $< $(INC) -o $@

clean:
		rm -vf $(LM) $(DL) $(OBJS) $(DLOBJS) ./src/*.d ./src/cmd_dbg/*.d

install:
		install $(LM) $(TARGETBIN_DIR)/$(LMNAME)
		cd $(TARGETBIN_DIR) ; \
			ln -fs $(LMNAME) cmd_eeprom_access ; \
			ln -fs $(LMNAME) cmd_eeprom_disp ; \
			ln -fs $(LMNAME) cmd_alm_hist ; \
			ln -fs $(LMNAME) cmd_rrhdbg ; \
			ln -fs $(LMNAME) cmd_reg_access ; \
			ln -fs $(LMNAME) cmd_spi_access ; \
			ln -fs $(LMNAME) cmd_mmi_cpld_access ; \
			ln -fs $(LMNAME) cmd_lm_write ; \
			ln -fs $(LMNAME) cmd_version ; \
			ln -fs $(LMNAME) cmd_shm_dmp ; \
			ln -fs $(LMNAME) cmd_shm_write ; \
			ln -fs $(LMNAME) cmd_trouble_log_get ; \
			ln -fs $(LMNAME) cmd_flash_read ; \
			ln -fs $(LMNAME) cmd_flash_slave_read ; \
			ln -fs $(LMNAME) trar ; \
			ln -fs $(LMNAME) traw ; \
			ln -fs $(LMNAME) sm ; \
			ln -fs $(LMNAME) pm ; \
			ln -fs $(LMNAME) cmd_pfdbg ; \
			ln -fs $(LMNAME) cmd_cprdbg ; \
			ln -fs $(LMNAME) cmd_mcprdbg ; \
			ln -fs $(LMNAME) help ; \
			ln -fs $(LMNAME) \? ; \
			ln -fs $(LMNAME) resetmask ;\
			ln -fs $(LMNAME) cmd_sfp_info;\
			ln -fs $(LMNAME) cmd_shm_getmon;\
			ln -fs $(LMNAME) cmd_sfp_log
		install $(DL) $(TARGETLIB_DIR)/$(DLVERNAME)
		cd $(TARGETLIB_DIR) ; \
			ln -fs $(DLVERNAME) $(DLNAME)

endif

ifeq ($(BUILD_QEMU_DEF),1)
#
# ZynqDCM
#
include ../../Makefile.in

OPT=-shared -fPIC
INC= -I../../pf/sys \
	-I../../cmn/dcm/inc \
	-I../../cmn/debug/inc \
	-I../../../src/sys/inc_mw/ \
	-I../../../src/sys/inc \
	-I../../sys/inc_arch \
	-I../../api/inc \
	-I./inc \
        -I../../mp/public \
        -I../../mp/server/interface/msg \
	-I../../pf/ft_com/com/src \
	-I../../pf/ft_com/com/inc \
	-I../../pf/ft_rfp/cca/inc \
	-I../../pf/ft_rfp/cca/src \
	-I../../cmn/com/inc \
	-I../../mw/oal/RU/hrtm/include  \
	-I../../mw/oal/HM/devc/include  \
	-I../../mw/oal/RU/ipcm/include \
	-I../../mw/inc \
	-I../../../src/mw/oal/COM/com/include

CFLAGS+= $(INC) -D_GNU_SOURCE
BPF_LIBS= -L$(TARGETLIB_DIR) -L$(TARGETLIB_DIR_QEMU)\
		-lbpf_devc_qemu \
		-lbpf_devc \
		-lbpf_facc \
		-lbpf_ipcm \
		-lbpf_itcm \
		-lbpf_log \
		-lbpf_plog \
		-lbpf_svrm \
		-lbpf_string \
		-lbpf_mblm \
		-lbpf_hrtm \
		-lbpf_com

LDFLAGS+=-lm

LOCAL_BIN_DIR	=	./bin
LOCAL_LIB_DIR	=	./lib
CMD_DBG_DIR		=	./cmd_dbg
TARGETBIN_DIR	=	../../../ramdisk/target/opt/debug
TARGETLIB_DIR	=	../../../ramdisk/target/lib
TARGETLIB_DIR_QEMU	=	../../../ramdisk/target/usr/local/lib64
LMNAME	=	cmd_dbg
LM		=	$(LOCAL_BIN_DIR)/$(LMNAME)
OBJS	=	src/cmd_dbg/f_dbg_cmd_check_parameter.o \
			src/cmd_dbg/f_dbg_cmd_cat_parameter.o \
			src/cmd_dbg/f_dbg_cmd_display_result.o \
			src/cmd_dbg/f_dbg_cmd_main.o \
			src/cmd_dbg/f_dbg_cmd_print_usage.o \
			src/cmd_dbg/f_dbg_cmd_recv_exec_cmd_rep.o  \
			src/cmd_dbg/f_dbg_cmd_rom.o \
			src/cmd_dbg/f_dbg_cmd_send_exec_cmd_req.o \
			src/cmd_dbg/f_dbg_exec_local_cmd.o

DLNAME	=	libDBGcmd.so
DLVER	=	1.0
DLVERNAME=	$(DLNAME).$(DLVER)
DL		=	$(LOCAL_LIB_DIR)/$(DLNAME).$(DLVER)
DLOBJS	=	src/cmd_lm_write.o	\
			src/cmd_rrhdbg.o	\
			src/traw.o		\
			src/trar.o		\
			src/sm.o		\
			src/pm.o		\
			src/cmd_version.o	\
			src/cmd_eeprom_disp.o	\
			src/cmd_eeprom_access.o \
			src/cmd_reg_access.o	\
			src/cmd_spi_access.o	\
			src/cmd_mmi_cpld_access.o \
			src/cmd_alm_hist_log.o 	\
			src/cmd_shm_dmp.o	\
			src/cmd_shm_write.o \
			src/cmd_shmgetmon.o \
			src/cmd_trouble_log_get.o \
			src/cmd_flash_read.o \
			src/cmd_flash_slave_read.o \
			src/resetmask.o \
			src/f_cmd_sfp_info.o \
			src/cmd_sfp_log_disp.o \
			src/dbg_common_lib.o

all:	$(LM) $(DL)

$(LM):	$(OBJS) $(DL)
	$(CC) $(CFLAGS) -o $(LM)	\
		$(OBJS) $(LIBS) -lm -ldl -L$(LOCAL_LIB_DIR) -lDBGcmd $(BPF_LIBS)

$(DL):	$(DLOBJS)
	$(CC) $(OPT) $(SYSROOT) -o $(DL)  $(DLOBJS)
	cd $(LOCAL_LIB_DIR) ; ln -fs $(DLVERNAME) $(DLNAME)
%.o: %.c
	$(CC) $(OPT) $(CFLAGS) -rdynamic -c $< $(INC) -o $@

clean:
		rm -vf $(LM) $(DL) $(OBJS) $(DLOBJS) ./src/*.d ./src/cmd_dbg/*.d

install:
		install $(LM) $(TARGETBIN_DIR)/$(LMNAME)
		cd $(TARGETBIN_DIR) ; \
			ln -fs $(LMNAME) cmd_eeprom_access ; \
			ln -fs $(LMNAME) cmd_eeprom_disp ; \
			ln -fs $(LMNAME) cmd_alm_hist ; \
			ln -fs $(LMNAME) cmd_rrhdbg ; \
			ln -fs $(LMNAME) cmd_reg_access ; \
			ln -fs $(LMNAME) cmd_spi_access ; \
			ln -fs $(LMNAME) cmd_mmi_cpld_access ; \
			ln -fs $(LMNAME) cmd_lm_write ; \
			ln -fs $(LMNAME) cmd_version ; \
			ln -fs $(LMNAME) cmd_shm_dmp ; \
			ln -fs $(LMNAME) cmd_shm_write ; \
			ln -fs $(LMNAME) cmd_trouble_log_get ; \
			ln -fs $(LMNAME) cmd_flash_read ; \
			ln -fs $(LMNAME) cmd_flash_slave_read ; \
			ln -fs $(LMNAME) trar ; \
			ln -fs $(LMNAME) traw ; \
			ln -fs $(LMNAME) sm ; \
			ln -fs $(LMNAME) pm ; \
			ln -fs $(LMNAME) cmd_pfdbg ; \
			ln -fs $(LMNAME) cmd_cprdbg ; \
			ln -fs $(LMNAME) cmd_mcprdbg ; \
			ln -fs $(LMNAME) help ; \
			ln -fs $(LMNAME) \? ; \
			ln -fs $(LMNAME) resetmask ;\
			ln -fs $(LMNAME) cmd_sfp_info;\
			ln -fs $(LMNAME) cmd_shm_getmon;\
			ln -fs $(LMNAME) cmd_sfp_log
		install $(DL) $(TARGETLIB_DIR)/$(DLVERNAME)
		cd $(TARGETLIB_DIR) ; \
			ln -fs $(DLVERNAME) $(DLNAME)

endif