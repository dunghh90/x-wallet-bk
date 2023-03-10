
#ifndef _H_u_o_ran_fan
#define _H_u_o_ran_fan
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
    module o-ran-fan
    revision 2019-02-04
    namespace urn:o-ran:fan:1.0
    organization O-RAN Alliance
    Created: 2019-12-20T02:20:29Z
    CLI parameters:
        format uh
        indent 4
        module o-ran-fan
        output u_o-ran-fan.h
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
#include "y_o-ran-fan.h"

#ifdef __cplusplus
extern "C" {
#endif

/* list /fan-tray/fan-state */
typedef struct y_o_ran_fan_T_fan_tray_fan_state_ {
    dlq_hdr_t qhdr;
    xmlChar *v_name;
    uint8 v_fan_location;
    boolean v_present_and_operating;
    uint8 v_vendor_code;
    uint16 v_fan_speed;
    uint16 v_target_speed;
} y_o_ran_fan_T_fan_tray_fan_state;

/* container /fan-tray */
typedef struct y_o_ran_fan_T_fan_tray_ {
    dlq_hdr_t v_fan_state;
} y_o_ran_fan_T_fan_tray;

/********************************************************************
* FUNCTION u_o_ran_fan_fan_tray_fan_state_get
*
* Get database object callback for list fan-state
* Path: /fan-tray/fan-state
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_fan_fan_tray_fan_state_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_fan_tray_fan_state_name,
    boolean name_fixed,
    boolean name_present);

/********************************************************************
* FUNCTION u_o_ran_fan_fan_tray_get
*
* Get database object callback for container fan-tray
* Path: /fan-tray
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_fan_fan_tray_get (
    getcb_get2_t *get2cb);

/********************************************************************
* FUNCTION u_o_ran_fan_init
*
* initialize the o-ran-fan server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_fan_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION u_o_ran_fan_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_fan_init2 (void);

/********************************************************************
* FUNCTION u_o_ran_fan_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void u_o_ran_fan_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
