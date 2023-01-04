#ifndef I_O_RAN_SOFTWARE_MANAGEMENT_H
#define I_O_RAN_SOFTWARE_MANAGEMENT_H

/*!
 * @addtogroup  SAA_MOD_O_RAN_SOFTWARE_MANAGEMENT
 * @brief       SAA o-ran-software-management
 * @note       
 * @{
 */


#include "agt_util.h"
#include "f_mpsw_msg_o-ran_software_management_typ.h"

#define D_SWM_MAX_MESSAGE_LEN  64

/* intialize */
extern void i_o_ran_software_management_init2(void);

/* get software-inventory */
extern T_MPSW_SOFTWARE_INVENTORY  * i_o_ran_software_management_inventory_update(void);

/* rpc */
extern int i_o_ran_software_management_software_download_invoke (
    const xmlChar *uri, const xmlChar *password, UINT *timeout, xmlChar *message);
extern int i_o_ran_software_management_software_install_invoke (
    const xmlChar *slot_name, const xmlChar *file_name, xmlChar *message);
extern int i_o_ran_software_management_software_activate_invoke (
    const xmlChar *slot_name, UINT *timeout, xmlChar *messages);

/* utilities */
extern int i_o_ran_software_management_file_find(int slot_indx, const xmlChar *name, boolean name_fixed, boolean name_present, boolean getnext, boolean *more_data);
extern int i_o_ran_software_management_slot_find(const xmlChar *name, boolean name_fixed, boolean name_present, boolean getnext, boolean *more_data);
extern xmlChar * i_o_ran_software_management_slot_status(int index);
extern xmlChar * i_o_ran_software_management_slot_access(int index);
extern xmlChar * i_o_ran_software_management_slot_file_name(int slot_indx, int file_indx);
extern xmlChar * i_o_ran_software_management_slot_file_version(int slot_indx, int file_indx);
extern xmlChar * i_o_ran_software_management_slot_file_local_path(int slot_indx, int file_indx);
extern xmlChar * i_o_ran_software_management_slot_file_integrity(int slot_indx, int file_indx);
extern boolean i_o_ran_software_management_slot_validate(const xmlChar *name);
extern int o_ran_software_management_get_download_param(rpc_msg_t *msg, xmlChar **p_remote_file_path, xmlChar **p_password);
extern boolean i_o_ran_software_management_download_validate(const xmlChar *remote_file_path, const xmlChar *password);

#endif

