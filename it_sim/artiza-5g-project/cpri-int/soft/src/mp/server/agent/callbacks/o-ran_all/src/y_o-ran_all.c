
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

    YumaPro SIL-SA bundle module
    bundle o-ran_all
    SIL-SA bundle wrapper:
        ietf-yang-types@2013-07-15
        ietf-inet-types@2013-07-15
        ietf-interfaces@2018-02-20
        iana-if-type@2017-01-19
        ietf-ip@2018-02-22
        iana-hardware@2018-03-13
        ietf-hardware@2018-03-13
        ietf-netconf-acm@2018-02-14
        o-ran-hardware@2019-02-04
        o-ran-software-management@2019-02-04
        o-ran-interfaces@2019-02-04
        o-ran-transceiver@2019-02-04
        o-ran-mplane-int@2019-02-04
        o-ran-dhcp@2019-02-04
        o-ran-compression-factors@2019-02-04
        o-ran-uplane-conf@2019-02-04
        o-ran-module-cap@2019-02-04
        o-ran-processing-element@2019-02-04
        o-ran-performance-management@2019-02-04
        o-ran-fm@2019-02-04
        o-ran-lbm@2019-02-04
        o-ran-supervision@2019-02-04
        o-ran-beamforming@2019-04-01
        o-ran-operations@2019-02-04
        o-ran-usermgmt@2019-02-04
        o-ran-sync@2019-02-04
    Created: 2019-12-20T02:20:31Z
    CLI parameters:
        defnames true
        format bc
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
#include "y_o-ran_all.h"
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

/********************************************************************
* FUNCTION y_o_ran_all_init
*
* initialize the o-ran_all server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
status_t y_o_ran_all_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;

    /* change if custom handling done */
    if (xml_strcmp(modname, y_o_ran_all_M_o_ran_all)) {
        return ERR_NCX_UNKNOWN_MODULE;
    }

    res = y_ietf_yang_types_init(y_ietf_yang_types_M_ietf_yang_types, y_ietf_yang_types_R_ietf_yang_types);
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_inet_types_init(y_ietf_inet_types_M_ietf_inet_types, y_ietf_inet_types_R_ietf_inet_types);
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_interfaces_init(y_ietf_interfaces_M_ietf_interfaces, y_ietf_interfaces_R_ietf_interfaces);
    if (res != NO_ERR) {
        return res;
    }

    res = y_iana_if_type_init(y_iana_if_type_M_iana_if_type, y_iana_if_type_R_iana_if_type);
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_ip_init(y_ietf_ip_M_ietf_ip, y_ietf_ip_R_ietf_ip);
    if (res != NO_ERR) {
        return res;
    }

    res = y_iana_hardware_init(y_iana_hardware_M_iana_hardware, y_iana_hardware_R_iana_hardware);
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_hardware_init(y_ietf_hardware_M_ietf_hardware, y_ietf_hardware_R_ietf_hardware);
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_netconf_acm_init(y_ietf_netconf_acm_M_ietf_netconf_acm, y_ietf_netconf_acm_R_ietf_netconf_acm);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_hardware_init(y_o_ran_hardware_M_o_ran_hardware, y_o_ran_hardware_R_o_ran_hardware);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_software_management_init(y_o_ran_software_management_M_o_ran_software_management, y_o_ran_software_management_R_o_ran_software_management);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_interfaces_init(y_o_ran_interfaces_M_o_ran_interfaces, y_o_ran_interfaces_R_o_ran_interfaces);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_transceiver_init(y_o_ran_transceiver_M_o_ran_transceiver, y_o_ran_transceiver_R_o_ran_transceiver);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_mplane_int_init(y_o_ran_mplane_int_M_o_ran_mplane_int, y_o_ran_mplane_int_R_o_ran_mplane_int);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_dhcp_init(y_o_ran_dhcp_M_o_ran_dhcp, y_o_ran_dhcp_R_o_ran_dhcp);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_compression_factors_init(y_o_ran_compression_factors_M_o_ran_compression_factors, y_o_ran_compression_factors_R_o_ran_compression_factors);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_uplane_conf_init(y_o_ran_uplane_conf_M_o_ran_uplane_conf, y_o_ran_uplane_conf_R_o_ran_uplane_conf);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_module_cap_init(y_o_ran_module_cap_M_o_ran_module_cap, y_o_ran_module_cap_R_o_ran_module_cap);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_processing_element_init(y_o_ran_processing_element_M_o_ran_processing_element, y_o_ran_processing_element_R_o_ran_processing_element);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_performance_management_init(y_o_ran_performance_management_M_o_ran_performance_management, y_o_ran_performance_management_R_o_ran_performance_management);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_fm_init(y_o_ran_fm_M_o_ran_fm, y_o_ran_fm_R_o_ran_fm);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_lbm_init(y_o_ran_lbm_M_o_ran_lbm, y_o_ran_lbm_R_o_ran_lbm);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_supervision_init(y_o_ran_supervision_M_o_ran_supervision, y_o_ran_supervision_R_o_ran_supervision);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_beamforming_init(y_o_ran_beamforming_M_o_ran_beamforming, y_o_ran_beamforming_R_o_ran_beamforming);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_operations_init(y_o_ran_operations_M_o_ran_operations, y_o_ran_operations_R_o_ran_operations);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_usermgmt_init(y_o_ran_usermgmt_M_o_ran_usermgmt, y_o_ran_usermgmt_R_o_ran_usermgmt);
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_sync_init(y_o_ran_sync_M_o_ran_sync, y_o_ran_sync_R_o_ran_sync);
    if (res != NO_ERR) {
        return res;
    }

    return res;

} /* y_o_ran_all_init */

