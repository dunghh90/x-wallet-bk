ifeq ($(BUILD_MODE_DEF), 0)
MODULE_NAME=o-ran-troubleshooting

# SIL-SA Makefile for YumaPro Server Instrumentation Library
#

CROSS_TARGET=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/aarch64-wrs-linux/aarch64-wrs-linux-
DESTDIR=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux
CFLAGS += --sysroot=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux

############### SOURCE PROFILE ##############################

SUBDIR_NM=$(MODULE_NAME)

SUBDIR_CPP=

############### TARGET PROFILE ##############################

ifdef MAC
 INSTALL_PREFIX ?= /usr/local
else
 INSTALL_PREFIX ?= /usr
endif

TARGET=../bin

LIB_INST=../lib

ifdef YUMAPRO_HOME
WORK_INST=$(YUMAPRO_HOME)/target/lib
else
WORK_INST=
endif

ifdef LIB64
REAL_INST=$(DESTDIR)$(INSTALL_PREFIX)/lib64/yumapro
REAL_BASE=$(DESTDIR)$(INSTALL_PREFIX)/lib64
else
REAL_INST=$(DESTDIR)$(INSTALL_PREFIX)/lib/yumapro
REAL_BASE=$(DESTDIR)$(INSTALL_PREFIX)/lib
endif


#################### MAKE RULES ########################

all: sil_dummy sil_lib

#################### PLATFORM DEFINITIONS ############
CINCBASE=$(DESTDIR)$(INSTALL_PREFIX)/include

TOP=../../../

CINC = -I.

CINC += -I$(TOP)/../interface/msg/ -I$(TOP)/../../public/ -I$(TOP)/com/

ifdef FORCE_YUMAPRO_HOME
CINC += -I$(YUMAPRO_HOME)/src/platform -I$(YUMAPRO_HOME)/src/ncx \
     -I$(YUMAPRO_HOME)/src/agt -I$(YUMAPRO_HOME)/src/sil-sa \
     -I$(YUMAPRO_HOME)/src/ycontrol
else
CINC += -I$(CINCBASE)/yumapro/platform -I$(CINCBASE)/yumapro/ncx \
     -I$(CINCBASE)/yumapro/agt -I$(CINCBASE)/yumapro/sil-sa \
     -I$(CINCBASE)/yumapro/ycontrol
endif

CINC += -I$(CINCBASE) -I$(CINCBASE)/libxml2 -I$(CINCBASE)/libxml2/libxml

ifdef FREEBSD
CINC += -I${SYSROOT}/usr/local/include -I${SYSROOT}/usr/local/include/libxml2 \
     -I${SYSROOT}/usr/local/include/libxml2/libxml
endif


ifdef COVERITY
 ifneq "$(INSTALL_PREFIX)" "/usr"
   CINC += -I${SYSROOT}/usr/include -I${SYSROOT}/usr/include/libxml2 \
   -I${SYSROOT}/usr/include/libxml2/libxml
 endif
endif  # COVERITY

ifdef MAC
# MACOSX version
XCODE ?= /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include

  CFLAGS += -DMACOSX=1
  CINC += -I$(XCODE) \
    -I$(XCODE)/libxml2 \
    -I$(XCODE)/libxml2/libxml \
    -I$(XCODE)/openssl
endif

ifdef DESTDIR
OWNER=
else
ifdef MAC
OWNER=-oroot
else
ifdef CYGWIN
OWNER=
else
OWNER= --owner=root
endif
endif
endif

### GCC + [LINUX or MACOSX]

ifdef USE_WERROR
WERROR= -Werror
else
WERROR=
endif

CWARN=-Wall -Wno-long-long -Wformat-y2k -Winit-self \
	-Wswitch-default \
	-Wextra -Wundef -Wshadow -Wpointer-arith \
	-Wwrite-strings -Wbad-function-cast -Wcast-qual \
	-Waggregate-return -Wstrict-prototypes -Wold-style-definition \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wpacked -Winvalid-pch \
	-Wjump-misses-init -Wlogical-op -Wunsafe-loop-optimizations -Wconversion \
	-Wredundant-decls -Wnested-externs -Winline -std=gnu99 $(WERROR)


YDPARMS= --sil-sa --indent=4 --module=$(SUBDIR_NM)

# -Wunreachable-code removed due to -O3
# -O3 changed to -O2 due to code bloat from inline functions

CDEFS=-DDEBUG=1 -DLINUX=1 -DGCC=1

ifndef NOFLOAT
  CDEFS += -DHAS_FLOAT=1
