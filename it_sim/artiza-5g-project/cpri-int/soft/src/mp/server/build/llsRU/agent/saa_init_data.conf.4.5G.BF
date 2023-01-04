##### Beamforming対応 saa_init_data.conf #####

#行頭の空白文字は無視する。
#行頭に#がある行はコメント行
#空行もコメント行
#リストはparameter.indexの型式(indexは0始まり)
#以下のスタイルで
#
#-----------------------------------
#[モジュールA]
#parameter1=val
#parameter2=val
#parameter3=val
#
#[モジュールB]
#parameter.0=val
#parameter.1=val
#parameter.2=val
#
#-----------------------------------

[ietf-hardware]
hardware.component.0.description=O-RAN-RADIO
hardware.component.1.description=PORT
hardware.component.2.description=PORT
hardware.component.0.physical_index=1
hardware.component.1.physical_index=2
hardware.component.2.physical_index=3

[o-ran-mplane-int]
mplane-info.m-plane-interfaces.m-plane-sub-interfaces0.interface-name=fheth0
mplane-info.m-plane-interfaces.m-plane-sub-interfaces0.client-info.mplane-ipv4-info.0.port=4334
mplane-info.m-plane-interfaces.m-plane-sub-interfaces1.interface-name=fheth1
mplane-info.m-plane-interfaces.m-plane-sub-interfaces1.client-info.mplane-ipv4-info.0.port=4334

[o-ran-operations]
operational-info.declarations.supported-mplane-version=1.0.0
operational-info.declarations.supported-cusplane-version=1.0.0
operational-info.declarations.supported-header-mechanism.protocol=ECPRI
operational-info.declarations.supported-header-mechanism.ecpri-concatenation-support=false
operational-info.declarations.supported-header-mechanism.protocol-version=1.0

[o-ran-sync]
sync.sync-status.supported-reference-types.0.item=PTP
sync.sync-status.supported-reference-types.1.item=SYNCE
sync.sync-capability.sync-t-tsc=CLASS_B
sync.ptp-status.partial-timing-supported=false

[o-ran-dhcp]
dhcp.interfaces.0.dhcpv4.interface-mtu=1500
dhcp.m-plane-dhcp.vendor-class-data=o-ran-ru/FJ

[o-ran-beamforming]
max_ant_num=2
max_band_num=1
beamforming-config.per-band-config.0.band-number=79
beamforming-config.per-band-config.0.tx-array.0=Tx-Array-0
beamforming-config.per-band-config.0.tx-array.1=Tx-Array-1
beamforming-config.per-band-config.0.rx-array.0=Rx-Array-0
beamforming-config.per-band-config.0.rx-array.1=Rx-Array-1
beamforming-config.per-band-config.0.static-properties.rt-bf-weights-update-support=false
beamforming-config.per-band-config.0.static-properties.beamforming-type=other
beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.max-number-of-beam-id=7
beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.initial-beam-id=1
beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.compression-type=STATIC
beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.bitwidth=14
beamforming-config.per-band-config.0.static-properties.beamforming-type.frequency.frequency-domain-beams.compression-format.block-floating-point.exponent=4
beamforming-config.per-band-config.0.static-properties.number-of-beams=7
beamforming-config.per-band-config.0.beam-information.number-of-beamforming-properties=7
beamforming-config.per-band-config.0.beam-information.beamforming-properties.0.beam-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.0.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.0.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.0.beamforming-property.neighbor-beam.0=2
beamforming-config.per-band-config.0.beam-information.beamforming-properties.1.beam-id=2
beamforming-config.per-band-config.0.beam-information.beamforming-properties.1.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.1.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.1.beamforming-property.neighbor-beam.0=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.1.beamforming-property.neighbor-beam.1=3
beamforming-config.per-band-config.0.beam-information.beamforming-properties.2.beam-id=3
beamforming-config.per-band-config.0.beam-information.beamforming-properties.2.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.2.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.2.beamforming-property.neighbor-beam.0=2
beamforming-config.per-band-config.0.beam-information.beamforming-properties.2.beamforming-property.neighbor-beam.1=4
beamforming-config.per-band-config.0.beam-information.beamforming-properties.3.beam-id=4
beamforming-config.per-band-config.0.beam-information.beamforming-properties.3.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.3.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.3.beamforming-property.neighbor-beam.0=3
beamforming-config.per-band-config.0.beam-information.beamforming-properties.3.beamforming-property.neighbor-beam.1=5
beamforming-config.per-band-config.0.beam-information.beamforming-properties.4.beam-id=5
beamforming-config.per-band-config.0.beam-information.beamforming-properties.4.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.4.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.4.beamforming-property.neighbor-beam.0=4
beamforming-config.per-band-config.0.beam-information.beamforming-properties.4.beamforming-property.neighbor-beam.1=6
beamforming-config.per-band-config.0.beam-information.beamforming-properties.5.beam-id=6
beamforming-config.per-band-config.0.beam-information.beamforming-properties.5.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.5.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.5.beamforming-property.neighbor-beam.0=5
beamforming-config.per-band-config.0.beam-information.beamforming-properties.5.beamforming-property.neighbor-beam.1=7
beamforming-config.per-band-config.0.beam-information.beamforming-properties.6.beam-id=7
beamforming-config.per-band-config.0.beam-information.beamforming-properties.6.beamforming-property.beam-type=FINE
beamforming-config.per-band-config.0.beam-information.beamforming-properties.6.beamforming-property.beam-groups-id=1
beamforming-config.per-band-config.0.beam-information.beamforming-properties.6.beamforming-property.neighbor-beam.0=6

