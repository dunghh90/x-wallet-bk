#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in

SRCDIR	= .
OBJDIR	= ./
TARGET_PATH	= ../../src
LFILE		= libct_units.so
PFSYSINC	= $(PFPATH)/sys
INCD		=  -I.$(MRUBY_PATH)/include -I../ -I$(SYSINC) -I$(BPFINC) -I$(CMNPATH)/inc -I$(PFSYSINC) -I$(APIINC) -I.$(CUNIT_PATH)
CFLAGS		+= -rdynamic
DEST		= ../lib
LIBS		= -L$(DEST) -lcunit 

#
SRCS	= ct_units.o 
OBJDS	= $(shell ls *.o)
OBJS	= $(SRCS:.c=.o)

#
all:	$(OBJS) $(LFILE)
#
.c.o:
	$(CC) $(CFLAGS) -c  $(INCD) $<
#
$(LFILE): $(OBJS) $(SRCS)
	$(CC) -fPIC -shared $(OBJDS) $(LIBS) -o $(LFILE)
	@rm -f $(OBJS) $(OBJDS) $(DEST)/$(LFILE) *.d
	@echo "$(LFILE) make success"

install:; @echo Installing $(LFILE) in $(DEST)
		  @$(CP) $(LFILE) $(DEST)

clean:;	 @rm -f *.o $(LFILE) $(DEST)/$(LFILE) *.d

#
