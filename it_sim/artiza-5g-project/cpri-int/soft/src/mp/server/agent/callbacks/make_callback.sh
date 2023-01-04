#!/bin/sh

echo "Starting make callback"
make_sil_sa_dir --split o-ran-externalio --sil-get2
make_sil_sa_dir --split o-ran-file-management --sil-get2
make_sil_sa_dir --split o-ran-troubleshooting --sil-get2
make_sil_sa_dir --split o-ran-delay-management --sil-get2
make_sil_sa_dir --split o-ran-fan --sil-get2
make_sil_sa_bundle o-ran_all ietf-yang-types ietf-inet-types ietf-interfaces iana-if-type ietf-ip iana-hardware ietf-hardware ietf-netconf-acm o-ran-hardware o-ran-software-management o-ran-interfaces o-ran-transceiver o-ran-mplane-int o-ran-dhcp o-ran-compression-factors o-ran-uplane-conf o-ran-module-cap o-ran-processing-element o-ran-performance-management o-ran-fm o-ran-lbm o-ran-supervision o-ran-beamforming o-ran-operations o-ran-usermgmt o-ran-sync --sil-get2
