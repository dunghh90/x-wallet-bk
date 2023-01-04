
#ifndef _H_y_ietf_ip
#define _H_y_ietf_ip
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
    module ietf-ip
    revision 2018-02-22
    namespace urn:ietf:params:xml:ns:yang:ietf-ip
    organization IETF NETMOD (Network Modeling) Working Group
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

#define y_ietf_ip_M_ietf_ip (const xmlChar *)"ietf-ip"

#define y_ietf_ip_R_ietf_ip (const xmlChar *)"2018-02-22"

#define y_ietf_ip_N_address (const xmlChar *)"address"
#define y_ietf_ip_N_autoconf (const xmlChar *)"autoconf"
#define y_ietf_ip_N_create_global_addresses (const xmlChar *)"create-global-addresses"
#define y_ietf_ip_N_create_temporary_addresses (const xmlChar *)"create-temporary-addresses"
#define y_ietf_ip_N_dup_addr_detect_transmits (const xmlChar *)"dup-addr-detect-transmits"
#define y_ietf_ip_N_enabled (const xmlChar *)"enabled"
#define y_ietf_ip_N_forwarding (const xmlChar *)"forwarding"
#define y_ietf_ip_N_ip (const xmlChar *)"ip"
#define y_ietf_ip_N_ipv4 (const xmlChar *)"ipv4"
#define y_ietf_ip_N_ipv6 (const xmlChar *)"ipv6"
#define y_ietf_ip_N_is_router (const xmlChar *)"is-router"
#define y_ietf_ip_N_link_layer_address (const xmlChar *)"link-layer-address"
#define y_ietf_ip_N_mtu (const xmlChar *)"mtu"
#define y_ietf_ip_N_neighbor (const xmlChar *)"neighbor"
#define y_ietf_ip_N_netmask (const xmlChar *)"netmask"
#define y_ietf_ip_N_origin (const xmlChar *)"origin"
#define y_ietf_ip_N_prefix_length (const xmlChar *)"prefix-length"
#define y_ietf_ip_N_state (const xmlChar *)"state"
#define y_ietf_ip_N_status (const xmlChar *)"status"
#define y_ietf_ip_N_subnet (const xmlChar *)"subnet"
#define y_ietf_ip_N_temporary_preferred_lifetime (const xmlChar *)"temporary-preferred-lifetime"
#define y_ietf_ip_N_temporary_valid_lifetime (const xmlChar *)"temporary-valid-lifetime"


/********************************************************************
* FUNCTION y_ietf_ip_init
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
extern status_t y_ietf_ip_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_ietf_ip_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_ietf_ip_init2 (void);

/********************************************************************
* FUNCTION y_ietf_ip_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_ietf_ip_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif