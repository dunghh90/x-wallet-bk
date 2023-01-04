# Settings for building target on Linux OS
#
# environment variable LNX_PATH can be used to specify additional include paths
# required for compilation
#


# Set Linux as the OS.
OS_TYPE = LINUX

# The following Linux build control flag must be specified ON A SEPARATE LINE WITH NO TRAILING COMMENTS:
#   TOOLS = DENX            # Builds using the DENX PPC LINUX gcc tool chain.
#   TOOLS = CODE_SOURCERY   # Builds using the CODE_SOURCERY ARM LINUX gcc tool chain.

# TOOLS = DENX
TOOLS = CODE_SOURCERY

# Set DENX as the default toolchain if one has not been defined yet.
ifndef TOOLS
  TOOLS = DENX
endif

# Default CPU type. Can be overridden from command line
#   CPU = 603    # Generic core (default)
#   CPU = 860    # builds for the 860
#   CPU = 8260   # builds for the 8260
#   CPU = 8313   # builds for the 8313

# Set the 603 as the default CPU if one has not been defined yet.
ifndef CPU
  CPU = ARMv8-LE
endif

CFLAGS_DEF += -D_ZL303XX_OS_SIGNAL_HANDLER -D_ZL303XX_OS_SIGNAL_HANDLER_USE_PIPE

ifdef FSM99XX_BOARD
   CFLAGS_DEF += -DFSM99XX_BOARD -DLINUX_3
   FSM99XX_BOARD = 1
   CPU = ARMv7-LE
   TOOLS = CODE_SOURCERY
else

  ifndef BOARD
   # If we wanted to build for a board, we could've defined it already.
   #
   # Defines for Zarlink Evaluation Boards only.
   # May be set in the Makefile
   # ZLE30310_BOARD = 1
   # ZLE30320_BOARD = 1
   # ZLE30360_BOARD = 1
   # ZLE1588_BOARD = 1
   BOARD = NONE
  endif 
   
   # Evaluation board-specific setup.
   ifdef ZLE30310_BOARD
      CFLAGS_DEF += -D_ZL303XX_ZLE30310_BOARD
      CPU = 8260
   endif
   
   ifdef ZLE30320_BOARD
      CFLAGS_DEF += -D_ZL303XX_ZLE30320_BOARD
      CPU = 8313
   endif
   
   ifdef ZLE30360_BOARD
     CFLAGS_INCLUDE += ../../zlPortingVitesse/src/include/ ../../zlPortingVitesse/src/base/phy/ts/ ../../zlPortingVitesse/src/base/phy/phy_1g/
     CFLAGS_DEF += -D_ZL303XX_ZLE30360_BOARD -D_ZL303XX_LINUX_CLI
     # CFLAGS_DEF += -DZLE30360_SW_VARIANT -DZLS30361_INCLUDED -DZLS30390_INCLUDED
     CFLAGS_DEF += -DZLE30360_SW_VARIANT -DZLS30361_INCLUDED
     CPU = 8313
     TOOLS = DENX
     BOARD = ZLE30360
   endif

   ifdef ZLE1588_BOARD
     CFLAGS_INCLUDE += ../../zlPortingVitesse/src/include/ ../../zlPortingVitesse/src/base/phy/ts/ ../../zlPortingVitesse/src/base/phy/phy_1g/
     CFLAGS_DEF += -D_ZL303XX_ZLE1588_BOARD -D_ZL303XX_LINUX_CLI
     CPU = 8313
     TOOLS = DENX
     BOARD = ZLE1588
   endif
endif

# Uncomment this line to compile in IPv6 socket support.
#CFLAGS_DEF += -DZL_INCLUDE_IPV6_SOCKET

# Specific compiler directives, such as optimisation can be specified
# using the environment variable CFLAGS_OPT. For example -O2 (SPEED), -O3 (SPEED and SPACE)
# WARNING: Optimizer flags can cause bugs
#CFLAGS_OPT=-O2
#CFLAGS_OPT=-O3

# Extensive warning flags [Add -Wcomment to ignore '#warning']
CFLAGS_WARN = -W -Wall -Wcast-align -Wcast-qual -Winline -Wmissing-prototypes -Wnested-externs \
              -Wpointer-arith -Wshadow -Wwrite-strings

CFLAGS_ANSI = -ansi
CFLAGS_PEDANTIC = -pedantic

# Define _DEBUG for trace and other debugging
CFLAGS_DEF += -D_DEBUG

