
#ifndef _H_y_iana_if_type
#define _H_y_iana_if_type
/*
 * Copyright (c) 2008 - 2012, Andy Bierman, All Rights Reserved.
 * Copyright (c) 2012 - 2019, YumaWorks, Inc., All Rights Reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *

*** Generated by yangdump-sdk 18.10-15

    YumaPro SIL-SA header
    bundle o-ran_all
    module iana-if-type
    revision 2017-01-19
    namespace urn:ietf:params:xml:ns:yang:iana-if-type
    organization IANA
    Created: 2019-12-20T02:20:30Z
    CLI parameters:
        defnames true
        format yh
        indent 4
        module ietf-yang-types
        module ietf-inet-types
        module ietf-interfaces
        module iana-if-type
        module ietf-ip
        module iana-hardware
        module ietf-hardware
        module ietf-netconf-acm
        module o-ran-hardware
        module o-ran-software-management
        module o-ran-interfaces
        module o-ran-transceiver
        module o-ran-mplane-int
        module o-ran-dhcp
        module o-ran-compression-factors
        module o-ran-uplane-conf
        module o-ran-module-cap
        module o-ran-processing-element
        module o-ran-performance-management
        module o-ran-fm
        module o-ran-lbm
        module o-ran-supervision
        module o-ran-beamforming
        module o-ran-operations
        module o-ran-usermgmt
        module o-ran-sync
        sil-bundle o-ran_all
        sil-get2
        sil-sa
        unified true

 */

#include <xmlstring.h>

#include "dlq.h"
#include "ncxtypes.h"
#include "op.h"
#include "status.h"
#include "val.h"

