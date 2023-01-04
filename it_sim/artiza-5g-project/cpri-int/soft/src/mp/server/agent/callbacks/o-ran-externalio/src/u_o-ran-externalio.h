
#ifndef _H_u_o_ran_externalio
#define _H_u_o_ran_externalio
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
    module o-ran-externalio
    revision 2019-02-04
    namespace urn:o-ran:external-io:1.0
    organization O-RAN Alliance
    Created: 2019-12-20T02:20:27Z
    CLI parameters:
        format uh
        indent 4
        module o-ran-externalio
        output u_o-ran-externalio.h
        sil-get2
        sil-sa
        unified true

 */

#include <xmlstring.h>

#include "agt.h"
#include "agt_cb.h"
#include "agt_not.h"
#include "dlq.h"
#include "ncxtypes.h"
#include "op.h"
#include "rpc.h"
#include "ses.h"
#include "status.h"
#include "val.h"
#include "y_o-ran-externalio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* list /external-io/input */
typedef struct y_o_ran_externalio_T_external_io_input_ {
    dlq_hdr_t qhdr;
    xmlChar *v_name;
    uint8 v_port_in;
    boolean v_line_in;
} y_o_ran_externalio_T_external_io_input;

/* list /external-io/output */
typedef struct y_o_ran_externalio_T_external_io_output_ {
    dlq_hdr_t qhdr;
    xmlChar *v_name;
    uint8 v_port_out;
} y_o_ran_externalio_T_external_io_output;

/* list /external-io/output-setting */
typedef struct y_o_ran_externalio_T_external_io_output_setting_ {
    dlq_hdr_t qhdr;
    xmlChar *v_name;
    boolean v_line_out;
} y_o_ran_externalio_T_external_io_output_setting;

/* container /external-io */
typedef struct y_o_ran_externalio_T_external_io_ {
    dlq_hdr_t v_input;
    dlq_hdr_t v_output;
    dlq_hdr_t v_output_setting;
} y_o_ran_externalio_T_external_io;

/* list /external-input-change/current-input-notification/external-input */
typedef struct y_o_ran_externalio_T_external_input_change_current_input_notification_external_input_ {
    dlq_hdr_t qhdr;
    xmlChar *v_name;
    xmlChar *v_io_port;
    xmlChar *v_line_in;
} y_o_ran_externalio_T_external_input_change_current_input_notification_external_input;

/* container /external-input-change/current-input-notification */
typedef struct y_o_ran_externalio_T_external_input_change_current_input_notification_ {
    dlq_hdr_t v_external_input;
} y_o_ran_externalio_T_external_input_change_current_input_notification;

/* notification /external-input-change */
typedef struct y_o_ran_externalio_T_external_input_change_ {
    y_o_ran_externalio_T_external_input_change_current_input_notification v_current_input_notification;
} y_o_ran_externalio_T_external_input_change;

/********************************************************************
* FUNCTION u_o_ran_externalio_external_io_input_get
*
* Get database object callback for list input
* Path: /external-io/input
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_external_io_input_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_external_io_input_name,
    boolean name_fixed,
    boolean name_present);

/********************************************************************
* FUNCTION u_o_ran_externalio_external_io_output_get
*
* Get database object callback for list output
* Path: /external-io/output
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_external_io_output_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_external_io_output_name,
    boolean name_fixed,
    boolean name_present);


/********************************************************************
* FUNCTION u_o_ran_externalio_external_io_output_setting_line_out_edit
*
* Edit database object callback
* Path: /external-io/output-setting/line-out
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_external_io_output_setting_line_out_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_external_io_output_setting_name);


/********************************************************************
* FUNCTION u_o_ran_externalio_external_io_output_setting_edit
*
* Edit database object callback
* Path: /external-io/output-setting
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_external_io_output_setting_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_external_io_output_setting_name);


/********************************************************************
* FUNCTION u_o_ran_externalio_external_io_edit
*
* Edit database object callback
* Path: /external-io
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_external_io_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval);


/********************************************************************
* FUNCTION u_o_ran_externalio_external_input_change_send
*
* Send a u_o_ran_externalio_external_input_change notification
* Called by your code when notification event occurs
*
********************************************************************/
extern void u_o_ran_externalio_external_input_change_send (
    y_o_ran_externalio_T_external_input_change_current_input_notification *v_current_input_notification);

/********************************************************************
* FUNCTION u_o_ran_externalio_init
*
* initialize the o-ran-externalio server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION u_o_ran_externalio_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_externalio_init2 (void);

/********************************************************************
* FUNCTION u_o_ran_externalio_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void u_o_ran_externalio_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif