#-----------------------------------------------------------------
# DEBUG_COMMAND(FJ) make file
#-----------------------------------------------------------------
#
# usage: make
#
#
#-----------------------------------------------------------------

#
# Directories
#
DEV_HOME=../..
#

include ../../Makefile.in

# RRH
RRH_DIR=$(DEV_HOME)/sys
RRH_INC_DIR=$(RRH_DIR)/inc
BPF_INC_DIR=$(RRH_DIR)/inc_mw
ARCH_INC_DIR=$(RRH_DIR)/inc_arch
PF_INC_DIR=$(DEV_HOME)/pf/sys
API_INC_DIR=$(DEV_HOME)/api/inc
COM_INC_DIR=$(DEV_HOME)/cmn/com/inc
DCM_INC_DIR=$(DEV_HOME)/cmn/dcm/inc


# SHELLD-CMD DIR
CMD_ROOT=$(DEV_HOME)/src/tools/shell

# DEBUG DIR
CMD_DEBUG=.

# CMD INC DIR
CMD_INC_DIR=$(CMD_ROOT)/inc

# CMD SRC DIR
CMD_SRC_DIR=$(CMD_ROOT)/src

# LIBS
LIBS= -L../../../ramdisk/target/lib \
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

##
## Compiler Linker Option
##

INCDIR= \
-I$(RRH_INC_DIR) \
-I$(BPF_INC_DIR) \
-I$(ARCH_INC_DIR) \
-I$(CMD_INC_DIR) \
-I$(COM_INC_DIR) \
-I$(DCM_INC_DIR) \
-I$(API_INC_DIR) \
-I$(PF_INC_DIR)

##
##
##
CFLAGS += $(INCDIR) $(ADDINC) $(ADDDEF)

CMD_OBJS=\
$(CMD_DEBUG)/coredump.o

CMD_LM		=	$(CMD_DEBUG)/cmd_coredump
TARGET		=	$(CMD_LM)

CMD_SRCS=$(CMD_OBJS:%.o=%.c)

$(CMD_LM):	$(CMD_OBJS) $(CMD_INCS)
	$(CC) $(SYSROOT) -o $(CMD_LM) $(CMD_OBJS) $(LIBS)

clean:
	rm -f $(CMD_LM) $(CMD_OBJS) *.d

#