[o-ran-hardware]
/hw:hardware/hw:component.label-content.model-name=true
/hw:hardware/hw:component.label-content.serial-number=true

[o-ran-module-cap]
max_scs_a_num_dl=0
max_scs_b_num_dl=0
max_scs_a_num_ul=1
max_scs_b_num_ul=1
max_band_cap_num=1
module-capability.ru-capabilities.ru-supported-category=CAT_A
module-capability.ru-capabilities.number-of-ru-ports=4
module-capability.ru-capabilities.number-of-spatial-streams=4
module-capability.ru-capabilities.max-power-per-pa-antenna=29.9000
module-capability.ru-capabilities.min-power-per-pa-antenna=20.9000
module-capability.ru-capabilities.fronthaul-split-option=7
module-capability.ru-capabilities.format-of-iq-sample.dynamic-compression-supported=false
module-capability.ru-capabilities.format-of-iq-sample.realtime-variable-bit-width-supported=false
module-capability.ru-capabilities.format-of-iq-sample.compression-method-supported.0.compression-method.0.compression-type=STATIC
module-capability.ru-capabilities.format-of-iq-sample.compression-method-supported.0.compression-method.0.bitwidth=14
module-capability.ru-capabilities.format-of-iq-sample.compression-method-supported.0.compression-method.0.compression-format.block-floating-point.exponent=4
module-capability.ru-capabilities.format-of-iq-sample.syminc-supported=false
module-capability.ru-capabilities.ul-mixed-num-required-guard-rbs.0.scs-a=KHZ_30
module-capability.ru-capabilities.ul-mixed-num-required-guard-rbs.0.scs-b=KHZ_30
module-capability.ru-capabilities.ul-mixed-num-required-guard-rbs.0.number-of-guard-rbs-ul=4
module-capability.ru-capabilities.energy-saving-by-transmission-blanks=false
module-capability.ru-capabilities.dynamic-transport-delay-management-supported=false
module-capability.band-capabilities.0.band-number=79
module-capability.band-capabilities.0.max-supported-frequency-dl=4600010000
module-capability.band-capabilities.0.min-supported-frequency-dl=4500010000
module-capability.band-capabilities.0.max-supported-bandwidth-dl=100000000
module-capability.band-capabilities.0.max-num-carriers-dl=1
module-capability.band-capabilities.0.max-carrier-bandwidth-dl=100000000
module-capability.band-capabilities.0.min-carrier-bandwidth-dl=100000000
module-capability.band-capabilities.0.max-supported-frequency-ul=4600010000
module-capability.band-capabilities.0.min-supported-frequency-ul=4500010000
module-capability.band-capabilities.0.max-supported-bandwidth-ul=100000000
module-capability.band-capabilities.0.max-num-carriers-ul=1
module-capability.band-capabilities.0.max-carrier-bandwidth-ul=100000000
module-capability.band-capabilities.0.min-carrier-bandwidth-ul=100000000
module-capability.band-capabilities.0.max-num-component-carriers=1
module-capability.band-capabilities.0.max-num-bands=1
module-capability.band-capabilities.0.max-num-sectors=1
module-capability.band-capabilities.0.max-power-per-antenna=29.9000
module-capability.band-capabilities.0.min-power-per-antenna=20.9000
module-capability.band-capabilities.0.codebook-configuration_ng=1
module-capability.band-capabilities.0.codebook-configuration_n1=2
module-capability.band-capabilities.0.codebook-configuration_n2=1

[o-ran-uplane-conf]
#beamforming_mode 0:BF OFF, 1:BF OFF
beamforming_mode=1
#unit_type 0:sub6, 1:mmW
unit_type=0
max_ant_num=4
max_car_num=1

user-plane-configuration.endpoint-types.0.supported-section-types.section-type=1
user-plane-configuration.endpoint-types.1.supported-section-types.section-type=1
user-plane-configuration.endpoint-types.2.supported-section-types.section-type=3

