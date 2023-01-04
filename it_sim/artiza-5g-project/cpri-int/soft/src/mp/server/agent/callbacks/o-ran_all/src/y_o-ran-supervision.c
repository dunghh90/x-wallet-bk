
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
    module o-ran-supervision
    revision 2019-02-04
    namespace urn:o-ran:supervision:1.0
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
#include "agt_not.h"
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
#include "ses.h"
#include "sil_sa.h"
#include "status.h"
#include "val.h"
#include "val_child.h"
#include "val_util.h"
#include "xml_util.h"
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
static ncx_module_t *o_ran_supervision_mod;
static obj_template_t *supervision_watchdog_reset_obj;

/********************************************************************
* FUNCTION y_o_ran_supervision_init_static_vars
*
* initialize module static variables
*
********************************************************************/
static void y_o_ran_supervision_init_static_vars (void)
{
    o_ran_supervision_mod = NULL;
    supervision_watchdog_reset_obj = NULL;

} /* y_o_ran_supervision_init_static_vars */

/********************************************************************
* FUNCTION y_o_ran_supervision_init
*
* initialize the o-ran-supervision server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
status_t y_o_ran_supervision_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;

    y_o_ran_supervision_init_static_vars();

    /* change if custom handling done */
    if (xml_strcmp(modname, y_o_ran_supervision_M_o_ran_supervision)) {
        return ERR_NCX_UNKNOWN_MODULE;
    }

    if (revision && xml_strcmp(revision, y_o_ran_supervision_R_o_ran_supervision)) {
        return ERR_NCX_WRONG_VERSION;
    }


    res = ncxmod_load_module(
        y_o_ran_supervision_M_o_ran_supervision,
        y_o_ran_supervision_R_o_ran_supervision,
        agt_get_savedevQ(),
        &o_ran_supervision_mod);
    if (res != NO_ERR) {
        return res;
    }

    supervision_watchdog_reset_obj = ncx_find_object(
        o_ran_supervision_mod,
        y_o_ran_supervision_N_supervision_watchdog_reset);
    if (supervision_watchdog_reset_obj == NULL) {
        return ERR_NCX_DEF_NOT_FOUND;
    }

    res = agt_rpc_register_method(
        y_o_ran_supervision_M_o_ran_supervision,
        y_o_ran_supervision_N_supervision_watchdog_reset,
        AGT_RPC_PH_VALIDATE,
        u_o_ran_supervision_supervision_watchdog_reset_validate);
    if (res != NO_ERR) {
        return res;
    }

    res = agt_rpc_register_method(
        y_o_ran_supervision_M_o_ran_supervision,
        y_o_ran_supervision_N_supervision_watchdog_reset,
        AGT_RPC_PH_INVOKE,
        u_o_ran_supervision_supervision_watchdog_reset_invoke);
    if (res != NO_ERR) {
        return res;
    }

    res = u_o_ran_supervision_init(modname, revision);
    return res;

} /* y_o_ran_supervision_init */

/********************************************************************
* FUNCTION y_o_ran_supervision_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
status_t y_o_ran_supervision_init2 (void)
{
    status_t res = NO_ERR;

    res = u_o_ran_supervision_init2();

    return res;

} /* y_o_ran_supervision_init2 */

/********************************************************************
* FUNCTION y_o_ran_supervision_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
void y_o_ran_supervision_cleanup (void)
{

    agt_rpc_unregister_method(
        y_o_ran_supervision_M_o_ran_supervision,
        y_o_ran_supervision_N_supervision_watchdog_reset);

    u_o_ran_supervision_cleanup();

} /* y_o_ran_supervision_cleanup */

/* END SIL-SA y_o_ran_supervision.c */