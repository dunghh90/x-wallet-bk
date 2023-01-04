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

# SHELLD-CMD DIR
CMD_ROOT=$(DEV_HOME)/src/tools/debug

# DEBUG DIR
CMD_DEBUG=.

# CMD INC DIR
CMD_INC_DIR=$(CMD_ROOT)/inc

# CMD SRC DIR
CMD_SRC_DIR=$(CMD_ROOT)/src

# LIBS
LIBS=

##
## Compiler Linker Option
##

INCDIR= 

##
##
##
CFLAGS += $(INCDIR) $(ADDINC) $(ADDDEF)

CMD_OBJS=\
$(CMD_DEBUG)/devctl.o 

CMD_LM		=	$(CMD_DEBUG)/cmd_devctl
TARGET		=	$(CMD_LM)

CMD_SRCS=$(CMD_OBJS:%.o=%.c)

$(CMD_LM):	$(CMD_OBJS) $(CMD_INCS)
	$(CC) $(SYSROOT) -o $(CMD_LM) $(CMD_OBJS) $(LIBS) 

clean:
	rm -f $(CMD_LM) $(CMD_OBJS) *.d

#




