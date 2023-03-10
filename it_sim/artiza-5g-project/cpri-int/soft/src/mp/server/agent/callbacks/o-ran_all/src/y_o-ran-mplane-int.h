
#ifndef _H_y_o_ran_mplane_int
#define _H_y_o_ran_mplane_int
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
    module o-ran-mplane-int
    revision 2019-02-04
    namespace urn:o-ran:mplane-interfaces:1.0
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

#define y_o_ran_mplane_int_M_o_ran_mplane_int (const xmlChar *)"o-ran-mplane-int"

#define y_o_ran_mplane_int_R_o_ran_mplane_int (const xmlChar *)"2019-02-04"

#define y_o_ran_mplane_int_N_call_home_ssh_port (const xmlChar *)"call-home-ssh-port"
#define y_o_ran_mplane_int_N_client_info (const xmlChar *)"client-info"
#define y_o_ran_mplane_int_N_highest_vlan_id (const xmlChar *)"highest-vlan-id"
#define y_o_ran_mplane_int_N_interface_name (const xmlChar *)"interface-name"
#define y_o_ran_mplane_int_N_lowest_vlan_id (const xmlChar *)"lowest-vlan-id"
#define y_o_ran_mplane_int_N_m_plane_interfaces (const xmlChar *)"m-plane-interfaces"
#define y_o_ran_mplane_int_N_m_plane_ssh_ports (const xmlChar *)"m-plane-ssh-ports"
#define y_o_ran_mplane_int_N_m_plane_sub_interfaces (const xmlChar *)"m-plane-sub-interfaces"
#define y_o_ran_mplane_int_N_mplane_fqdn (const xmlChar *)"mplane-fqdn"
#define y_o_ran_mplane_int_N_mplane_info (const xmlChar *)"mplane-info"
#define y_o_ran_mplane_int_N_mplane_ipv4 (const xmlChar *)"mplane-ipv4"
#define y_o_ran_mplane_int_N_mplane_ipv4_info (const xmlChar *)"mplane-ipv4-info"
#define y_o_ran_mplane_int_N_mplane_ipv6 (const xmlChar *)"mplane-ipv6"
#define y_o_ran_mplane_int_N_mplane_ipv6_info (const xmlChar *)"mplane-ipv6-info"
#define y_o_ran_mplane_int_N_port (const xmlChar *)"port"
#define y_o_ran_mplane_int_N_searchable_access_vlans (const xmlChar *)"searchable-access-vlans"
#define y_o_ran_mplane_int_N_searchable_mplane_access_vlans_info (const xmlChar *)"searchable-mplane-access-vlans-info"
#define y_o_ran_mplane_int_N_server_ssh_port (const xmlChar *)"server-ssh-port"
#define y_o_ran_mplane_int_N_sub_interface (const xmlChar *)"sub-interface"
#define y_o_ran_mplane_int_N_vlan_range (const xmlChar *)"vlan-range"


/********************************************************************
* FUNCTION y_o_ran_mplane_int_init
*
* initialize the o-ran-mplane-int server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_mplane_int_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_o_ran_mplane_int_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_mplane_int_init2 (void);

/********************************************************************
* FUNCTION y_o_ran_mplane_int_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_o_ran_mplane_int_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
