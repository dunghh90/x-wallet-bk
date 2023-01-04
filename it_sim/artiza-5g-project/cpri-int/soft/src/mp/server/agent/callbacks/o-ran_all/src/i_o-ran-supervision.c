
/*
 * @file    i_o-ran-supervision.c
 */

/*!
 * @addtogroup  SAA_MOD_O_RAN_SUPERVISION
 * @brief       SAA o-ran-supervision
 * @note        
 * @{
 */

/********************************************************************************************************************/
/*  include file                                                                                                    */
/********************************************************************************************************************/
#include "u_o-ran-supervision.h"
#include "i_o-ran-supervision.h"

#include "f_mpsw_msg_o-ran_supervison_def.h"
#include "f_mpsw_msg_o-ran_supervison_typ.h"

#include "f_saa_com_init.h"
#include "f_saa_msg_buffer.h"
#include "f_saa_ntf_lib.h"
#include "f_saa_utc_lib.h"

struct supervision_t {
    bool rpc_receive_state;
    bool guard_time_create_state;
    bool notification_time_create_state;
    uint16 guard_timer;
    uint16 notification_interval;
    timer_t guard_timerid;
    timer_t notification_timerid;
};

static struct supervision_t supervision_state;
static pthread_mutex_t mutex_for_supervision_state = PTHREAD_MUTEX_INITIALIZER;
static bool supervision_start_ind = false;

/********************************************************************************************************************/
/*  function                                                                                                        */
/********************************************************************************************************************/

/*!
 * @brief   rpc watchdog_reset処理関数
 * @note    rpc watchdog_reset処理を実施する
 * @param   [in] v_supervision_notification_interval
 * @param   [in] v_guard_timer_overhead
 * @date    2019/02/12 KCN)南條
 */
status_t i_o_ran_supervision_supervision_watchdog_reset_invoke(uint16 v_supervision_notification_interval, uint16 v_guard_timer_overhead, char *next_update_at)
{
    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
        log_dev1("\n%s: notification_interval:%ds", __func__, v_supervision_notification_interval);
        log_dev1("\n%s: guard_timer:%ds", __func__, v_guard_timer_overhead);
    }

    status_t res = NO_ERR;
    int rc = D_SYS_OK;
    struct tm local_tm;
    time_t timer;

    /* guardタイマ削除 */
    res = i_o_ran_supervision_guard_timer_delete();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_delete(guard) is failed. res=%d", __func__, res);
        }
    }

    /* notificationタイマ削除 */
    res = i_o_ran_supervision_notification_timer_delete();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_delete(notification) is failed. res=%d", __func__, res);
        }
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_supervision_state);

    /* notificationタイマ起動 */
    res = i_o_ran_supervision_notification_operation(v_supervision_notification_interval);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: i_o_ran_supervision_notification_operation is failed. res=%d", __func__, res);
        }
    }
    else
    {
        /* rpc input保持 */
        supervision_state.rpc_receive_state = true;
        supervision_state.guard_timer = v_guard_timer_overhead;
        supervision_state.notification_interval = v_supervision_notification_interval;

        /* 次回notification通知時刻作成 */
        /* 現在時刻 + notificationタイマ */
        timer = time(&timer);
        /* エラーチェック */
        /* エラーの場合はnotification通知を実施しない */
        if(timer == -1)
        {
            /* unlock */
            pthread_mutex_unlock(&mutex_for_supervision_state);
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: time() is failed.", __func__);
            }
            res = ERR_NCX_OPERATION_FAILED;
            return res;
        }

        timer += supervision_state.notification_interval;
        /* 時刻取得 */
        localtime_r(&timer, &local_tm);
        /* yyyy-mm-ddThh:mm:ss.0000+00:00 */
        rc = f_saa_com_utc_to_offset_string(
            (uint16_t)(local_tm.tm_year + D_TIME_YEAR_OFFSET),
            (uint8_t)(local_tm.tm_mon + D_TIME_MON_OFFSET),
            (uint8_t)local_tm.tm_mday,
            (uint8_t)local_tm.tm_hour,
            (uint8_t)local_tm.tm_min,
            (uint8_t)local_tm.tm_sec,
            next_update_at);
        if(rc != D_SYS_OK)
        {
            if(LOGWARN) {
                log_warn("\n%s: f_saa_com_utc_to_offset_string() failed.", __func__);
            }
            res = ERR_NCX_OPERATION_FAILED;
            return res;
        }
        /* 初回のrpcのみ実施する */
        if(supervision_start_ind == false){
           (void)f_saa_com_add_request(D_MSG_MSI_O_RAN_SUPERVISION_NTF_START_IND, 0, NULL);
           supervision_start_ind = true;
        }
    }

    /* unlock */
    pthread_mutex_unlock(&mutex_for_supervision_state);

    return res;
} /* i_o_ran_supervision_supervision_watchdog_reset_invoke */

/*!
 * @brief   rpc watchdog_reset timeout関数
 * @note    rpc watchdog_reset timeout処理を実施する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_o_ran_supervision_supervision_watchdog_reset_timeout(sigval_t arg)
{
    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;

    (void)arg;

    res = i_o_ran_supervision_guard_timer_delete();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_delete(guard) is failed. res=%d", __func__, res);
        }
    }

    /* 装置リセット */
    res = i_o_ran_supervision_send_reset_msi(E_RESTART_CAUSE_SUPERVISION_WATCHDOG);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed. res=%d", __func__, res);
        }
    }

    return;
} /* i_o_ran_supervision_supervision_watchdog_reset_timeout */

/*!
 * @brief   supervision-notification 初回タイマ起動関数
 * @note    supervision-notification 初回タイマ起動を実施する
 * @param   [in] -
 * @date    2019/02/12 KCN)南條
 */
status_t i_o_ran_supervision_supervision_notification_start(void)
{
    status_t res = NO_ERR;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_supervision_state);
    /* 構造体初期化 */
    supervision_state.rpc_receive_state                 = false;
    supervision_state.guard_time_create_state           = false;
    supervision_state.notification_time_create_state    = false;
    supervision_state.guard_timer                       = D_O_RAN_SUPERVISION_GUARD_TIMER;
    supervision_state.notification_interval             = D_O_RAN_SUPERVISION_NOTIFICATION_INTERVAL;
    supervision_state.guard_timerid                     = 0;
    supervision_state.notification_timerid              = 0;

    /* notificationタイマ起動 */
    res = i_o_ran_supervision_notification_operation(supervision_state.notification_interval);
    /* unlock */
    pthread_mutex_unlock(&mutex_for_supervision_state);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: i_o_ran_supervision_notification_operation is failed. res=%d", __func__, res);
        }
    }

    return res;
} /* i_o_ran_supervision_supervision_notification_start */

/*!
 * @brief   notificationタイマ timeoutハンドラ
 * @note    notificationタイマ timeoutハンドラ
 * @param   [in] arg
 * @date    2019/02/12 KCN)南條
 */
void i_o_ran_supervision_supervision_notification_timeout(sigval_t arg)
{
    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    status_t res = NO_ERR;
    (void)arg;

    /* notificationタイマ削除 */
    res = i_o_ran_supervision_notification_timer_delete();
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_delete(notification) is failed. res=%d", __func__, res);
        }
    }

    /* notificationスレッドに登録 */
    int add_ret = D_MSG_BUFFER_ADD_OK;
    add_ret = f_saa_com_add_notification(D_MSG_MSI_O_RAN_SUPERVISION_NTF_TIMEOUT_IND, 0, NULL);
    if(add_ret == D_MSG_BUFFER_ADD_ERR) {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: f_saa_com_add_notification failed. messegeID:%d.", __func__, D_MSG_MSI_O_RAN_SUPERVISION_NTF_TIMEOUT_IND);
        }
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_supervision_state);

    /* rpc未受信の場合notificationタイマ起動 */
    if(!supervision_state.rpc_receive_state)
    {
        res = i_o_ran_supervision_notification_operation(supervision_state.notification_interval);
        if(res != NO_ERR)
        {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: i_o_ran_supervision_notification_operation is failed. res=%d", __func__, res);
            }
        }
    }
    else
    {
        /* guardタイマ起動 */
        res = i_o_ran_supervision_guard_operation(supervision_state.guard_timer);
        if(res != NO_ERR)
        {
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: i_o_ran_supervision_guard_operation is failed. res=%d", __func__, res);
            }
        }
    }

    /* unlock */
    pthread_mutex_unlock(&mutex_for_supervision_state);

    return;
} /* i_o_ran_supervision_supervision_notification_timeout */