/********************************************************************
* FUNCTION y_o_ran_all_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
status_t y_o_ran_all_init2 (void)
{
    status_t res = NO_ERR;

    res = y_ietf_yang_types_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_inet_types_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_interfaces_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_iana_if_type_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_ip_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_iana_hardware_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_hardware_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_ietf_netconf_acm_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_hardware_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_software_management_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_interfaces_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_transceiver_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_mplane_int_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_dhcp_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_compression_factors_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_uplane_conf_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_module_cap_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_processing_element_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_performance_management_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_fm_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_lbm_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_supervision_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_beamforming_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_operations_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_usermgmt_init2();
    if (res != NO_ERR) {
        return res;
    }

    res = y_o_ran_sync_init2();
    if (res != NO_ERR) {
        return res;
    }

    return res;

} /* y_o_ran_all_init2 */

/********************************************************************
* FUNCTION y_o_ran_all_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
void y_o_ran_all_cleanup (void)
{

    y_ietf_yang_types_cleanup();
    y_ietf_inet_types_cleanup();
    y_ietf_interfaces_cleanup();
    y_iana_if_type_cleanup();
    y_ietf_ip_cleanup();
    y_iana_hardware_cleanup();
    y_ietf_hardware_cleanup();
    y_ietf_netconf_acm_cleanup();
    y_o_ran_hardware_cleanup();
    y_o_ran_software_management_cleanup();
    y_o_ran_interfaces_cleanup();
    y_o_ran_transceiver_cleanup();
    y_o_ran_mplane_int_cleanup();
    y_o_ran_dhcp_cleanup();
    y_o_ran_compression_factors_cleanup();
    y_o_ran_uplane_conf_cleanup();
    y_o_ran_module_cap_cleanup();
    y_o_ran_processing_element_cleanup();
    y_o_ran_performance_management_cleanup();
    y_o_ran_fm_cleanup();
    y_o_ran_lbm_cleanup();
    y_o_ran_supervision_cleanup();
    y_o_ran_beamforming_cleanup();
    y_o_ran_operations_cleanup();
    y_o_ran_usermgmt_cleanup();
    y_o_ran_sync_cleanup();

} /* y_o_ran_all_cleanup */

/* END SIL-SA y_o_ran_all.c */
