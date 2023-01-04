DESCRIPTION = "PETALINUX image definition for Xilinx boards"
LICENSE = "MIT"

require recipes-core/images/petalinux-image-common.inc 

inherit extrausers
IMAGE_LINGUAS = " "

IMAGE_INSTALL = "\
		kernel-modules \
		busybox \
		procps \
		strace \
		python \
		libxml2 \
		ncurses \
		ncurses-terminfo \
		libpcap \
		libstdc++ \
		gdb \
		glibc \
		net-tools \
		packagegroup-core-boot \
		bridge-utils \
		tcpdump \
		userfs \
		dhcp-client \
		vlan \
		vsftpd \
		"
EXTRA_USERS_PARAMS = "usermod -P root root;"
