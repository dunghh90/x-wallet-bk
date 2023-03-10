
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

    YumaPro SIL-SA module
    bundle o-ran_all
    module o-ran-dhcp
    revision 2019-02-04
    namespace urn:o-ran:dhcp:1.0
    organization O-RAN Alliance
    Created: 2019-12-20T02:20:31Z
    CLI parameters:
        defnames true
        format yc
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

#include "procdefs.h"
#include "agt.h"
#include "agt_action.h"
#include "agt_cb.h"
#include "agt_rpc.h"
#include "agt_sil_lib.h"
#include "agt_timer.h"
#include "agt_util.h"
#include "agt_val.h"
#include "cfg.h"
#include "dlq.h"
#include "getcb.h"
#include "ncx.h"
#include "ncx_feature.h"
#include "ncxmod.h"
#include "ncxtypes.h"
#include "obj.h"
#include "rpc.h"
#include "sil_sa.h"
#include "status.h"
#include "val.h"
#include "val_child.h"
#include "val_util.h"
#include "xml_val.h"

/* SIL bundle: o-ran_all */
#include "y_ietf-yang-types.h"
#include "u_ietf-yang-types.h"
#include "y_ietf-inet-types.h"
#include "u_ietf-inet-types.h"
#include "y_ietf-interfaces.h"
#include "u_ietf-interfaces.h"
#include "y_iana-if-type.h"
#include "u_iana-if-type.h"
#include "y_ietf-ip.h"
#include "u_ietf-ip.h"
#include "y_iana-hardware.h"
#include "u_iana-hardware.h"
#include "y_ietf-hardware.h"
#include "u_ietf-hardware.h"
#include "y_ietf-netconf-acm.h"
#include "u_ietf-netconf-acm.h"
#include "y_o-ran-hardware.h"
#include "u_o-ran-hardware.h"
#include "y_o-ran-software-management.h"
#include "u_o-ran-software-management.h"
#include "y_o-ran-interfaces.h"
#include "u_o-ran-interfaces.h"
#include "y_o-ran-transceiver.h"
#include "u_o-ran-transceiver.h"
#include "y_o-ran-mplane-int.h"
#include "u_o-ran-mplane-int.h"
#include "y_o-ran-dhcp.h"
#include "u_o-ran-dhcp.h"
#include "y_o-ran-compression-factors.h"
#include "u_o-ran-compression-factors.h"
#include "y_o-ran-uplane-conf.h"
#include "u_o-ran-uplane-conf.h"
#include "y_o-ran-module-cap.h"
#include "u_o-ran-module-cap.h"
#include "y_o-ran-processing-element.h"
#include "u_o-ran-processing-element.h"
#include "y_o-ran-performance-management.h"
#include "u_o-ran-performance-management.h"
#include "y_o-ran-fm.h"
#include "u_o-ran-fm.h"
#include "y_o-ran-lbm.h"
#include "u_o-ran-lbm.h"
#include "y_o-ran-supervision.h"
#include "u_o-ran-supervision.h"
#include "y_o-ran-beamforming.h"
#include "u_o-ran-beamforming.h"
#include "y_o-ran-operations.h"
#include "u_o-ran-operations.h"
#include "y_o-ran-usermgmt.h"
#include "u_o-ran-usermgmt.h"
#include "y_o-ran-sync.h"
#include "u_o-ran-sync.h"

/* module static variables */
static ncx_module_t *o_ran_dhcp_mod;
static obj_template_t *dhcp_obj;

/********************************************************************
* FUNCTION y_o_ran_dhcp_init_static_vars
*
* initialize module static variables
*
********************************************************************/
static void y_o_ran_dhcp_init_static_vars (void)
{
    o_ran_dhcp_mod = NULL;
    dhcp_obj = NULL;

} /* y_o_ran_dhcp_init_static_vars */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv4_netconf_clients_get
*
* Get database object callback for list netconf-clients
* Path: /dhcp/interfaces/dhcpv4/netconf-clients
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv4_netconf_clients_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    /* local key o-ran-dhcp:client */
    val_value_t *k_dhcp_interfaces_dhcpv4_netconf_clients_client = 0;
    boolean client_fixed = FALSE;
    boolean client_present = FALSE;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_client);
    if (keyval) {
        k_dhcp_interfaces_dhcpv4_netconf_clients_client = keyval;
        client_fixed = VAL_IS_FIXED_VALUE(keyval);
        client_present = TRUE;
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv4_netconf_clients_get(
        get2cb,
        k_dhcp_interfaces_interface,
        k_dhcp_interfaces_dhcpv4_netconf_clients_client,
        client_fixed,
        client_present);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv4_netconf_clients_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv4_get
*
* Get database object callback for container dhcpv4
* Path: /dhcp/interfaces/dhcpv4
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv4_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv4_get(
        get2cb,
        k_dhcp_interfaces_interface);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv4_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_duid_type_get
*
* Get database object callback for choice duid-type
* Path: /dhcp/interfaces/dhcpv6/dhcp-client-identifier/duid-type
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_duid_type_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_duid_type_get(
        get2cb,
        k_dhcp_interfaces_interface);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_duid_type_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_get
*
* Get database object callback for container dhcp-client-identifier
* Path: /dhcp/interfaces/dhcpv6/dhcp-client-identifier
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_get(
        get2cb,
        k_dhcp_interfaces_interface);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_duid_type_get
*
* Get database object callback for choice duid-type
* Path: /dhcp/interfaces/dhcpv6/dhcp-server-identifier/duid-type
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_duid_type_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_duid_type_get(
        get2cb,
        k_dhcp_interfaces_interface);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_duid_type_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_get
*
* Get database object callback for container dhcp-server-identifier
* Path: /dhcp/interfaces/dhcpv6/dhcp-server-identifier
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_get(
        get2cb,
        k_dhcp_interfaces_interface);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv6_netconf_clients_get
*
* Get database object callback for list netconf-clients
* Path: /dhcp/interfaces/dhcpv6/netconf-clients
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv6_netconf_clients_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    /* local key o-ran-dhcp:client */
    val_value_t *k_dhcp_interfaces_dhcpv6_netconf_clients_client = 0;
    boolean client_fixed = FALSE;
    boolean client_present = FALSE;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_client);
    if (keyval) {
        k_dhcp_interfaces_dhcpv6_netconf_clients_client = keyval;
        client_fixed = VAL_IS_FIXED_VALUE(keyval);
        client_present = TRUE;
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv6_netconf_clients_get(
        get2cb,
        k_dhcp_interfaces_interface,
        k_dhcp_interfaces_dhcpv6_netconf_clients_client,
        client_fixed,
        client_present);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv6_netconf_clients_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_dhcpv6_get
*
* Get database object callback for container dhcpv6
* Path: /dhcp/interfaces/dhcpv6
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_dhcpv6_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* ancestor key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
    }

    return u_o_ran_dhcp_dhcp_interfaces_dhcpv6_get(
        get2cb,
        k_dhcp_interfaces_interface);

} /* o_ran_dhcp_dhcp_interfaces_dhcpv6_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_interfaces_get
*
* Get database object callback for list interfaces
* Path: /dhcp/interfaces
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_interfaces_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    val_value_t *keyval = NULL;

    /* local key o-ran-dhcp:interface */
    const xmlChar *k_dhcp_interfaces_interface = 0;
    boolean interface_fixed = FALSE;
    boolean interface_present = FALSE;
    keyval = getcb_find_key_str(get2cb,
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_N_interface);
    if (keyval) {
        k_dhcp_interfaces_interface = VAL_STRING(keyval);
        interface_fixed = VAL_IS_FIXED_VALUE(keyval);
        interface_present = TRUE;
    }

    return u_o_ran_dhcp_dhcp_interfaces_get(
        get2cb,
        k_dhcp_interfaces_interface,
        interface_fixed,
        interface_present);

} /* o_ran_dhcp_dhcp_interfaces_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_m_plane_dhcp_get
*
* Get database object callback for container m-plane-dhcp
* Path: /dhcp/m-plane-dhcp
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_m_plane_dhcp_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    return u_o_ran_dhcp_dhcp_m_plane_dhcp_get(get2cb);

} /* o_ran_dhcp_dhcp_m_plane_dhcp_get */

/********************************************************************
* FUNCTION o_ran_dhcp_dhcp_get
*
* Get database object callback for container dhcp
* Path: /dhcp
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
static status_t o_ran_dhcp_dhcp_get (
    ses_cb_t *scb,
    xml_msg_hdr_t *msg,
    getcb_get2_t *get2cb)
{

    (void)scb;
    (void)msg;
    return u_o_ran_dhcp_dhcp_get(get2cb);

} /* o_ran_dhcp_dhcp_get */

/********************************************************************
* FUNCTION y_o_ran_dhcp_init
*
* initialize the o-ran-dhcp server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
status_t y_o_ran_dhcp_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;

    y_o_ran_dhcp_init_static_vars();

    /* change if custom handling done */
    if (xml_strcmp(modname, y_o_ran_dhcp_M_o_ran_dhcp)) {
        return ERR_NCX_UNKNOWN_MODULE;
    }

    if (revision && xml_strcmp(revision, y_o_ran_dhcp_R_o_ran_dhcp)) {
        return ERR_NCX_WRONG_VERSION;
    }


    res = ncxmod_load_module(
        y_o_ran_dhcp_M_o_ran_dhcp,
        y_o_ran_dhcp_R_o_ran_dhcp,
        agt_get_savedevQ(),
        &o_ran_dhcp_mod);
    if (res != NO_ERR) {
        return res;
    }

    dhcp_obj = ncx_find_object(
        o_ran_dhcp_mod,
        y_o_ran_dhcp_N_dhcp);
    if (dhcp_obj == NULL) {
        return ERR_NCX_DEF_NOT_FOUND;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv4",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv4_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv4/o-ran-dhcp:netconf-clients",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv4_netconf_clients_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv6_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-client-identifier",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-client-identifier/o-ran-dhcp:duid-type",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_client_identifier_duid_type_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-server-identifier",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-server-identifier/o-ran-dhcp:duid-type",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv6_dhcp_server_identifier_duid_type_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:netconf-clients",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_interfaces_dhcpv6_netconf_clients_get);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_cb_register_get_callback(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:m-plane-dhcp",
        y_o_ran_dhcp_R_o_ran_dhcp,
        o_ran_dhcp_dhcp_m_plane_dhcp_get);
    if (res != NO_ERR) {
        return res;
    }

    res = u_o_ran_dhcp_init(modname, revision);
    return res;

} /* y_o_ran_dhcp_init */

/********************************************************************
* FUNCTION y_o_ran_dhcp_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
status_t y_o_ran_dhcp_init2 (void)
{
    status_t res = NO_ERR;

    res = u_o_ran_dhcp_init2();

    return res;

} /* y_o_ran_dhcp_init2 */

/********************************************************************
* FUNCTION y_o_ran_dhcp_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
void y_o_ran_dhcp_cleanup (void)
{

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv4");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv4/o-ran-dhcp:netconf-clients");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-client-identifier");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-client-identifier/o-ran-dhcp:duid-type");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-server-identifier");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:dhcp-server-identifier/o-ran-dhcp:duid-type");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:interfaces/o-ran-dhcp:dhcpv6/o-ran-dhcp:netconf-clients");

    agt_cb_unregister_callbacks(
        y_o_ran_dhcp_M_o_ran_dhcp,
        (const xmlChar *)"/o-ran-dhcp:dhcp/o-ran-dhcp:m-plane-dhcp");

    u_o_ran_dhcp_cleanup();

} /* y_o_ran_dhcp_cleanup */

/* END SIL-SA y_o_ran_dhcp.c */