#================================ TOOLCHAINS ===================================

ifeq "$(TOOLS)" "DENX"
  #  OS_LNX_VER = 26  # Ver 2.6 of the Linux compiler will invoke code specific to the Linux platform
  OS_LNX_VER = 26

  LNX_VARIANT = LNX_DENX

  # Warning!!!! Change hard-coded paths to your toolchain!!!
  ifeq "$(HOSTNAME)" "rdswlinux1"
    TOOLCHAIN = /opt/eldk
  else
    TOOLCHAIN = /opt/crosstool/eldk-ppc-4.2
  endif

  CROSS_COMPILE = ppc_6xx-
  LDFLAGS = -nostdlib -r

ifeq "$(CPU)" "PPC-6500"
  # PowerPC E6500 
  CROSS_COMPILE = powerpc-linux-
  CFLAGS_MISC += -v -Wa,-me6500,-a64,-be
  LDFLAGS = -nostdlib -r -m elf64ppc
  OBJ_DIR = obj6500
endif

  CC = $(TOOLCHAIN)/usr/bin/$(CROSS_COMPILE)gcc
  LD = $(TOOLCHAIN)/usr/bin/$(CROSS_COMPILE)ld

  CFLAGS_MISC += -std=c99
  CFLAGS_DEF += -D__USE_POSIX

  # Warning!!!! Hard-coded path for the DENX PPC CrossCompiler enviroment!!!
  CFLAGS_INCLUDE +=  $(TOOLCHAIN)/usr/lib/gcc/powerpc-linux/4.2.2/include/ \
                     $(TOOLCHAIN)/ppc_6xx/usr/include \
                     $(TOOLCHAIN)/ppc_6xx/usr/include/linux

  CFLAGS_MISC += -mstrict-align -nostdinc -fno-builtin

endif

#-------------------------------------------------------------------------------

ifeq "$(TOOLS)" "MIPS64"
  #  OS_LNX_VER = 26  # Ver 2.6 of the Linux compiler will invoke code specific to the Linux platform
  OS_LNX_VER = 26

  LNX_VARIANT = LNX_DENX
  OS_TYPE = LINUX

  # Warning!!!! Change hard-coded paths to your toolchain!!!
  ifeq "$(HOSTNAME)" "rdswlinux1"
    TOOLCHAIN = /home/dann/bigdrive/eldk
  else
    TOOLCHAIN = /opt/crosstool/eldk-mips-4.1
  endif

  CROSS_COMPILE = mips_4KC-

  CC = $(TOOLCHAIN)/usr/bin/$(CROSS_COMPILE)gcc
  LD = $(TOOLCHAIN)/usr/bin/$(CROSS_COMPILE)ld

  LDFLAGS = -r -A mips64

  CFLAGS_MISC += -std=c99
  CFLAGS_DEF += -D__USE_POSIX

  # Defines to support 64-bit compilers
  CFLAGS_DEF += -D_64BIT_COMPILER

  # Warning!!!! Hard-coded path for the MIPS Cross Compiler enviroment on a RedHat 4!!!
  CFLAGS_INCLUDE +=  $(TOOLCHAIN)/usr/lib/gcc/mips-linux/4.0.0/include \
                     $(TOOLCHAIN)/mips_4KC/usr/include \
                     $(TOOLCHAIN)/mips_4KC/usr/include/linux

  CFLAGS_MISC += -nostdinc -fno-builtin
endif

#-------------------------------------------------------------------------------

ifeq "$(TOOLS)" "MIPS32"
#  OS_LNX_VER = 26  # Ver 2.6 of the Linux compiler will invoke code specific to the Linux platform
  OS_LNX_VER = 26

  LNX_VARIANT = LNX_DENX
  OS_TYPE = LINUX

  # Warning!!!! Change hard-coded paths to your toolchain!!!
  ifeq "$(HOSTNAME)" "rdswlinux1"
    TOOLCHAIN = /home/dann/bigdrive/eldk
  else
    TOOLCHAIN = /opt/crosstool/eldk-mips-4.1
  endif

  CROSS_COMPILE = mips-linux-

  CC = $(TOOLCHAIN)/usr/bin/$(CROSS_COMPILE)gcc
  LD = $(TOOLCHAIN)/usr/bin/$(CROSS_COMPILE)ld

  LDFLAGS = -r -A mips32

  CFLAGS_MISC += -std=c99
  CFLAGS_DEF += -D__USE_POSIX

  CFLAGS_DEF += -DZL_LNX_DENX_MIPS32

  # Warning!!!! Hard-coded path for the MIPS Cross Compiler enviroment on a RedHat 4!!!
  CFLAGS_INCLUDE =  $(TOOLCHAIN)/usr/lib/gcc/mips-linux/4.0.0/include \
                    $(TOOLCHAIN)/mips_4KC/usr/include \
                    $(TOOLCHAIN)/mips_4KC/usr/include/linux

  CFLAGS_MISC += -nostdinc -fno-builtin
