
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
    module o-ran-externalio
    revision 2019-02-04
    namespace urn:o-ran:external-io:1.0
    organization O-RAN Alliance
    Created: 2019-06-21T02:49:12Z
    CLI parameters:
        format uc
        indent 4
        module o-ran-externalio
        output u_o-ran-externalio.c
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
#include "u_o-ran-externalio.h"
#include "y_o-ran-externalio.h"
#include "i_o-ran-externalio.h"
#include "val_child.h"

static obj_template_t *external_input_change_obj;

/* put your static variables here */

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
status_t u_o_ran_externalio_external_io_input_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_external_io_input_name,
    boolean name_fixed,
    boolean name_present)
{
    boolean getnext = FALSE;

    (void)getnext; /* remove yumaworks warning */

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
        log_info("\n    present=%d ,fixed=%d", name_present, name_fixed);
        log_info("\n    key_name=%s", k_external_io_input_name);
    }

    /* check the callback mode type */
    getcb_mode_t cbmode = GETCB_GET2_CBMODE(get2cb);
    switch (cbmode) {
    case GETCB_GET_VALUE:
        break;
    case GETCB_GETNEXT_VALUE:
        getnext = TRUE;
        break;
    default:
        return SET_ERROR(ERR_INTERNAL_VAL);
    }

    obj_template_t *obj = GETCB_GET2_OBJ(get2cb);
    status_t res = NO_ERR;

    uint32 max_entries = GETCB_GET2_MAX_ENTRIES(get2cb);

    (void)max_entries; /* remove yumaworks warning */

    /* For GET, find the entry that matches the key values
     * For GETNEXT, find the entry that matches the next key value
     * If the 'present' flag is false then return first key instance
     * If the 'fixed' flag is true then no GETNEXT advance for the key
     * Create a new return key val_value_t, then getcb_add_return_key */

    T_MPSW_MSI_O_RAN_EXTERNALIO_GET_CFM  get_data;
    boolean line_in = 0;

    /* MSI_O_RAN_EXTERNALIO_GET_REQ送信 */
    res = i_o_ran_externalio_send_msi_get(&get_data);
    if(res != NO_ERR)
    {
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed to get param via MSI", __func__);
        }
        return res;
    }

    res = i_o_ran_externalio_conv_line_state_to_bool(get_data.line_in, &line_in);
    if(res != NO_ERR)
    {
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed to convert line-in", __func__);
        }
        return res;
    }


    /* if we are here, then the index is valid */
    obj_template_t *name_obj = obj_find_child(
        obj,
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_name);
    if(name_obj == NULL) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: name_obj is NULL", __func__);
        }
        return ERR_NCX_NO_INSTANCE;
    }

    val_value_t *retval = agt_make_leaf2(name_obj,
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_name,
        i_o_ran_externalio_N_input_name,
        &res);
    if (retval) {
        if (name_fixed) {
            VAL_SET_FIXED_VALUE(retval);
        }
        getcb_add_return_key(get2cb, retval);
    }

    /***** ADD RETURN KEYS AND REMOVE THIS COMMENT ****/

    //if (GETCB_GET2_FIRST_RETURN_KEY(get2cb) == NULL) {
    //    return ERR_NCX_NO_INSTANCE;
    //}

    /* optional: check if any content-match nodes are present */
    boolean match_test_done = FALSE;
    val_value_t *match_val = GETCB_GET2_FIRST_MATCH(get2cb);
    for (; match_val; match_val =
        GETCB_GET2_NEXT_MATCH(get2cb, match_val)) {

        /**** CHECK CONTENT NODES AGAINST THIS ENTRY ***/

    }
    GETCB_GET2_MATCH_TEST_DONE(get2cb) = match_test_done;

    /* For GETNEXT, set the more_data flag true if not sure */
    boolean more_data = FALSE;

    /**** SET more_data FLAG ****/

    GETCB_GET2_MORE_DATA(get2cb) = more_data;

    /* go through all the requested terminal child objects */
    obj_template_t *childobj =
        getcb_first_requested_child(get2cb, obj);
    for (; childobj; childobj =
        getcb_next_requested_child(get2cb, childobj)) {

        const xmlChar *name = obj_get_name(childobj);

        /* Retrieve the value of this terminal node and 
         * add with getcb_add_return_val */

        if (!xml_strcmp(name, y_o_ran_externalio_N_port_in)) {
            /* leaf port-in (uint8) */
            val_value_t *return_val = agt_make_leaf2(
                obj,
                y_o_ran_externalio_M_o_ran_externalio,
                y_o_ran_externalio_N_port_in,
                i_o_ran_externalio_N_input_port_in,
                &res);
            if (return_val) {
                getcb_add_return_val(get2cb, return_val);
            }
        } else if (!xml_strcmp(name, y_o_ran_externalio_N_line_in)) {
            /* leaf line-in (boolean) */
            val_value_t *return_val = agt_make_boolean_leaf(
                obj,
                y_o_ran_externalio_M_o_ran_externalio,
                y_o_ran_externalio_N_line_in,
                line_in,
                &res);
            if (return_val) {
                getcb_add_return_val(get2cb, return_val);
            }
        }
    }

    return res;

} /* u_o_ran_externalio_external_io_input_get */

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
status_t u_o_ran_externalio_external_io_output_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_external_io_output_name,
    boolean name_fixed,
    boolean name_present)
{
    boolean getnext = FALSE;

    (void)getnext; /* remove yumaworks warning */

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
        log_info("\n    present=%d ,fixed=%d", name_present, name_fixed);
        log_info("\n    key_name=%s", k_external_io_output_name);
    }

    /* check the callback mode type */
    getcb_mode_t cbmode = GETCB_GET2_CBMODE(get2cb);
    switch (cbmode) {
    case GETCB_GET_VALUE:
        break;
    case GETCB_GETNEXT_VALUE:
        getnext = TRUE;
        break;
    default:
        return SET_ERROR(ERR_INTERNAL_VAL);
    }

    obj_template_t *obj = GETCB_GET2_OBJ(get2cb);
    status_t res = NO_ERR;

    uint32 max_entries = GETCB_GET2_MAX_ENTRIES(get2cb);

    (void)max_entries; /* remove yumaworks warning */

    /* For GET, find the entry that matches the key values
     * For GETNEXT, find the entry that matches the next key value
     * If the 'present' flag is false then return first key instance
     * If the 'fixed' flag is true then no GETNEXT advance for the key
     * Create a new return key val_value_t, then getcb_add_return_key */
    obj_template_t *name_obj = obj_find_child(
        obj,
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_name);
    if(name_obj == NULL) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: name_obj is NULL", __func__);
        }
        return ERR_NCX_NO_INSTANCE;
    }

    val_value_t *retval = agt_make_leaf2(name_obj,
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_name,
        i_o_ran_externalio_N_output_name,
        &res);
    if (retval) {
        if (name_fixed) {
            VAL_SET_FIXED_VALUE(retval);
        }
        getcb_add_return_key(get2cb, retval);
    }

    /***** ADD RETURN KEYS AND REMOVE THIS COMMENT ****/

