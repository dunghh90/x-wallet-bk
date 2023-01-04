#
#  Makefile for

MAKEINC_PATH = ../
include $(MAKEINC_PATH)Makefile.in

SRCDIR	= .
OBJDIR	= ./
INCD00	= ./
LFILE	= libct_lib_dmy.so
LIBS	= 
INCD	= 
CFLAGS	+= -rdynamic -I../$(MRUBY_PATH)/include -I$(MAKEINC_PATH) -I$(SYSINC) -I$(BPFINC) -I.$(CUNIT_PATH)
DEST	= ../lib

#
SRCS	= $(shell ls ./*.c)
OBJS	= $(SRCS:.c=.o)

#
all:	$(OBJS) $(LFILE)
#
$(LFILE): $(OBJS) $(SRCS)
	$(CC) -shared -fPIC $(OBJS) $(LIBS) -o $(LFILE)
	@rm -f $(OBJS)
	@echo "$(LFILE) make success"

install:; @echo Installing $(LFILE) in $(DEST)
		  @$(CP) $(LFILE) $(DEST)

clean:;	 @rm -f $(OBJS) $(LFILE) $(DEST)/$(LFILE) *.d

#
