#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in

SRCDIR		= .
OBJDIR		= ./
TARGET_PATH	= ../../src
LFILE		= libct_cmn.so
LIBS		= 
INCD		= -I$(SYSINC) -I$(BPFINC) -I$(ARCHINC) -I$(CMNPATH)/com/inc -I$(CMNPATH)/dcm/inc -I$(CMNPATH)/cmu/inc -I$(CMNPATH)/debug/inc -I$(PFSYSINC) \
			  -I$(MPSW_PUB) -I$(MPSW_SRVPATH_MSG) -I$(MPSW_SRVPATH_FUNC)
CFLAGS		+= -rdynamic
DEST		= ../lib

SRCS	= $(shell ls $(CMNPATH)/com/src/*.c) \
		  $(shell ls $(CMNPATH)/com/lib/*.c) \
		  $(shell ls $(CMNPATH)/debug/src/*.c) \
		  $(shell ls $(CMNPATH)/cmu/src/*.c)
OBJDS	= $(shell ls *.o)
OBJS	= $(SRCS:.c=.o)

#
all:	$(OBJS) $(LFILE)
#
.c.o:
	$(CC) $(CFLAGS) -c  $(INCD) $<
#
$(LFILE): $(OBJS) $(SRCS)
	$(CC) -shared -fPIC $(OBJDS) $(LIBS) -o $(LFILE)
	@rm -f $(OBJS) $(OBJDS)
	@echo "$(LFILE) make success"

install:; @echo Installing $(LFILE) in $(DEST)
		  @$(CP) $(LFILE) $(DEST)

clean:;	 @rm -f $(OBJS) $(LFILE) $(DEST)/$(LFILE) *.d
#
