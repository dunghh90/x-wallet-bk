
#ifndef _H_u_o_ran_hardware
#define _H_u_o_ran_hardware
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
    module o-ran-hardware
    revision 2019-02-04
    namespace urn:o-ran:hardware:1.0
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
#include "dlq.h"
#include "ncxtypes.h"
#include "op.h"
#include "rpc.h"
#include "ses.h"
#include "status.h"
#include "val.h"
#include "y_o-ran-hardware.h"
#include "u_ietf-yang-types.h"
#include "u_ietf-inet-types.h"
#include "u_ietf-interfaces.h"
#include "u_iana-if-type.h"
#include "u_ietf-ip.h"
#include "u_iana-hardware.h"
#include "u_ietf-hardware.h"
#include "u_ietf-netconf-acm.h"
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
#include "u_o-ran-supervision.h"
#include "u_o-ran-beamforming.h"
#include "u_o-ran-operations.h"
#include "u_o-ran-usermgmt.h"
#include "u_o-ran-sync.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Feature o-ran-hardware:ENERGYSAVING
 * Comment out to disable */
#define u_o_ran_hardware_F_ENERGYSAVING 1



/* container /hardware/component/label-content */
typedef struct y_o_ran_hardware_T_hardware_component_label_content_ {
    boolean v_model_name;
    boolean v_serial_number;
} y_o_ran_hardware_T_hardware_component_label_content;


#ifdef u_o_ran_hardware_F_ENERGYSAVING
/********************************************************************
* FUNCTION u_o_ran_hardware_hardware_component_state_power_state_get
*
* Get database object callback for leaf power-state
* Path: /hardware/component/state/power-state
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_hardware_component_state_power_state_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_hardware_component_name);

#endif /* u_o_ran_hardware_F_ENERGYSAVING */
/********************************************************************
* FUNCTION u_o_ran_hardware_hardware_component_state_availability_state_get
*
* Get database object callback for leaf availability-state
* Path: /hardware/component/state/availability-state
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_hardware_component_state_availability_state_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_hardware_component_name);

/********************************************************************
* FUNCTION u_o_ran_hardware_hardware_component_label_content_get
*
* Get database object callback for container label-content
* Path: /hardware/component/label-content
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_hardware_component_label_content_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_hardware_component_name);

/********************************************************************
* FUNCTION u_o_ran_hardware_hardware_component_product_code_get
*
* Get database object callback for leaf product-code
* Path: /hardware/component/product-code
* Fill in 'get2cb' response fields
*
* INPUTS:
*     see ncx/getcb.h for details (getcb_fn2_t)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_hardware_component_product_code_get (
    getcb_get2_t *get2cb,
    const xmlChar *k_hardware_component_name);


#ifdef u_o_ran_hardware_F_ENERGYSAVING
/********************************************************************
* FUNCTION u_o_ran_hardware_hardware_component_energy_saving_enabled_edit
*
* Edit database object callback
* Path: /hardware/component/energy-saving-enabled
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_hardware_component_energy_saving_enabled_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_hardware_component_name);

#endif /* u_o_ran_hardware_F_ENERGYSAVING */

/********************************************************************
* FUNCTION u_o_ran_hardware_hardware_component_o_ran_name_edit
*
* Edit database object callback
* Path: /hardware/component/o-ran-name
* Add object instrumentation in COMMIT phase.
*
* INPUTS:
*     see agt/agt_cb.h for details
*     k_ parameters are ancestor list key values.
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_hardware_component_o_ran_name_edit (
    ses_cb_t *scb,
    rpc_msg_t *msg,
    agt_cbtyp_t cbtyp,
    op_editop_t editop,
    val_value_t *newval,
    val_value_t *curval,
    const xmlChar *k_hardware_component_name);

/********************************************************************
* FUNCTION u_o_ran_hardware_init
*
* initialize the o-ran-hardware server instrumentation library
*
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_init (
    const xmlChar *modname,
    const xmlChar *revision);

/********************************************************************
* FUNCTION u_o_ran_hardware_init2
*
* SIL init phase 2: non-config data structures
* Called after running config is loaded
*
* RETURNS:
*     error status
********************************************************************/
extern status_t u_o_ran_hardware_init2 (void);

/********************************************************************
* FUNCTION u_o_ran_hardware_cleanup
*    cleanup the server instrumentation library
*
********************************************************************/
extern void u_o_ran_hardware_cleanup (void);

#ifdef __cplusplus
} /* end extern 'C' */
#endif

#endif
