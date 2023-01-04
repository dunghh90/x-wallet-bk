/*
 * @file    i_o-ran-lbm.h
 */

#ifndef _H_i_o_ran_lbm
#define _H_i_o_ran_lbm


/*!
 * @addtogroup  SAA_MOD_O_RAN_LBM
 * @brief       SAA o-ran-lbm
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include "u_o-ran-mplane-int.h"

#include "f_mpsw_msg_o-ran_lbm_def.h"
#include "f_mpsw_msg_o-ran_lbm_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* 初期化 */
extern void i_o_ran_lbm_config_table_init(void);
/* nameのedit */
extern status_t i_o_ran_lbm_maintenance_domain_name_edit(const xmlChar *k_maintenance_domain_id, const xmlChar *v_name, agt_cbtyp_t cbtyp);
/* md-levelのedit */
extern status_t i_o_ran_lbm_maintenance_domain_md_level_edit(const xmlChar *k_maintenance_domain_id, int32 v_md_level, agt_cbtyp_t cbtyp);
/* msiの送信 */
extern status_t i_o_ran_lbm_send_msi(int count);

#endif
