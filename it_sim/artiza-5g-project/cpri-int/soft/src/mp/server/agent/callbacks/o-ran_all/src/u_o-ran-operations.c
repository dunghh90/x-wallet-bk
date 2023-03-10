
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
    module o-ran-operations
    revision 2019-02-04
    namespace urn:o-ran:operations:1.0
    organization O-RAN Alliance
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
#include "y_o-ran-operations.h"
#include "u_o-ran-operations.h"
#include "i_o-ran-operations.h"

/* put your static variables here */
/* put your static variables here */
static char restart_cause_on[]          = i_o_ran_operations_N_restart_cause_on;
static char restart_cause_watchdog[]    = i_o_ran_operations_N_restart_cause_watchdog;
static char restart_cause_restart[]     = i_o_ran_operations_N_restart_cause_restart;
static char restart_cause_failure[]     = i_o_ran_operations_N_restart_cause_failure;
static char restart_cause_other[]       = i_o_ran_operations_N_restart_cause_other;
static char restart_cause_unknown[]      = i_o_ran_operations_N_restart_cause_unknown;

/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_declarations_supported_header_mechanism_get
*
* Get database object callback for list supported-header-mechanism
* Path: /operational-info/declarations/supported-header-mechanism
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_declarations_supported_header_mechanism_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_operational_info_declarations_supported_header_mechanism_protocol,
    boolean protocol_fixed,
    boolean protocol_present)
{
    boolean getnext = FALSE;
    (void)getnext; /* for warning */
    (void)k_operational_info_declarations_supported_header_mechanism_protocol; /* for warning */
    (void)protocol_fixed; /* for warning */
    (void)protocol_present; /* for warning */

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
    (void)max_entries; /* for warning */

    /* For GET, find the entry that matches the key values
     * For GETNEXT, find the entry that matches the next key value
     * If the 'present' flag is false then return first key instance
     * If the 'fixed' flag is true then no GETNEXT advance for the key
     * Create a new return key val_value_t, then getcb_add_return_key */

    /***** ADD RETURN KEYS AND REMOVE THIS COMMENT ****/

//  if (GETCB_GET2_FIRST_RETURN_KEY(get2cb) == NULL) {
//      return ERR_NCX_NO_INSTANCE;
//  }

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
    /* get key's number from key */
    xmlChar *init_data = 0;
    res = i_o_ran_operations_operational_info_declarations_supported_header_mechanism_keyget( &init_data );
    if(res != NO_ERR) {
        return ERR_NCX_NO_INSTANCE;
    }

    /* set key */
    const xmlChar *v_key_data = (const xmlChar *)init_data;

    /* if we are here, then the index is valid */
    /* leaf protocol (enumeration) */
    obj_template_t  *name_obj   = obj_find_child( obj,      y_o_ran_operations_M_o_ran_operations, y_o_ran_operations_N_protocol );
    val_value_t     *return_val = agt_make_leaf2( name_obj, y_o_ran_operations_M_o_ran_operations, y_o_ran_operations_N_protocol, v_key_data, &res);
    if (return_val) {
        if (protocol_fixed) {
            VAL_SET_FIXED_VALUE(return_val);
        }
        getcb_add_return_key(get2cb, return_val);
    }
    GETCB_GET2_MORE_DATA(get2cb) = more_data;

    /* go through all the requested terminal child objects */
    obj_template_t *childobj  = getcb_first_requested_child(get2cb, obj);
    for (; childobj; childobj = getcb_next_requested_child(get2cb, childobj)) {

        const xmlChar *name = obj_get_name(childobj);

        /* Retrieve the value of this terminal node and add with getcb_add_return_val */
        init_data = 0;
        if( (!xml_strcmp(name, y_o_ran_operations_N_ecpri_concatenation_support))
         || (!xml_strcmp(name, y_o_ran_operations_N_protocol_version)) ) {
            /* leaf ecpri-concatenation-support (boolean) */
            /* leaf protocol-version (string) */

            if( (!xml_strcmp(name, y_o_ran_operations_N_ecpri_concatenation_support)) && (xml_strcmp(v_key_data, (const xmlChar *)"ECPRI")))
            {
                /* when "../protocol = 'ECPRI'" */
                /* protocol != ECPRI?????????SKIP  */
                if (LOGDEBUG) {
                    log_debug("\n%s:%d ecpri-concatenation-support is not displayed because the value of protocol is %s", __func__, __LINE__, v_key_data);
                }
                continue;
            }

            /* leaf parameter read from "saa_init_data.conf" */
            const xmlChar *v_init_data = (const xmlChar *)init_data;
            res = i_o_ran_operations_operational_info_declarations_supported_header_mechanism_get(name, &init_data);

            if(res != NO_ERR) {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s:%d wrong status. leaf_name=%s, res=%d", __func__, __LINE__, name, res);
                }
                return res;
            }

            v_init_data = (const xmlChar *)init_data;
            return_val = agt_make_leaf2(obj,
                                        y_o_ran_operations_M_o_ran_operations,
                                        name,
                                        v_init_data,
                                        &res);
            if (return_val) {
                getcb_add_return_val(get2cb, return_val);
            }

        } else {
            res = ERR_NCX_DATA_MISSING;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s:%d not applicable leaf. leaf_name=%s, res=%d", __func__, __LINE__, name, res);
            }
            return res;
        }
    }

    return res;

} /* u_o_ran_operations_operational_info_declarations_supported_header_mechanism_get */

