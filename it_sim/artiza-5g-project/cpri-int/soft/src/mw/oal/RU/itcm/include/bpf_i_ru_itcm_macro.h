/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_i_ru_itcm_macro.h
 *	@brief	Inter Thread Communication Management Macro.
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/
#ifndef _BPF_I_RU_ITCM_MACRO_H_
#define _BPF_I_RU_ITCM_MACRO_H_

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_itcm.h"



/* 範囲チェックマクロ */
/* セマフォタイプの範囲チェック */
#define ITCM_SEM_TYPECHECK(type)      \
  do { \
    if( type >= ITCM_SEM_TYPE_END ){  \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "semafo type check NG. semafo type = %d", \
          type); \
      return BPF_RU_ITCM_EPARAM;      \
    }                                 \
  } while(0)
/* セマフォ番号の範囲チェック */
#define ITCM_SEM_NUMCHECK(num) \
  do { \
    if( num >= itcm_sem.max ){ \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "semafo number check NG. semafo max = %d, num = %u", \
          itcm_sem.max,num); \
      return BPF_RU_ITCM_EPARAM; \
    } \
  } while(0) 

/* フラグチェックマクロ */
#define ITCM_SEM_FLAGCHECK(flag) \
  do { \
    if( flag != BPF_RU_ITCM_WAIT && \
        flag != BPF_RU_ITCM_NOWAIT ){ \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "wait flag check NG. flag = %d", \
          flag); \
      return BPF_RU_ITCM_EPARAM; \
    } \
  } while(0)

/* セマフォ使用中判定マクロ */
#define ITCM_SEM_USEDCHECK(num) \
  do { \
    if( itcm_sem.data[num].used == ITCM_SEM_UNUSED ){ \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "semafo is unused. semafo num = %u", \
          num); \
      return BPF_RU_ITCM_ENOSEMNUM; \
    } \
  } while(0)

/* リストに tを挿入．
   n <--> p となっているところを n <--> t <--> p に 
*/
#define ITCM_LIST_INSERT(next, prev, tgt) \
  do{ \
    (tgt)->n = next; \
    (tgt)->p = prev; \
    next = prev = tgt; \
  } while(0)

/* リストの最後eにtを追加． */
#define ITCM_LIST_ADD(e, t) \
  do{ \
    (e)->n = t;				  \
    /* 追加したエントリのpに前のエントリのアドレスを設定する　*/  \
    (t)->p = e;    \
    /* 追加したエントリのnは最後尾なのでNULLを設定する */ \
    (t)->n = NULL; \
    /* eを最後のエントリを指すように変更する */ \
    (e) = t;					\
  } while(0)

/* リストの先頭に追加する */
#define ITCM_LIST_ADD_HEAD(h, t) \
  do{ \
    (h) = t;			\
    (t)->n = NULL;	\
    (t)->p = NULL;	\
  } while(0)

/* リストからtを削除．
   n <--> t <--> p を n <--> p に 
*/
#define ITCM_LIST_REMOVE(next, tgt)		\
  do{						\
    if( (tgt)->n == NULL ){			\
      /* tgtの次のデータがない時 */		\
      next = NULL;				\
    } else {					\
      /* tgtの次のデータがある時 */		\
      next = (tgt)->n;				\
      (tgt)->n->p = (tgt)->p;			\
    }						\
  } while(0)

/* 足し算 */
#define ITCM_TS_ADD(result, t1, t2) \
  do{ \
    (result)->tv_sec = (t1)->tv_sec + (t2)->tv_sec; \
    (result)->tv_nsec = (t1)->tv_nsec + (t2)->tv_nsec; \
    if( (result)->tv_nsec >= 1000000000 ){ \
      (result)->tv_sec = (result)->tv_sec + (result)->tv_nsec / 1000000000; \
      (result)->tv_nsec = (result)->tv_nsec % 1000000000; \
    }\
  } while(0)

/* timespec型の引き算．もしマイナスになるなら0を設定する */
/* t1 = t1 - t2 をやる */
#define ITCM_TS_SUB(result, t1, t2)		\
  do{ \
    if((t1)->tv_nsec < (t2)->tv_nsec){ \
      if((t1)->tv_sec <= (t2)->tv_sec){ \
        (result)->tv_sec = (result)->tv_nsec = 0; \
      } else { \
	(result)->tv_sec = (t1)->tv_sec - (t2)->tv_sec - 1; \
	(result)->tv_nsec = 1000000000 + (t1)->tv_nsec - (t2)->tv_nsec; \
      } \
    } else { \
      if((t1)->tv_sec < (t2)->tv_sec){ \
	(result)->tv_sec = (result)->tv_nsec = 0; \
      } else { \
	(result)->tv_nsec = (t1)->tv_nsec - (t2)->tv_nsec; \
	(result)->tv_sec = (t1)->tv_sec - (t2)->tv_sec; \
      } \
    } \
  } while(0) 

/* 範囲チェックマクロ */
/* メッセージキュータイプの範囲チェック */
#define ITCM_MSGQ_TYPECHECK(type)     \
  do { \
    if( type >= ITCM_MSGQ_TYPE_END ){ \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "msgq type range NG. msgq type = %d", type);\
      return BPF_RU_ITCM_EPARAM;      \
    }                                 \
  } while(0)
/* メッセージキュー番号の範囲チェック */
#define ITCM_MSGQ_NUMCHECK(num)       \
  do {                                \
    if( num >= itcm_msg.max ){  \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "msgq number check NG. msgq num = %u", \
          num); \
      return BPF_RU_ITCM_EPARAM;      \
    }                                 \
  } while(0) 

/* メッセージサイズチェックマクロ */
#define ITCM_MSGQ_MSGSZCHECK(num, size)              \
  do {                                               \
    if( size > itcm_msg.msgq[num].maxmsglen ){ \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "msg size check NG. msgq  num = %u, size = %u",  \
          num, size); \
      return BPF_RU_ITCM_EPARAM;                        \
    }                                                \
  } while(0)

/* メッセージキュー使用中判定マクロ */
#define ITCM_MSGQ_USEDCHECK(num)				\
  do {								\
    if( itcm_msg.msgq[num].used == ITCM_MSGQ_UNUSED ){	\
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "msgq is unused. msgq num = %u", \
          num); \
      return BPF_RU_ITCM_ENOQNUM;					\
    }								\
  } while(0)

/* バッファチェック */
#define ITCM_MSGQ_MSGPCHECK(size, msgp) \
  do {                                  \
    if( size != 0 && msgp == NULL ){    \
      bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "msgp check NG. msgp = NULL"); \
      return BPF_RU_ITCM_EPARAM;         \
    }                                   \
  } while(0)

/* フラグチェック */
#define ITCM_MSGQ_FLAGCHECK(flag)  \
  do {                             \
    if( flag != BPF_RU_ITCM_WAIT &&   \
        flag != BPF_RU_ITCM_NOWAIT ){ \
          bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2, \
          "wait flag check NG. flag = %d", flag); \
      return BPF_RU_ITCM_EPARAM;      \
    }                              \
  } while(0)


#endif /* _BPF_I_RU_ITCM_MACRO_H_ */

