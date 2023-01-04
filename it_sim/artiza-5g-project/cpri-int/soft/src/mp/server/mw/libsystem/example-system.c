/*
 * Copyright (c) 2012 - 2016, YumaWorks, Inc., All Rights Reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
/*  FILE: example-system.c

  Example External System Library

*********************************************************************
*                                                                   *
*                     I N C L U D E    F I L E S                    *
*                                                                   *
*********************************************************************/

#include <stdlib.h>
#include <xmlstring.h>

#include "procdefs.h"
#include "agt.h"
#include "agt_acm.h"
#include "agt_acm_ietf.h"
#include "agt_acm_extern.h"
#include "agt_util.h"
#include "agt_cfg.h"
#include "agt_cb.h"
#include "agt_hook_util.h"
#include "dlq.h"
#include "example-system.h"
#include "log.h"
#include "log_vendor.h"
#include "log_vendor_extern.h"
#include "ncx.h"
#include "ncxmod.h"
#include "ncxtypes.h"
#include "obj.h"
#include "ses.h"
#include "status.h"
#include "typ_userdef.h"
#include "val.h"
#include "val_util.h"
#include "xml_util.h"



#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION my_ipv6_fn
*
* canonical callback for an IPv6 address
* convert the inval to the canonical format for the type
*
* INPUTS:
*   typdef == type definition for the user defined type
*   val == input value to convert
*   cookie == cookie value passed to register function
*
* OUTPUTS:
*   val == can be changed to canonical format
*
* RETURNS:
*   status
*
*********************************************************************/
static status_t
    my_ipv6_fn (typ_def_t *typdef,
                val_value_t *val,
                void *cookie)
{
    (void)typdef;
    (void)cookie;

    log_debug("\nmy_ipv6_fn called ******************************");
    return NO_ERR;

}  /* my_ipv6_fn */
#endif    // 0


/* add to include the NACM external group example */
// #define WITH_NACM_GROUP 1

/****************  Example External NACM Hooks *******************/

#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION acm_extern_rpc
*
* Check if the specified user is allowed to invoke an RPC
*
* INPUTS:
*   msg == XML header in incoming message in progress
*   user == user name string
*   rpcobj == obj_template_t for the RPC method to check
*
* RETURNS:
*   TRUE if user allowed invoke this RPC; FALSE otherwise
*********************************************************************/
static boolean
    acm_extern_rpc (xml_msg_hdr_t *msg,
                    const xmlChar *user,
                    const obj_template_t *rpcobj)
{
    (void)msg;
    (void)user;
    (void)rpcobj;
    log_debug("\nacm_extern_rpc: return OK\n");
    return TRUE;
}
#endif   // 0


#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION acm_extern_notif
*
* Check if the specified user is allowed to receive
* a notification event
*
* INPUTS:
*   user == user name string
*   notifobj == obj_template_t for the notification event to check
*
* RETURNS:
*   TRUE if user allowed receive this notification event;
*   FALSE otherwise
*********************************************************************/
static boolean
    acm_extern_notif (const xmlChar *user,
                      const obj_template_t *notifobj)
{
    (void)user;
    (void)notifobj;
    log_debug("\nacm_extern_notif: return OK\n");
    return TRUE;
}
#endif   // 0


#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION acm_extern_write_fn
*
* Check if the specified user is allowed to access a value node
* The val->obj template will be checked against the val->editop
* requested access and the user's configured max-access
*
* INPUTS:
*   msg == XML header from incoming message in progress
*   newval  == val_value_t in progress to check
*                (may be NULL, if curval set)
*   curval  == val_value_t in progress to check
*                (may be NULL, if newval set)
*   val  == val_value_t in progress to check
*   editop == requested CRUD operation
*
* RETURNS:
*   TRUE if user allowed this level of access to the value node
*********************************************************************/
static boolean
    acm_extern_write (xml_msg_hdr_t *msg,
                      const xmlChar *user,
                      const val_value_t *newval,
                      const val_value_t *curval,
                      op_editop_t editop)
{
    (void)msg;
    (void)user;
    (void)newval;
    (void)curval;
    (void)editop;
    log_debug("\nacm_extern_write: return OK\n");
    return TRUE;
}
#endif   // 0