endif

#-------------------------------------------------------------------------------

ifeq "$(TOOLS)" "CODE_SOURCERY"
  #  OS_LNX_VER = 26  # Ver 2.6 of the Linux compiler will invoke code specific to the Linux platform
  #OS_LNX_VER = 26

  LNX_VARIANT = LNX_CODESOURCERY
  OS_TYPE = LINUX

    # More, processor-specific flags later
    TOOLCHAIN = /opt/windriver/wrlinux-tiny/10.17.41/sysroots/x86_64-wrlinuxsdk-linux
    SYSROOT = /opt/windriver/wrlinux-tiny/10.17.41/sysroots/aarch64-wrs-linux
    CROSS_COMPILE = aarch64-wrs-linux-

    CFLAGS_INCLUDE += $(SYSROOT)/usr/include/ \
                      $(TOOLCHAIN)/usr/include/ \
                      $(TOOLCHAIN)/usr/lib/aarch64-wrs-linux/gcc/aarch64-wrs-linux/7.3.0/include/ \
                      ../../../mw/hal/HM/devc/include/ \
                      ../../../../../sys/inc \
                      ../../../../../sys/inc_arch \
                      ../../../../../sys/inc_mw \
                      ../../../../../cmn/com/inc \
                      ../../../../../pf/sys \
                      ../../../../../pf/ft_com/com/inc \
                      ../../../../../mp/server/interface/msg \
                      ../../../../../mp/public 

    LDFLAGS_INCLUDE = -L$(SYSROOT)/lib/ \
                      -L$(SYSROOT)/lib64/ \

  CC = $(TOOLCHAIN)/usr/bin/aarch64-wrs-linux/$(CROSS_COMPILE)gcc
  LD = $(TOOLCHAIN)/usr/bin/aarch64-wrs-linux/$(CROSS_COMPILE)ld

  LDFLAGS = -nostdlib -r

  CFLAGS_MISC += -std=c99
  CFLAGS_DEF += -D__USE_POSIX -DOS_LINUX -DLINUX_3 -DZL_LNX_CODESOURCERY -DPROGRAM_TXT_ONTHEFLY -D_DEFAULT_SOURCE


  CFLAGS_MISC += -nostdinc -fno-builtin

endif

#-------------------------------------------------------------------------------

ifeq "$(TOOLS)" "X86-32"
  OS_LNX_VER = 26

  LNX_VARIANT = LNX_INTEL

  ifeq "$(HOSTNAME)" "rdswlinux1"
        PATH=/bin:/usr/bin:/usr/local/bin

        CROSS = /usr/bin/i386-redhat-linux-
        TOOLCHAIN = /usr/bin
        STRIP = /usr/bin/strip

        LDFLAGS += -nostdlib -m32 -LE
        LDFLAGS += -r -L /lib \
                   -L /usr/lib

        CFLAGS_DEF += -DOLDER_LINUX -DMISSING_SNPRINTF
        OLDER_LINUX = TRUE
  else
        STRIP = $(CROSS)strip
        LDFLAGS += -r -nostdlib -m32 -LE
  endif

  AR    = $(CROSS)ar
  CC    = $(CROSS)gcc
  CXX   = $(CROSS)g++
  LD    = $(CROSS)gcc

  CFLAGS_MISC += -std=c99
  CFLAGS_DEF += -D__USE_POSIX

  CFLAGS_MISC += -m32 -LE

  # Command line options for a Linux build
  # Linux command line defines
  CFLAGS_DEF += -DRW_MULTI_THREAD -D_REENTRANT

  # Defines to support 32-bit compilers
  CFLAGS_DEF += -D_32BIT_COMPILER

  CFLAGS_MISC += -fno-builtin #-fno-for-scope

  CFLAGS_DEF += -DZL_LNX_INTEL

  LDFLAGS_RELOC = -r

  ifndef BOARD
    BOARD = PC
  endif

