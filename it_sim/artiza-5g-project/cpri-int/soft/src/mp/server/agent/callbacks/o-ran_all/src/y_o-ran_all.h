
#ifndef _H_y_o_ran_all
#define _H_y_o_ran_all
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

    YumaPro SIL-SA bundle header
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
        format bh
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

#define y_o_ran_all_M_o_ran_all (const xmlChar *)"o-ran_all"


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
extern status_t y_o_ran_all_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_o_ran_all_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_all_init2 (void);

/********************************************************************
* FUNCTION y_o_ran_all_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_o_ran_all_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
