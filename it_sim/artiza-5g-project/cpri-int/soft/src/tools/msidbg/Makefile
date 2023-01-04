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

MP_DIR=$(DEV_HOME)/mp
MP_PUBLIC_INC_DIR=$(MP_DIR)/public
MP_SRV_INC_DIR=$(MP_DIR)/server/public
MP_SRV_MSG_INC_DIR=$(MP_DIR)/server/interface/msg
MP_SRV_FUNC_INC_DIR=$(MP_DIR)/server/interface/function

# DEBUG DIR
CMD_DEBUG=.


# LIBS
LIBS= -L../../../ramdisk/target/lib \
		-lmpsw_srv

##
## Compiler Linker Option
##

INCDIR= \
-I$(MP_PUBLIC_INC_DIR) \
-I$(MP_SRV_INC_DIR) \
-I$(MP_SRV_MSG_INC_DIR) \
-I$(MP_SRV_FUNC_INC_DIR)

##
##
##
CFLAGS += $(INCDIR) $(ADDINC) $(ADDDEF)

CMD_OBJS=\
$(CMD_DEBUG)/msidbg.o

CMD_LM		=	$(CMD_DEBUG)/cmd_msidbg
TARGET		=	$(CMD_LM)

CMD_SRCS=$(CMD_OBJS:%.o=%.c)

$(CMD_LM):	$(CMD_OBJS) $(CMD_INCS)
	$(CC) $(SYSROOT) -o $(CMD_LM) $(CMD_OBJS) $(LIBS) 

clean:
	rm -f $(CMD_LM) $(CMD_OBJS) *.d

#