#  OLDER_LINUX = TRUE    #NO shared object
endif

ifeq "$(TOOLS)" "X86-64"
  OS_LNX_VER = 26

  LNX_VARIANT = LNX_INTEL

  CC = gcc
  LD = gcc

  LDFLAGS_RELOC = -r
  LDFLAGS = -r -nostdlib -m64 -LE

  CFLAGS_MISC += -std=c99
  CFLAGS_DEF += -D__USE_POSIX

  CFLAGS_MISC += -m64 -fPIC 

  # Command line options for a Linux build
  # Linux command line defines
  CFLAGS_DEF += -DRW_MULTI_THREAD -D_REENTRANT

  # Defines to support 64-bit compilers
  CFLAGS_DEF += -D_64BIT_COMPILER

  CFLAGS_MISC += -fno-builtin #-fno-for-scope

  CFLAGS_DEF += -DZL_LNX_INTEL
  
endif

ifeq "$(TOOLS)" "LNX_VXW-69"
  OS_VXW_VER = 69
  WIND_BIN = /opt/WindRiver/gnu/4.3.3-vxworks-6.9/x86-linux2/bin/

  # default settings for building PowerPC target (implies VxWorks OS)
  OS_TYPE = VXWORKS
  CC = $(WIND_BIN)/ccppc
  LD = $(WIND_BIN)/ldppc
  LDFLAGS = -r -X -nostdlib

  # Command line options for a VxWorks build
  # VxWorks command line defines
  CFLAGS_DEF += -DRW_MULTI_THREAD -D_REENTRANT

  # Common VxWorks flags:
  CFLAGS_MISC = -msoft-float -DCPU=PPCEC603 -mcpu=603
  
  # processor specific CFLAGS for a VxWorks build
  ifeq "$(CPU)" "8260"
    CFLAGS_DEF += -DMPC8260  -D___DEV_ZLE30310_BOARD
  endif
  ifeq "$(CPU)" "8313"
    CFLAGS_DEF += -DMPC8313 -D___DEV_ZLE30320_BOARD
  endif
  ifeq "$(CPU)" "8313-360"
    CFLAGS_DEF += -DMPC8313 -D___DEV_ZLE30360_BOARD
  endif
  ifeq "$(CPU)" "8313-1588"
    CFLAGS_DEF += -DMPC8313 -D___DEV_ZLE1588_BOARD
  endif

  # VxWorks/Tornado include paths
  ifeq "$(OS_VXW_VER)" "69"
    WIND_HOME = /opt/WindRiver/
    WIND_BASE = /opt/WindRiver/vxworks-6.9/
    VXW_LIBS = $(WIND_BASE)/vxworks-6.9/target/lib/ppc/PPC32/
    # If compiling for VxWorks 6.9, this will add 6.5-specific compile-time flags and include paths:
    CFLAGS_DEF += -DMPC8260 -DOS_VXWORKS
    CFLAGS_DEF += -D__VXWORKS_69 -D__VXWORKS_65 -DZL_USE_SNARF
    CFLAGS_INCLUDE = $(WIND_BASE)/target/h \
                     $(WIND_BASE)/target/ \
                     $(WIND_BASE)/target/h/drv/intrCtl \
                     $(WIND_BASE)/target/h/drv/sio \
                     $(WIND_BASE)/target/h/arpa \
                     $(WIND_BASE)/target/h/net \
                     $(WIND_BASE)/target/h/wrn/coreip \
                     $(WIND_BASE)/target/usr/h \
                     $(WIND_BASE)/target/usr/h/coreip \
                     $(WIND_HOME)/gnu/4.3.3-vxworks-6.9/lib/gcc/powerpc-wrs-vxworks/4.3.3/include 

    LDFLAGS_INCLUDE = -L$(VXW_LIBS)/ \
                      -L/opt/WindRiver/lmapi-5.0/x86-linux2/lib/

    CFLAGS_MISC += -mstrict-align -nostdinc  -fvolatile -fno-builtin #-fno-for-scope
    CFLAGS_DEF += -D___DEV_USE_VXW_INTERNAL_TIMESTAMP
    CFLAGS_ANSI = -ansi

    # Suppress long-long warnings
    CFLAGS_DEF += -Wno-long-long

  endif
endif

#=============================== CPU OPTIONS ===================================

# Processor-specific CFLAGS for build

# Freescale Section
 #Note: PPC E6500 is in TOOLS=DENX
