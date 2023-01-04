/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fc03_r.c
 * @brief  ルート
 * @date   2015/07/29 FPT)Hieu create
 *
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2015
 */
/****************************************************************************/
#include "f_trp_rec.h"
#include "rrhApi_L2.h"

/*! @addtogroup TRIF_REC
 * @{ */
#ifdef FHM_DEBUG_FOR_IT1
/****************************************************************************/
/*!
 * @brief  debug function for IT1
 * @note   イベント受信時の処理を行う
 *         - イベント変換
 *         - 状態番号取得
 *         - マトリクス関数起動
 * @param  bufp [in]  バッファポインタ
 * @return None
 * @date   
 */
/****************************************************************************/
VOID f_trp_rec_FcDB_r(                                            /* なし:R */
   VOID* bufp                                         /* バッファポインタ:I */
) {
    
    UINT                   msg_id;                          /* メッセージID */
	USHORT                 cpk = 0x0000;                    /* CPRI信号種別 */
    VOID*                  datp;
    USHORT                 dummy;


	printf( "\n***FHM***[%d]%s Start\n",__LINE__,__FUNCTION__);

    /* 関数トレースログ */
    F_COMF_CMM_FFUNCTRCLOG(D_TCM_TRCLOGLV_OTH, NULL, NULL);

    /********************/
    /* メッセージID取得 */
    /********************/
    msg_id = ((CMT_TSKIF_MSGID_GET*)bufp)->head.msgid;

	switch(msg_id)
	{
		/* Test event */
	    case 0xFFFF:
			/* CPRI信号種別取得 */
   			cpk  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)bufp)->signal_kind;
			dummy  = ((CMT_TSKIF_CPRIRCV_SIGNALGET*)bufp)->dummy;
			break;
		default:
			return;
	}

	/* メッセージIDで分岐 */
    switch (msg_id) 
	{
		/* Test event */
	    case 0xFFFF:
	        /*  */
			switch(cpk)
			{
				/* Set 3g status */
				case 0:
					f_trp_rec_Fc02_t_ownstn_set_3g(dummy);
					break;
				/* Set lte status */
				case 1:
					f_trp_rec_Fc02_t_ownstn_set(dummy);
					break;
				/* Set 3g cpri no */
				case 2:
					f_trp_rec_Wc02_prm_tbl_3g.cpr_no = dummy;
					break;
				/* Set lte cpri no */
				case 3:
					f_trp_rec_Wc02_prm_tbl.cpr_no = dummy;
					break;
	            /* ■RE装置構成情報報告プロシージャ実行指示■ */
	            case CMD_TSKIF_REEQPINFPRCIND:
	                f_trp_rec_Fc02_t_it_reeqpinfprc_snd();
	                break;
/*				case 4:
					memset(&ltMsg, 0, sizeof(ltMsg));
					ltMsg.signal_kind = 0x5711;
					ltMsg.result = 0x0000;
					ltMsg.cardsts_inf.slot_no = 1;
					ltMsg.cardsts_inf.card_name = 0;
					ltMsg.cardsts_inf.year = 0x2000;
					ltMsg.cardsts_inf.month_day = 0x0101;
					ltMsg.cardsts_inf.minsec = 0x0936;
					ltMsg.cardsts_inf.card_svitem = 0x0280;
					ltMsg.cardsts_inf.card_extitem = 0;
					ltMsg.cardsts_inf.card_cntitem = 0;
					ltMsg.cardsts_inf.card_extcntitem = 0;

	                f_trp_rec_Fc02_t_recrdssc_i_set(1, &ltMsg);
	                break; */
				default:
					printf( "\n***FHM***[%d]%s unknow msg: signal_kind:0x%x\n",__LINE__,__FUNCTION__,cpk);
					break;
			}
			break;
	}

	printf( "\n***FHM***[%d]%s End\n",__LINE__,__FUNCTION__);
    return;
}
#endif /* FHM_DEBUG_FOR_IT1 */
/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