/*!
 * @brief   supervision-notification timeout関数
 * @note    supervision-notification timeout処理を実施する
 * @param   [in] data
 * @param   [in] size
 * @date    2019/02/12 KCN)南條
 */
void i_o_ran_supervision_supervision_notification_handler(void *data, uint32_t size)
{
    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }
    (void)data;
    (void)size;

    /* notification通知 */
    u_o_ran_supervision_supervision_notification_send();

    return;
} /* i_o_ran_supervision_supervision_notification_handler */

/*!
 * @brief   notificationタイマ設定の動作一式を行う関数
 * @note    notificationタイマ設定の動作一式を行う
 * @param   [in] second
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/27 KCN)南條
 */
status_t i_o_ran_supervision_notification_operation(uint16 second)
{
    status_t res = NO_ERR;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    res = i_o_ran_supervision_timer_create(D_O_RAN_SUPERVISION_SIVAL_INT_NTF, i_o_ran_supervision_supervision_notification_timeout, &supervision_state.notification_timerid);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_create(notification) is failed. res=%d", __func__, res);
        }
        return res;
    }
    res = i_o_ran_supervision_timer_settime(supervision_state.notification_timerid, second);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_settime(notification) is failed. res=%d", __func__, res);
        }
        return res;
    }

    /* notification-time作成状態更新 */
    supervision_state.notification_time_create_state = true;

    return res;
}

/*!
 * @brief   guardタイマ設定の動作一式を行う関数
 * @note    guardタイマ設定の動作一式を行う
 * @param   -
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/27 KCN)南條
 */
status_t i_o_ran_supervision_guard_operation(uint16 second)
{
    status_t res = NO_ERR;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    res = i_o_ran_supervision_timer_create(D_O_RAN_SUPERVISION_SIVAL_INT_RPC, i_o_ran_supervision_supervision_watchdog_reset_timeout, &supervision_state.guard_timerid);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_create(guard) is failed. res=%d", __func__, res);
        }
        return res;
    }
    res = i_o_ran_supervision_timer_settime(supervision_state.guard_timerid, second);
    if(res != NO_ERR)
    {
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_settime(guard) is failed. res=%d", __func__, res);
        }
        return res;
    }

    /* guard-time作成状態更新 */
    supervision_state.guard_time_create_state = true;

    return res;
}

/*!
 * @brief   timer create関数
 * @note    timer create
 * @param   [out] *timer_id
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/27 KCN)南條
 */
status_t i_o_ran_supervision_timer_create(int v_sival_int, void (* handler)(sigval_t arg), timer_t *v_timer_id)
{
    status_t res = NO_ERR;
    struct sigevent    evp;
    timer_t timerid = 0;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    memset(&evp, 0, sizeof(evp));
    evp.sigev_notify            = (int)SIGEV_THREAD;
    evp.sigev_notify_function   = handler;
    evp.sigev_signo             = SIGRTMIN;
    evp.sigev_value.sival_int   = v_sival_int;

    if(timer_create(CLOCK_MONOTONIC, &evp, &timerid) < 0)
    {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_create failed.", __func__);
        }
        return res;
    }
    *v_timer_id = timerid;

    return res;
}

