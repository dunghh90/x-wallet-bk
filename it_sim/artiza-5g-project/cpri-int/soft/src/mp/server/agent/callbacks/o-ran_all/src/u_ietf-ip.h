
#ifndef _H_u_ietf_ip
#define _H_u_ietf_ip
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

    User SIL-SA header
    bundle o-ran_all
    module ietf-ip
    revision 2018-02-22
    namespace urn:ietf:params:xml:ns:yang:ietf-ip
    organization IETF NETMOD (Network Modeling) Working Group
    Created: 2019-12-20T02:20:29Z
    CLI parameters:
        defnames true
        format uh
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

#include "agt.h"
#include "agt_cb.h"
#include "dlq.h"
#include "ncxtypes.h"
#include "op.h"
#include "rpc.h"
#include "ses.h"
#include "status.h"
#include "val.h"
#include "y_ietf-ip.h"
#include "u_ietf-yang-types.h"
#include "u_ietf-inet-types.h"
#include "u_ietf-interfaces.h"
#include "u_iana-if-type.h"
#include "u_iana-hardware.h"
#include "u_ietf-hardware.h"
#include "u_ietf-netconf-acm.h"
#include "u_o-ran-hardware.h"
#include "u_o-ran-software-management.h"
#include "u_o-ran-interfaces.h"
#include "u_o-ran-transceiver.h"
#include "u_o-ran-mplane-int.h"
#include "u_o-ran-dhcp.h"
#include "u_o-ran-compression-factors.h"
#include "u_o-ran-uplane-conf.h"
#include "u_o-ran-module-cap.h"
#include "u_o-ran-processing-element.h"
#include "u_o-ran-performance-management.h"
#include "u_o-ran-fm.h"
#include "u_o-ran-lbm.h"
#include "u_o-ran-supervision.h"
#include "u_o-ran-beamforming.h"
#include "u_o-ran-operations.h"
#include "u_o-ran-usermgmt.h"
#include "u_o-ran-sync.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Feature ietf-ip:ipv4-non-contiguous-netmasks
 * Comment out to disable */
#define u_ietf_ip_F_ipv4_non_contiguous_netmasks 1

/* Feature ietf-ip:ipv6-privacy-autoconf
 * Comment out to disable */
#define u_ietf_ip_F_ipv6_privacy_autoconf 1



/* case /interfaces/interface/ipv4/address/subnet/prefix-length */
typedef struct y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_prefix_length_ {
    uint8 v_prefix_length;
} y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_prefix_length;

/* case /interfaces/interface/ipv4/address/subnet/netmask */
typedef struct y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_netmask_ {

#ifdef u_ietf_ip_F_ipv4_non_contiguous_netmasks
    xmlChar *v_netmask;
#endif /* u_ietf_ip_F_ipv4_non_contiguous_netmasks */
} y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_netmask;

/* choice /interfaces/interface/ipv4/address/subnet */
typedef union y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_ {
    y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_prefix_length v_prefix_length;
    y_ietf_ip_T_interfaces_interface_ipv4_address_subnet_netmask v_netmask;
} y_ietf_ip_T_interfaces_interface_ipv4_address_subnet;

/* list /interfaces/interface/ipv4/address */
typedef struct y_ietf_ip_T_interfaces_interface_ipv4_address_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    y_ietf_ip_T_interfaces_interface_ipv4_address_subnet v_subnet;
    xmlChar *v_origin;
} y_ietf_ip_T_interfaces_interface_ipv4_address;

/* list /interfaces/interface/ipv4/neighbor */
typedef struct y_ietf_ip_T_interfaces_interface_ipv4_neighbor_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    xmlChar *v_link_layer_address;
    xmlChar *v_origin;
} y_ietf_ip_T_interfaces_interface_ipv4_neighbor;

/* container /interfaces/interface/ipv4 */
typedef struct y_ietf_ip_T_interfaces_interface_ipv4_ {
    boolean v_enabled;
    boolean v_forwarding;
    uint16 v_mtu;
    dlq_hdr_t v_address;
    dlq_hdr_t v_neighbor;
    // y_o_ran_interfaces_T_interfaces_interface_ipv4_diffserv_markings o_ran_interfaces_diffserv_markings;
    uint8 o_ran_interfaces_m_plane_marking;
} y_ietf_ip_T_interfaces_interface_ipv4;

/* list /interfaces/interface/ipv6/address */
typedef struct y_ietf_ip_T_interfaces_interface_ipv6_address_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    uint8 v_prefix_length;
    xmlChar *v_origin;
    xmlChar *v_status;
} y_ietf_ip_T_interfaces_interface_ipv6_address;

/* list /interfaces/interface/ipv6/neighbor */
typedef struct y_ietf_ip_T_interfaces_interface_ipv6_neighbor_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    xmlChar *v_link_layer_address;
    xmlChar *v_origin;
    boolean v_is_router;
    xmlChar *v_state;
} y_ietf_ip_T_interfaces_interface_ipv6_neighbor;