/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_declarations_get
*
* Get database object callback for container declarations
* Path: /operational-info/declarations
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_declarations_get (
    getcb_get2_t *get2cb)
{
    /* check the callback mode type */
    getcb_mode_t cbmode = GETCB_GET2_CBMODE(get2cb);
    switch (cbmode) {
    case GETCB_GET_VALUE:
        break;
    case GETCB_GETNEXT_VALUE:
        return ERR_NCX_NO_INSTANCE;
    default:
        return SET_ERROR(ERR_INTERNAL_VAL);
    }

    /* an NP container always exists so no test for node_exists
     * by the SIL or SIL-SA callback is needed */
    obj_template_t *obj = GETCB_GET2_OBJ(get2cb);
    status_t res = NO_ERR;

    /* optional: check if any content-match nodes are present */
    boolean match_test_done = FALSE;
    val_value_t *match_val = GETCB_GET2_FIRST_MATCH(get2cb);
    for (; match_val; match_val =
        GETCB_GET2_NEXT_MATCH(get2cb, match_val)) {

        /**** CHECK CONTENT NODES AGAINST THIS ENTRY ***/

    }
    GETCB_GET2_MATCH_TEST_DONE(get2cb) = match_test_done;

    /* go through all the requested terminal child objects */
    obj_template_t *childobj  = getcb_first_requested_child(get2cb, obj);
    for (; childobj; childobj = getcb_next_requested_child(get2cb, childobj)) {

        const xmlChar *name = obj_get_name(childobj);

        /* Retrieve the value of this terminal node and add with getcb_add_return_val */
        xmlChar *init_data = 0;
        if( (!xml_strcmp(name, y_o_ran_operations_N_supported_mplane_version))
         || (!xml_strcmp(name, y_o_ran_operations_N_supported_cusplane_version)) ) {
            /* leaf supported-mplane-version (string) */
            /* leaf supported-cusplane-version (string) */

            /* leaf parameter read from "saa_init_data.conf" */
            res = i_o_ran_operations_operational_info_declarations_get(name, &init_data);

            if(res != NO_ERR) {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s:%d wrong status. leaf_name=%s, res=%d", __func__, __LINE__, name, res);
                }
                return res;
            }

            const xmlChar *v_init_data = (const xmlChar *)init_data;
            val_value_t *return_val = agt_make_leaf2(obj,
                                                     y_o_ran_operations_M_o_ran_operations,
                                                     name,
                                                     v_init_data,
                                                     &res);
            if (return_val) {
                getcb_add_return_val(get2cb, return_val);
            }

        } else {
            res = ERR_NCX_DATA_MISSING;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s:%d not applicable leaf. leaf_name=%s, res=%d", __func__, __LINE__, name, res);
            }
            return res;
        }
    }

    return res;

} /* u_o_ran_operations_operational_info_declarations_get */

