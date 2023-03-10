
#ifndef _H_y_o_ran_dhcp
#define _H_y_o_ran_dhcp
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
    module o-ran-dhcp
    revision 2019-02-04
    namespace urn:o-ran:dhcp:1.0
    organization O-RAN Alliance
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

#define y_o_ran_dhcp_M_o_ran_dhcp (const xmlChar *)"o-ran-dhcp"

#define y_o_ran_dhcp_R_o_ran_dhcp (const xmlChar *)"2019-02-04"

#define y_o_ran_dhcp_N_client (const xmlChar *)"client"
#define y_o_ran_dhcp_N_client_id (const xmlChar *)"client-id"
#define y_o_ran_dhcp_N_data (const xmlChar *)"data"
#define y_o_ran_dhcp_N_default_gateways (const xmlChar *)"default-gateways"
#define y_o_ran_dhcp_N_dhcp (const xmlChar *)"dhcp"
#define y_o_ran_dhcp_N_dhcp_client_identifier (const xmlChar *)"dhcp-client-identifier"
#define y_o_ran_dhcp_N_dhcp_server_identifier (const xmlChar *)"dhcp-server-identifier"
#define y_o_ran_dhcp_N_dhcpv4 (const xmlChar *)"dhcpv4"
#define y_o_ran_dhcp_N_dhcpv6 (const xmlChar *)"dhcpv6"
#define y_o_ran_dhcp_N_domain_name (const xmlChar *)"domain-name"
#define y_o_ran_dhcp_N_domain_name_servers (const xmlChar *)"domain-name-servers"
#define y_o_ran_dhcp_N_duid_en (const xmlChar *)"duid-en"
#define y_o_ran_dhcp_N_duid_en_enterprise_number (const xmlChar *)"duid-en-enterprise-number"
#define y_o_ran_dhcp_N_duid_en_identifier (const xmlChar *)"duid-en-identifier"
#define y_o_ran_dhcp_N_duid_ll (const xmlChar *)"duid-ll"
#define y_o_ran_dhcp_N_duid_ll_hardware_type (const xmlChar *)"duid-ll-hardware-type"
#define y_o_ran_dhcp_N_duid_ll_link_layer_addr (const xmlChar *)"duid-ll-link-layer-addr"
#define y_o_ran_dhcp_N_duid_llt (const xmlChar *)"duid-llt"
#define y_o_ran_dhcp_N_duid_llt_hardware_type (const xmlChar *)"duid-llt-hardware-type"
#define y_o_ran_dhcp_N_duid_llt_link_layer_addr (const xmlChar *)"duid-llt-link-layer-addr"
#define y_o_ran_dhcp_N_duid_llt_time (const xmlChar *)"duid-llt-time"
#define y_o_ran_dhcp_N_duid_type (const xmlChar *)"duid-type"
#define y_o_ran_dhcp_N_duid_unknown (const xmlChar *)"duid-unknown"
#define y_o_ran_dhcp_N_duid_uuid (const xmlChar *)"duid-uuid"
#define y_o_ran_dhcp_N_interface (const xmlChar *)"interface"
#define y_o_ran_dhcp_N_interface_mtu (const xmlChar *)"interface-mtu"
#define y_o_ran_dhcp_N_interfaces (const xmlChar *)"interfaces"
#define y_o_ran_dhcp_N_m_plane_dhcp (const xmlChar *)"m-plane-dhcp"
#define y_o_ran_dhcp_N_netconf_clients (const xmlChar *)"netconf-clients"
#define y_o_ran_dhcp_N_optional_port (const xmlChar *)"optional-port"
#define y_o_ran_dhcp_N_private_enterprise_number (const xmlChar *)"private-enterprise-number"
#define y_o_ran_dhcp_N_type_code (const xmlChar *)"type-code"
#define y_o_ran_dhcp_N_uuid (const xmlChar *)"uuid"
#define y_o_ran_dhcp_N_vendor_class_data (const xmlChar *)"vendor-class-data"


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
extern status_t y_o_ran_dhcp_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_o_ran_dhcp_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_dhcp_init2 (void);

/********************************************************************
* FUNCTION y_o_ran_dhcp_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_o_ran_dhcp_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
