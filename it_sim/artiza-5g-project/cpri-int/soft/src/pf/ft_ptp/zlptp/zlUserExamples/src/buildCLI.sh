#!/bin/sh

echo "###################### BUILD Release CLI ###"

echo "ASSUMING a -  603  - build!; If not change APILIB and TOOLCHAIN in this file!"

APILIB=../../obj603/zlApiLib.out
OUTFILE=zl303xx_LinuxCli

if [ "${HOSTNAME}" == "rdswlinux1" ]; then
    CROSS_COMPILE=ppc_6xx-
    TOOLCHAIN=/opt/eldk
else
    CROSS_COMPILE=ppc_6xx-
    TOOLCHAIN=/opt/crosstool/eldk-ppc-4.2
fi

CC=${TOOLCHAIN}/usr/bin/${CROSS_COMPILE}gcc
LD=${TOOLCHAIN}/usr/bin/${CROSS_COMPILE}gcc
AR=${TOOLCHAIN}/usr/bin/${CROSS_COMPILE}ar
LD_LIBPATHS="-L ${TOOLCHAIN}/lib -L ${TOOLCHAIN}/usr/lib/gcc/powerpc-linux/4.2.2/"

cp CompiledSeparately/zl303xx_LinuxCli.c ./

echo "Creating zlApiLib archive"
rm -f zlApiLib.a
${AR} -q zlApiLib.a ${APILIB}
echo "Creating zlApiLib archive...DONE"

echo "Compiling zl303xx_LinuxCli"
${CC} -g -DOS_LINUX -D_ZL303XX_OS_SIGNAL_HANDLER -DZLS30771_INCLUDED -c zl303xx_LinuxCli.c -I${TOOLCHAIN}/usr/include -I../../zlApi/include -I../../zlUserPorting/include  -I../../zlDpll/include -I../../zlApr/include -I../../zlPtp/include -I../../zlUserExamples/include -I../../zlUserUtils/include -o zl303xx_LinuxCli.o
echo "Compiling zl303xx_LinuxCli...DONE"

rm zl303xx_LinuxCli.c

echo "Linking zl303xx_LinuxCli"
${LD} zl303xx_LinuxCli.o zlApiLib.a ${LD_LIBPATHS} -lc -lgcc -lpthread -lrt -o zl303xx_LinuxCli_tmp || exit $?
echo "Linking zl303xx_LinuxCli...DONE"

rm -f zlApiLib.a

cp zl303xx_LinuxCli_tmp ${OUTFILE}
rm -f zl303xx_LinuxCli_tmp
echo "Linux CLI created ${OUTFILE}"

echo "################### DONE ####################"