//    if (GETCB_GET2_FIRST_RETURN_KEY(get2cb) == NULL) {
//        return ERR_NCX_NO_INSTANCE;
//    }

    /* optional: check if any content-match nodes are present */
    boolean match_test_done = FALSE;
    val_value_t *match_val = GETCB_GET2_FIRST_MATCH(get2cb);
    for (; match_val; match_val =
        GETCB_GET2_NEXT_MATCH(get2cb, match_val)) {

        /**** CHECK CONTENT NODES AGAINST THIS ENTRY ***/

    }
    GETCB_GET2_MATCH_TEST_DONE(get2cb) = match_test_done;

    /* For GETNEXT, set the more_data flag true if not sure */
    boolean more_data = FALSE;

    /**** SET more_data FLAG ****/

    GETCB_GET2_MORE_DATA(get2cb) = more_data;

    /* go through all the requested terminal child objects */
    obj_template_t *childobj =
        getcb_first_requested_child(get2cb, obj);
    for (; childobj; childobj =
        getcb_next_requested_child(get2cb, childobj)) {

        const xmlChar *name = obj_get_name(childobj);

        /* Retrieve the value of this terminal node and 
         * add with getcb_add_return_val */

        if (!xml_strcmp(name, y_o_ran_externalio_N_port_out)) {
            /* leaf port-out (uint8) */
            val_value_t *return_val = agt_make_leaf2(
                obj,
                y_o_ran_externalio_M_o_ran_externalio,
                y_o_ran_externalio_N_port_out,
                i_o_ran_externalio_N_port_out,
                &res);
            if (return_val) {
                getcb_add_return_val(get2cb, return_val);
            }

        }
    }

    return res;

} /* u_o_ran_externalio_external_io_output_get */


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
status_t u_o_ran_externalio_external_io_output_setting_line_out_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_external_io_output_setting_name)
{
    status_t res = NO_ERR;
    boolean newval_val = (newval) ? VAL_BOOL(newval) : 0;
    boolean curval_val = (curval) ? VAL_BOOL(curval) : 0;

    (void)scb; /* remove yumaworks warning */
    (void)msg; /* remove yumaworks warning */
    (void)curval_val; /* remove yumaworks warning */

    if (LOGINFO) {
        log_info("\nEnter %s: phase=%s", __func__, agt_cbtype_name(cbtyp));
        log_info("\n   key_name=%s",k_external_io_output_setting_name);
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
        case OP_EDITOP_REPLACE:
        case OP_EDITOP_CREATE:
            res = i_o_ran_externalio_send_msi_edit(newval_val);
            break;
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }
    return res;

} /* u_o_ran_externalio_external_io_output_setting_line_out_edit */


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
status_t u_o_ran_externalio_external_io_output_setting_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_external_io_output_setting_name)
{
    status_t res = NO_ERR;

    (void)scb; /* remove yumaworks warning */
    (void)msg; /* remove yumaworks warning */
    (void)newval; /* remove yumaworks warning */
    (void)curval; /* remove yumaworks warning */

    if (LOGINFO) {
        log_info("\nEnter %s: phase=%s", __func__, agt_cbtype_name(cbtyp));
        log_info("\n   key_name=%s",k_external_io_output_setting_name);
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        if(xml_strcmp(k_external_io_output_setting_name, i_o_ran_externalio_N_output_name)){
            res = ERR_NCX_BAD_ELEMENT;
        }
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
        case OP_EDITOP_REPLACE:
        case OP_EDITOP_CREATE:
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }
    return res;

} /* u_o_ran_externalio_external_io_output_setting_edit */


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
status_t u_o_ran_externalio_external_io_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;

    (void)scb; /* remove yumaworks warning */
    (void)msg; /* remove yumaworks warning */
    (void)newval; /* remove yumaworks warning */
    (void)curval; /* remove yumaworks warning */

    if (LOGINFO) {
        log_info("\nEnter %s: phase=%s", __func__, agt_cbtype_name(cbtyp));
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            break;
        case OP_EDITOP_MERGE:
            break;
        case OP_EDITOP_REPLACE:
            break;
        case OP_EDITOP_CREATE:
            break;
        case OP_EDITOP_DELETE:
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }
    return res;

} /* u_o_ran_externalio_external_io_edit */


