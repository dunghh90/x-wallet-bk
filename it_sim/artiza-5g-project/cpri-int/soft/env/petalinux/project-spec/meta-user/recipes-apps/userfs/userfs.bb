#
# This file is the userfs recipe.
#

SUMMARY = "Simple userfs application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://DUstartup.sh \
	  "
FILES_${PN} += "/opt"
FILES_${PN} += "/usr/local"
FILES_${PN} += "/usr/local/lib64"
FILES_${PN} += "/etc/init.d"

S = "${WORKDIR}"

do_install() {
	     install -d ${D}/opt
	     install -d ${D}/opt/boot
	     install -d ${D}/opt/debug
	     install -d ${D}/opt/lkm
	     install -d ${D}/opt/lnk
	     install -d ${D}/opt/proc
	     install -d ${D}/opt/tp
	     install -d ${D}/opt/sh
	     install -d ${D}/usr
	     install -d ${D}/usr/local
	     install -d ${D}/usr/local/lib64
	     install -d ${D}/etc/init.d

	     install -m 0755 ${S}/DUstartup.sh ${D}/etc/init.d
	     update-rc.d -r ${D} DUstartup.sh start 90 5 .

}