#ifdef __cplusplus
extern "C" {
#endif

#define y_iana_if_type_M_iana_if_type (const xmlChar *)"iana-if-type"

#define y_iana_if_type_R_iana_if_type (const xmlChar *)"2017-01-19"

#define y_iana_if_type_I_iana_interface_type (const xmlChar *)"iana-interface-type"
#define y_iana_if_type_I_other (const xmlChar *)"other"
#define y_iana_if_type_I_regular1822 (const xmlChar *)"regular1822"
#define y_iana_if_type_I_hdh1822 (const xmlChar *)"hdh1822"
#define y_iana_if_type_I_ddnX25 (const xmlChar *)"ddnX25"
#define y_iana_if_type_I_rfc877x25 (const xmlChar *)"rfc877x25"
#define y_iana_if_type_I_ethernetCsmacd (const xmlChar *)"ethernetCsmacd"
#define y_iana_if_type_I_iso88023Csmacd (const xmlChar *)"iso88023Csmacd"
#define y_iana_if_type_I_iso88024TokenBus (const xmlChar *)"iso88024TokenBus"
#define y_iana_if_type_I_iso88025TokenRing (const xmlChar *)"iso88025TokenRing"
#define y_iana_if_type_I_iso88026Man (const xmlChar *)"iso88026Man"
#define y_iana_if_type_I_starLan (const xmlChar *)"starLan"
#define y_iana_if_type_I_proteon10Mbit (const xmlChar *)"proteon10Mbit"
#define y_iana_if_type_I_proteon80Mbit (const xmlChar *)"proteon80Mbit"
#define y_iana_if_type_I_hyperchannel (const xmlChar *)"hyperchannel"
#define y_iana_if_type_I_fddi (const xmlChar *)"fddi"
#define y_iana_if_type_I_lapb (const xmlChar *)"lapb"
#define y_iana_if_type_I_sdlc (const xmlChar *)"sdlc"
#define y_iana_if_type_I_ds1 (const xmlChar *)"ds1"
#define y_iana_if_type_I_e1 (const xmlChar *)"e1"
#define y_iana_if_type_I_basicISDN (const xmlChar *)"basicISDN"
#define y_iana_if_type_I_primaryISDN (const xmlChar *)"primaryISDN"
#define y_iana_if_type_I_propPointToPointSerial (const xmlChar *)"propPointToPointSerial"
#define y_iana_if_type_I_ppp (const xmlChar *)"ppp"
#define y_iana_if_type_I_softwareLoopback (const xmlChar *)"softwareLoopback"
#define y_iana_if_type_I_eon (const xmlChar *)"eon"
#define y_iana_if_type_I_ethernet3Mbit (const xmlChar *)"ethernet3Mbit"
#define y_iana_if_type_I_nsip (const xmlChar *)"nsip"
#define y_iana_if_type_I_slip (const xmlChar *)"slip"
#define y_iana_if_type_I_ultra (const xmlChar *)"ultra"
#define y_iana_if_type_I_ds3 (const xmlChar *)"ds3"
#define y_iana_if_type_I_sip (const xmlChar *)"sip"
#define y_iana_if_type_I_frameRelay (const xmlChar *)"frameRelay"
#define y_iana_if_type_I_rs232 (const xmlChar *)"rs232"
#define y_iana_if_type_I_para (const xmlChar *)"para"
#define y_iana_if_type_I_arcnet (const xmlChar *)"arcnet"
#define y_iana_if_type_I_arcnetPlus (const xmlChar *)"arcnetPlus"
#define y_iana_if_type_I_atm (const xmlChar *)"atm"
#define y_iana_if_type_I_miox25 (const xmlChar *)"miox25"
#define y_iana_if_type_I_sonet (const xmlChar *)"sonet"
#define y_iana_if_type_I_x25ple (const xmlChar *)"x25ple"
#define y_iana_if_type_I_iso88022llc (const xmlChar *)"iso88022llc"
#define y_iana_if_type_I_localTalk (const xmlChar *)"localTalk"
#define y_iana_if_type_I_smdsDxi (const xmlChar *)"smdsDxi"
#define y_iana_if_type_I_frameRelayService (const xmlChar *)"frameRelayService"
#define y_iana_if_type_I_v35 (const xmlChar *)"v35"
#define y_iana_if_type_I_hssi (const xmlChar *)"hssi"
#define y_iana_if_type_I_hippi (const xmlChar *)"hippi"
#define y_iana_if_type_I_modem (const xmlChar *)"modem"
#define y_iana_if_type_I_aal5 (const xmlChar *)"aal5"
#define y_iana_if_type_I_sonetPath (const xmlChar *)"sonetPath"
#define y_iana_if_type_I_sonetVT (const xmlChar *)"sonetVT"
#define y_iana_if_type_I_smdsIcip (const xmlChar *)"smdsIcip"
#define y_iana_if_type_I_propVirtual (const xmlChar *)"propVirtual"
#define y_iana_if_type_I_propMultiplexor (const xmlChar *)"propMultiplexor"
#define y_iana_if_type_I_ieee80212 (const xmlChar *)"ieee80212"
#define y_iana_if_type_I_fibreChannel (const xmlChar *)"fibreChannel"
#define y_iana_if_type_I_hippiInterface (const xmlChar *)"hippiInterface"
#define y_iana_if_type_I_frameRelayInterconnect (const xmlChar *)"frameRelayInterconnect"
#define y_iana_if_type_I_aflane8023 (const xmlChar *)"aflane8023"
#define y_iana_if_type_I_aflane8025 (const xmlChar *)"aflane8025"
#define y_iana_if_type_I_cctEmul (const xmlChar *)"cctEmul"
#define y_iana_if_type_I_fastEther (const xmlChar *)"fastEther"
#define y_iana_if_type_I_isdn (const xmlChar *)"isdn"
#define y_iana_if_type_I_v11 (const xmlChar *)"v11"
#define y_iana_if_type_I_v36 (const xmlChar *)"v36"
#define y_iana_if_type_I_g703at64k (const xmlChar *)"g703at64k"
#define y_iana_if_type_I_g703at2mb (const xmlChar *)"g703at2mb"
#define y_iana_if_type_I_qllc (const xmlChar *)"qllc"
#define y_iana_if_type_I_fastEtherFX (const xmlChar *)"fastEtherFX"
#define y_iana_if_type_I_channel (const xmlChar *)"channel"
#define y_iana_if_type_I_ieee80211 (const xmlChar *)"ieee80211"
#define y_iana_if_type_I_ibm370parChan (const xmlChar *)"ibm370parChan"
#define y_iana_if_type_I_escon (const xmlChar *)"escon"
#define y_iana_if_type_I_dlsw (const xmlChar *)"dlsw"
#define y_iana_if_type_I_isdns (const xmlChar *)"isdns"
#define y_iana_if_type_I_isdnu (const xmlChar *)"isdnu"
#define y_iana_if_type_I_lapd (const xmlChar *)"lapd"
#define y_iana_if_type_I_ipSwitch (const xmlChar *)"ipSwitch"
#define y_iana_if_type_I_rsrb (const xmlChar *)"rsrb"
#define y_iana_if_type_I_atmLogical (const xmlChar *)"atmLogical"
#define y_iana_if_type_I_ds0 (const xmlChar *)"ds0"
#define y_iana_if_type_I_ds0Bundle (const xmlChar *)"ds0Bundle"
#define y_iana_if_type_I_bsc (const xmlChar *)"bsc"
#define y_iana_if_type_I_async (const xmlChar *)"async"
#define y_iana_if_type_I_cnr (const xmlChar *)"cnr"
#define y_iana_if_type_I_iso88025Dtr (const xmlChar *)"iso88025Dtr"
#define y_iana_if_type_I_eplrs (const xmlChar *)"eplrs"
#define y_iana_if_type_I_arap (const xmlChar *)"arap"
#define y_iana_if_type_I_propCnls (const xmlChar *)"propCnls"
#define y_iana_if_type_I_hostPad (const xmlChar *)"hostPad"
#define y_iana_if_type_I_termPad (const xmlChar *)"termPad"
#define y_iana_if_type_I_frameRelayMPI (const xmlChar *)"frameRelayMPI"
#define y_iana_if_type_I_x213 (const xmlChar *)"x213"
#define y_iana_if_type_I_adsl (const xmlChar *)"adsl"
#define y_iana_if_type_I_radsl (const xmlChar *)"radsl"
#define y_iana_if_type_I_sdsl (const xmlChar *)"sdsl"
#define y_iana_if_type_I_vdsl (const xmlChar *)"vdsl"
#define y_iana_if_type_I_iso88025CRFPInt (const xmlChar *)"iso88025CRFPInt"
#define y_iana_if_type_I_myrinet (const xmlChar *)"myrinet"
#define y_iana_if_type_I_voiceEM (const xmlChar *)"voiceEM"
#define y_iana_if_type_I_voiceFXO (const xmlChar *)"voiceFXO"
#define y_iana_if_type_I_voiceFXS (const xmlChar *)"voiceFXS"
#define y_iana_if_type_I_voiceEncap (const xmlChar *)"voiceEncap"
#define y_iana_if_type_I_voiceOverIp (const xmlChar *)"voiceOverIp"
#define y_iana_if_type_I_atmDxi (const xmlChar *)"atmDxi"
#define y_iana_if_type_I_atmFuni (const xmlChar *)"atmFuni"
#define y_iana_if_type_I_atmIma (const xmlChar *)"atmIma"
#define y_iana_if_type_I_pppMultilinkBundle (const xmlChar *)"pppMultilinkBundle"
#define y_iana_if_type_I_ipOverCdlc (const xmlChar *)"ipOverCdlc"
#define y_iana_if_type_I_ipOverClaw (const xmlChar *)"ipOverClaw"
#define y_iana_if_type_I_stackToStack (const xmlChar *)"stackToStack"
#define y_iana_if_type_I_virtualIpAddress (const xmlChar *)"virtualIpAddress"
#define y_iana_if_type_I_mpc (const xmlChar *)"mpc"
#define y_iana_if_type_I_ipOverAtm (const xmlChar *)"ipOverAtm"
#define y_iana_if_type_I_iso88025Fiber (const xmlChar *)"iso88025Fiber"
#define y_iana_if_type_I_tdlc (const xmlChar *)"tdlc"
#define y_iana_if_type_I_gigabitEthernet (const xmlChar *)"gigabitEthernet"
#define y_iana_if_type_I_hdlc (const xmlChar *)"hdlc"
#define y_iana_if_type_I_lapf (const xmlChar *)"lapf"
#define y_iana_if_type_I_v37 (const xmlChar *)"v37"
#define y_iana_if_type_I_x25mlp (const xmlChar *)"x25mlp"
#define y_iana_if_type_I_x25huntGroup (const xmlChar *)"x25huntGroup"
#define y_iana_if_type_I_transpHdlc (const xmlChar *)"transpHdlc"
#define y_iana_if_type_I_interleave (const xmlChar *)"interleave"
#define y_iana_if_type_I_fast (const xmlChar *)"fast"
#define y_iana_if_type_I_ip (const xmlChar *)"ip"
#define y_iana_if_type_I_docsCableMaclayer (const xmlChar *)"docsCableMaclayer"
#define y_iana_if_type_I_docsCableDownstream (const xmlChar *)"docsCableDownstream"
#define y_iana_if_type_I_docsCableUpstream (const xmlChar *)"docsCableUpstream"
#define y_iana_if_type_I_a12MppSwitch (const xmlChar *)"a12MppSwitch"
#define y_iana_if_type_I_tunnel (const xmlChar *)"tunnel"
#define y_iana_if_type_I_coffee (const xmlChar *)"coffee"
#define y_iana_if_type_I_ces (const xmlChar *)"ces"
#define y_iana_if_type_I_atmSubInterface (const xmlChar *)"atmSubInterface"
#define y_iana_if_type_I_l2vlan (const xmlChar *)"l2vlan"
#define y_iana_if_type_I_l3ipvlan (const xmlChar *)"l3ipvlan"
#define y_iana_if_type_I_l3ipxvlan (const xmlChar *)"l3ipxvlan"
#define y_iana_if_type_I_digitalPowerline (const xmlChar *)"digitalPowerline"
#define y_iana_if_type_I_mediaMailOverIp (const xmlChar *)"mediaMailOverIp"
#define y_iana_if_type_I_dtm (const xmlChar *)"dtm"
#define y_iana_if_type_I_dcn (const xmlChar *)"dcn"
#define y_iana_if_type_I_ipForward (const xmlChar *)"ipForward"
#define y_iana_if_type_I_msdsl (const xmlChar *)"msdsl"
#define y_iana_if_type_I_ieee1394 (const xmlChar *)"ieee1394"
#define y_iana_if_type_I_if_gsn (const xmlChar *)"if-gsn"
#define y_iana_if_type_I_dvbRccMacLayer (const xmlChar *)"dvbRccMacLayer"
#define y_iana_if_type_I_dvbRccDownstream (const xmlChar *)"dvbRccDownstream"
#define y_iana_if_type_I_dvbRccUpstream (const xmlChar *)"dvbRccUpstream"
#define y_iana_if_type_I_atmVirtual (const xmlChar *)"atmVirtual"
#define y_iana_if_type_I_mplsTunnel (const xmlChar *)"mplsTunnel"
#define y_iana_if_type_I_srp (const xmlChar *)"srp"
#define y_iana_if_type_I_voiceOverAtm (const xmlChar *)"voiceOverAtm"
#define y_iana_if_type_I_voiceOverFrameRelay (const xmlChar *)"voiceOverFrameRelay"
#define y_iana_if_type_I_idsl (const xmlChar *)"idsl"
#define y_iana_if_type_I_compositeLink (const xmlChar *)"compositeLink"
#define y_iana_if_type_I_ss7SigLink (const xmlChar *)"ss7SigLink"
#define y_iana_if_type_I_propWirelessP2P (const xmlChar *)"propWirelessP2P"
#define y_iana_if_type_I_frForward (const xmlChar *)"frForward"
#define y_iana_if_type_I_rfc1483 (const xmlChar *)"rfc1483"
#define y_iana_if_type_I_usb (const xmlChar *)"usb"
#define y_iana_if_type_I_ieee8023adLag (const xmlChar *)"ieee8023adLag"
#define y_iana_if_type_I_bgppolicyaccounting (const xmlChar *)"bgppolicyaccounting"
#define y_iana_if_type_I_frf16MfrBundle (const xmlChar *)"frf16MfrBundle"
#define y_iana_if_type_I_h323Gatekeeper (const xmlChar *)"h323Gatekeeper"
#define y_iana_if_type_I_h323Proxy (const xmlChar *)"h323Proxy"
#define y_iana_if_type_I_mpls (const xmlChar *)"mpls"
#define y_iana_if_type_I_mfSigLink (const xmlChar *)"mfSigLink"
#define y_iana_if_type_I_hdsl2 (const xmlChar *)"hdsl2"
#define y_iana_if_type_I_shdsl (const xmlChar *)"shdsl"
#define y_iana_if_type_I_ds1FDL (const xmlChar *)"ds1FDL"
#define y_iana_if_type_I_pos (const xmlChar *)"pos"
#define y_iana_if_type_I_dvbAsiIn (const xmlChar *)"dvbAsiIn"
#define y_iana_if_type_I_dvbAsiOut (const xmlChar *)"dvbAsiOut"
#define y_iana_if_type_I_plc (const xmlChar *)"plc"
#define y_iana_if_type_I_nfas (const xmlChar *)"nfas"
#define y_iana_if_type_I_tr008 (const xmlChar *)"tr008"
#define y_iana_if_type_I_gr303RDT (const xmlChar *)"gr303RDT"
#define y_iana_if_type_I_gr303IDT (const xmlChar *)"gr303IDT"
#define y_iana_if_type_I_isup (const xmlChar *)"isup"
#define y_iana_if_type_I_propDocsWirelessMaclayer (const xmlChar *)"propDocsWirelessMaclayer"
#define y_iana_if_type_I_propDocsWirelessDownstream (const xmlChar *)"propDocsWirelessDownstream"
#define y_iana_if_type_I_propDocsWirelessUpstream (const xmlChar *)"propDocsWirelessUpstream"
#define y_iana_if_type_I_hiperlan2 (const xmlChar *)"hiperlan2"
#define y_iana_if_type_I_propBWAp2Mp (const xmlChar *)"propBWAp2Mp"
#define y_iana_if_type_I_sonetOverheadChannel (const xmlChar *)"sonetOverheadChannel"
#define y_iana_if_type_I_digitalWrapperOverheadChannel (const xmlChar *)"digitalWrapperOverheadChannel"
#define y_iana_if_type_I_aal2 (const xmlChar *)"aal2"
#define y_iana_if_type_I_radioMAC (const xmlChar *)"radioMAC"
#define y_iana_if_type_I_atmRadio (const xmlChar *)"atmRadio"
#define y_iana_if_type_I_imt (const xmlChar *)"imt"
#define y_iana_if_type_I_mvl (const xmlChar *)"mvl"
#define y_iana_if_type_I_reachDSL (const xmlChar *)"reachDSL"
#define y_iana_if_type_I_frDlciEndPt (const xmlChar *)"frDlciEndPt"
#define y_iana_if_type_I_atmVciEndPt (const xmlChar *)"atmVciEndPt"
#define y_iana_if_type_I_opticalChannel (const xmlChar *)"opticalChannel"
#define y_iana_if_type_I_opticalTransport (const xmlChar *)"opticalTransport"
#define y_iana_if_type_I_propAtm (const xmlChar *)"propAtm"
#define y_iana_if_type_I_voiceOverCable (const xmlChar *)"voiceOverCable"
#define y_iana_if_type_I_infiniband (const xmlChar *)"infiniband"
#define y_iana_if_type_I_teLink (const xmlChar *)"teLink"
#define y_iana_if_type_I_q2931 (const xmlChar *)"q2931"
#define y_iana_if_type_I_virtualTg (const xmlChar *)"virtualTg"
#define y_iana_if_type_I_sipTg (const xmlChar *)"sipTg"
#define y_iana_if_type_I_sipSig (const xmlChar *)"sipSig"
#define y_iana_if_type_I_docsCableUpstreamChannel (const xmlChar *)"docsCableUpstreamChannel"
#define y_iana_if_type_I_econet (const xmlChar *)"econet"
#define y_iana_if_type_I_pon155 (const xmlChar *)"pon155"
#define y_iana_if_type_I_pon622 (const xmlChar *)"pon622"
#define y_iana_if_type_I_bridge (const xmlChar *)"bridge"
#define y_iana_if_type_I_linegroup (const xmlChar *)"linegroup"
#define y_iana_if_type_I_voiceEMFGD (const xmlChar *)"voiceEMFGD"
#define y_iana_if_type_I_voiceFGDEANA (const xmlChar *)"voiceFGDEANA"
#define y_iana_if_type_I_voiceDID (const xmlChar *)"voiceDID"
#define y_iana_if_type_I_mpegTransport (const xmlChar *)"mpegTransport"
#define y_iana_if_type_I_sixToFour (const xmlChar *)"sixToFour"
#define y_iana_if_type_I_gtp (const xmlChar *)"gtp"
#define y_iana_if_type_I_pdnEtherLoop1 (const xmlChar *)"pdnEtherLoop1"
#define y_iana_if_type_I_pdnEtherLoop2 (const xmlChar *)"pdnEtherLoop2"
#define y_iana_if_type_I_opticalChannelGroup (const xmlChar *)"opticalChannelGroup"
#define y_iana_if_type_I_homepna (const xmlChar *)"homepna"
#define y_iana_if_type_I_gfp (const xmlChar *)"gfp"
#define y_iana_if_type_I_ciscoISLvlan (const xmlChar *)"ciscoISLvlan"
#define y_iana_if_type_I_actelisMetaLOOP (const xmlChar *)"actelisMetaLOOP"
#define y_iana_if_type_I_fcipLink (const xmlChar *)"fcipLink"
#define y_iana_if_type_I_rpr (const xmlChar *)"rpr"
#define y_iana_if_type_I_qam (const xmlChar *)"qam"
#define y_iana_if_type_I_lmp (const xmlChar *)"lmp"
#define y_iana_if_type_I_cblVectaStar (const xmlChar *)"cblVectaStar"
#define y_iana_if_type_I_docsCableMCmtsDownstream (const xmlChar *)"docsCableMCmtsDownstream"
#define y_iana_if_type_I_adsl2 (const xmlChar *)"adsl2"
#define y_iana_if_type_I_macSecControlledIF (const xmlChar *)"macSecControlledIF"
#define y_iana_if_type_I_macSecUncontrolledIF (const xmlChar *)"macSecUncontrolledIF"
#define y_iana_if_type_I_aviciOpticalEther (const xmlChar *)"aviciOpticalEther"
#define y_iana_if_type_I_atmbond (const xmlChar *)"atmbond"
#define y_iana_if_type_I_voiceFGDOS (const xmlChar *)"voiceFGDOS"
#define y_iana_if_type_I_mocaVersion1 (const xmlChar *)"mocaVersion1"
#define y_iana_if_type_I_ieee80216WMAN (const xmlChar *)"ieee80216WMAN"
#define y_iana_if_type_I_adsl2plus (const xmlChar *)"adsl2plus"
#define y_iana_if_type_I_dvbRcsMacLayer (const xmlChar *)"dvbRcsMacLayer"
#define y_iana_if_type_I_dvbTdm (const xmlChar *)"dvbTdm"
#define y_iana_if_type_I_dvbRcsTdma (const xmlChar *)"dvbRcsTdma"
#define y_iana_if_type_I_x86Laps (const xmlChar *)"x86Laps"
#define y_iana_if_type_I_wwanPP (const xmlChar *)"wwanPP"
#define y_iana_if_type_I_wwanPP2 (const xmlChar *)"wwanPP2"
#define y_iana_if_type_I_voiceEBS (const xmlChar *)"voiceEBS"
#define y_iana_if_type_I_ifPwType (const xmlChar *)"ifPwType"
#define y_iana_if_type_I_ilan (const xmlChar *)"ilan"
#define y_iana_if_type_I_pip (const xmlChar *)"pip"
#define y_iana_if_type_I_aluELP (const xmlChar *)"aluELP"
#define y_iana_if_type_I_gpon (const xmlChar *)"gpon"
#define y_iana_if_type_I_vdsl2 (const xmlChar *)"vdsl2"
#define y_iana_if_type_I_capwapDot11Profile (const xmlChar *)"capwapDot11Profile"
#define y_iana_if_type_I_capwapDot11Bss (const xmlChar *)"capwapDot11Bss"
#define y_iana_if_type_I_capwapWtpVirtualRadio (const xmlChar *)"capwapWtpVirtualRadio"
#define y_iana_if_type_I_bits (const xmlChar *)"bits"
#define y_iana_if_type_I_docsCableUpstreamRfPort (const xmlChar *)"docsCableUpstreamRfPort"
#define y_iana_if_type_I_cableDownstreamRfPort (const xmlChar *)"cableDownstreamRfPort"
#define y_iana_if_type_I_vmwareVirtualNic (const xmlChar *)"vmwareVirtualNic"
#define y_iana_if_type_I_ieee802154 (const xmlChar *)"ieee802154"
#define y_iana_if_type_I_otnOdu (const xmlChar *)"otnOdu"
#define y_iana_if_type_I_otnOtu (const xmlChar *)"otnOtu"
#define y_iana_if_type_I_ifVfiType (const xmlChar *)"ifVfiType"
#define y_iana_if_type_I_g9981 (const xmlChar *)"g9981"
#define y_iana_if_type_I_g9982 (const xmlChar *)"g9982"
#define y_iana_if_type_I_g9983 (const xmlChar *)"g9983"
#define y_iana_if_type_I_aluEpon (const xmlChar *)"aluEpon"
#define y_iana_if_type_I_aluEponOnu (const xmlChar *)"aluEponOnu"
#define y_iana_if_type_I_aluEponPhysicalUni (const xmlChar *)"aluEponPhysicalUni"
#define y_iana_if_type_I_aluEponLogicalLink (const xmlChar *)"aluEponLogicalLink"
#define y_iana_if_type_I_aluGponOnu (const xmlChar *)"aluGponOnu"
#define y_iana_if_type_I_aluGponPhysicalUni (const xmlChar *)"aluGponPhysicalUni"
#define y_iana_if_type_I_vmwareNicTeam (const xmlChar *)"vmwareNicTeam"
#define y_iana_if_type_I_docsOfdmDownstream (const xmlChar *)"docsOfdmDownstream"
#define y_iana_if_type_I_docsOfdmaUpstream (const xmlChar *)"docsOfdmaUpstream"
#define y_iana_if_type_I_gfast (const xmlChar *)"gfast"
#define y_iana_if_type_I_sdci (const xmlChar *)"sdci"
#define y_iana_if_type_I_xboxWireless (const xmlChar *)"xboxWireless"
#define y_iana_if_type_I_fastdsl (const xmlChar *)"fastdsl"
#define y_iana_if_type_I_docsCableScte55d1FwdOob (const xmlChar *)"docsCableScte55d1FwdOob"
#define y_iana_if_type_I_docsCableScte55d1RetOob (const xmlChar *)"docsCableScte55d1RetOob"
#define y_iana_if_type_I_docsCableScte55d2DsOob (const xmlChar *)"docsCableScte55d2DsOob"
#define y_iana_if_type_I_docsCableScte55d2UsOob (const xmlChar *)"docsCableScte55d2UsOob"
#define y_iana_if_type_I_docsCableNdf (const xmlChar *)"docsCableNdf"
#define y_iana_if_type_I_docsCableNdr (const xmlChar *)"docsCableNdr"
#define y_iana_if_type_I_ptm (const xmlChar *)"ptm"


/********************************************************************
* FUNCTION y_iana_if_type_init
*
* initialize the iana-if-type server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_iana_if_type_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_iana_if_type_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_iana_if_type_init2 (void);

/********************************************************************
* FUNCTION y_iana_if_type_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_iana_if_type_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