#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION acm_extern_read_fn
*
* Check if the specified user is allowed to read a value node
*
* INPUTS:
*   msg == XML header from incoming message in progress
*   user == user name string
*   val  == val_value_t in progress to check
*
* RETURNS:
*   TRUE if user allowed read access to the value node
*********************************************************************/
static boolean
    acm_extern_read (xml_msg_hdr_t *msg,
                     const xmlChar *user,
                     const val_value_t *val)
{
    (void)msg;
    (void)user;
    (void)val;
    log_debug("\nacm_extern_read: return OK\n");
    return TRUE;
}
#endif   // 0



#ifdef WITH_NACM_GROUP
/********************************************************************
* FUNCTION nacm_external_group_cbfn
*
 * Get the list of group names for this username
 * These groups are added to the usergroup cache for the session
 * INPUTS:
 *   username: return the list of group names that this username
 *             is a member
 *   retgroups == address of return malloced string
 * OUTPUTS:
 *   *retgroups is set to a malloced string that will be parsed.
 *         It contains a whitespace delimited list of group named
 *            ' group1 group2 group3'
 *         The caller will free this string with m__free
 * RETURNS:
 *   status: if an error occurs the session will only use NACM groups
*********************************************************************/
static status_t
    nacm_external_group_cbfn (const xmlChar *username,
                              xmlChar **retgroups)
{
    if (retgroups == NULL) {
        return ERR_NCX_INVALID_VALUE;
    }

    (void)username;

    /* MUST use a function that allocates memory with m__getMem
     * Will be freed by the caller with m__free macro
     */
    *retgroups = xml_strdup("group1 group2 group5");
    if (*retgroups == NULL) {
        return ERR_INTERNAL_MEM;
    }

    return NO_ERR;
}
#endif   // WITH_NACM_GROUP


/**************  Example External Log Vendor Hooks ***************/

#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION log_vendor_init1_fn
*
* Vendor init function to support log output (optional)
*
*********************************************************************/
static void
    log_vendor_init1_fn ( boolean pre_cli )
{
    // Action TBD. This function is available for required vendor init.
    if (pre_cli) {
      ;
    } else {
      ;
    }
}
#endif   // 0


#if 0 // ********** REMOVE #if 0 to enable this example callback
/********************************************************************
* FUNCTION log_vendor_send_fn
*
* Vendor function to consume log output (mandatory)
*
* INPUTS:
*   app == "Facility" (Yangcli or Netconfd)
*   level == Logging level (error, warn, info, debug, ..., debug4)
*   fstr == format string (like printf)
*   args == variable argument list
*
* RETURNS:
*   void
*********************************************************************/
static void
    log_vendor_send_fn (log_debug_app_t app, log_debug_t level,
			const char *fstr, va_list args)
{
    (void)app;
    (void)level;
    (void)fstr;
    (void)args;
    return;
}
#endif   // 0



#if 1 // ********** REMOVE #if 0 to enable this example callback
extern void MPMW_SRV_LOG(int app , int loglevel,const char *format,va_list args);

/********************************************************************
* FUNCTION log_vendor_send_fn
*
* Vendor function to consume log output (mandatory)
*
* INPUTS:
*   app == "Facility" (Yangcli or Netconfd)
*   level == Logging level (error, warn, info, debug, ..., debug4)
*   fstr == format string (like printf)
*   args == variable argument list
*
* RETURNS:
*   void
*********************************************************************/
static void
    log_vendor_send_fn (log_debug_app_t app, log_debug_t level,
            const char *fstr, va_list args)
{
	MPMW_SRV_LOG(app , level,fstr,args);
    return;
}
#endif   // 0



