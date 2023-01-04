
#ifndef _H_y_o_ran_interfaces
#define _H_y_o_ran_interfaces
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
    module o-ran-interfaces
    revision 2019-02-04
    namespace urn:o-ran:interfaces:1.0
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

#define y_o_ran_interfaces_M_o_ran_interfaces (const xmlChar *)"o-ran-interfaces"

#define y_o_ran_interfaces_R_o_ran_interfaces (const xmlChar *)"2019-02-04"

#define y_o_ran_interfaces_N_alias_macs (const xmlChar *)"alias-macs"
#define y_o_ran_interfaces_N_base_interface (const xmlChar *)"base-interface"
#define y_o_ran_interfaces_N_c_plane_marking (const xmlChar *)"c-plane-marking"
#define y_o_ran_interfaces_N_class_of_service (const xmlChar *)"class-of-service"
#define y_o_ran_interfaces_N_diffserv_markings (const xmlChar *)"diffserv-markings"
#define y_o_ran_interfaces_N_enhanced_marking (const xmlChar *)"enhanced-marking"
#define y_o_ran_interfaces_N_enhanced_uplane_markings (const xmlChar *)"enhanced-uplane-markings"
#define y_o_ran_interfaces_N_l2_mtu (const xmlChar *)"l2-mtu"
#define y_o_ran_interfaces_N_last_cleared (const xmlChar *)"last-cleared"
#define y_o_ran_interfaces_N_m_plane_marking (const xmlChar *)"m-plane-marking"
#define y_o_ran_interfaces_N_mac_address (const xmlChar *)"mac-address"
#define y_o_ran_interfaces_N_other_marking (const xmlChar *)"other-marking"
#define y_o_ran_interfaces_N_port_name (const xmlChar *)"port-name"
#define y_o_ran_interfaces_N_port_number (const xmlChar *)"port-number"
#define y_o_ran_interfaces_N_port_reference (const xmlChar *)"port-reference"
#define y_o_ran_interfaces_N_reset_interface_counters (const xmlChar *)"reset-interface-counters"
#define y_o_ran_interfaces_N_s_plane_marking (const xmlChar *)"s-plane-marking"
#define y_o_ran_interfaces_N_u_plane_marking (const xmlChar *)"u-plane-marking"
#define y_o_ran_interfaces_N_up_marking_name (const xmlChar *)"up-marking-name"
#define y_o_ran_interfaces_N_vlan_id (const xmlChar *)"vlan-id"
#define y_o_ran_interfaces_N_vlan_tagging (const xmlChar *)"vlan-tagging"


/********************************************************************
* FUNCTION y_o_ran_interfaces_init
*
* initialize the o-ran-interfaces server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_interfaces_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_o_ran_interfaces_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_interfaces_init2 (void);

/********************************************************************
* FUNCTION y_o_ran_interfaces_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_o_ran_interfaces_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif