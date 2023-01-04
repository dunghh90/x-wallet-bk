/*
 * @file    i_o-ran-mplane-int.h
 */

#ifndef _H_i_o_ran_mplane_int
#define _H_i_o_ran_mplane_int


/*!
 * @addtogroup  SAA_MOD_O_RAN_MPLANE_INT
 * @brief       SAA o-ran-mplane-int
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <unistd.h>
#include "db_api.h"
#include "ycontrol.h"
#include "u_o-ran-mplane-int.h"

#include "f_mpsw_msg_o-ran_mplane_int_def.h"
#include "f_mpsw_msg_o-ran_mplane_int_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define i_mod_o_ran_mplane_int               "o-ran-mplane-int"/* module_name */
#define i_para_o_ran_mplane_int_num_of_vlan  "num_of_seachable_vlan_id"  /* parameter_name */

#define D_O_RAN_MPLANE_INT_MAX_VLANS     4094    /* vlan-id最大数 */
#define D_O_RAN_MPLANE_INT_MIN_VLANS        1    /* vlan-id最小数 */
#define D_O_RAN_MPLANE_INT_VLAN_INFO_INDEFINITE  0xffff      /* 不定値(USHORT) */
#define D_O_RAN_MPLANE_INT_SSH_PORT_INDEFINITE   0xffffffff  /* 不定値(UINT)   */

#define D_O_RAN_MPLANE_INT_DB_SEND_MAX_LEN   262144  /* dbapi送信時の最大文字列長 */

#define D_O_RAN_MPLANE_INT_1BYTE_SHIFT     8       /* 1byteシフト演算用 */
#define D_O_RAN_MPLANE_INT_2BYTE_SHIFT     16      /* 2byteシフト演算用 */
#define D_O_RAN_MPLANE_INT_3BYTE_SHIFT     24      /* 3byteシフト演算用 */
#define D_O_RAN_MPLANE_INT_1BYTE_MASK      0xFF    /* 1byteマスク処理用 */

#define i_para_o_ran_mplane_int_call_home_ssh_port  "mplane-info.m-plane-interfaces.m-plane-ssh-ports.call-home-ssh-port"  /* parameter_name */
#define i_para_o_ran_mplane_int_server_ssh_port  "mplane-info.m-plane-interfaces.m-plane-ssh-ports.server-ssh-port"  /* parameter_name */

/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/


/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* init処理 */
extern status_t i_o_ran_mplane_int_init2(void);
extern void i_o_ran_mplane_int_msi_table_init(void);
/* validate処理 */
extern status_t i_o_ran_mplane_int_vlan_range_validate(uint16 vlan_id);
extern status_t i_o_ran_mplane_int_searchable_access_vlan_validate(uint16 v_searchable_access_vlan);
extern void i_o_ran_mplane_int_lowest_vlan_id_validate(uint16 v_lowest_vlan_id);
extern void i_o_ran_mplane_int_highest_vlan_id_validate(uint16 v_highest_vlan_id);
extern status_t i_o_ran_mplane_int_call_home_ssh_port_validate(uint16 v_call_home_ssh_port);
extern status_t i_o_ran_mplane_int_server_ssh_port_validate(uint16 v_server_ssh_port);
/* commit処理 */
extern status_t i_o_ran_mplane_int_searchable_access_vlan_commit(void);
extern status_t i_o_ran_mplane_int_lowest_vlan_id_commit(void);
extern status_t i_o_ran_mplane_int_highest_vlan_id_commit(void);
extern status_t i_o_ran_mplane_int_call_home_ssh_port_commit(void);
extern status_t i_o_ran_mplane_int_server_ssh_port_commit(void);
/* MSI送信 */
extern status_t i_o_ran_mplane_int_send_msi(void);
extern status_t i_o_ran_mplain_int_get_send_msi(void *get_data);
/* initデータ取得 */
extern status_t i_o_ran_mplane_int_get_init_data (char *init_param_name, char **init_data);
/* DB更新 */
extern status_t i_o_ran_mplane_int_send_edit_config (void);

#endif
