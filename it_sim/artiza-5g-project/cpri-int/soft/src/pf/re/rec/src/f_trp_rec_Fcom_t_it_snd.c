/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_it_snd.c
 * @brief  スレッド間送信
 * @date   2007/03/20 FJT)Nagasima create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2007
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "f_sys_inc.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  Send message to re/rec or l2/lpb
 * @note   Send message to re/rec or l2/lpb
 * @param  bufp   [in]  バッファポインタ
 * @param  msg_id [in]  メッセージID
 * @param  thrino [in]  スレッド内部番号
 * @param  msglen [in]  メッセージ長
 * @return 終了コード
 * @date   2007/03/20 FJT)Nagasima create.
 * @TBD_No 修正未完了
 */
/****************************************************************************/
INT f_trp_rec_Fcom_t_it_snd(                                /* 終了コード:R */
    VOID*  bufp,                                      /* バッファポインタ:I */
    UINT   msg_id,                                        /* メッセージID:I */
    USHORT thrino,                                    /* スレッド内部番号:I */
    UINT   msglen                                         /* メッセージ長:I */
) {
//	UINT	ldwRtn			= CMD_RES_OK;					/* 戻り値格納変数*/
	USHORT	lwReNo;
	USHORT	lwLength;
    INT        sit_ret;                                       /* 終了コード */
    UINT       rha_ret;                                       /* 終了コード */
//	VOID*  		bufp;
	/* パラメータチェック */
	if(( NULL == bufp ) || ( 0 == msglen))
	{
		return D_RRH_NG;
	}

	/* Send msg to l2/lpb */
	if( D_TCM_THRINO_LPB == thrino )
	{
		/* タスク間フレームのデータ作成 */
		((CMT_TSKIF_HEAD *)bufp)->uiSrcPQueueID = D_RRH_PROCQUE_RE;
		((CMT_TSKIF_HEAD *)bufp)->uiSrcTQueueID = CMD_TSKID_REC;
		/* Length(Head部 + Data部) */
		((CMT_TSKIF_HEAD *)bufp)->uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGSND_INF) + msglen ;

		lwReNo = ((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->cprircv_inf.link_num;
		/**************/
	    /* RUNHIS追加 */
	    /**************/
	    rha_ret = f_trp_rec_Fcom_t_runhis_add(                  /* 終了コード:R */
	        D_REC_RUNHIS_RHK_SND,                                     /* 送信:I */
	        bufp                                          /* バッファポインタ:I */
	    );
#ifdef FHM_DEBUG_TIMER_FOR_IT1
    printf( "%s %s %d <cpri>%d <Evt>0x%x Send to lpb\n", __FILE__, __FUNCTION__, __LINE__, lwReNo,((CMT_TSKIF_CPRIRCV_SIGNALGET *)bufp)->signal_kind);
#endif

		/* L3信号中継送信 */
		sit_ret = m_cm_L3MsgSend_RE( lwReNo, bufp );
	}
	/* Send msg to Re/rec */
	else
	{
		lwLength = msglen +  sizeof(T_MSGHEAD);
		((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid = msg_id;
		/**************/
	    /* RUNHIS追加 */
	    /**************/
	    rha_ret = f_trp_rec_Fcom_t_runhis_add(                  /* 終了コード:R */
	        D_REC_RUNHIS_RHK_SND,                                     /* 送信:I */
	        bufp                                          /* バッファポインタ:I */
	    );
#ifdef FHM_DEBUG_TIMER_FOR_IT1
    printf( "%s %s %d <msg_id>0x%x Send to re/rec\n", __FILE__, __FUNCTION__, __LINE__, msg_id);
#endif
		/* Send msg */
		sit_ret = f_trp_rec_Fcom_t_msg_snd( bufp, msg_id, D_RRH_PROCQUE_RE, CMD_TSKID_REC, lwLength);
	}

	if(sit_ret != CMD_RES_OK)
	{
		cm_Assert(CMD_ASL_DBGHIGH, sit_ret, "Send Msg NG" );
	}

    /* RUNHIS削除判定 */
    if ((sit_ret != CMD_OK) &&
        (rha_ret == CMD_OK)) {
        /**************/
        /* RUNHIS削除 */
        /**************/
        f_trp_rec_Fcom_t_runhis_del(                              /* なし:R */
            (USHORT)sit_ret                             /* RUNHIS種別NG要因:I */
        );
    }
	return sit_ret;
}

/****************************************************************************/
/*!
 * @brief  Send message to process/thread
 * @note   Send message to process/thread
 * @param  bufp   [in]  バッファポインタ
 * @param  msg_id [in]  メッセージID
 * @param  dst_pNo [in]  Destination process no
 * @param  dst_tNo [in]  Destination thread no
 * @param  msglen [in]  メッセージ長
 * @return 終了コード
 * @date   2007/08/22 FPT)Yen create.
 */
/****************************************************************************/
INT f_trp_rec_Fcom_t_msg_snd(                                /* 終了コード:R */
    VOID*  bufp,                                      /* バッファポインタ:I */
    UINT   msg_id,                                        /* メッセージID:I */
    UINT   dst_pNo,                             /* Destination process no:I */
    UINT   dst_tNo,                              /* Destination thread no:I */
    UINT   msglen                                         /* メッセージ長:I */
) {
	VOID	*shm_msgp;
	UINT	ldwRtn			= CMD_RES_OK;					/* 戻り値格納変数*/
	UINT	tskNo;
	UINT    keyID;

#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d \n", __FILE__, __FUNCTION__, __LINE__);
#endif

#ifdef FHM_DEBUG_FOR_IT1
	printf( "**FHM**[%d]%s start\n",__LINE__,__FUNCTION__);
	printf( "**FHM**[%d]%s msg_id: 0x%x, dst_pNo:0x%x, dst_tNo:0x%x, msglen: %d\n",__LINE__,__FUNCTION__,msg_id,dst_pNo,dst_tNo,msglen);
#endif
	/* バッファ取得 */
	ldwRtn = cm_BReq( CMD_BUFCA_TSKIF, msglen, (VOID **)&shm_msgp );
	/* 戻り値判定 */
	if( ldwRtn != CMD_RES_OK ){
		/* バッファ取得失敗時はアボート処理 */
		cm_Assert( CMD_ASL_USELOW, ldwRtn,"f_trp_rec_Fcom_t_it_snd cm_L2BfGet NG");
		return ldwRtn;
	}

	keyID = ((CMT_TSKIF_HEAD*)shm_msgp)->uiKeyID;
	memcpy(shm_msgp,bufp,msglen);
	((CMT_TSKIF_HEAD*)shm_msgp)->uiKeyID = keyID;

	/* タスク間フレームのデータ作成 */
	((CMT_TSKIF_HEAD *)shm_msgp)->uiEventNo = msg_id;
	((CMT_TSKIF_HEAD *)shm_msgp)->uiSrcPQueueID = D_RRH_PROCQUE_RE;
	((CMT_TSKIF_HEAD *)shm_msgp)->uiSrcTQueueID = CMD_TSKID_REC;
	
	((CMT_TSKIF_HEAD *)shm_msgp)->uiDstPQueueID = dst_pNo;
	((CMT_TSKIF_HEAD *)shm_msgp)->uiDstTQueueID = dst_tNo;
	((CMT_TSKIF_HEAD *)shm_msgp)->uiLength = msglen;

	/* Internal msg (re/rec -> re/rec) */
	if(D_RRH_PROCQUE_RE == dst_pNo)
	{
		tskNo = CMD_TSKID_REC;
	}
	else
	{
		tskNo = dst_pNo;
	}
#ifdef FHM_DEBUG_C02_FOR_IT1
    printf( "%s %s %d <dst>0x%x <evt>0x%x\n", __FILE__, __FUNCTION__, __LINE__, dst_pNo, msg_id );
#endif

#ifdef FHM_DEBUG_FOR_IT1
 printf( "**FHM**[%d]%s tskNo: %d \n",__LINE__,__FUNCTION__,tskNo);
#endif
	/* Send msg */
	ldwRtn = cm_Enter( tskNo, CMD_QRB_NORMAL, msg_id, shm_msgp );
#ifdef FHM_DEBUG_FOR_IT1
 printf( "**FHM**[%d]%s end,ldwRtn: %d \n",__LINE__,__FUNCTION__, ldwRtn);
#endif
	return ldwRtn;
}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