/********************************************************************
* FUNCTION u_o_ran_externalio_external_input_change_send
*
* Send a u_o_ran_externalio_external_input_change notification
* Called by your code when notification event occurs
*
********************************************************************/
void u_o_ran_externalio_external_input_change_send (
    y_o_ran_externalio_T_external_input_change_current_input_notification *v_current_input_notification)
{
    val_value_t *contval = NULL;
    val_value_t *listval = NULL;
    val_value_t *parmval = NULL;
    status_t res = NO_ERR;
    boolean line_in = 0;
    T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND  *data = NULL;

    /* パラメータは無視して別途データを取得する。(お行儀悪し) */
    (void)v_current_input_notification;
    data = i_o_ran_externalio_get_notification_data();

    if (LOGINFO) {
        log_info("\nEnter %s", __func__);
    }

    if (!agt_notifications_enabled()) {
        log_debug2("\nSkipping <external-input-change> notification; disabled");
        return;
    }
    
    if (LOGDEBUG) {
        log_debug("\nGenerating <external-input-change> notification");
    }
    
    agt_not_msg_t *notif = agt_not_new_notification(external_input_change_obj);
    if (notif == NULL) {
        log_error("\nError: malloc failed, cannot send "
        "<external-input-change> notification");
        return;
    }
    
    /* add container current_input_notification to payload
     * replace following line with real code
     */
    //(void)v_current_input_notification;

    res = i_o_ran_externalio_conv_line_state_to_bool(
        ((T_MPSW_MSI_O_RAN_EXTERNALIO_INPUT_CHANGE_NTF_IND *)data)->line_in,
        &line_in);
    if(res != NO_ERR)
    {
        if(LOGWARN) {
            log_warn("\n%s: Operation Failed to convert line-in", __func__);
        }
        agt_not_free_notification(notif);
        return;
    }

    /* container */
    contval = agt_make_object(
        external_input_change_obj,
        y_o_ran_externalio_N_current_input_notification,
        &res);
    if(contval == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to make list(%s), Error=%d", __func__, y_o_ran_externalio_N_current_input_notification, res);
        }
        agt_not_free_notification(notif);
        return;
    }

    agt_not_add_to_payload(notif, contval);

    /* list */
    listval = agt_make_object(
        VAL_OBJ(contval),
        y_o_ran_externalio_N_external_input,
        &res);
    if(listval == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to make list(%s), Error=%d", __func__, y_o_ran_externalio_N_external_input, res);
        }
        agt_not_free_notification(notif);
        return;
    }
    else {
        res = val_child_add(listval, contval);
        if(res != NO_ERR) {
            val_free_value(listval);
        }
    }

    /* name設定 */
    parmval = agt_make_leaf2(
        VAL_OBJ(listval),
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_name,
        i_o_ran_externalio_N_input_name,
        &res);
    if(parmval == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to make leaf(%s), Error=%d", __func__, y_o_ran_externalio_N_name, res);
        }
        agt_not_free_notification(notif);
        return;
    }
    else {
        res = val_child_add(parmval, listval);
        if(res != NO_ERR) {
            val_free_value(parmval);
        }
    }

    /* io-port設定 */
    parmval = agt_make_leaf2(
        VAL_OBJ(listval),
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_io_port,
        i_o_ran_externalio_N_input_port_in,
        &res);
    if(parmval == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to make leaf(%s), Error=%d", __func__, y_o_ran_externalio_N_io_port, res);
        }
        agt_not_free_notification(notif);
        return;
    }
    else {
        res = val_child_add(parmval, listval);
        if(res != NO_ERR) {
            val_free_value(parmval);
        }
    }

    /* line-in設定 */
    parmval = agt_make_boolean_leaf(
        VAL_OBJ(listval),
        y_o_ran_externalio_M_o_ran_externalio,
        y_o_ran_externalio_N_line_in,
        line_in,
        &res);
    if(parmval == NULL) {
        if(LOGWARN) {
            log_warn("\n%s: Failed to make leaf(%s), Error=%d", __func__, y_o_ran_externalio_N_line_in, res);
        }
        agt_not_free_notification(notif);
        return;
    }
    else {
        res = val_child_add(parmval, listval);
        if(res != NO_ERR) {
            val_free_value(parmval);
        }
    }

    sil_sa_queue_notification(notif);

    if (LOGINFO) {
        log_info("\n%s:Send Noticication name=%s, io-port=%s, line-in=%d",
            __func__,
            i_o_ran_externalio_N_input_name,
            i_o_ran_externalio_N_input_port_in,
            line_in);
    }

} /* u_o_ran_externalio_external_input_change_send */

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
status_t u_o_ran_externalio_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;
    ncx_module_t *o_ran_externalio_mod = NULL;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    o_ran_externalio_mod = ncx_find_module(modname, revision);
    if (o_ran_externalio_mod == NULL) {
        return ERR_NCX_OPERATION_FAILED;
    }

    external_input_change_obj = ncx_find_object(
        o_ran_externalio_mod,
        y_o_ran_externalio_N_external_input_change);
    if (external_input_change_obj == NULL) {
        return ERR_NCX_DEF_NOT_FOUND;
    }

    /* put your module initialization code here */
    
    return res;

} /* u_o_ran_externalio_init */

/********************************************************************
* FUNCTION u_o_ran_externalio_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_externalio_init2 (void)
{
    status_t res = NO_ERR;

    /* put your init2 code here */
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    i_o_ran_externalio_init2();

    return res;

} /* u_o_ran_externalio_init2 */

/********************************************************************
* FUNCTION u_o_ran_externalio_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
void u_o_ran_externalio_cleanup (void)
{

    /* put your cleanup code here */
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

} /* u_o_ran_externalio_cleanup */

/* END SIL-SA u_o_ran_externalio.c */
