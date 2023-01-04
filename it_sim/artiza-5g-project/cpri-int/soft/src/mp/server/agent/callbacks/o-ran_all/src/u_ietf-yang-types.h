
#ifndef _H_u_ietf_yang_types
#define _H_u_ietf_yang_types
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
    module ietf-yang-types
    revision 2013-07-15
    namespace urn:ietf:params:xml:ns:yang:ietf-yang-types
    organization IETF NETMOD (NETCONF Data Modeling Language) Working Group
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
#include "y_ietf-yang-types.h"
#include "u_ietf-inet-types.h"
#include "u_ietf-interfaces.h"
#include "u_iana-if-type.h"
#include "u_ietf-ip.h"
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

/********************************************************************
* FUNCTION u_ietf_yang_types_init
*
* initialize the ietf-yang-types server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_yang_types_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION u_ietf_yang_types_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_ietf_yang_types_init2 (void);

/********************************************************************
* FUNCTION u_ietf_yang_types_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void u_ietf_yang_types_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
