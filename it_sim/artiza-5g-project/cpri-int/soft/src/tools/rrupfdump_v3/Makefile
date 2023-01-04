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
BPF_INC_DIR_COM=$(DEV_HOME)/mw/oal/COM/plog/include
BPF_INC_DIR_MW=$(DEV_HOME)/mw/inc

MP_DIR=$(DEV_HOME)/mp
MP_PUBLIC_INC_DIR=$(MP_DIR)/public
MP_SRV_INC_DIR=$(MP_DIR)/server/public
MP_SRV_MSG_INC_DIR=$(MP_DIR)/server/interface/msg
MP_SRV_FUNC_INC_DIR=$(MP_DIR)/server/interface/function


# SHELLD-CMD DIR
CMD_ROOT=$(DEV_HOME)/src/tools/shell

# DEBUG DIR
CMD_DEBUG=.

# CMD INC DIR
CMD_INC_DIR=$(CMD_ROOT)/inc

# CMD SRC DIR
CMD_SRC_DIR=$(CMD_ROOT)/src

# LIBS
LIBS=-L./lib -lpcap
ifeq ($(BUILD_MODE_DEF), 0)
##
## Compiler Linker Option
##

INCDIR= \
-I/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux/usr/include \
-I$(RRH_INC_DIR) \
-I$(BPF_INC_DIR) \
-I$(BPF_INC_DIR_COM) \
-I$(ARCH_INC_DIR) \
-I$(CMD_INC_DIR) \
-I$(COM_INC_DIR) \
-I$(DCM_INC_DIR) \
-I$(API_INC_DIR) \
-I$(BPF_INC_DIR_MW) \
-I$(PF_INC_DIR) \
-I$(MP_PUBLIC_INC_DIR) \
-I$(MP_SRV_INC_DIR) \
-I$(MP_SRV_MSG_INC_DIR) \
-I$(MP_SRV_FUNC_INC_DIR)

endif
ifeq ($(BUILD_MODE_DEF), 1)
INCDIR= \
-I/usr/include \
-I$(RRH_INC_DIR) \
-I$(BPF_INC_DIR) \
-I$(BPF_INC_DIR_COM) \
-I$(ARCH_INC_DIR) \
-I$(CMD_INC_DIR) \
-I$(COM_INC_DIR) \
-I$(DCM_INC_DIR) \
-I$(API_INC_DIR) \
-I$(BPF_INC_DIR_MW) \
-I$(PF_INC_DIR) \
-I$(MP_PUBLIC_INC_DIR) \
-I$(MP_SRV_INC_DIR) \
-I$(MP_SRV_MSG_INC_DIR) \
-I$(MP_SRV_FUNC_INC_DIR)

endif
##
##
##
CFLAGS += $(INCDIR) $(ADDINC) $(ADDDEF)

CMD_OBJS=\
$(CMD_DEBUG)/rrupfdump.o \
$(CMD_DEBUG)/rrupfdump_com.o \
$(CMD_DEBUG)/rrupfdump_if.o

CMD_LM		=	$(CMD_DEBUG)/cmd_rrupfdump
TARGET		=	$(CMD_LM)

CMD_SRCS=$(CMD_OBJS:%.o=%.c)

$(CMD_LM):	$(CMD_OBJS) $(CMD_INCS)
	$(CC) $(SYSROOT) -o $(CMD_LM) $(CMD_OBJS) $(LIBS)

clean:
	rm -f $(CMD_LM) $(CMD_OBJS) *.d

#