user-plane-configuration.endpoint-types.0.supported-frame-structures=193
user-plane-configuration.endpoint-types.1.supported-frame-structures=193
user-plane-configuration.endpoint-types.2.supported-frame-structures=129

user-plane-configuration.endpoint-types.0.managed-delay-support=MANAGED
user-plane-configuration.endpoint-types.1.managed-delay-support=MANAGED
user-plane-configuration.endpoint-types.2.managed-delay-support=MANAGED

user-plane-configuration.endpoint-types.0.max-control-sections-per-data-section=2
user-plane-configuration.endpoint-types.1.max-control-sections-per-data-section=1
user-plane-configuration.endpoint-types.2.max-control-sections-per-data-section=1

user-plane-configuration.endpoint-types.0.max-sections-per-symbol=36
user-plane-configuration.endpoint-types.1.max-sections-per-symbol=60
user-plane-configuration.endpoint-types.2.max-sections-per-symbol=4

user-plane-configuration.endpoint-types.0.max-sections-per-slot=98
user-plane-configuration.endpoint-types.1.max-sections-per-slot=160
user-plane-configuration.endpoint-types.2.max-sections-per-slot=4

user-plane-configuration.endpoint-types.0.max-beams-per-symbol=8
user-plane-configuration.endpoint-types.1.max-beams-per-symbol=8
user-plane-configuration.endpoint-types.2.max-beams-per-symbol=4

user-plane-configuration.endpoint-types.0.max-beams-per-slot=8
user-plane-configuration.endpoint-types.1.max-beams-per-slot=8
user-plane-configuration.endpoint-types.2.max-beams-per-slot=4

user-plane-configuration.endpoint-types.0.max-prb-per-symbol=273
user-plane-configuration.endpoint-types.1.max-prb-per-symbol=273
user-plane-configuration.endpoint-types.2.max-prb-per-symbol=48

user-plane-configuration.static-low-level-tx-endpoints.0.name=Low-Level-Tx-Endpoint-0
user-plane-configuration.static-low-level-tx-endpoints.1.name=Low-Level-Tx-Endpoint-1
user-plane-configuration.static-low-level-tx-endpoints.2.name=Low-Level-Tx-Endpoint-2
user-plane-configuration.static-low-level-tx-endpoints.3.name=Low-Level-Tx-Endpoint-3

user-plane-configuration.static-low-level-tx-endpoints.0.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-tx-endpoints.1.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-tx-endpoints.2.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-tx-endpoints.3.restricted-interfaces=fheth0

user-plane-configuration.static-low-level-tx-endpoints.0.array=Tx-Array-0
user-plane-configuration.static-low-level-tx-endpoints.1.array=Tx-Array-0
user-plane-configuration.static-low-level-tx-endpoints.2.array=Tx-Array-1
user-plane-configuration.static-low-level-tx-endpoints.3.array=Tx-Array-1

user-plane-configuration.static-low-level-tx-endpoints.0.endpoint-type=0
user-plane-configuration.static-low-level-tx-endpoints.1.endpoint-type=0
user-plane-configuration.static-low-level-tx-endpoints.2.endpoint-type=0
user-plane-configuration.static-low-level-tx-endpoints.3.endpoint-type=0

user-plane-configuration.static-low-level-rx-endpoints.0.name=Low-Level-Rx-Endpoint-0
user-plane-configuration.static-low-level-rx-endpoints.1.name=Low-Level-Rx-Endpoint-1
user-plane-configuration.static-low-level-rx-endpoints.2.name=Low-Level-Rx-Endpoint-2
user-plane-configuration.static-low-level-rx-endpoints.3.name=Low-Level-Rx-Endpoint-3
user-plane-configuration.static-low-level-rx-endpoints.4.name=Low-Level-Rx-Endpoint-4
user-plane-configuration.static-low-level-rx-endpoints.5.name=Low-Level-Rx-Endpoint-5
user-plane-configuration.static-low-level-rx-endpoints.6.name=Low-Level-Rx-Endpoint-6
user-plane-configuration.static-low-level-rx-endpoints.7.name=Low-Level-Rx-Endpoint-7

user-plane-configuration.static-low-level-rx-endpoints.0.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.1.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.2.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.3.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.4.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.5.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.6.restricted-interfaces=fheth0
user-plane-configuration.static-low-level-rx-endpoints.7.restricted-interfaces=fheth0

user-plane-configuration.static-low-level-rx-endpoints.0.array=Rx-Array-0
user-plane-configuration.static-low-level-rx-endpoints.1.array=Rx-Array-0
user-plane-configuration.static-low-level-rx-endpoints.2.array=Rx-Array-0
user-plane-configuration.static-low-level-rx-endpoints.3.array=Rx-Array-0
user-plane-configuration.static-low-level-rx-endpoints.4.array=Rx-Array-1
user-plane-configuration.static-low-level-rx-endpoints.5.array=Rx-Array-1
user-plane-configuration.static-low-level-rx-endpoints.6.array=Rx-Array-1
user-plane-configuration.static-low-level-rx-endpoints.7.array=Rx-Array-1

