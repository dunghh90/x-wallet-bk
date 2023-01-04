#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in

SRCDIR		= .
OBJDIR		= ./
TARGET_PATH	= ../../src
LFILE		= libct_bpf.so
LIBS		= 
INCD		= -I$(SYSINC) -I$(BPFINC) -I$(MWPATH)/inc -I$(PFPATH)/ft_com/com/inc
CFLAGS		+= -rdynamic
DEST		= ../lib

#
SRCS	= $(shell ls ./*.c)
OBJS	= $(SRCS:.c=.o)

#
all:	$(OBJS) $(LFILE)
#
.c.o:
	$(CC) $(CFLAGS) -c  $(INCD) $<
#
$(LFILE): $(OBJS) $(SRCS)
	$(CC) -shared -fPIC $(OBJS) $(LIBS) -o $(LFILE)
	@rm -f $(OBJS)
	@echo "$(LFILE) make success"

install:; @echo Installing $(LFILE) in $(DEST)
		  @$(CP) $(LFILE) $(DEST)

clean:;	 @rm -f $(OBJS) $(LFILE) $(DEST)/$(LFILE) *.d
#
