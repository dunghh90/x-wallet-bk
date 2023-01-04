
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

*** Generated by yangdump-sdk 18.10-9

    User SIL-SA module
    bundle o-ran_all
    module ietf-inet-types
    revision 2013-07-15
    namespace urn:ietf:params:xml:ns:yang:ietf-inet-types
    organization IETF NETMOD (NETCONF Data Modeling Language) Working Group
    Created: 2019-06-21T02:49:14Z
    CLI parameters:
        defnames true
        format uc
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


/* put your static variables here */

/********************************************************************
* FUNCTION u_ietf_inet_types_init
*
* initialize the ietf-inet-types server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
status_t u_ietf_inet_types_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;
    ncx_module_t *ietf_inet_types_mod = NULL;

    ietf_inet_types_mod = ncx_find_module(modname, revision);
    if (ietf_inet_types_mod == NULL) {
        return ERR_NCX_OPERATION_FAILED;
    }

    /* put your module initialization code here */
    
    return res;

} /* u_ietf_inet_types_init */

/********************************************************************
* FUNCTION u_ietf_inet_types_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
status_t u_ietf_inet_types_init2 (void)
{
    status_t res = NO_ERR;

    /* put your init2 code here */

    return res;

} /* u_ietf_inet_types_init2 */

/********************************************************************
* FUNCTION u_ietf_inet_types_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
void u_ietf_inet_types_cleanup (void)
{

    /* put your cleanup code here */
    
} /* u_ietf_inet_types_cleanup */

/* END SIL-SA u_ietf_inet_types.c */