/***********  Example External NV-Storage Handler Hooks **********/


#define EXAMPLE_CONFIG_SPEC (const xmlChar *)"/tmp/example-config.xml"


#if 0 // ********** REMOVE #if 0 to enable this example callback
/* nvsave callback
 * this callback is invoked when some config needs to be saved
 * to non-volatile storage
 *
 * INPUTS:
 *    encoding == encoding format for the config (xml only allowed value)
 *    filespec == filespec containing the config to save
 * RETURNS:
 *    status; error indicates NV-save failed somehow
 */
static status_t
  example_nvsave (ncx_display_mode_t encoding,
                  const xmlChar *filespec)
{
    status_t res = NO_ERR;

    if (filespec == NULL || *filespec == 0) {
        res = ERR_NCX_INVALID_VALUE;
    } else if (encoding != NCX_DISPLAY_MODE_XML) {
        res = ERR_NCX_INVALID_VALUE;
    } else {
        res = ncxmod_copy_text_file(filespec, EXAMPLE_CONFIG_SPEC);
    }

    return res;

}  /* example_nvsave */


/* nvload callback
 * this callback is invoked when some config needs to be read
 * from non-volatile storage
 *
 * INPUTS:
 *    encoding == address of encoding for the config
 *    filespec == address of filespec containing the config that was loaded
 *
 * OUTPUTS:
 *    *encoding == set to the enum for the encoding used in the config
 *    *filespec == malloced filespec containing the config that was loaded
 *
 * RETURNS:
 *    status; error indicates NV-load failed somehow
 *    If return NO_ERR and *filespec == NULL then use the factory config
 */
static status_t
    example_nvload (ncx_display_mode_t *encoding,
                    xmlChar **filespec)
{
    log_debug("\nEnter example_nvload");

    *filespec = NULL;
    *encoding = NCX_DISPLAY_MODE_XML;

    status_t res = NO_ERR;

    if (ncxmod_test_filespec(EXAMPLE_CONFIG_SPEC)) {
        /* file exists so copy the filespec */
        *filespec = xml_strdup(EXAMPLE_CONFIG_SPEC);
        if (*filespec == NULL) {
            res = ERR_INTERNAL_MEM;
        }
    }

    return res;

}  /* example_nvload */
#endif   // 0


/***********  Example Transaction Management Hooks **********/

// set the defpath for the object
#define EXAMPLE_DEFPATH (const xmlChar *)"/test:some-object"

#if 0 // ********** REMOVE #if 0 to enable this example callback

/* SET/TRANSACTION HOOK callback
 * Callback function for server object handler
 * Used to provide a callback for a specific named object
 *
 * Set-Hook: Similar to the EDIT-1 or EDIT-2 callbacks,
 * except this callback is only invoked once at the start
 * of the edit for a particular data node.
 * This callback can alter the data in the current edit and
 * also add new edits to the current transaction.
 * This callback will be invoked before a EDIT-1 or EDIT2
 * callback for the same object.
 *
 * Transaction-Hook: Similar to a Set-Hook except this
 * callback is invoked just before the data is committed
 * to the running datastore. This callback will be invoked
 * after a EDIT-1 or EDIT2 callbacks for the same object.
 *
 * INPUTS:
 *   scb == session control block making the request
 *   msg == incoming rpc_msg_t in progress
 *   txcb == transaction control block in progress
 *   editop == edit operation enumeration for the node being edited
 *   newval == container object holding the proposed changes to
 *           apply to the current config, depending on
 *           the editop value. Will not be NULL.
 *   curval == current container values from the <running>
 *           or <candidate> configuration, if any. Could be NULL
 *           for create and other operations.
 *
 * RETURNS:
 *    status
 */
