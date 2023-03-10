/*
 * @file    i_o-ran-supervision.h
 */

#ifndef _H_i_o_ran_supervision
#define _H_i_o_ran_supervision


/*!
 * @addtogroup  SAA_MOD_O_RAN_SUPERVISION
 * @brief       SAA o-ran-supervision
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "f_mpsw_msg_o-ran_operations_def.h"
#include "f_mpsw_msg_o-ran_operations_typ.h"
/********************************************************************************************************************/
/*  define                                                                                                          */
/********************************************************************************************************************/
#define D_O_RAN_SUPERVISION_NOTIFICATION_INTERVAL    60
#define D_O_RAN_SUPERVISION_GUARD_TIMER              10

#define D_O_RAN_SUPERVISION_SIVAL_INT_NTF            1
#define D_O_RAN_SUPERVISION_SIVAL_INT_RPC            2

#define D_TIME_YEAR_OFFSET  1900
#define D_TIME_MON_OFFSET   1
/********************************************************************************************************************/
/*  variable                                                                                                        */
/********************************************************************************************************************/

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/
/* rpc???????????? */
extern status_t i_o_ran_supervision_supervision_watchdog_reset_invoke(uint16 v_supervision_notification_interval, uint16 v_guard_timer_overhead, char *next_update_at);

/* guard?????????timeout?????? */
extern void i_o_ran_supervision_supervision_watchdog_reset_timeout(sigval_t arg);

/* notification????????????????????? */
extern status_t i_o_ran_supervision_supervision_notification_start(void);

/* notification?????????timeout?????? */
extern void i_o_ran_supervision_supervision_notification_timeout(sigval_t arg);

/* O_RAN_OPERATION_RESET_IND???????????? */
extern status_t i_o_ran_supervision_send_reset_msi (E_RESTART_CAUSE reset_cause);

/* ??????????????? */
extern status_t i_o_ran_supervision_notification_operation(uint16 second);
extern status_t i_o_ran_supervision_guard_operation(uint16 second);
extern status_t i_o_ran_supervision_timer_create(int v_sival_int, void (* handler)(sigval_t arg), timer_t *v_timer_id);
extern status_t i_o_ran_supervision_timer_settime(timer_t v_timer_id, uint16 second);
extern status_t i_o_ran_supervision_notification_timer_delete(void);
extern status_t i_o_ran_supervision_guard_timer_delete(void);

/* notification???????????? */
extern void i_o_ran_supervision_supervision_notification_handler(void *data, uint32_t size);

#endif