/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_operational_state_get
*
* Get database object callback for container operational-state
* Path: /operational-info/operational-state
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_operational_state_get (
    getcb_get2_t *get2cb)
{
    /* check the callback mode type */
    getcb_mode_t cbmode = GETCB_GET2_CBMODE(get2cb);
    switch (cbmode) {
    case GETCB_GET_VALUE:
        break;
    case GETCB_GETNEXT_VALUE:
        return ERR_NCX_NO_INSTANCE;
    default:
        return SET_ERROR(ERR_INTERNAL_VAL);
    }

    /* an NP container always exists so no test for node_exists
     * by the SIL or SIL-SA callback is needed */
    obj_template_t *obj = GETCB_GET2_OBJ(get2cb);
    status_t res = NO_ERR;

    /* optional: check if any content-match nodes are present */
    boolean match_test_done = FALSE;
    val_value_t *match_val = GETCB_GET2_FIRST_MATCH(get2cb);
    for (; match_val; match_val =
        GETCB_GET2_NEXT_MATCH(get2cb, match_val)) {

        /**** CHECK CONTENT NODES AGAINST THIS ENTRY ***/

    }
    GETCB_GET2_MATCH_TEST_DONE(get2cb) = match_test_done;

    /* go through all the requested terminal child objects */
    obj_template_t *childobj  = getcb_first_requested_child(get2cb, obj);
    int            get_restart_cause    = D_MPSW_OFF;
    int            get_restart_datetime = D_MPSW_OFF;
    for (; childobj; childobj = getcb_next_requested_child(get2cb, childobj)) {

        const xmlChar *name = obj_get_name(childobj);

        /* Retrieve the value of this terminal node and add with getcb_add_return_val */
        if( !xml_strcmp(name, y_o_ran_operations_N_restart_cause) ) {
            /* leaf restart-cause (enumeration) */
            get_restart_cause = D_MPSW_ON;

        } else if( !xml_strcmp(name, y_o_ran_operations_N_restart_datetime) ) {
            /* leaf restart-datetime (string) */
            get_restart_datetime = D_MPSW_ON;

        } else {
            res = ERR_NCX_DATA_MISSING;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s:%d not applicable leaf. leaf_name=%s, res=%d", __func__, __LINE__, name, res);
            }
            return res;
        }
    }
    if( (get_restart_cause == D_MPSW_OFF) && (get_restart_datetime == D_MPSW_OFF) ) {
        res = ERR_NCX_DATA_MISSING;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s:%d count of get leaf NG.", __func__, __LINE__);
        }
        return res;
    } else {

        char                                msg_data[max(sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM), sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_GET_REJ))];
        T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM *get_cfm_data;

        res = i_o_ran_operations_operational_info_operational_state_get( msg_data );
        if(res != NO_ERR) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: MSI failed. res=%d", __func__, res);
            }
            return res;
        } else {
            get_cfm_data = (T_MPSW_MSI_O_RAN_OPERATIONS_GET_CFM *)msg_data;
            if( get_restart_cause == D_MPSW_ON ) {
                /* leaf restart-cause (enumeration) */
                xmlChar *restart_cause = 0;
                switch(get_cfm_data->restart_cause) {
                    case E_RESTART_CAUSE_POWER_ON:
                        restart_cause = (xmlChar *)restart_cause_on;
                        break;
                    case E_RESTART_CAUSE_SUPERVISION_WATCHDOG:
                        restart_cause = (xmlChar *)restart_cause_watchdog;
                        break;
                    case E_RESTART_CAUSE_MPLANE_TRIGGERED_RESTART:
                        restart_cause = (xmlChar *)restart_cause_restart;
                        break;
                    case E_RESTART_CAUSE_SOFTWARE_FAILURE:
                        restart_cause = (xmlChar *)restart_cause_failure;
                        break;
                    case E_RESTART_CAUSE_OTHER_WATCHDOG_TIMER:
                        restart_cause = (xmlChar *)restart_cause_other;
                        break;
                    case E_RESTART_CAUSE_UNKNOWN:
                        restart_cause = (xmlChar *)restart_cause_unknown;
                        break;
                    default:
                        restart_cause = (xmlChar *)restart_cause_unknown;
                        /* error log */
                        if(LOGWARN) {
                            log_warn("\n%s:%d wrong status. restart_cause=%d", __func__, __LINE__, get_cfm_data->restart_cause);
                        }
                        break;
                }
                const xmlChar *v_restart_cause = (const xmlChar *)restart_cause;
                val_value_t *return_val = agt_make_leaf2( obj,
                                                          y_o_ran_operations_M_o_ran_operations,
                                                          y_o_ran_operations_N_restart_cause,
                                                          v_restart_cause,
                                                          &res);
                if (return_val) {
                    getcb_add_return_val(get2cb, return_val);
                }
            }

            if( get_restart_datetime == D_MPSW_ON ) {
                /* leaf restart-datetime (string) */
                xmlChar *restart_datetime = 0;
                char    datetime[64];
                int     ret = D_MPSW_OK;

                memset( datetime, 0, sizeof(datetime) ) ;
                restart_datetime = (xmlChar *)datetime;
                ret = f_saa_com_utc_to_offset_string( get_cfm_data->restart_time.year, 
                                                      get_cfm_data->restart_time.month, 
                                                      get_cfm_data->restart_time.day, 
                                                      get_cfm_data->restart_time.hour, 
                                                      get_cfm_data->restart_time.min, 
                                                      get_cfm_data->restart_time.sec, 
                                                      (char *)restart_datetime );
                if(ret != D_MPSW_OK) {
                    res = ERR_NCX_DATA_MISSING;
                    /* error log */
                    if(LOGWARN) {
                        log_warn("\n%s: MSI failed. res=%d", __func__, res);
                    }
                    return res;
                }
                const xmlChar *v_restart_datetime = (const xmlChar *)restart_datetime;
                val_value_t *return_val2 = agt_make_leaf2( obj,
                                                          y_o_ran_operations_M_o_ran_operations,
                                                          y_o_ran_operations_N_restart_datetime,
                                                          v_restart_datetime,
                                                          &res);
                if (return_val2) {
                    getcb_add_return_val(get2cb, return_val2);
                }
            }
        }
    }

    return res;

} /* u_o_ran_operations_operational_info_operational_state_get */


