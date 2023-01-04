
#ifndef _H_y_o_ran_file_management
#define _H_y_o_ran_file_management
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
    module o-ran-file-management
    revision 2019-02-04
    namespace urn:o-ran:file-management:1.0
    organization O-RAN Alliance
    Created: 2019-12-20T02:20:28Z
    CLI parameters:
        format yh
        indent 4
        module o-ran-file-management
        output y_o-ran-file-management.h
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

#define y_o_ran_file_management_M_o_ran_file_management (const xmlChar *)"o-ran-file-management"

#define y_o_ran_file_management_R_o_ran_file_management (const xmlChar *)"2019-02-04"

#define y_o_ran_file_management_N_certificate (const xmlChar *)"certificate"
#define y_o_ran_file_management_N_credentials (const xmlChar *)"credentials"
#define y_o_ran_file_management_N_file_download (const xmlChar *)"file-download"
#define y_o_ran_file_management_N_file_download_event (const xmlChar *)"file-download-event"
#define y_o_ran_file_management_N_file_list (const xmlChar *)"file-list"
#define y_o_ran_file_management_N_file_name_filter (const xmlChar *)"file-name-filter"
#define y_o_ran_file_management_N_file_upload (const xmlChar *)"file-upload"
#define y_o_ran_file_management_N_file_upload_notification (const xmlChar *)"file-upload-notification"
#define y_o_ran_file_management_N_local_logical_file_path (const xmlChar *)"local-logical-file-path"
#define y_o_ran_file_management_N_logical_path (const xmlChar *)"logical-path"
#define y_o_ran_file_management_N_password (const xmlChar *)"password"
#define y_o_ran_file_management_N_reject_reason (const xmlChar *)"reject-reason"
#define y_o_ran_file_management_N_remote_file_path (const xmlChar *)"remote-file-path"
#define y_o_ran_file_management_N_retrieve_file_list (const xmlChar *)"retrieve-file-list"
#define y_o_ran_file_management_N_status (const xmlChar *)"status"


/********************************************************************
* FUNCTION y_o_ran_file_management_init
*
* initialize the o-ran-file-management server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_file_management_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_o_ran_file_management_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_file_management_init2 (void);

/********************************************************************
* FUNCTION y_o_ran_file_management_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_o_ran_file_management_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif