#
#  Makefile for

OFSPATH = ../../
BASEPATH = $(OFSPATH)../..
SYSDIR  = $(BASEPATH)/sys/
APIDIR  = $(BASEPATH)/api/

include ../../$(OFSPATH)Makefile.in
include $(OFSPATH)Makefile.in

SRCDIR	= .
OBJDIR	= ./
INCD00	= ./
LFILE	= libdu.a
LIBS	= 
INCD	= -I$(INCD00) -I$(OFSPATH)$(MRUBY_PATH)/include \
-I$(SYSDIR)inc_arch/ \
-I$(SYSDIR)inc_mw/ \
-I$(SYSDIR)inc/ \
-I$(APIDIR)inc/ \
-I$(BASEPATH)/pf/sys \
-I$(BASEPATH)/pf/ft_rfp/dpd/inc \
-I$(BASEPATH)/cmn/dcm/inc \
-I$(BASEPATH)/pf/ft_com/com/inc \
-I$(BASEPATH)/cmn/com/inc \
-I$(BASEPATH)/mw/inc

vpath %.h $(INCD00) 
vpath %.c $(INCD00) 

SRCS =\
	$(INCD00)dulib.c \
	$(INCD00)dulib_device_access.c \
	$(INCD00)dulib_api.c \
	$(INCD00)dulib_tbl.c

OBJS = $(SRCS:.c=.o)

##PGRelief
PGRDIR = ./pgr
SRCLIST := $(SRCS)
ifdef CROSS_COMPILE_DIR
	CROSS_COMPILE_INC:=$(shell cd $(CROSS_COMPILE_DIR)/lib/gcc/*/*/include;pwd)
	PGR_YLIB = -Y$(CROSS_COMPILE_INC) -Y$(CROSS_COMPILE_DIR)/target/usr/include/ -Y$(CROSS_COMPILE_DIR)/target/usr/local/include/
else
	GCC_INC:=$(shell cd $(GCC_DIR)/*/*/include;pwd)
	PGR_YLIB := -Y"$(GCC_INC)" -Y/usr/include/ -Y/usr/local/include/ 
endif
SEDDIR = ../../$(OFSPATH)pgrchk.conf
PRG_CFLAGS  = $(INCD) $(PGR_YLIB)
AOLIST = $(SRCLIST:.c=.c.ao)
PGR_MESSAGE_A=agorup.log
PGR_MESSAGE_ALL=all.log
PGRFILELIST:= \
$(SRCLIST:.c=.c.aa) \
$(SRCLIST:.c=.c.ao) \
$(SRCLIST:.c=.c.ar) \
$(SRCLIST:.c=.c.fd) \
$(SRCLIST:.c=.c.mes) \
$(SRCLIST:.c=.c.pgr) \
$(SRCLIST:.c=.c.pgrall)

#
all:	$(OBJS) $(LFILE)
#
.c.o:
	$(CC) $(CFLAGS) -c $(INCD) $<

#
$(LFILE): $(OBJS) $(SRCS)
	$(AR) $(ARFLAGS) $(LFILE) $(OBJS) $(COM_OBJS)
	@rm -f $(OBJS)
	@echo "libdu.a make success"

#install: $(LFILE)
#	@echo Installing $(LFILE) in $(DEST)
#	@$(CP) $(LFILE) $(DEST)

clear clean:;	@rm -rf $(OBJS)
				@rm -rf $(LFILE)
				@rm -rf $(PGRDIR)/*.log *.d

#PGRelief
pgr:$(AOLIST)
	cat *.pgr >> $(PGR_MESSAGE_A)
	cat *.pgrall >> $(PGR_MESSAGE_ALL)
	mv $(PGR_MESSAGE_A) $(PGR_MESSAGE_ALL) $(PGRDIR)/
	rm $(PGRFILELIST)

.SUFFIXES: .c .c.ao

.c.c.ao:
	@$(EPOMDIR)/pgr5 -Z $(SEDDIR) -K "GNU/GCC3" --C99 --enable_gcc_keywords -c $(PRG_CFLAGS) $< | sort -u | sort '-t(' -k 1,1 -k 2n,2 > $<.pgrall
	@sed -n -e '/! pgr/p' -e '/a pgr/p' $<.pgrall | \
	sort '-t(' -k 1,1 -k 2n,2 -o $<.pgr

pgr_clean:
	rm $(PGRDIR)/$(PGR_MESSAGE_A) $(PGRDIR)/$(PGR_MESSAGE_ALL)