endif

# production (0) or debug (1) build
ifdef DEBUG
  CFLAGS += -ggdb3
else
  CFLAGS += -g -O2
endif

ifdef PTHREADS
  CFLAGS += -DPTHREADS=1
  CFLAGS += -pthread
endif

CFLAGS += $(CDEFS) $(CWARN)

ifndef CYGWIN
   CFLAGS += -fPIC
endif

# memory leak debugging mode
ifdef MEMTRACE
  CFLAGS += -DMEMORY_DEBUG=1
endif

ifdef RELEASE
  CFLAGS += -DRELEASE=$(RELEASE)
endif

ifdef LIB64
   CFLAGS += -DLIB64=1
endif


ifdef MAC
   GRP=
else
ifdef DESTDIR
   GRP=
else
ifdef CYGWIN
   GRP=
else
   GRP=--group=root
endif
endif
endif

ifdef STATIC
LIBSUFFIX=a
else
 ifdef MAC
  ifdef COVERITY
LIBSUFFIX=so
  else
LIBSUFFIX=dylib
  endif # COVERITY
else
LIBSUFFIX=so
endif # MAC
endif # STATIC

CC=$(CROSS_TARGET)gcc
LINK?=$(CC)
LINT=splint
LINTFLAGS= '-weak -macrovarprefix "m_"'
##LIBFLAGS=-lsocket

LIBTOOL=$(CROSS_TARGET)ar
#LFLAGS=-v --no-as-needed
LFLAGS=-lm
LPATH=-L$(LIB_INST)

CEES = $(wildcard *.c)

HEES = $(wildcard *.h)

################ OBJS RULE #############
OBJS = $(patsubst %.c,$(TARGET)/%.o,$(CEES))

################ DEPS RULE #############
DEPS = $(patsubst %.c,%.D,$(wildcard *.c))

######################## PLATFORM DEFINITIONS #############
PLATFORM_CPP=

######################### MAKE DEPENDENCIES ###############
COMPILE.c= $(CC) $(CFLAGS) $(CPPFLAGS) $(PLATFORM_CPP) \
           $(CINC) $(SUBDIR_CPP) $(TARGET_ARCH) -c


$(TARGET)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(PLATFORM_CPP) \
        $(CINC) $(SUBDIR_CPP) $(TARGET_ARCH) -c -o $@ $<


# Common library rule

$(LIB_INST)/lib%.a: $(OBJS)
	$(LIBTOOL) cr $@ $(OBJS)
	ranlib $@

# dependency rule to make temp .D files from .c sources
# all the .D files are collected and appended to the
# appropriate Makefile when 'make depend' is run
# this rule is kept here to make sure it matches COMPILE.c
#%.D: %.c
#	$(CC) -MM -MG -MT $(TARGET)/$(patsubst %.c,%.o,$<) \
#	-Wall -Wcomment $(CPPFLAGS) $(PLATFORM_CPP) $(CINC) \
#	$(SUBDIR_CPP) $(TARGET_ARCH) -c $< > $@


################## MAKE DEPENDENCIES #####################
# following depend rule is the GNU version! Other versions TBD
#depend: dependencies

#dependencies: $(DEPS)
#	@if [ ! -f Makefile ]; then \
#	    echo "Error: Makefile missing!"; \
#	    exit 1; \
#	fi
#	@rm -f dependencies
#	@for i in $(DEPS); do \
#	  if [ -f $$i ] ; then \
#	    (cat $$i >> dependencies; echo "" >> dependencies) ; \
#	  else \
#	    (echo "*** Warning: Dependency file $i.D is missing! (Skipping...) ***"; \
#	     echo "# Warning: Missing file $$i !!!") ; \
#	  fi; \
#	done
#	@echo "" >> dependencies
# delete the .D files to force make depend to rebuild them each time
# that target is built
#	@rm -f $(DEPS)


test:

install:
	mkdir -p $(REAL_INST)
	install $(LIB_INST)/lib$(SUBDIR_NM).$(LIBSUFFIX) \
        $(REAL_INST)/lib$(SUBDIR_NM)_sa.$(LIBSUFFIX)

work:
ifdef YUMAPRO_HOME
	mkdir -p $(WORK_INST)
	install $(LIB_INST)/lib$(SUBDIR_NM).$(LIBSUFFIX) \
        $(WORK_INST)/lib$(SUBDIR_NM)_sa.$(LIBSUFFIX)
endif

sil_lib: $(LIB_INST)/lib$(SUBDIR_NM).$(LIBSUFFIX)

