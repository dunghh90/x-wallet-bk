ifeq ($(BUILD_MODE_DEF), 0)
#------------------------------------------------------
# Makefile for mpsw srv library
#------------------------------------------------------
# corss platform difinition
PREFIX=/usr


CROSS_TARGET=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/aarch64-wrs-linux/aarch64-wrs-linux-
DESTDIR=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux
CFLAGS = --sysroot=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux

ifdef USE_WERROR
WERROR=-Werror
else
WERROR=
endif

CWARN=-Wall -Wno-long-long -Wformat-y2k -Winit-self \
        -Wswitch-default -Wunused-parameter \
        -Wextra -Wundef -Wshadow -Wpointer-arith \
        -Wwrite-strings -Wbad-function-cast -Wcast-qual \
        -Waggregate-return -Wstrict-prototypes -Wold-style-definition \
        -Wmissing-prototypes -Wmissing-declarations \
        -Wpacked -Winvalid-pch -Wformat -Wformat-security \
        -Wredundant-decls -Wnested-externs -Winline -std=gnu99 $(WERROR)


CC	= $(CROSS_TARGET)gcc
CFLAGS += -shared -fPIC -rdynamic -O2 -D_FORTIFY_SOURCE=2 $(CWARN) -fstack-protector-all -MMD
LFLAGS =

SRCS = $(wildcard *.c)
INCS = $(wildcard *.h)

REAL_INC_BASE=$(DESTDIR)$(PREFIX)/include/yumapro

MPINC= -I../../../public \
       -I../../public \
       -I$(DESTDIR)$(PREFIX)/include \
       -I$(DESTDIR)$(PREFIX)/include/libxml2 \
       -I$(DESTDIR)$(PREFIX)/include/libxml2/libxml \
       -I$(DESTDIR)$(PREFIX)/include \
       -I$(REAL_INC_BASE)/platform \
       -I$(REAL_INC_BASE)/ncx \
       -I$(REAL_INC_BASE)/agt


OBJS = $(patsubst %.c,%.o,$(SRCS))

LIB = libmpmw_srv.so

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(LIB) $(OBJS)

$(OBJS): $(SRCS) $(INCS)
	$(CC) $(CFLAGS) -c $(SRCS) $(MPINC)

clean:
	rm $(OBJS) $(LIB) *.d

endif
ifeq ($(BUILD_MODE_DEF), 1)
#------------------------------------------------------
# Makefile for mpsw srv library
#------------------------------------------------------
# corss platform difinition
PREFIX=/usr


CROSS_TARGET=/usr/bin/
DESTDIR=/
CFLAGS=--sysroot=/

ifdef USE_WERROR
WERROR=-Werror
else
WERROR=
endif

CWARN=-Wall -Wno-long-long -Wformat-y2k -Winit-self \
        -Wswitch-default -Wunused-parameter \
        -Wextra -Wundef -Wshadow -Wpointer-arith \
        -Wwrite-strings -Wbad-function-cast -Wcast-qual \
        -Waggregate-return -Wstrict-prototypes -Wold-style-definition \
        -Wmissing-prototypes -Wmissing-declarations \
        -Wpacked -Winvalid-pch -Wformat -Wformat-security \
        -Wredundant-decls -Wnested-externs -Winline -std=gnu99 $(WERROR)


CC	= $(CROSS_TARGET)gcc
CFLAGS += -shared -fPIC -rdynamic -O2 -D_FORTIFY_SOURCE=2 $(CWARN) -fstack-protector-all -MMD
LFLAGS =

SRCS = $(wildcard *.c)
INCS = $(wildcard *.h)

REAL_INC_BASE=$(DESTDIR)$(PREFIX)/include/yumapro

MPINC= -I../../../public \
       -I../../public \
       -I$(DESTDIR)$(PREFIX)/include \
       -I$(DESTDIR)$(PREFIX)/include/libxml2 \
       -I$(DESTDIR)$(PREFIX)/include/libxml2/libxml \
       -I$(DESTDIR)$(PREFIX)/include \
       -I$(REAL_INC_BASE)/platform \
       -I$(REAL_INC_BASE)/ncx \
       -I$(REAL_INC_BASE)/agt


OBJS = $(patsubst %.c,%.o,$(SRCS))

LIB = libmpmw_srv.so

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(LIB) $(OBJS)

$(OBJS): $(SRCS) $(INCS)
	$(CC) $(CFLAGS) -c $(SRCS) $(MPINC)

clean:
	rm $(OBJS) $(LIB) *.d

endif