SRC_URI_append += " file://platform-top.h \
                    file://platform-auto.h \
                    file://xilinx.patch \
                    file://spi_flash_ids.patch \
                    file://common.patch \
                    file://zynq_gem.patch \
                   "

SRC_URI += "file://bsp.cfg \
           "

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"