/*!
 * @brief   timer set関数
 * @note    timer set
 * @param   [in] v_timer_id
 * @param   [in] second
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/27 KCN)南條
 */
status_t i_o_ran_supervision_timer_settime(timer_t v_timer_id, uint16 second)
{
    status_t res = NO_ERR;
    struct itimerspec ispec;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    /* 繰り返しタイマーは無効 */
    ispec.it_interval.tv_sec = 0;
    ispec.it_interval.tv_nsec = 0;
    ispec.it_value.tv_sec = second;
    ispec.it_value.tv_nsec = 0;
    if(timer_settime(v_timer_id, 0, &ispec, NULL) < 0)
    {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: timer_settime failed.", __func__);
        }
    }

    return res;
}

/*!
 * @brief   notification timer delete関数
 * @note    notification timer delete
 * @param   -
 * @return  -
 * @date    2019/02/27 KCN)南條
 */
status_t i_o_ran_supervision_notification_timer_delete(void)
{
    status_t res = NO_ERR;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_supervision_state);
    /* notificationタイマ起動中の場合 */
    if(supervision_state.notification_time_create_state)
    {
        if(timer_delete(supervision_state.notification_timerid) < 0)
        {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: timer_delete failed.", __func__);
            }
            /* unlock */
            pthread_mutex_unlock(&mutex_for_supervision_state);
            return res;
        }
        supervision_state.notification_time_create_state = false;
    }
    /* unlock */
    pthread_mutex_unlock(&mutex_for_supervision_state);

    return res;
}

/*!
 * @brief   guard timer delete関数
 * @note    guard timer delete
 * @param   -
 * @return  -
 * @date    2019/02/27 KCN)南條
 */
status_t i_o_ran_supervision_guard_timer_delete(void)
{
    status_t res = NO_ERR;

    if (LOGDEV1) {
        log_dev1("\nEnter %s:", __func__);
    }

    /* lock */
    pthread_mutex_lock(&mutex_for_supervision_state);
    /* guardタイマ起動中の場合 */
    if(supervision_state.guard_time_create_state)
    {
        if(timer_delete(supervision_state.guard_timerid) < 0)
        {
            /* error */
            res = ERR_NCX_OPERATION_FAILED;
            /* error log */
            if(LOGWARN) {
                log_warn("\n%s: timer_delete failed.", __func__);
            }
            /* unlock */
            pthread_mutex_unlock(&mutex_for_supervision_state);
            return res;
        }
        supervision_state.guard_time_create_state = false;
    }
    /* unlock */
    pthread_mutex_unlock(&mutex_for_supervision_state);

    return res;
}

/*!
 * @brief   O_RAN_OPERATION_RESET_IND送信関数
 * @note    O_RAN_OPERATION_RESET_INDを送信する
 * @param   [in] reset_cause
 * @return  [正常]NO_ERR
 *          [異常]ERR_NCX_OPERATION_FAILED
 * @date    2019/02/07 KCN)南條
 */
status_t i_o_ran_supervision_send_reset_msi (E_RESTART_CAUSE reset_cause)
{
    status_t res = NO_ERR;
    T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND  req_data;
    uint32_t response_id = 0;

    if (LOGINFO) {
        log_info("\nEnter %s:", __func__);
    }

    req_data.restart_cause = reset_cause;
    /* O_RAN_OPERATION_RESET_IND送信 */
    response_id = f_saa_com_add_request(D_MSG_MSI_O_RAN_OPERATIONS_RESET_IND, sizeof(T_MPSW_MSI_O_RAN_OPERATIONS_RESET_IND), (void*)&req_data);
    if(response_id == 0) {
        /* error */
        res = ERR_NCX_OPERATION_FAILED;
        /* error log */
        if(LOGWARN) {
            log_warn("\n%s: MSI failed.: cannot add a request.", __func__);
        }
    }

    return res;
} /* i_o_ran_operations_send_reset_msi */
