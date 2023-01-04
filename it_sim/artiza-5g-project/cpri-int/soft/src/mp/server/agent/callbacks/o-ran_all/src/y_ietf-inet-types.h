
#ifndef _H_y_ietf_inet_types
#define _H_y_ietf_inet_types
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
    module ietf-inet-types
    revision 2013-07-15
    namespace urn:ietf:params:xml:ns:yang:ietf-inet-types
    organization IETF NETMOD (NETCONF Data Modeling Language) Working Group
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

#define y_ietf_inet_types_M_ietf_inet_types (const xmlChar *)"ietf-inet-types"

#define y_ietf_inet_types_R_ietf_inet_types (const xmlChar *)"2013-07-15"


/********************************************************************
* FUNCTION y_ietf_inet_types_init
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
extern status_t y_ietf_inet_types_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_ietf_inet_types_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_ietf_inet_types_init2 (void);

/********************************************************************
* FUNCTION y_ietf_inet_types_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_ietf_inet_types_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
