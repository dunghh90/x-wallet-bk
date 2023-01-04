
#ifndef _H_y_o_ran_performance_management
#define _H_y_o_ran_performance_management
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
    module o-ran-performance-management
    revision 2019-02-04
    namespace urn:o-ran:performance-management:1.0
    organization O-RAN Alliance
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

#define y_o_ran_performance_management_M_o_ran_performance_management (const xmlChar *)"o-ran-performance-management"

#define y_o_ran_performance_management_R_o_ran_performance_management (const xmlChar *)"2019-02-04"

#define y_o_ran_performance_management_N_EAXC_ID (const xmlChar *)"EAXC_ID"
#define y_o_ran_performance_management_N_RU (const xmlChar *)"RU"
#define y_o_ran_performance_management_N_TRANSPORT (const xmlChar *)"TRANSPORT"
#define y_o_ran_performance_management_N_active (const xmlChar *)"active"
#define y_o_ran_performance_management_N_bin_count (const xmlChar *)"bin-count"
#define y_o_ran_performance_management_N_certificate (const xmlChar *)"certificate"
#define y_o_ran_performance_management_N_count (const xmlChar *)"count"
#define y_o_ran_performance_management_N_credentials (const xmlChar *)"credentials"
#define y_o_ran_performance_management_N_eaxc_id (const xmlChar *)"eaxc-id"
#define y_o_ran_performance_management_N_eaxc_measured_result (const xmlChar *)"eaxc-measured-result"
#define y_o_ran_performance_management_N_enable_SFTP_upload (const xmlChar *)"enable-SFTP-upload"
#define y_o_ran_performance_management_N_enable_random_file_upload (const xmlChar *)"enable-random-file-upload"
#define y_o_ran_performance_management_N_end_time (const xmlChar *)"end-time"
#define y_o_ran_performance_management_N_file_upload_interval (const xmlChar *)"file-upload-interval"
#define y_o_ran_performance_management_N_first (const xmlChar *)"first"
#define y_o_ran_performance_management_N_frequeny_table (const xmlChar *)"frequeny-table"
#define y_o_ran_performance_management_N_function (const xmlChar *)"function"
#define y_o_ran_performance_management_N_latest (const xmlChar *)"latest"
#define y_o_ran_performance_management_N_lower_bound (const xmlChar *)"lower-bound"
#define y_o_ran_performance_management_N_max (const xmlChar *)"max"
#define y_o_ran_performance_management_N_max_bin_count (const xmlChar *)"max-bin-count"
#define y_o_ran_performance_management_N_measurement_object (const xmlChar *)"measurement-object"
#define y_o_ran_performance_management_N_measurement_result_stats (const xmlChar *)"measurement-result-stats"
#define y_o_ran_performance_management_N_min (const xmlChar *)"min"
#define y_o_ran_performance_management_N_name (const xmlChar *)"name"
#define y_o_ran_performance_management_N_notification_interval (const xmlChar *)"notification-interval"
#define y_o_ran_performance_management_N_object_unit (const xmlChar *)"object-unit"
#define y_o_ran_performance_management_N_object_unit_id (const xmlChar *)"object-unit-id"
#define y_o_ran_performance_management_N_password (const xmlChar *)"password"
#define y_o_ran_performance_management_N_performance_measurement_objects (const xmlChar *)"performance-measurement-objects"
#define y_o_ran_performance_management_N_remote_SFTP_upload_path (const xmlChar *)"remote-SFTP-upload-path"
#define y_o_ran_performance_management_N_remote_SFTP_uploads (const xmlChar *)"remote-SFTP-uploads"
#define y_o_ran_performance_management_N_report_info (const xmlChar *)"report-info"
#define y_o_ran_performance_management_N_rx_window_measurement_interval (const xmlChar *)"rx-window-measurement-interval"
#define y_o_ran_performance_management_N_rx_window_measurement_objects (const xmlChar *)"rx-window-measurement-objects"
#define y_o_ran_performance_management_N_rx_window_stats (const xmlChar *)"rx-window-stats"
#define y_o_ran_performance_management_N_start_time (const xmlChar *)"start-time"
#define y_o_ran_performance_management_N_time (const xmlChar *)"time"
#define y_o_ran_performance_management_N_tr_measured_result (const xmlChar *)"tr-measured-result"
#define y_o_ran_performance_management_N_transceiver_measurement_interval (const xmlChar *)"transceiver-measurement-interval"
#define y_o_ran_performance_management_N_transceiver_measurement_objects (const xmlChar *)"transceiver-measurement-objects"
#define y_o_ran_performance_management_N_transceiver_measurement_result (const xmlChar *)"transceiver-measurement-result"
#define y_o_ran_performance_management_N_transceiver_stats (const xmlChar *)"transceiver-stats"
#define y_o_ran_performance_management_N_transport_name (const xmlChar *)"transport-name"
#define y_o_ran_performance_management_N_upper_bound (const xmlChar *)"upper-bound"
#define y_o_ran_performance_management_N_value (const xmlChar *)"value"


/********************************************************************
* FUNCTION y_o_ran_performance_management_init
*
* initialize the o-ran-performance-management server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_performance_management_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION y_o_ran_performance_management_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t y_o_ran_performance_management_init2 (void);

/********************************************************************
* FUNCTION y_o_ran_performance_management_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void y_o_ran_performance_management_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