user-plane-configuration.static-low-level-rx-endpoints.0.endpoint-type=1
user-plane-configuration.static-low-level-rx-endpoints.1.endpoint-type=1
user-plane-configuration.static-low-level-rx-endpoints.2.endpoint-type=2
user-plane-configuration.static-low-level-rx-endpoints.3.endpoint-type=2
user-plane-configuration.static-low-level-rx-endpoints.4.endpoint-type=1
user-plane-configuration.static-low-level-rx-endpoints.5.endpoint-type=1
user-plane-configuration.static-low-level-rx-endpoints.6.endpoint-type=2
user-plane-configuration.static-low-level-rx-endpoints.7.endpoint-type=2

user-plane-configuration.tx-array-carriers.0.type=NR
user-plane-configuration.tx-array-carriers.0.duplex-scheme=TDD

user-plane-configuration.rx-array-carriers.0.type=NR
user-plane-configuration.rx-array-carriers.0.duplex-scheme=TDD

user-plane-configuration.tx-arrays.0.name=Tx-Array-0
user-plane-configuration.tx-arrays.1.name=Tx-Array-1

user-plane-configuration.tx-arrays.0.number-of-rows=1
user-plane-configuration.tx-arrays.1.number-of-rows=1

user-plane-configuration.tx-arrays.0.number-of-columns=2
user-plane-configuration.tx-arrays.1.number-of-columns=2

user-plane-configuration.tx-arrays.0.number-of-array-layers=2
user-plane-configuration.tx-arrays.1.number-of-array-layers=2

user-plane-configuration.tx-arrays.0.polarisations.0.p=1
user-plane-configuration.tx-arrays.1.polarisations.0.p=1

user-plane-configuration.tx-arrays.0.polarisations.0.polarisation=MINUS_45
user-plane-configuration.tx-arrays.1.polarisations.0.polarisation=PLUS_45

user-plane-configuration.tx-arrays.0.band-number=79
user-plane-configuration.tx-arrays.1.band-number=79

user-plane-configuration.tx-arrays.0.max-gain=32.9
user-plane-configuration.tx-arrays.1.max-gain=32.9

user-plane-configuration.tx-arrays.0.independent-power-budget=true
user-plane-configuration.tx-arrays.1.independent-power-budget=true

user-plane-configuration.rx-arrays.0.name=Rx-Array-0
user-plane-configuration.rx-arrays.1.name=Rx-Array-1

user-plane-configuration.rx-arrays.0.number-of-rows=1
user-plane-configuration.rx-arrays.1.number-of-rows=1

user-plane-configuration.rx-arrays.0.number-of-columns=2
user-plane-configuration.rx-arrays.1.number-of-columns=2

user-plane-configuration.rx-arrays.0.number-of-array-layers=2
user-plane-configuration.rx-arrays.1.number-of-array-layers=2

user-plane-configuration.rx-arrays.0.polarisations.0.p=1
user-plane-configuration.rx-arrays.1.polarisations.0.p=1

user-plane-configuration.rx-arrays.0.polarisations.0.polarisation=MINUS_45
user-plane-configuration.rx-arrays.1.polarisations.0.polarisation=PLUS_45

user-plane-configuration.rx-arrays.0.band-number=79
user-plane-configuration.rx-arrays.1.band-number=79

user-plane-configuration.rx-arrays.0.gain-correction-range.max=0
user-plane-configuration.rx-arrays.1.gain-correction-range.max=0

user-plane-configuration.rx-arrays.0.gain-correction-range.min=0
user-plane-configuration.rx-arrays.1.gain-correction-range.min=0

[ietf-ip]
ietf-ip.ipv4.address.origin=dhcp

[o-ran-delay-management]
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.t2a-min-up=134000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.t2a-max-up=345000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.t2a-min-cp-dl=259000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.t2a-max-cp-dl=470000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.tcp-adv-dl=125000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.ta3-min=50000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.ta3-max=171000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.t2a-min-cp-ul=125000
bandwidth-scs-delay-state.100000.30000.ru-delay-profile.t2a-max-cp-ul=336000

[o-ran-fm]
fm_llsdu=45GLLA-SDU
fm_port0=fheth0
fm_port1=fheth1
fm_fan1=FAN-1
fm_fan2=FAN-2
fm_fan3=FAN-3
fm_external=External-Devices

[o-ran-fan]
fan-tray.fan-state.0.name=FAN-1
fan-tray.fan-state.1.name=FAN-2
fan-tray.fan-state.2.name=FAN-3
