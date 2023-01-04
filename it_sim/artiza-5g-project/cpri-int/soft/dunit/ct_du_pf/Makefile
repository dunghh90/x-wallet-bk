#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in


SRCDIR	= .
OBJDIR	= ./
TARGET_PATH	= ../../src
COMPATH		= $(PFCOMPATH)/com
MAINPATH	= $(PFPATH)/ft_main/main
HNDLPATH	= $(PFPATH)/ft_hdl/hdl
DHCPPATH	= $(PFPATH)/ft_dhcp/dhcp
PTPPATH		= $(PFPATH)/ft_ptp/ptp
ZPTPPATH	= $(PFPATH)/ft_ptp/zlptp
EQSPATH		= $(PFPATH)/ft_eqs/eqs
CCAPATH		= $(PFPATH)/ft_rfp/cca
RFAPATH		= $(PFPATH)/ft_rfp/rfa
SFPPATH		= $(PFPATH)/ft_sfp/sfp
WDTPATH		= $(PFPATH)/ft_wdt/wdt
NMAPATH		= $(PFPATH)/ft_nma/nma
LOGCPATH	= $(PFPATH)/ft_lgc/lgc
RFAAPIPATH	= $(PFPATH)/ft_rfp/api
FHSPATH		= $(PFPATH)/ft_fhs/fhs
EOMPATH		= $(PFPATH)/ft_eom/eom
PFMPATH		= $(PFPATH)/ft_pfm/pfm
FDLPATH		= $(PFPATH)/ft_flm/fdl
FULPATH		= $(PFPATH)/ft_flm/ful
NMASPATH	= $(PFPATH)/ft_nma/nmas
EIOPATH		= $(PFPATH)/ft_eio/eio
MTIPATH		= $(PFPATH)/ft_mti/mti
RFICPATH_DEV	= $(MWPATH)/rfic_tal/devices/talise
RFICPATH_AP	= $(MWPATH)/rfic_tal/app/example


LFILE		= libct_pf.so
LIBS		= -L../lib -lct_cmn -L../lib -lct_pf_com
INCD		= -I.$(MRUBY_PATH)/include -I../ -I$(XMLINC) -I$(SYSINC) -I$(BPFINC) -I$(BPFXMLINC) -I$(ARCHINC) -I$(APIINC) \
			  -I$(CMNPATH)/cmu/inc -I$(CMNPATH)/com/inc -I$(CMNPATH)/dcm/inc -I$(CMNPATH)/debug/inc \
			  -I$(PFSYSINC) -I$(COMPATH)/inc -I$(HNDLPATH)/inc -I$(MAINPATH)/inc -I$(PTPPATH)/inc \
			  -I$(DHCPPATH)/inc -I$(EQSPATH)/inc -I$(CCAPATH)/inc -I$(RFAPATH)/inc -I$(SFPPATH)/inc \
			  -I$(WDTPATH)/inc  -I$(NMAPATH)/inc -I$(LOGCPATH)/inc -I$(FHSPATH)/inc \
			  -I$(EOMPATH)/inc  -I$(PFMPATH)/inc -I$(FDLPATH)/inc -I$(FULPATH)/inc -I$(NMASPATH)/inc -I$(EIOPATH)/inc -I$(MTIPATH)/inc \
			  -I$(ZPTPPATH)/zlUserExamples/include -I$(ZPTPPATH)/zlUserPorting/include -I$(ZPTPPATH)/zlApi/include -I$(ZPTPPATH)/zlApr/include \
			  -I$(ZPTPPATH)/zlPtp/include -I$(MPSW_PUB) -I$(MPSW_SRVPATH_MSG) -I$(MPSW_SRVPATH_FUNC) -I$(RFICPATH_DEV) -I$(RFICPATH_AP) \
			  -I.$(CUNIT_PATH)

CFLAGS		+= -rdynamic -Wall -DAPR_INCLUDED -DAPR_ONLY -DLINUX_3 -DOS_LINUX -DPROGRAM_TXT_ONTHEFLY -DZLS3038X_INCLUDED -DAPR_INCLUDED -DAPR_ONLY -DZLS30390_INCLUDED -DZLS30771_INCLUDED

DEST		= ../lib

#
SRCS		=  $(shell ls $(MAINPATH)/src/*.c) \
			   $(shell ls $(HNDLPATH)/src/*.c) \
			   $(shell ls $(EQSPATH)/src/*.c) \
			   $(shell ls $(WDTPATH)/src/*.c) \
			   $(shell ls $(NMAPATH)/src/*.c) \
			   $(shell ls $(FDLPATH)/src/*.c) \
			   $(shell ls $(FULPATH)/src/*.c) \
			   $(shell ls $(NMASPATH)/src/*.c) \
			   $(shell ls $(RFAPATH)/src/*.c) \
			   $(shell ls $(CCAPATH)/src/*.c) \
			   $(shell ls $(SFPPATH)/src/*.c) \
			   $(shell ls $(EOMPATH)/src/*.c) \
			   $(shell ls $(FHSPATH)/src/*.c) \
			   $(shell ls $(LOGCPATH)/src/*.c) \
			   $(shell ls $(PFMPATH)/src/*.c) \
			   $(shell ls $(EIOPATH)/src/*.c) \
			   $(shell ls $(DHCPPATH)/src/*.c) \
			   $(shell ls $(PTPPATH)/src/*.c) \
			   $(shell ls $(MTIPATH)/src/*.c) \
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