# this dummy rule keeps make from deleting the $(OBJS) as
# intermediate files
#sil_dummy: dependencies $(OBJS)

clean:
	rm -f $(OBJS) $(LIB_INST)/lib$(SUBDIR_NM).*

superclean:
	rm -f *~  $(DEPS) dependencies $(OBJS) \
	$(LIB_INST)/lib$(SUBDIR_NM).*

ifndef MAC
$(LIB_INST)/lib$(SUBDIR_NM).so: $(OBJS)
	$(LINK) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -rdynamic -Wl,-soname,lib$(SUBDIR_NM).so -o $@ $(OBJS)
	mv $(LIB_INST)/lib$(SUBDIR_NM).so  $(LIB_INST)/lib$(SUBDIR_NM)_sa.so
else
 ifdef COVERITY
$(LIB_INST)/lib$(SUBDIR_NM).so: $(OBJS)
	$(LINK) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -rdynamic -Wl,-soname,lib$(SUBDIR_NM).so -o $@ $(OBJS)
 else
$(LIB_INST)/lib$(SUBDIR_NM).so: $(OBJS)
	$(LINK) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -bundle -std=gnu99 \
	-current_version 1.0 \
	-undefined dynamic_lookup \
	-o $@ -install_name lib$(SUBDIR_NM).so $(OBJS)
 endif  # COVERITY
endif  # MAC

code:
	yangdump-sdk format=h $(YDPARMS) output=$(SUBDIR_NM).h
	yangdump-sdk format=c $(YDPARMS) output=$(SUBDIR_NM).c

splitcode:
	yangdump-sdk format=yh $(YDPARMS) output=y_$(SUBDIR_NM).h
	yangdump-sdk format=yc $(YDPARMS) output=y_$(SUBDIR_NM).c
	yangdump-sdk format=uh $(YDPARMS) output=u_$(SUBDIR_NM).h
	yangdump-sdk format=uc $(YDPARMS) output=u_$(SUBDIR_NM).c


.PHONY: all superclean clean test install uninstall \
        distclean depend lint code splitcode work sil_lib sil_dummy


# prevent the make program from choking on all the symbols
# that get generated from autogenerated make rules
.NOEXPORT:

#include dependencies
endif
ifeq ($(BUILD_MODE_DEF), 1)
MODULE_NAME=o-ran-troubleshooting

# SIL-SA Makefile for YumaPro Server Instrumentation Library
#

CROSS_TARGET=/usr/bin/
DESTDIR=/
CFLAGS=--sysroot=/

############### SOURCE PROFILE ##############################

SUBDIR_NM=$(MODULE_NAME)

SUBDIR_CPP=

############### TARGET PROFILE ##############################

ifdef MAC
 INSTALL_PREFIX ?= /usr/local
else
 INSTALL_PREFIX ?= /usr
endif

TARGET=../bin

LIB_INST=../lib

ifdef YUMAPRO_HOME
WORK_INST=$(YUMAPRO_HOME)/target/lib
else
WORK_INST=
endif

ifdef LIB64
REAL_INST=$(DESTDIR)$(INSTALL_PREFIX)/lib64/yumapro
REAL_BASE=$(DESTDIR)$(INSTALL_PREFIX)/lib64
else
REAL_INST=$(DESTDIR)$(INSTALL_PREFIX)/lib/yumapro
REAL_BASE=$(DESTDIR)$(INSTALL_PREFIX)/lib
endif


#################### MAKE RULES ########################

all: sil_dummy sil_lib

#################### PLATFORM DEFINITIONS ############
CINCBASE=$(DESTDIR)$(INSTALL_PREFIX)/include

TOP=../../../

CINC = -I.

CINC += -I$(TOP)/../interface/msg/ -I$(TOP)/../../public/ -I$(TOP)/com/

ifdef FORCE_YUMAPRO_HOME
CINC += -I$(YUMAPRO_HOME)/src/platform -I$(YUMAPRO_HOME)/src/ncx \
     -I$(YUMAPRO_HOME)/src/agt -I$(YUMAPRO_HOME)/src/sil-sa \
     -I$(YUMAPRO_HOME)/src/ycontrol
else
CINC += -I$(CINCBASE)/yumapro/platform -I$(CINCBASE)/yumapro/ncx \
     -I$(CINCBASE)/yumapro/agt -I$(CINCBASE)/yumapro/sil-sa \
     -I$(CINCBASE)/yumapro/ycontrol
endif

