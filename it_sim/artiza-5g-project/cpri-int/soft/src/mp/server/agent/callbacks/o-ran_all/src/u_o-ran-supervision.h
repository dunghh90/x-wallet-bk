
#ifndef _H_u_o_ran_supervision
#define _H_u_o_ran_supervision
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
    module o-ran-supervision
    revision 2019-02-04
    namespace urn:o-ran:supervision:1.0
    organization O-RAN Alliance
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
#include "agt_not.h"
#include "agt_rpc.h"
#include "dlq.h"
#include "ncxtypes.h"
#include "op.h"
#include "rpc.h"
#include "ses.h"
#include "status.h"
#include "val.h"
#include "y_o-ran-supervision.h"
#include "u_ietf-yang-types.h"
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
#include "u_o-ran-beamforming.h"
#include "u_o-ran-operations.h"
#include "u_o-ran-usermgmt.h"
#include "u_o-ran-sync.h"

#ifdef __cplusplus
extern "C" {
#endif

/* container /supervision-watchdog-reset/input */
typedef struct y_o_ran_supervision_T_supervision_watchdog_reset_input_ {
    uint16 v_supervision_notification_interval;
    uint16 v_guard_timer_overhead;
} y_o_ran_supervision_T_supervision_watchdog_reset_input;

/* container /supervision-watchdog-reset/output */
typedef struct y_o_ran_supervision_T_supervision_watchdog_reset_output_ {
    xmlChar *v_next_update_at;
} y_o_ran_supervision_T_supervision_watchdog_reset_output;

/* rpc /supervision-watchdog-reset */
typedef struct y_o_ran_supervision_T_supervision_watchdog_reset_ {
    y_o_ran_supervision_T_supervision_watchdog_reset_input v_input;
    y_o_ran_supervision_T_supervision_watchdog_reset_output v_output;
} y_o_ran_supervision_T_supervision_watchdog_reset;

/* notification /supervision-notification */
typedef struct y_o_ran_supervision_T_supervision_notification_ {
} y_o_ran_supervision_T_supervision_notification;


/********************************************************************
* FUNCTION u_o_ran_supervision_supervision_watchdog_reset_validate
*
* RPC validation phase
* All YANG constraints have passed at this point.
* Add description-stmt checks in this function.
*
* INPUTS:
*     see agt/agt_rpc.h for details
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_supervision_supervision_watchdog_reset_validate (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    xml_node_t *methnode);


/********************************************************************
* FUNCTION u_o_ran_supervision_supervision_watchdog_reset_invoke
*
* RPC invocation phase
* All constraints have passed at this point.
* Call device instrumentation code in this function.
*
* INPUTS:
*     see agt/agt_rpc.h for details
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_supervision_supervision_watchdog_reset_invoke (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    xml_node_t *methnode);


/********************************************************************
* FUNCTION u_o_ran_supervision_supervision_notification_send
*
* Send a u_o_ran_supervision_supervision_notification notification
* Called by your code when notification event occurs
*
********************************************************************/
extern void u_o_ran_supervision_supervision_notification_send (void);

/********************************************************************
* FUNCTION u_o_ran_supervision_init
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
extern status_t u_o_ran_supervision_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION u_o_ran_supervision_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_supervision_init2 (void);

/********************************************************************
* FUNCTION u_o_ran_supervision_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void u_o_ran_supervision_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
