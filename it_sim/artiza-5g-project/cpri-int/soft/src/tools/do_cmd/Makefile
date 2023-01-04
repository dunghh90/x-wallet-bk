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

# DEBUG DIR
CMD_DEBUG=.

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
$(CMD_DEBUG)/do_cmd.o 

CMD_LM		=	$(CMD_DEBUG)/cmd_do_cmd
TARGET		=	$(CMD_LM)

CMD_SRCS=$(CMD_OBJS:%.o=%.c)

$(CMD_LM):	$(CMD_OBJS) $(CMD_INCS)
	$(CC) $(SYSROOT) -o $(CMD_LM) $(CMD_OBJS) $(LIBS) 

clean:
	rm -f $(CMD_LM) $(CMD_OBJS) *.d

#


