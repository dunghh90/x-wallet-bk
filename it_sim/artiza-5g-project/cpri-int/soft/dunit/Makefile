#################
# 'make' build parameters
#################

MAKEINC_PATH = ./
include $(MAKEINC_PATH)Makefile.in

CUNIT_PATH		= ./CUnit/CUnit/Headers
MRUBY_PATH		= ./mruby
ALL_LIBPATH		= ./lib

LDLIBS			+= -L$(ALL_LIBPATH) -lct_bpf -L$(ALL_LIBPATH) -lct_lib_dmy -L$(ALL_LIBPATH) -lmruby -L$(ALL_LIBPATH) -lcunit -L$(ALL_LIBPATH) -lct_units -lpthread -lmenu -lncurses -lm
CFLAGS			+= -I$(MRUBY_PATH)/include -I$(SYSINC) -I$(BPFINC) -I$(MWPATH)/inc -I$(OALPATH)/RU/ipcm/include -I$(CUNIT_PATH)
MAKEFILE		= Makefile

#################
# Program parameters
#################
PROGRAM		= dunitcore
SRCPATH		= .
SRCS		= $(shell ls ./*.c)
OBJS		= $(SRCS:.c=.o)
DEST		= ./bin

#################
# Target Rules
#################
all:	$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS) $(MAKEFILE)
		@echo "Linking $(PROGRAM) ..."
		$(LD) -o $(PROGRAM) $(LDFLAGS) $(NONPRELINK) \
		$(OBJS) $(LIBS) $(SYSLIBS) $(LDLIBS)
		@echo "done"
#		$(STRIP) $(PROGRAM)

install: $(PROGRAM)
	@echo Installing $(PROGRAM) in $(DEST)
	@$(CP) $(PROGRAM) $(DEST)
	@rm -rf *.o *.d

clean:;		@rm -rf $(OBJS) $(PROGRAM) core ./*.map ./*.d ./*.su
			@rm -rf bin/$(PROGRAM)

