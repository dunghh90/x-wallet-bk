ifeq ($(BUILD_MODE_DEF), 0)
# corss platform difinition
PREFIX=/usr


CROSS_TARGET=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/x86_64-wrlinuxsdk-linux/usr/bin/aarch64-wrs-linux/aarch64-wrs-linux-
DESTDIR=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux
CFLAGS = --sysroot=/opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux

CINC +=-I. \
    -I../../../public \
    -I../../libmpmw_srv \
    -I$(DESTDIR)$(PREFIX)/include \
    -I$(DESTDIR)$(PREFIX)/include/libxml2 \
    -I$(DESTDIR)$(PREFIX)/include/libxml2/libxml

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
CFLAGS +=$(CWARN)
LFLAGS = -shared -fPIC -rdynamic -L../libmpmw_srv -lmpmw_srv

CINC +=-I. \
    -I$(DESTDIR)$(PREFIX)/include

# yumaworsk SDK definition
REAL_INC_BASE=$(DESTDIR)$(PREFIX)/include/yumapro
# add the directories inside
SUBDIR_CPP= -I$(REAL_INC_BASE)/platform \
  -I$(REAL_INC_BASE)/ncx \
  -I$(REAL_INC_BASE)/agt

SRCS = $(wildcard *.c)
INCS = $(wildcard *.h)
OBJS = $(patsubst %.c,%.o,$(SRCS))

LIB = libyp_system.so

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(LIB) $(OBJS)

$(OBJS): $(SRCS) $(INCS)
	$(CC) $(CFLAGS) $(CINC) $(SUBDIR_CPP) -c $(SRCS)

clean:
	rm -rf $(OBJS) $(LIB) *.d *.D

endif
ifeq ($(BUILD_MODE_DEF), 1)
# corss platform difinition
PREFIX=/usr

CROSS_TARGET=/usr/bin/
DESTDIR=/
CFLAGS=--sysroot=/

CINC +=-I. \
    -I../../../public \
    -I../../libmpmw_srv \
    -I$(DESTDIR)$(PREFIX)/include \
    -I$(DESTDIR)$(PREFIX)/include/libxml2 \
    -I$(DESTDIR)$(PREFIX)/include/libxml2/libxml

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
CFLAGS +=$(CWARN)
LFLAGS = -shared -fPIC -rdynamic -L../libmpmw_srv -lmpmw_srv

CINC +=-I. \
    -I$(DESTDIR)$(PREFIX)/include

# yumaworsk SDK definition
REAL_INC_BASE=$(DESTDIR)$(PREFIX)/include/yumapro
# add the directories inside
SUBDIR_CPP= -I$(REAL_INC_BASE)/platform \
  -I$(REAL_INC_BASE)/ncx \
  -I$(REAL_INC_BASE)/agt

SRCS = $(wildcard *.c)
INCS = $(wildcard *.h)
OBJS = $(patsubst %.c,%.o,$(SRCS))

LIB = libyp_system.so

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(LIB) $(OBJS)

$(OBJS): $(SRCS) $(INCS)
	$(CC) $(CFLAGS) -fPIC $(CINC) $(SUBDIR_CPP) -c $(SRCS)

clean:
	rm -rf $(OBJS) $(LIB) *.d *.D


endif