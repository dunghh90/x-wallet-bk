/******************************************************************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_cpr_typ.h
 *  @brief  pf_cpri thread common structure head file defination
 *  @date   2013/11/25 FFCS)niemsh create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-
 */
/******************************************************************************************************************************/
/***************************************************************************************************************************//**
 *
 * @addtogroup RRH_PF_CPR
 * @{
 *
 ******************************************************************************************************************************/
#ifndef     F_CPR_TYP
#define     F_CPR_TYP

/** 走行履歴情報ヘッダ */
typedef struct
{
    UCHAR           setFlg;                                             /**< ダミー1                                          */
    UCHAR           month;                                              /**< 月                                               */
    UCHAR           day;                                                /**< 日                                               */
    UCHAR           hour;                                               /**< 時                                               */
    UCHAR           min;                                                /**< 分                                               */
    UCHAR           sec;                                                /**< 秒                                               */
    UCHAR           msec;                                               /**< ミリ秒                                           */
    UCHAR           sndrcvFlg;                                          /**< 送信/受信フラグ                                  */
    UINT            msgId;                                              /**< イベントID                                       */
    USHORT          taskId;                                             /**< タスクID                                         */
    USHORT          sig_kind;                                           /**< 信号種別                                         */
}T_CPR_RUNHIS_HEAD;

/** 走行履歴情報 */
typedef struct
{
    T_CPR_RUNHIS_HEAD   head;                                           /**< ヘッダ                                           */
    CHAR                body[32];                                       /**< データ                                           */
}T_CPR_RUNHIS_INF;

/** 走行履歴情報テーブル */
typedef struct
{
    UINT write_idx;                                                     /**< 書込位置                                         */
    T_CPR_RUNHIS_INF inf[D_CPR_RUNHIS_FACENUM_MAX];                     /**< 走行履歴情報                                     */
}T_CPR_RUNHIS_TBL;

#endif
/***************************************************************************************************************************//**
 *
 * @} addtogroup RRH_PF_CPR
 *
 ******************************************************************************************************************************/
