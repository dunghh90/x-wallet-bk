SRC_URI += "file://bsp.cfg \
            "
SRC_URI_append += "file://m25p80.patch \
                   file://spi-nor.patch \
                   file://spi-cadence.patch \
                  "


FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