static status_t
    example_hook_edit (ses_cb_t *scb,
                       rpc_msg_t *msg,
                       agt_cfg_transaction_t *txcb,
                       op_editop_t editop,
                       val_value_t  *newval,
                       val_value_t  *curval)
{
    log_debug("\nEnter example_hook_edit\n");

    status_t res = NO_ERR;

    val_value_t *errorval = (curval) ? curval : newval;
    const xmlChar *newval_val = (newval) ? VAL_STRING(newval) : 0;
    const xmlChar *curval_val = (curval) ? VAL_STRING(curval) : 0;

    val_value_t *edit_value =
        agt_make_leaf(setleaf_obj,
                      (const xmlChar *)"setleaf",
                      (const xmlChar *)"leafstrval",
                      &res);
    if (!edit_value) {
        return ERR_INTERNAL_MEM;
    }

    switch (editop) {
    case OP_EDITOP_LOAD:
        break;
    case OP_EDITOP_MERGE:
        break;
    case OP_EDITOP_REPLACE:
        break;
    case OP_EDITOP_CREATE:
        if (edit_value && newval_val &&
            !xml_strcmp(newval_val, (const xmlChar *)"example")) {

            if (res == NO_ERR) {
                res = agt_val_add_edit(txcb,
                                       (const xmlChar *)"/hookt:setleaf",
                                       edit_value,
                                       editop);
            }
        }
        break;
    case OP_EDITOP_DELETE:
        if (curval_val &&
            !xml_strcmp(curval_val, (const xmlChar *)"example")) {

            if (res == NO_ERR) {
                res = agt_val_add_edit(txcb,
                                       (const xmlChar *)"/hookt:setleaf",
                                       NULL,
                                       editop);
            }
        }
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_CONTENT,
            res,
            NULL,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval,
            (errorval) ? NCX_NT_VAL : NCX_NT_NONE,
            errorval);
    }

    return res;
}  /* example_hook_edit */


/* Start Transaction callback
 * The Start Transaction function is the user/system
 * callback that is invoked before any changes to the
 * candidate database will be committed.
 *
 * Max Callbacks: 4 (set by AGT_CB_MAX_TRANSACTION_CBFN)
 * 1 Per SIL
 *
 * INPUTS:
 *   txcb == transaction control block in progress
 *
 * RETURNS:
 *   status
 */
static status_t
    example_transaction_start (agt_cfg_transaction_t *txcb)
{
    (void)txcb;
    log_debug("\nEnter transaction_start example");
    return NO_ERR;

}  /* example_transaction_start */


/* Complete Transaction callback
 * The Transaction Complete function is the
 * user/system callback that is invoked after
 * the transactions has been processed.
 *
 * Max Callbacks: 4 (set by AGT_CB_MAX_TRANSACTION_CBFN)
 * 1 Per SIL
 *
 * INPUTS:
 *   txcb == transaction control block in progress
 *
 * RETURNS:
 *   none
 */
static void
    example_transaction_complete (agt_cfg_transaction_t *txcb)
{
    (void)txcb;
    log_debug("\nEnter transaction_complete example");
    return;

}  /* example_transaction_complete */

#endif   // 0



/***********  Example YANG Object Template Callback **********/

/*
 * Callback is invoked to check extensions in an obj_template_t
 * representing YANG data nodes
 *
 * Assume YANG module foo exists with extension acme1
 *
 * module foo {
 *   prefix f;
 *   ...
 *   extension acme1 { ... }
 *
 *
 * The extension is used inside object definitions. e.g:
 *
 *    leaf X {
 *      f:acme1;
 *      type string;
 *    }
 *
 * Assume there is a vendor bit defined for the user flags field
 */

#define FL_ACME_1  0x1


#if 0 // ********** REMOVE #if 0 to enable this example callback
/* user function callback template when a YANG object is
 * parsed by yang_obj.c. This API is invoked at the
 * end of the resolve phase if the status is NO_ERR
 * It is skipped if the object has errors detected at the time
 *
 * ncx_yang_obj_cbfn_t
 *
 *  Run an instrumentation-defined function
 *  for a 'object parsed' event
 *
 * INPUTS:
 *   mod == module that is being parsed now
 *   obj == object being parsed
 */
static void
    example_obj_template_cbfn (ncx_module_t *mod,
                               struct obj_template_t_ *obj)
{
    /* optional: use the module to check cetain module names to
     * pre-filter the callback
     */
    (void)mod;

    /* get the appinfoQ for the object */
    dlq_hdr_t *appinfoQ = obj_get_appinfoQ(obj);
    if (appinfoQ == NULL) {
        return;   // error!
    }

    /* check the object template appinfoQ to see if the vendor
     * extensions are present
     */
    ncx_appinfo_t *appinfo =
        ncx_find_appinfo(appinfoQ,
                         (const xmlChar *)"f",
                         (const xmlChar *)"acme1");
    if (appinfo) {
        OBJ_USER_FLAGS(obj) |= FL_ACME_1;
    }

}
#endif  // 0


/***********  Example YANG Extension Handler Callback **********/

/*
 * Callback is invoked to check a specific extension in an
 * obj_template_t, typ_template_t, typ_def_t
 *
 * Assume the same YANG module foo exists with extension acme1
 *
 * The example callback does the same task as the
 * example_obj_template_cbfn, using the per-callback approach
 */

#if 0 // ********** REMOVE #if 0 to enable this example callback
/* One YANG Extension Handler Callback
 *
 * example_ext_cbfn
 *
 * Handle the parsing and processing of an external statement
 * using the associated YANG extension statement
 *
 * This callback is invoked when the external statement is
 * first encountered. The current token is the argument string
 * if any or the identifier token if none.
 * The next token is expected to be a semi-colon or a left brace
 * The callback is expected to parse the closing semi-colon or
 * entire sub-section including starting brace
 *
 * INPUTS:
 *   rawpcb == parser control block in progress (cast as void *)
 *   mod == module being processed
 *   tkc == token chain of module tokens parse in progress
 *   ext == extension definition record (allows a handler to
 *         process multiple extension types)
 *   cookie == cbfn_cookie from the extension 'ext'
 *   arg == argument string used in the external statement (if any)
 *   node_type == type of node being processed; direct parent
 *         statement of the external statement using the extension
 *         If NULL, then the parent statement is the module itself,
 *         and 'mod' should be used as the 'node' pointer
 *   node == pointer to node indicated by node_type
 * OUTPUTS:
 *
 * RETURNS:
 *   status of processing
 */
static status_t
    example_ext_cbfn (void *rawpcb,  // struct yang_pcb_t_ *pcb
                      ncx_module_t *mod,
                      tk_chain_t *tkc,
                      struct ext_template_t_ *ext,
                      void *cookie,
                      const xmlChar *arg,
                      ncx_node_t node_type,
                      void *node)
{
    (void)rawpcb;
    (void)mod;
    (void)tkc;
    (void)ext;
    (void)cookie;
    (void)arg;

    /* ignore this extension in all contexts except object template */
    if (node_type != NCX_NT_OBJ) {
        return NO_ERR;
    }

    /* get the object template */
    obj_template_t *obj = (obj_template_t *)node;

    /* set the acme1 bit */
    OBJ_USER_FLAGS(obj) |= FL_ACME_1;

    return NO_ERR;
}
#endif  // 0


#if 0 // ********** REMOVE #if 0 to enable this example callback
/* One RPC Command Complete Handler Callback
*
* example_command_complete_cbfn
*
*
* The Command Complete callback is the user/system callback
* that is invoked after each client command is executed
* for the NETCONF or RESTCONF protocols.
*
* The Command Complete is typically used for retrieval
* operations (get, get-config, get-bulk) to release resources
* used during GET2 callbacks invoked during the operation.
*
* Max Callbacks: 4 (set by AGT_CB_MAX_COMMAND_COMPLETE_CBFN)
*
* INPUTS:
*   scb == session control block making the request
*   msg == incoming rpc_msg_t in progress
*   command_modname == YANG module name of command that is completed
*   command_name == YANG RPC object name of command that is completed
*
* OUTPUTS:
*    none
*
* RETURNS:
*    none
*/
static void
    example_command_complete_cbfn (ses_cb_t *scb,
                                   rpc_msg_t *msg,
                                   const xmlChar *command_modname,
                                   const xmlChar *command_name)
{
    (void)command_modname;

    if (!xml_strcmp(command_name, (const xmlChar *)"get") ||
        !xml_strcmp(command_name, (const xmlChar *)"get-config") ||
        !xml_strcmp(command_name, (const xmlChar *)"get-bulk")) {

        /* cleanup our internal get cache data
         * example_clean_get2_cache(scb, msg);
         */
    }

}
#endif  // 0


/****************  Required System Library Hooks *******************/

/* system init server profile callback
 *
 * Initialize the server profile if needed
 *
 * INPUTS:
 *  profile == server profile to change if needed
 */
void yp_system_init_profile (agt_profile_t *profile)
{
    (void) profile;
    log_debug("\nyp_system init profile\n");
    /* example: use an external ACM module */
    //profile->agt_acm_model = AGT_ACM_MODEL_EXTERNAL;

    /* example: set the with-defaults also-supported bits
     * from agt/agt.h:
     * bitmask of the with-defaults enumerations that should be
     * enabled in the server
     *    explicit: bit0
     *    trim: bit1
     *    report-all: bit2
     *    report-all-tagged: bit3
     *
     * uint8 agt_withdef_enabled;
     *
     * The basic-mode (agt_defaultStyle and agt_defaultStyleEnum)
     * will be added by the server automatically, so a value of 0
     * will not enable any 'also-supported' retrieval modes
     * for the 'with-defaults' leaf
     */
    //profile->agt_withdef_enabled = bit1 | bit3;

    // add to enable callhome reconnect after a close-session
	profile->agt_callhome_reconnect = TRUE;		/* reconnect enabled by Fujitsu */
	log_debug("\nagt_callhome_reconnect TRUE\n");

}


/* system init1 callback
 * init1 system call
 * this callback is invoked twice; before and after CLI processing
 * INPUTS:
 * pre_cli == TRUE if this call is before the CLI parameters
 *            have been read
 *            FALSE if this call is after the CLI parameters
 *            have been read
 * RETURNS:
 *  status; error will abort startup
 */
status_t yp_system_init1 (boolean pre_cli)
{
    status_t res = NO_ERR;
    log_debug("\nyp_system init1\n");

    if (pre_cli) {
	// example -- register vendor callback to consume logging output.
        /* Note that output will not be re-directed to the vendor stream
         * until AFTER --log-vendor is parsed by CLI processing or log_vendor
         * is parsed by config file processing.
         * Uncomment the following 2 lines to enable!!
         */
        //log_vendor_init1_fn(pre_cli); /* Optional */
		log_vendor_extern_register_send_fn(log_vendor_send_fn);		/* Fujitsu modify */


    } else {
        /* example -- external NACM callbacks
         * load module for external module
         * with ncxmod_load_module
         * register the external ACM callbacks
         * this will have no affect unless the
         * yp_system_init_profile fn sets the
         * agt_acm_model to AGT_ACM_MODEL_EXTERNAL
         * uncomment the following function call to enable external ACM
         */
        //agt_acm_extern_register_callbacks(acm_extern_rpc,
        //                                  acm_extern_notif,
        //                                  acm_extern_write,
        //                                  acm_extern_read);

        /* example -- register vendor callback to consume logging output.
         * uncomment the following line to enable
         */
        //log_vendor_init1_fn(pre_cli); /* Optional */

        /* example -- register NV-storage handler to load/save config
         * uncomment following to enable
         */
        //res = agt_register_local_nv_handler(example_nvload,
        //                                    example_nvsave);

        /* example -- Register an object specific Set Hook callback.
         * uncomment following to enable
         */
        // res = agt_cb_hook_register(EXAMPLE_DEFPATH,
        //                            AGT_HOOKFMT_NODE,
        //                            AGT_HOOK_TYPE_SETHOOK,
        //                            example_hook_edit);

        // res = agt_cb_hook_register(EXAMPLE_DEFPATH,
        //                            AGT_HOOKFMT_NODE,
        //                            AGT_HOOK_TYPE_TRANSACTION,
        //                            example_hook_edit);

        /* example -- Register a Transaction Start callback.
         * uncomment following to enable
         */
        // res = agt_cb_trans_start_register(example_transaction_start);

        /* example -- Register a Transaction Complete callback.
         * uncomment following to enable
         */
        // res = agt_cb_trans_complete_register(example_transaction_complete);

        /* example -- Register a YANG Object Template Callback
         * uncomment following to enable
         */
        // res = ncx_set_yang_obj_callback(example_obj_template_cbfn);

        /* example -- Register an Extension Handler Callback
         * uncomment following to enable
         */
        /* res = ext_register_cbfn((const xmlChar *)"acme-ext",
         *                         (const xmlChar *)"acme1",
         *                         example_ext_cbfn,
         *                         NULL);   // cookie
         */

        /* Example: register a type-specific callback
         * In real usage at least 1 of the 3 callback functions
         * must be set
         */
        //res = typ_userdef_register(NCXMOD_IETF_INET_TYPES,
        //                           (const xmlChar *)"ipv6-address-no-zone",
        //                           NULL,   // validate_fn
        //                           my_ipv6_fn,   // canonical_fn
        //                           NULL,   // compare_fn
        //                           NULL);  // cookie

    }
    return res;

}  /* yp_system_init1 */


/* system init2 callback
 * init2 system call
 * this callback is invoked twice; before and after
 * load_running_config processing
 *
 * INPUTS:
 * pre_load == TRUE if this call is before the running config
 *            has been loaded
 *            FALSE if this call is after the running config
 *            has been loaded
 * RETURNS:
 *  status; error will abort startup
 */

status_t yp_system_init2 (boolean pre_load)
{
    log_debug("\nyp_system init2 %d\n",pre_load);

    if (pre_load) {
#ifdef WITH_NACM_GROUP
        /* example -- Register a NACM External Groups Callback
         * uncomment following to enable
         */
        agt_acm_ietf_register_group_cbfn(nacm_external_group_cbfn);
#endif  // WITH_NACM_GROUP

        // initialize a Command Complete callback
        // res = agt_cb_command_complete_register(
        //   example_command_complete_cbfn);
    } else {
        int ret = system("/opt/sh/cmd_mpsocat");
        log_debug("\nStarting M-Plane proxy mode(%d) \n",ret);
    }
    return NO_ERR;

}  /* yp_system_init2 */


/* system cleanup callback
 * this callback is invoked once during agt_cleanup
 */
void yp_system_cleanup (void)
{
    log_debug("\nyp_system cleanup\n");

    /* example -- Unregister an object specific Set Hook callback function
     * uncomment following to enable
     */
    // agt_cb_hook_unregister(EXAMPLE_DEFPATH);

    /* example -- Unregister a Transaction Start callback.
     * uncomment following to enable
     */
    // agt_cb_trans_start_unregister(example_transaction_start);

    /* example -- Unregister a Transaction Complete callback.
     * uncomment following to enable
     */
    // agt_cb_trans_complete_unregister(example_transaction_complete);


} /* yp_system_cleanup */


/* END example-system.c */