/* container /interfaces/interface/ipv6/autoconf */
typedef struct y_ietf_ip_T_interfaces_interface_ipv6_autoconf_ {
    boolean v_create_global_addresses;

#ifdef u_ietf_ip_F_ipv6_privacy_autoconf
    boolean v_create_temporary_addresses;
#endif /* u_ietf_ip_F_ipv6_privacy_autoconf */

#ifdef u_ietf_ip_F_ipv6_privacy_autoconf
    uint32 v_temporary_valid_lifetime;
#endif /* u_ietf_ip_F_ipv6_privacy_autoconf */

#ifdef u_ietf_ip_F_ipv6_privacy_autoconf
    uint32 v_temporary_preferred_lifetime;
#endif /* u_ietf_ip_F_ipv6_privacy_autoconf */
} y_ietf_ip_T_interfaces_interface_ipv6_autoconf;

/* container /interfaces/interface/ipv6 */
typedef struct y_ietf_ip_T_interfaces_interface_ipv6_ {
    boolean v_enabled;
    boolean v_forwarding;
    uint32 v_mtu;
    dlq_hdr_t v_address;
    dlq_hdr_t v_neighbor;
    uint32 v_dup_addr_detect_transmits;
    y_ietf_ip_T_interfaces_interface_ipv6_autoconf v_autoconf;
    // y_o_ran_interfaces_T_interfaces_interface_ipv6_diffserv_markings o_ran_interfaces_diffserv_markings;
    uint8 o_ran_interfaces_m_plane_marking;
} y_ietf_ip_T_interfaces_interface_ipv6;

/* case /interfaces-state/interface/ipv4/address/subnet/prefix-length */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_prefix_length_ {
    uint8 v_prefix_length;
} y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_prefix_length;

/* case /interfaces-state/interface/ipv4/address/subnet/netmask */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_netmask_ {

#ifdef u_ietf_ip_F_ipv4_non_contiguous_netmasks
    xmlChar *v_netmask;
#endif /* u_ietf_ip_F_ipv4_non_contiguous_netmasks */
} y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_netmask;

/* choice /interfaces-state/interface/ipv4/address/subnet */
typedef union y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_ {
    y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_prefix_length v_prefix_length;
    y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet_netmask v_netmask;
} y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet;

/* list /interfaces-state/interface/ipv4/address */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv4_address_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    y_ietf_ip_T_interfaces_state_interface_ipv4_address_subnet v_subnet;
    xmlChar *v_origin;
} y_ietf_ip_T_interfaces_state_interface_ipv4_address;

/* list /interfaces-state/interface/ipv4/neighbor */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv4_neighbor_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    xmlChar *v_link_layer_address;
    xmlChar *v_origin;
} y_ietf_ip_T_interfaces_state_interface_ipv4_neighbor;

/* container /interfaces-state/interface/ipv4 */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv4_ {
    boolean v_forwarding;
    uint16 v_mtu;
    dlq_hdr_t v_address;
    dlq_hdr_t v_neighbor;
} y_ietf_ip_T_interfaces_state_interface_ipv4;

/* list /interfaces-state/interface/ipv6/address */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv6_address_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    uint8 v_prefix_length;
    xmlChar *v_origin;
    xmlChar *v_status;
} y_ietf_ip_T_interfaces_state_interface_ipv6_address;

/* list /interfaces-state/interface/ipv6/neighbor */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv6_neighbor_ {
    dlq_hdr_t qhdr;
    xmlChar *v_ip;
    xmlChar *v_link_layer_address;
    xmlChar *v_origin;
    boolean v_is_router;
    xmlChar *v_state;
} y_ietf_ip_T_interfaces_state_interface_ipv6_neighbor;

/* container /interfaces-state/interface/ipv6 */
typedef struct y_ietf_ip_T_interfaces_state_interface_ipv6_ {
    boolean v_forwarding;
    uint32 v_mtu;
    dlq_hdr_t v_address;
    dlq_hdr_t v_neighbor;
} y_ietf_ip_T_interfaces_state_interface_ipv6;


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_enabled_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/enabled
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_enabled_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_forwarding_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/forwarding
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_forwarding_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_mtu_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/mtu
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_mtu_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_address_subnet_prefix_length_prefix_length_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/address/subnet/prefix-length/prefix-length
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_address_subnet_prefix_length_prefix_length_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_address_ip);


#ifdef u_ietf_ip_F_ipv4_non_contiguous_netmasks
/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_address_subnet_netmask_netmask_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/address/subnet/netmask/netmask
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_address_subnet_netmask_netmask_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_address_ip);

