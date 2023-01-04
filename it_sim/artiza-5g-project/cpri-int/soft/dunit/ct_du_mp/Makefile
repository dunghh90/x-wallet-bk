#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in

PREFIX=/usr
#DESTDIR=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux
DESTDIR=

SRCDIR	= .
OBJDIR	= ./
TARGET_PATH	= ../../src
MPSRV_PATH			= $(MPPATH)/server
MPSRV_PUB_INC		= $(MPSRV_PATH)/public
SAA_PATH			= $(MPSRV_PATH)/agent
SAA_COM_INCPATH		= $(SAA_PATH)/com
SAA_AGENT_INCPATH	= $(SAA_PATH)/saa
MPMW_INCPATH		= $(MPSRV_PATH)/mw/libmpmw_srv
MPSW_MWPATH			= $(MPSRV_PATH)/interface/function

LFILE		= libct_mp.so
LIBS		= -lxml2
INCD		= -I.$(MRUBY_PATH)/include -I../ -I$(MPSW_PUB) -I$(MPSW_SRVPATH_MSG) -I$(MPSW_SRVPATH_FUNC) \
			  -I$(MPSRV_PUB_INC) -I$(MPMW_INCPATH) -I$(SAA_COM_INCPATH) -I$(SAA_AGENT_INCPATH) \
			  -I.$(CUNIT_PATH) \
			  -I$(PREFIX)/include/libxml2/libxml \
			  -I$(PREFIX)/include/libxml2 \
			  -I$(DESTDIR)$(PREFIX)/include/yumapro/platform \
			  -I$(DESTDIR)$(PREFIX)/include/yumapro/ncx \
			  -I$(DESTDIR)$(PREFIX)/include/yumapro/agt \
			  -I$(DESTDIR)$(PREFIX)/include/yumapro/sil-sa \
			  -I$(DESTDIR)$(PREFIX)/include/yumapro/ycontrol \
			  -I$(DESTDIR)$(PREFIX)/include/yumapro/db-api

CFLAGS		+= -rdynamic -Wall 

DEST		= ../lib

#
SRCS		=  $(shell ls $(SAA_COM_INCPATH)/*.c) \
			   $(shell ls $(SAA_AGENT_INCPATH)/*.c) \
			   $(shell ls $(MPMW_INCPATH)/*.c) \
			   $(shell ls $(MPSW_MWPATH)/*.c) \
			   $(shell ls ./*.c)

OBJDS		= $(shell ls *.o)
OBJS		= $(SRCS:.c=.o)

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
