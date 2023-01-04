#
#  Makefile for

OFSPATH = ../../../
BASEPATH = $(OFSPATH)../../
SYSDIR  = $(BASEPATH)/sys/
APIDIR  = $(BASEPATH)/api/

include ../../$(OFSPATH)Makefile.in
include $(OFSPATH)Makefile.in

SRCDIR	= .
OBJDIR	= ./
INCD00	= ../
LFILE	= librrhct.a
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

#
SRCS =\
	./apidummy_for_ct.c \
	./dum_BPF_HM_DEVC_SFP_READ.c

#

OBJS = $(SRCS:.c=.o)


#
all:	$(OBJS) $(LFILE)
#
.c.o:
	$(CC) $(CFLAGS) -c  $(INCD) $<
#
$(LFILE): $(OBJS) $(SRCS)
	$(AR) $(ARFLAGS) $(LFILE) $(OBJS)
	@rm -f $(OBJS)
	@echo "librrhct.a make success"

#install: $(LFILE)
#	@echo Installing $(LFILE) in $(DEST)
#	@$(CP) $(LFILE) $(DEST)

clear clean:;	@rm -rf $(OBJS)
				@rm -rf $(LFILE)
				@rm -rf $(PGRDIR)/*.log