#endif /* u_ietf_ip_F_ipv4_non_contiguous_netmasks */
/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_address_origin_get
*
* Get database object callback for leaf origin
* Path: /interfaces/interface/ipv4/address/origin
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_address_origin_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_address_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_address_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/address
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_address_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_address_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_neighbor_link_layer_address_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/neighbor/link-layer-address
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_neighbor_link_layer_address_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_neighbor_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_neighbor_origin_get
*
* Get database object callback for leaf origin
* Path: /interfaces/interface/ipv4/neighbor/origin
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_neighbor_origin_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_neighbor_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_neighbor_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4/neighbor
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_neighbor_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv4_neighbor_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv4_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv4
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv4_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_enabled_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/enabled
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_enabled_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_forwarding_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/forwarding
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_forwarding_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_mtu_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/mtu
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_mtu_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_address_prefix_length_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/address/prefix-length
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_address_prefix_length_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_address_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_address_origin_get
*
* Get database object callback for leaf origin
* Path: /interfaces/interface/ipv6/address/origin
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_address_origin_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_address_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_address_status_get
*
* Get database object callback for leaf status
* Path: /interfaces/interface/ipv6/address/status
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_address_status_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_address_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_address_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/address
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_address_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_address_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_neighbor_link_layer_address_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/neighbor/link-layer-address
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_neighbor_link_layer_address_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_neighbor_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_neighbor_origin_get
*
* Get database object callback for leaf origin
* Path: /interfaces/interface/ipv6/neighbor/origin
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_neighbor_origin_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_neighbor_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_neighbor_is_router_get
*
* Get database object callback for leaf is-router
* Path: /interfaces/interface/ipv6/neighbor/is-router
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_neighbor_is_router_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_neighbor_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_neighbor_state_get
*
* Get database object callback for leaf state
* Path: /interfaces/interface/ipv6/neighbor/state
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_neighbor_state_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_neighbor_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_neighbor_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/neighbor
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_neighbor_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name,
    const xmlChar *k_interfaces_interface_ipv6_neighbor_ip);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_dup_addr_detect_transmits_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/dup-addr-detect-transmits
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_dup_addr_detect_transmits_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_autoconf_create_global_addresses_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/autoconf/create-global-addresses
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_autoconf_create_global_addresses_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


#ifdef u_ietf_ip_F_ipv6_privacy_autoconf
/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_autoconf_create_temporary_addresses_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/autoconf/create-temporary-addresses
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_autoconf_create_temporary_addresses_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);

#endif /* u_ietf_ip_F_ipv6_privacy_autoconf */

#ifdef u_ietf_ip_F_ipv6_privacy_autoconf
/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_autoconf_temporary_valid_lifetime_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/autoconf/temporary-valid-lifetime
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_autoconf_temporary_valid_lifetime_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);

#endif /* u_ietf_ip_F_ipv6_privacy_autoconf */

#ifdef u_ietf_ip_F_ipv6_privacy_autoconf
/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_autoconf_temporary_preferred_lifetime_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/autoconf/temporary-preferred-lifetime
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_autoconf_temporary_preferred_lifetime_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);

#endif /* u_ietf_ip_F_ipv6_privacy_autoconf */

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_autoconf_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6/autoconf
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_autoconf_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);


/********************************************************************
* FUNCTION u_ietf_ip_interfaces_interface_ipv6_edit
*
* Edit database object callback
* Path: /interfaces/interface/ipv6
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_interface_ipv6_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_interfaces_interface_name);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv4_address_subnet_get
*
* Get database object callback for choice subnet
* Path: /interfaces-state/interface/ipv4/address/subnet
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv4_address_subnet_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name,
    const xmlChar *k_interfaces_state_interface_ipv4_address_ip);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv4_address_get
*
* Get database object callback for list address
* Path: /interfaces-state/interface/ipv4/address
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv4_address_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name,
    const xmlChar *k_interfaces_state_interface_ipv4_address_ip,
    boolean ip_fixed,
    boolean ip_present);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv4_neighbor_get
*
* Get database object callback for list neighbor
* Path: /interfaces-state/interface/ipv4/neighbor
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv4_neighbor_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name,
    const xmlChar *k_interfaces_state_interface_ipv4_neighbor_ip,
    boolean ip_fixed,
    boolean ip_present);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv4_get
*
* Get database object callback for container ipv4
* Path: /interfaces-state/interface/ipv4
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv4_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv6_address_get
*
* Get database object callback for list address
* Path: /interfaces-state/interface/ipv6/address
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv6_address_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name,
    const xmlChar *k_interfaces_state_interface_ipv6_address_ip,
    boolean ip_fixed,
    boolean ip_present);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv6_neighbor_get
*
* Get database object callback for list neighbor
* Path: /interfaces-state/interface/ipv6/neighbor
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv6_neighbor_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name,
    const xmlChar *k_interfaces_state_interface_ipv6_neighbor_ip,
    boolean ip_fixed,
    boolean ip_present);

/********************************************************************
* FUNCTION u_ietf_ip_interfaces_state_interface_ipv6_get
*
* Get database object callback for container ipv6
* Path: /interfaces-state/interface/ipv6
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_interfaces_state_interface_ipv6_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_interfaces_state_interface_name);

/********************************************************************
* FUNCTION u_ietf_ip_init
*
* initialize the ietf-ip server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION u_ietf_ip_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_ip_init2 (void);

/********************************************************************
* FUNCTION u_ietf_ip_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void u_ietf_ip_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
