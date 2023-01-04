#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in

SRCDIR	= .
OBJDIR	= ./
TARGET_PATH	= ../../src

COMPATH		= $(PFCOMPATH)/com
LFILE		= libct_init.so
LIBS		= -L../lib -lct_cmn -L../lib -lct_pf_com
INCD		= -I.$(MRUBY_PATH)/include -I../ -I$(SYSINC) -I$(BPFINC) -I$(ARCHINC) \
			  -I$(CMNPATH)/cmu/inc -I$(CMNPATH)/com/inc -I$(CMNPATH)/dcm/inc -I$(CMNPATH)/debug/inc \
			  -I$(PFSYSINC) -I$(INITPATH)/inc -I$(INITMWPATH)/inc -I$(COMPATH)/inc -I.$(CUNIT_PATH) \
			  -I$(MPSW_PUB) -I$(MPSW_SRVPATH_MSG) -I$(MPSW_SRVPATH_FUNC) -I$(APIINC)
CFLAGS		+= -rdynamic -Wall
DEST		= ../lib

#
SRCS	= $(shell ls $(INITPATH)/src/*.c) \
		  $(shell ls ./*.c)
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
	@rm -f $(OBJS) $(OBJDS) $(DEST)/$(LFILE) *.d
	@echo "$(LFILE) make success"

install:; @echo Installing $(LFILE) in $(DEST)
		  @$(CP) $(LFILE) $(DEST)

clean:;	 @rm -f *.o $(LFILE) $(DEST)/$(LFILE) *.d

#