/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_clock_timezone_utc_offset_edit
*
* Edit database object callback
* Path: /operational-info/clock/timezone-utc-offset
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_clock_timezone_utc_offset_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    int16 newval_val = (newval) ? VAL_INT16(newval) : 0;
    int16 curval_val = (curval) ? VAL_INT16(curval) : 0;

    (void)curval_val; /* for warning */
    (void)scb; /* for warning */
    (void)msg; /* for warning */

    if (LOGDEBUG) {
        log_debug("\nEnter u_o_ran_operations_operational_info_clock_timezone_utc_offset_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
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
            res = i_o_ran_operations_operational_info_clock_timezone_utc_offset_edit(newval_val);
            if(res != NO_ERR) {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s:%d utc_offset edit error. timezone_utc_offset=%x, res=%d", __func__, __LINE__, newval_val, res);
                }
            }
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

} /* u_o_ran_operations_operational_info_clock_timezone_utc_offset_edit */


/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_clock_edit
*
* Edit database object callback
* Path: /operational-info/clock
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_clock_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;

    (void)scb; /* for warning */
    (void)msg; /* for warning */
    (void)newval; /* for warning */
    (void)curval; /* for warning */

    if (LOGDEBUG) {
        log_debug("\nEnter u_o_ran_operations_operational_info_clock_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
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

} /* u_o_ran_operations_operational_info_clock_edit */


/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit
*
* Edit database object callback
* Path: /operational-info/re-call-home-no-ssh-timer
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    uint16 newval_val = (newval) ? VAL_UINT16(newval) : 0;
    uint16 curval_val = (curval) ? VAL_UINT16(curval) : 0;

    (void)scb; /* for warning */
    (void)msg; /* for warning */
    (void)curval_val; /* for warning */

    if (LOGDEBUG) {
        log_debug("\nEnter u_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
    }

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        res = i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_validate(newval_val);
        if(res != NO_ERR) {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s:%d re_call_home_no_ssh_timer validate error. re_call_home_no_ssh_timer=%x, res=%d", __func__, __LINE__, newval_val, res);
            }
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
            res = i_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit(newval_val);
            if(res != NO_ERR) {
                /* error log */
                if(LOGWARN) {
                    log_warn("\n%s:%d re_call_home_no_ssh_timer edit edit error. re_call_home_no_ssh_timer=%x, res=%d", __func__, __LINE__, newval_val, res);
                }
            }
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

} /* u_o_ran_operations_operational_info_re_call_home_no_ssh_timer_edit */


/********************************************************************
* FUNCTION u_o_ran_operations_operational_info_edit
*
* Edit database object callback
* Path: /operational-info
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_operational_info_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval)
{
    status_t res = NO_ERR;
    (void)scb; /* for warning */
    (void)msg; /* for warning */
    (void)newval; /* for warning */
    (void)curval; /* for warning */

    if (LOGDEBUG) {
        log_debug("\nEnter u_o_ran_operations_operational_info_edit callback for %s phase",
            agt_cbtype_name(cbtyp));
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

} /* u_o_ran_operations_operational_info_edit */


/********************************************************************
* FUNCTION u_o_ran_operations_reset_validate
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
status_t u_o_ran_operations_reset_validate (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    xml_node_t *methnode)
{
    status_t res = NO_ERR;
    val_value_t *errorval = NULL;

    if (LOGDEBUG) {
        log_debug("\nStart SIL validate rpc <reset> from module o-ran-operations");
    }

    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_OPERATION,
            res,
            methnode,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval);
    }
    return res;

} /* u_o_ran_operations_reset_validate */


/********************************************************************
* FUNCTION u_o_ran_operations_reset_invoke
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
status_t u_o_ran_operations_reset_invoke (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    xml_node_t *methnode)
{
    status_t res = NO_ERR;

    if (LOGDEBUG) {
        log_debug("\nStart SIL invoke rpc <reset> from module o-ran-operations");
    }

    /* remove the next line if scb is used */
    (void)scb;

    /* remove the next line if msg is used */
    (void)msg;

    /* remove the next line if methnode is used */
    (void)methnode;

    /* invoke your device instrumentation code here */
    res = i_o_ran_operations_reset_invoke(E_RESTART_CAUSE_MPLANE_TRIGGERED_RESTART);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed. res=%d", __func__, res);
        }
    }

    return res;

} /* u_o_ran_operations_reset_invoke */

/********************************************************************
* FUNCTION u_o_ran_operations_init
*
* initialize the o-ran-operations server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_init (
    const xmlChar *modname,
    const xmlChar *revision)
{
    status_t res = NO_ERR;
    ncx_module_t *o_ran_operations_mod = NULL;

    o_ran_operations_mod = ncx_find_module(modname, revision);
    if (o_ran_operations_mod == NULL) {
        return ERR_NCX_OPERATION_FAILED;
    }

    /* put your module initialization code here */
    
    return res;

} /* u_o_ran_operations_init */

/********************************************************************
* FUNCTION u_o_ran_operations_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
status_t u_o_ran_operations_init2 (void)
{
    status_t res = NO_ERR;

    /* put your init2 code here */
    i_o_ran_operations_init2();

    return res;

} /* u_o_ran_operations_init2 */

/********************************************************************
* FUNCTION u_o_ran_operations_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
void u_o_ran_operations_cleanup (void)
{

    /* put your cleanup code here */
    
} /* u_o_ran_operations_cleanup */

/* END SIL-SA u_o_ran_operations.c */
