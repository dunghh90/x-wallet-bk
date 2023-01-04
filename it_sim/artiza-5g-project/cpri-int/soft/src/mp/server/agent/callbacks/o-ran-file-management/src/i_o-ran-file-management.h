#ifndef I_O_RAN_FILE_MANAGEMENT_H
#define I_O_RAN_FILE_MANAGEMENT_H

/*!
 * @addtogroup  SAA_MOD_O_RAN_FILE_MANAGEMENT
 * @brief       SAA o-ran-file-management
 * @note        
 * @{
 */

#include "agt_util.h"
#include "f_mpsw_msg_o-ran_file_management_typ.h"

#define D_FLM_MAX_FILE_NUM 1024
#define D_FLM_MAX_MESSAGE_LEN 64

/* intialize */
extern void i_o_ran_file_management_init2(void);

/* rpc */
extern int i_o_ran_file_management_file_upload_invoke (
                const xmlChar *local_logical_file_path,
                const xmlChar *remote_file_path,
                const xmlChar *password, xmlChar *message);
extern int i_o_ran_file_management_retrieve_file_list_invoke(
                const xmlChar *logical_path,
                const xmlChar *file_name_filter,
                xmlChar ***file_list, xmlChar *message);

/* utilities */
extern int i_o_ran_file_management_file_upload_validate (
                const xmlChar *local_logical_file_path,
                const xmlChar *remote_file_path,
                const xmlChar *password);
extern int i_o_ran_file_management_retrieve_file_list_validate(
                const xmlChar *logical_path,
                const xmlChar *file_name_filter);
#endif