ifeq "$(CPU)" "860"
# PowerPC 860
  CFLAGS_MISC += -mcpu=860
  CFLAGS_DEF += -DCPU=PPC860 -DMPC860 -DMPC8260
  OBJ_DIR = obj860
endif
ifeq "$(CPU)" "PPC"
# limited PowerPC instruction set
  CFLAGS_MISC += -mcpu=powerpc -msoft-float
  CFLAGS_DEF += -DCPU=PPCEC603 -DMPC8260
  OBJ_DIR = objPPC
endif
ifeq "$(CPU)" "603"
# Default: PowerPC 603 (82xx)
  CFLAGS_MISC += -msoft-float -mcpu=603
  CFLAGS_DEF += -DCPU=PPCEC603 -DMPC8260
  OBJ_DIR = obj603
endif
ifeq "$(CPU)" "603-HF"
# PowerPC 603 (82xx) with "-mhard-float"
  CFLAGS_MISC += -mhard-float -mcpu=603
  CFLAGS_DEF += -DCPU=PPCEC603 -DMPC8260
  OBJ_DIR = obj603-HF
endif
ifeq "$(CPU)" "603-LC"
# PowerPC 603 (82xx) with "-mlongcall"
  CFLAGS_MISC += -msoft-float -mcpu=603 -mlongcall
  CFLAGS_DEF += -DCPU=PPCEC603 -DMPC8260
  OBJ_DIR = obj603-LC
endif
ifeq "$(CPU)" "8260"
  CFLAGS_MISC += -msoft-float -mcpu=603
  CFLAGS_DEF += -DCPU=PPCEC603 -DMPC8260
  OBJ_DIR = obj603
endif
ifeq "$(CPU)" "8313"
  CFLAGS_MISC += -msoft-float -mcpu=603
  CFLAGS_DEF += -DCPU=PPCEC603 -DMPC8313
  OBJ_DIR = obj603
endif


# MIPS Section
ifeq "$(CPU)" "MIPS64"
  CFLAGS_MISC += -EB -mabi=64 -march=mips64 -msoft-float -fno-strict-aliasing
  OBJ_DIR = objMIPS64
  LDFLAGS += --oformat elf64-tradbigmips
endif
ifeq "$(CPU)" "MIPS64-LE"
  CFLAGS_MISC += -EL -mabi=64 -march=mips64 -msoft-float -fno-strict-aliasing
  OBJ_DIR = objMIPS64
  LDFLAGS += --oformat elf64-tradlittlemips
endif
ifeq "$(CPU)" "MIPS32"
  CFLAGS_MISC += -EB -mabi=32 -march=mips32 -msoft-float -fno-strict-aliasing
  OBJ_DIR = objMIPS32
  LDFLAGS += --oformat elf32-tradbigmips
endif
ifeq "$(CPU)" "MIPS32-fPIC"
  CFLAGS_MISC += -mips32 -mabi=32 -march=mips32 -msoft-float -fno-strict-aliasing -fPIC
  OBJ_DIR = objMIPS32-fPIC
  LDFLAGS += --oformat elf32-tradbigmips
endif
ifeq "$(CPU)" "MIPS32-LE"
  CFLAGS_MISC += -EL -mabi=32 -march=mips32 -msoft-float -fno-strict-aliasing
  OBJ_DIR = objMIPS32-LE
  LDFLAGS += --oformat elf32-tradlittlemips
endif
ifeq "$(CPU)" "MIPS32-VTSS"
  # Little Endian, EABI, non-PIC
  CFLAGS_MISC += -EL -mabi=eabi -march=from-abi -mno-abicalls -msoft-float -fno-strict-aliasing 
  OBJ_DIR = objMIPS32-VTSS
  LDFLAGS += --oformat elf32-tradlittlemips
endif


# ARM Section
ifeq "$(CPU)" "202"
  CFLAGS_MISC += -msoft-float -mcpu=arm920 -mbig-endian
  OBJ_DIR = obj
endif
ifeq "$(CPU)" "302"
  CFLAGS_DEF += -DZL_PICO_730X
  CFLAGS_MISC += -msoft-float -mcpu=arm1176jz-s
  OBJ_DIR = objarm
endif
ifeq "$(CPU)" "Cortex-A8-BE"
  CFLAGS_MISC += -msoft-float -mcpu=cortex-a8 -mbig-endian
  OBJ_DIR = objarm
  LDFLAGS += -EB
endif
ifeq "$(CPU)" "Cortex-A8-LE"
  CFLAGS_MISC += -mcpu=cortex-a8 -mlittle-endian
  OBJ_DIR = objarm-LE
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "Cortex-A8-LE-HF"
  CFLAGS_MISC += -mfloat-abi=hard -mfpu=neon -mcpu=cortex-a8 -mlittle-endian
  OBJ_DIR = objarm-LE
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv3-LE"
  CFLAGS_MISC += -mtune=arm3 -mthumb -mlong-calls -mlittle-endian -mfloat-abi=soft -Wl,-meabi=armelf_linux_eabi
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv5-LE"
  CFLAGS_MISC += -msoft-float -march=armv5te -mlittle-endian
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv7-LE"
  CFLAGS_MISC += -msoft-float -march=armv7-a -mlittle-endian
  OBJ_DIR = objARMv7-LE
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv7-LE-HF"
  CFLAGS_MISC += -mhard-float -mfloat-abi=hard -march=armv7-a -mlittle-endian
  OBJ_DIR = objARMv7-LE-HF
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv7-LE-fPIC"
  CFLAGS_MISC += -msoft-float -march=armv7-a -mlittle-endian -fPIC
  OBJ_DIR = objARMv7-LE-fPIC
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv7-BE"
  CFLAGS_MISC += -msoft-float -march=armv7-a -mbig-endian
  OBJ_DIR = objARMv7-BE
  LDFLAGS += -EB
endif
ifeq "$(CPU)" "ARMv8-LE"
  CFLAGS_MISC += -march=armv8-a -mlittle-endian
  OBJ_DIR = objARMv8-LE
  LDFLAGS += -EL
endif
ifeq "$(CPU)" "ARMv8-LE-fPIC"
  CFLAGS_MISC += -march=armv8-a -mlittle-endian -fPIC
  OBJ_DIR = objARMv8-LE-fPIC
  LDFLAGS += -EL
endif

#================================ OTHER FLAGS ==================================

ifeq "$(OS_LNX_VER)" "26"
  # The provided ZL3033x SPI Interrupt code (high and low) use "Opens" or "Signals" to inform the user-space
  #   applications that the intr. fired is an either/or situation in the API, NOT BOTH!
  #   (and must match with the SPI driver (which can provide support for both concurrently)).
  #  The default is to use "Opens"; override with the following flag.
  #CFLAGS_DEF += -DZL_INTR_USES_SIGACTION

  # Level Triggered Interrupt lines may require mask and un-masking (slower due to more SPI I/O)
  #CFLAGS_DEF += -DZL_INTR_MASK_OFF_ON

  # If you have direct access to a high freq. system tick (in user-space) then don't define ZL_NO_DIRECT_ACCESS_TO_SYSTEM_TICKS
  CFLAGS_DEF += -DZL_NO_DIRECT_ACCESS_TO_SYSTEM_TICKS

  # Command line options for a Linux build
  CFLAGS_DEF += -DRW_MULTI_THREAD -D_REENTRANT -DOS_LINUX

  ifneq "$(LNX_VARIANT)" ""
    CFLAGS_DEF += -DZL_$(LNX_VARIANT)
  endif

endif

ifdef LNX_PATH
  CFLAGS_INCLUDE += $(LNX_PATH)
endif


ifdef GMP_DIR
  # include file directories for compiler
  CFLAGS_INCLUDE +=  $(GMP_DIR) $(GMP_DIR)/mpn/generic
  CFLAGS_DEF += -D_ZL_DISABLE_64_BIT_OPERATIONS
  LIB_INT64 = $(GMP_DIR)/gmp.a
else
  # Linker commands to include 64 bit math library
  ifeq "$(OS_VXW_VER)" "54"
    LIB_INT64 = -L$(WIND_BASE)/host/x86-win32/lib/gcc-lib/powerpc-wrs-vxworks/cygnus-2.7.2-960126 -lgcc
  endif

endif

# Set up the tool chains common CFLAGS
CFLAGS = -g -c \
          $(CFLAGS_ANSI) \
          $(CFLAGS_PEDANTIC) \
          $(CFLAGS_OPT) \
          $(sort $(CFLAGS_MISC)) \
          $(sort $(CFLAGS_DEF)) \
          $(sort $(CFLAGS_WARN)) \
          -I./ $(foreach X, $(CFLAGS_INCLUDE), -I$(X))

