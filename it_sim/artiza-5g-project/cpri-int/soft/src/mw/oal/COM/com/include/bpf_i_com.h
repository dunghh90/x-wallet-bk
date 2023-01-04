/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_comh
 *  @brief  Definitions for BPF LOG Local header
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#ifndef _BPF_I_COM_H_
#define _BPF_I_COM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include <string.h>
#include "BPF_COM.h"

/********************************************************************************************************************/
/* Internal Definition                                                                                              */
/********************************************************************************************************************/
#define LOG_STRING_FILE_MAX     40
#define LOG_STRING_FUNC_MAX     40
#define LOG_STRING_TNAME_MAX    32
#define LOG_STRING_EVENT_MAX    128
#define LOG_DATA_LOG_DATA_MAX   300
#define LOG_DATA_LINE           300

#define BPF_LOGFUNC_SYSCALL_NG (-1)

#define LOG_FLG_OFF             0
#define LOG_FLG_ON              1

/********************************************************************************************************************/
/* Internal Structure                                                                                               */
/********************************************************************************************************************/

/********************************************************************************************************************/
/* Internal enum                                                                                                    */
/********************************************************************************************************************/
typedef struct{
    unsigned short   year;                         /* 年                       */
    unsigned char    month;                        /* 月(1～12)                */
    unsigned char    day;                          /* 日(1～31)                */
    unsigned char    hour;                         /* 時(0～23)                */
    unsigned char    min;                          /* 分(0～59)                */
    unsigned char    sec;                          /* 秒(0～59)                */
    unsigned char    msec;                         /* ミリ秒(0～99)(10ms単位)  */
}t_bpf_com_log_data_time;


typedef struct{
    unsigned int    no;                                 /* ログNo           */
    T_RRH_SYSTEM_TIME    time;                    		/* 時刻             */
    char            filename[LOG_STRING_FILE_MAX];      /* ファイル名       */
    char            funcname[LOG_STRING_FUNC_MAX];      /* 関数名           */
    unsigned int    line;                               /* 行番号           */
    char            threadname[LOG_STRING_TNAME_MAX];	/* スレッド名       */
    char            event[LOG_STRING_EVENT_MAX];        /* イベント名       */
    unsigned int    logsize;                            /* ログ情報サイズ   */
    char            logdata[LOG_DATA_LOG_DATA_MAX];     /* ログ情報         */
}t_bpf_ru_log_data_tbl;

typedef struct {
    unsigned int            index;                      /* ログ格納INDEX     */
    unsigned int            pre_no;                     /* 前回情報No        */
    pthread_mutex_t         mutex;                      /* mutexオブジェクト */
    t_bpf_ru_log_data_tbl   data_info[LOG_DATA_LINE];   /* ログ格納情報      */
} t_bpf_ru_log_data;

extern t_bpf_ru_log_data* di_log_data_p;


/********************************************************************************************************************/
/* Internal Function Routine Prototype                                                                              */
/********************************************************************************************************************/
extern unsigned int  di_log_assign_flag;

/********************************************* EOF ******************************************************************/
#endif /* _BPF_I_COM_H_ */

