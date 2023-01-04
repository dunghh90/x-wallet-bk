ifeq ($(BUILD_QEMU_DEF),0)
#################
# 'make' build parameters
#################

OFSPATH = ../
BASEPATH = ../../$(OFSPATH)

include $(BASEPATH)Makefile.in
include $(OFSPATH)Makefile.in

MAKEFILE	= Makefile

SHELL		= /bin/sh

INC_PATH	= -I.$(MRUBY_PATH)/include \
			  -I.$(MRUBY_PATH)/src \
			  -I./dulib \
			  -I$(BASEPATH)api/inc \
			  -I$(BASEPATH)sys/inc_arch \
			  -I$(BASEPATH)sys/inc_mw \
			  -I$(BASEPATH)sys/inc

TARGETOPT	=

CFLAGS		+= $(INC_PATH)

LDLIBS		+= -L./dulib -ldu

LDLIBS		+= -L$(BASEPATH)../ramdisk/target/lib \
	-lrrhapi_com \
	-lbpf_svrm \
	-lbpf_facc \
	-lbpf_hrtm \
	-lbpf_string \
	-lbpf_ipcm \
	-lbpf_itcm \
	-lbpf_mblm \
	-lbpf_devc \
	-lbpf_log \
	-lbpf_plog \
	-lbpf_com \
	-lrrhapi_ptp \
	-lrrhapi_rfp

#################
# Program parameters
#################
PROGRAM		= du

SRCPATH		= .

SRCS		= $(SRCPATH)/du.c


OBJS		= $(SRCS:.c=.o)

DEST		= ./bin

#################
# PGRelief parameters
#################
PGRDIR = ./pgr
SRCLIST := $(SRCS)
ifndef CROSS_COMPILE_DIR
	CROSS_COMPILE_INC:=$(shell cd /opt/pkg/petalinux/tools/linux-i386/aarch64-linux-gnu/aarch64-linux-gnu/libc/usr/*/*/include;pwd)
	PGR_YLIB = -Y$(CROSS_COMPILE_INC) -Y$(CROSS_COMPILE_DIR)/target/usr/include/ -Y$(CROSS_COMPILE_DIR)/target/usr/local/include/
else
	GCC_INC:=$(shell cd $(GCC_DIR)/*/*/include;pwd)
	PGR_YLIB := -Y"$(GCC_INC)" -Y/usr/include/ -Y/usr/local/include/
endif
SEDDIR = $(BASEPATH)pgrchk.conf
PRG_CFLAGS  = $(INC_PATH) $(PGR_YLIB)
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

#################
# Target Rules
#################

all:	$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS) $(MAKEFILE)
		@echo "Linking $(PROGRAM) ..."
		$(LD) $(SYSROOT) -o $(PROGRAM) $(LDFLAGS) $(NONPRELINK) \
		$(OBJS) $(LIBS) $(SYSLIBS) $(LDLIBS) -L$(BASEPATH)../src/tools/tp/lib_pc/ -lm
		@echo "done"
#		$(STRIP) $(PROGRAM)

install: $(PROGRAM)
	@echo Installing $(PROGRAM) in $(DEST)
	@$(CP) $(PROGRAM) $(DEST)


clean:;		@rm -rf $(OBJS) $(PROGRAM) core
			@rm -rf bin/$(PROGRAM)
			@rm -rf $(PGRDIR)/*.log *.d *.o

#################
# PGRelief
#################
pgr:$(AOLIST)
	cd ./rrhlib;make pgr;
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


###

endif

ifeq ($(BUILD_QEMU_DEF),1)
#################
# 'make' build parameters
#################

OFSPATH = ../
BASEPATH = ../../$(OFSPATH)

include $(BASEPATH)Makefile.in
include $(OFSPATH)Makefile.in

MAKEFILE	= Makefile

SHELL		= /bin/sh

INC_PATH	= -I.$(MRUBY_PATH)/include \
			  -I.$(MRUBY_PATH)/src \
			  -I./dulib \
			  -I$(BASEPATH)api/inc \
			  -I$(BASEPATH)sys/inc_arch \
			  -I$(BASEPATH)sys/inc_mw \
			  -I$(BASEPATH)sys/inc

TARGETOPT	=

CFLAGS		+= $(INC_PATH)

LDLIBS		+= -L./dulib -ldu

LDLIBS		+= -L$(BASEPATH)../ramdisk/target/lib -L$(BASEPATH)../ramdisk/target/usr/local/lib64 \
	-lrrhapi_com \
	-lbpf_svrm \
	-lbpf_facc \
	-lbpf_hrtm \
	-lbpf_string \
	-lbpf_ipcm \
	-lbpf_itcm \
	-lbpf_mblm \
	-lbpf_devc_qemu \
	-lbpf_devc \
	-lbpf_log \
	-lbpf_plog \
	-lbpf_com \
	-lrrhapi_ptp \
	-lrrhapi_rfp \
	-ldl

#################
# Program parameters
#################
PROGRAM		= du

SRCPATH		= .

SRCS		= $(SRCPATH)/du.c


OBJS		= $(SRCS:.c=.o)

DEST		= ./bin

#################
# PGRelief parameters
#################
PGRDIR = ./pgr
SRCLIST := $(SRCS)
ifndef CROSS_COMPILE_DIR
	CROSS_COMPILE_INC:=$(shell cd /opt/pkg/petalinux/tools/linux-i386/aarch64-linux-gnu/aarch64-linux-gnu/libc/usr/*/*/include;pwd)
	PGR_YLIB = -Y$(CROSS_COMPILE_INC) -Y$(CROSS_COMPILE_DIR)/target/usr/include/ -Y$(CROSS_COMPILE_DIR)/target/usr/local/include/
else
	GCC_INC:=$(shell cd $(GCC_DIR)/*/*/include;pwd)
	PGR_YLIB := -Y"$(GCC_INC)" -Y/usr/include/ -Y/usr/local/include/
endif
SEDDIR = $(BASEPATH)pgrchk.conf
PRG_CFLAGS  = $(INC_PATH) $(PGR_YLIB)
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

#################
# Target Rules
#################

all:	$(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS) $(MAKEFILE)
		@echo "Linking $(PROGRAM) ..."
		$(LD) $(SYSROOT) -o $(PROGRAM) $(LDFLAGS) $(NONPRELINK) \
		$(OBJS) $(LIBS) $(SYSLIBS) $(LDLIBS) -L$(BASEPATH)../src/tools/tp/lib_pc/ -lm
		@echo "done"
#		$(STRIP) $(PROGRAM)

install: $(PROGRAM)
	@echo Installing $(PROGRAM) in $(DEST)
	@$(CP) $(PROGRAM) $(DEST)


clean:;		@rm -rf $(OBJS) $(PROGRAM) core
			@rm -rf bin/$(PROGRAM)
			@rm -rf $(PGRDIR)/*.log *.d *.o

#################
# PGRelief
#################
pgr:$(AOLIST)
	cd ./rrhlib;make pgr;
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


###

endif