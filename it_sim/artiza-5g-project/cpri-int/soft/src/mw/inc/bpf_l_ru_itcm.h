/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_l_ru_itcm.h
 *  @brief  Definitions for BPF itcm library
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_L_RU_ITCM_H_
#define _BPF_L_RU_ITCM_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_ITCM.h"

#define BPF_RU_ITCM_OK        0    /* OK */
#define BPF_RU_ITCM_NG        1    /* NG */
#define BPF_RU_ITCM_ASSIGN_NG 2    /* ASSIGN NG */


/* スレッド間メッセージキュー関係 */
/* メッセージキュータイプの定義 */
enum itcm_msgq_type {
  ITCM_MSGQ_USER = 0,
  ITCM_MSGQ_TYPE_END,
};

/* スレッド間セマフォ関係 */
/* セマフォタイプの定義 */
enum itcm_sem_type {
  ITCM_SEM_USER = 0,
  ITCM_SEM_TYPE_END,
};

/* Function Prototype */
int bpf_ru_itcm_assign(unsigned int process_desc);
int bpf_ru_itcm_unassign(unsigned int process_desc);

int bpf_ru_itcm_thread_sem_take2(enum itcm_sem_type, unsigned int, int, struct bpf_timespec *);
int bpf_ru_itcm_thread_sem_give2(enum itcm_sem_type, unsigned int);
int bpf_ru_itcm_thread_msgq_send2(enum itcm_msgq_type, unsigned int, unsigned int, const void *,
                                  int, struct bpf_timespec *);
int bpf_ru_itcm_thread_msgq_receive2(enum itcm_msgq_type, unsigned int,unsigned int, void **msgp, 
                                     unsigned int *, int, struct bpf_timespec *);
int bpf_ru_itcm_thread_msgq_nummsgs2(enum itcm_msgq_type , unsigned int , unsigned int *);

/********************************************* EOF ******************************************************************/
#endif /* _BPF_L_RU_ITCM_H_ */