CINC += -I$(CINCBASE) -I$(CINCBASE)/libxml2 -I$(CINCBASE)/libxml2/libxml

ifdef FREEBSD
CINC += -I${SYSROOT}/usr/local/include -I${SYSROOT}/usr/local/include/libxml2 \
     -I${SYSROOT}/usr/local/include/libxml2/libxml
endif


ifdef COVERITY
 ifneq "$(INSTALL_PREFIX)" "/usr"
   CINC += -I${SYSROOT}/usr/include -I${SYSROOT}/usr/include/libxml2 \
   -I${SYSROOT}/usr/include/libxml2/libxml
 endif
endif  # COVERITY

ifdef MAC
# MACOSX version
XCODE ?= /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include

  CFLAGS += -DMACOSX=1
  CINC += -I$(XCODE) \
    -I$(XCODE)/libxml2 \
    -I$(XCODE)/libxml2/libxml \
    -I$(XCODE)/openssl
endif

ifdef DESTDIR
OWNER=
else
ifdef MAC
OWNER=-oroot
else
ifdef CYGWIN
OWNER=
else
OWNER= --owner=root
endif
endif
endif

### GCC + [LINUX or MACOSX]

ifdef USE_WERROR
WERROR= -Werror
else
WERROR=
endif

CWARN=-Wall -Wno-long-long -Wformat-y2k -Winit-self \
	-Wswitch-default \
	-Wextra -Wundef -Wshadow -Wpointer-arith \
	-Wwrite-strings -Wbad-function-cast -Wcast-qual \
	-Waggregate-return -Wstrict-prototypes -Wold-style-definition \
	-Wmissing-prototypes -Wmissing-declarations \
	-Wpacked -Winvalid-pch \
	-Wjump-misses-init -Wlogical-op -Wunsafe-loop-optimizations -Wconversion \
	-Wredundant-decls -Wnested-externs -Winline -std=gnu99 $(WERROR)


YDPARMS= --sil-sa --indent=4 --module=$(SUBDIR_NM)

# -Wunreachable-code removed due to -O3
# -O3 changed to -O2 due to code bloat from inline functions

CDEFS=-DDEBUG=1 -DLINUX=1 -DGCC=1

ifndef NOFLOAT
  CDEFS += -DHAS_FLOAT=1
endif

# production (0) or debug (1) build
ifdef DEBUG
  CFLAGS += -ggdb3
else
  CFLAGS += -g -O2
endif

ifdef PTHREADS
  CFLAGS += -DPTHREADS=1
  CFLAGS += -pthread
endif

CFLAGS += $(CDEFS) $(CWARN)

ifndef CYGWIN
   CFLAGS += -fPIC
endif

# memory leak debugging mode
ifdef MEMTRACE
  CFLAGS += -DMEMORY_DEBUG=1
endif

ifdef RELEASE
  CFLAGS += -DRELEASE=$(RELEASE)
endif

ifdef LIB64
   CFLAGS += -DLIB64=1
endif


ifdef MAC
   GRP=
else
ifdef DESTDIR
   GRP=
else
ifdef CYGWIN
   GRP=
else
   GRP=--group=root
endif
endif
endif

ifdef STATIC
LIBSUFFIX=a
else
 ifdef MAC
  ifdef COVERITY
LIBSUFFIX=so
  else
LIBSUFFIX=dylib
  endif # COVERITY
else
LIBSUFFIX=so
endif # MAC
endif # STATIC

CC=$(CROSS_TARGET)gcc
LINK?=$(CC)
LINT=splint
LINTFLAGS= '-weak -macrovarprefix "m_"'
##LIBFLAGS=-lsocket

LIBTOOL=$(CROSS_TARGET)ar
#LFLAGS=-v --no-as-needed
LFLAGS=-lm
LPATH=-L$(LIB_INST)

CEES = $(wildcard *.c)

HEES = $(wildcard *.h)

################ OBJS RULE #############
OBJS = $(patsubst %.c,$(TARGET)/%.o,$(CEES))

################ DEPS RULE #############
DEPS = $(patsubst %.c,%.D,$(wildcard *.c))

######################## PLATFORM DEFINITIONS #############
PLATFORM_CPP=

######################### MAKE DEPENDENCIES ###############
COMPILE.c= $(CC) $(CFLAGS) $(CPPFLAGS) $(PLATFORM_CPP) \
           $(CINC) $(SUBDIR_CPP) $(TARGET_ARCH) -c


$(TARGET)/%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(PLATFORM_CPP) \
        $(CINC) $(SUBDIR_CPP) $(TARGET_ARCH) -c -o $@ $<


# Common library rule

$(LIB_INST)/lib%.a: $(OBJS)
	$(LIBTOOL) cr $@ $(OBJS)
	ranlib $@

# dependency rule to make temp .D files from .c sources
# all the .D files are collected and appended to the
# appropriate Makefile when 'make depend' is run
# this rule is kept here to make sure it matches COMPILE.c
#%.D: %.c
#	$(CC) -MM -MG -MT $(TARGET)/$(patsubst %.c,%.o,$<) \
#	-Wall -Wcomment $(CPPFLAGS) $(PLATFORM_CPP) $(CINC) \
#	$(SUBDIR_CPP) $(TARGET_ARCH) -c $< > $@


################## MAKE DEPENDENCIES #####################
# following depend rule is the GNU version! Other versions TBD
#depend: dependencies

#dependencies: $(DEPS)
#	@if [ ! -f Makefile ]; then \
#	    echo "Error: Makefile missing!"; \
#	    exit 1; \
#	fi
#	@rm -f dependencies
#	@for i in $(DEPS); do \
#	  if [ -f $$i ] ; then \
#	    (cat $$i >> dependencies; echo "" >> dependencies) ; \
#	  else \
#	    (echo "*** Warning: Dependency file $i.D is missing! (Skipping...) ***"; \
#	     echo "# Warning: Missing file $$i !!!") ; \
#	  fi; \
#	done
#	@echo "" >> dependencies
# delete the .D files to force make depend to rebuild them each time
# that target is built
#	@rm -f $(DEPS)


test:

install:
	mkdir -p $(REAL_INST)
	install $(LIB_INST)/lib$(SUBDIR_NM).$(LIBSUFFIX) \
        $(REAL_INST)/lib$(SUBDIR_NM)_sa.$(LIBSUFFIX)

work:
ifdef YUMAPRO_HOME
	mkdir -p $(WORK_INST)
	install $(LIB_INST)/lib$(SUBDIR_NM).$(LIBSUFFIX) \
        $(WORK_INST)/lib$(SUBDIR_NM)_sa.$(LIBSUFFIX)
endif

sil_lib: $(LIB_INST)/lib$(SUBDIR_NM).$(LIBSUFFIX)

# this dummy rule keeps make from deleting the $(OBJS) as
# intermediate files
#sil_dummy: dependencies $(OBJS)

clean:
	rm -f $(OBJS) $(LIB_INST)/lib$(SUBDIR_NM).*

superclean:
	rm -f *~  $(DEPS) dependencies $(OBJS) \
	$(LIB_INST)/lib$(SUBDIR_NM).*

ifndef MAC
$(LIB_INST)/lib$(SUBDIR_NM).so: $(OBJS)
	$(LINK) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -rdynamic -Wl,-soname,lib$(SUBDIR_NM).so -o $@ $(OBJS)
	mv $(LIB_INST)/lib$(SUBDIR_NM).so  $(LIB_INST)/lib$(SUBDIR_NM)_sa.so
else
 ifdef COVERITY
$(LIB_INST)/lib$(SUBDIR_NM).so: $(OBJS)
	$(LINK) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -rdynamic -Wl,-soname,lib$(SUBDIR_NM).so -o $@ $(OBJS)
 else
$(LIB_INST)/lib$(SUBDIR_NM).so: $(OBJS)
	$(LINK) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -shared -bundle -std=gnu99 \
	-current_version 1.0 \
	-undefined dynamic_lookup \
	-o $@ -install_name lib$(SUBDIR_NM).so $(OBJS)
 endif  # COVERITY
endif  # MAC

code:
	yangdump-sdk format=h $(YDPARMS) output=$(SUBDIR_NM).h
	yangdump-sdk format=c $(YDPARMS) output=$(SUBDIR_NM).c

splitcode:
	yangdump-sdk format=yh $(YDPARMS) output=y_$(SUBDIR_NM).h
	yangdump-sdk format=yc $(YDPARMS) output=y_$(SUBDIR_NM).c
	yangdump-sdk format=uh $(YDPARMS) output=u_$(SUBDIR_NM).h
	yangdump-sdk format=uc $(YDPARMS) output=u_$(SUBDIR_NM).c


.PHONY: all superclean clean test install uninstall \
        distclean depend lint code splitcode work sil_lib sil_dummy


# prevent the make program from choking on all the symbols
# that get generated from autogenerated make rules
.NOEXPORT:

#include dependencies

endif