/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file m_rc_reStartupRSP.c
 *  @brief  RE Startup Response受信
 *  @date   2015/08/07 TDIPS)maruyama create
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_L3_RCT
* @{ */

#include "m_cm_header.h"			/* TRA共通ヘッダファイル				*/
#include "m_rc_header.h"

/*********************************************************************************/
/**
 *  @brief  RE Startup Response受信
 *  @param [in] buff_adr
 *  @return D_RRH_OK:全RE応答  D_RRH_NG：未応答REあり
 *  @Bug_No M-RRU-ZSYS-01691
 *  @Bug_No M-RRU-ZSYS-01692                   
 *  @Bug_No M-RRU-ZSYS-01965
 *  @date   2015/08/07 TDIPS)maruyama create
 *  @date	2015/08/24	TDIPS) maruyama modify RE接続判定の修正
 *  @date	2015/08/26	TDIPS) maruyama modify RE接続判定の修正
 *  @date	2015/09/02	TDIPS) maruyama modify 不要変数削除（connResult）
 *  @date	2015/09/30	TDIPS) maruyama modify re_StartUp_First_flgをre_StartUp_CPRI_Conn_flgに変更、立ち上げ判定処理の見直し
 *  @date	2015/10/11	TDIPS) maruyama modify すべて応答時のタイマ停止処理追加
 *  @date	2015/11/06	TDIPS) sasaki M-RRU-ZSYS-01965 update IT2問処No.158対処
 *  @date	2015/11/07	TDIPS) sasaki update デバッグログ追加
 */
/*********************************************************************************/
UINT m_rc_reStartupRSP(VOID* buff_adr)
{
	CMT_TSKIF_RESTR_RSP *parm_p	= NULL;		/* RE 立ち上げ応答(受信MSG)	*/
	UINT	cprLinkNo	= 0;				/* CPRI Link番号	*/
	UINT	system_type	= 0;				/* system type		*/
	UINT	ret_Val		= 0;				/* 戻り値			*/
	UINT	cnt			= 0;				/* ループカウンタ	*/
	INT		errcd		= 0;				/* エラーコード		*/
	
	parm_p = (CMT_TSKIF_RESTR_RSP*)buff_adr;

	/* RE立ち上げタイマがタイムアウトしていたら、以降の応答REは無視する	*/
	if( re_StartUp_CPRI_Conn_flg == D_RRH_ON )
	{
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s end [cpri:%d][sys:%d][result:%d]\n",__LINE__,__FUNCTION__,parm_p->link_num,parm_p->system_type,parm_p->result);
#endif
		/* タイムアウトしている	*/
		return(D_RRH_NG);
	}

	if( buff_adr == NULL ){		/* これはないはず	*/
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s end [cpri:%d][sys:%d][result:%d]\n",__LINE__,__FUNCTION__,parm_p->link_num,parm_p->system_type,parm_p->result);
#endif
		return(D_RRH_OK);
	}
	

	cm_Assert( CMD_ASL_USELOW, 
		(((parm_p->system_type << 24 ) & 0xFF000000 ) | ((parm_p->link_num << 16 ) & 0x00FF0000 )), 
		"RE Start Ack (RE StartUP)" );

	/* cprからのLink番号取得		*/
	cprLinkNo = parm_p->link_num;
	/* system type					*/
	system_type = parm_p->system_type;

	/* 応答REが立ち上げREか判定		*/
    if( re_startup[cprLinkNo] == D_RRH_ON )
    {
        /* 3G/LTE別に応答結果を設定	*/
        re_startupRSP[cprLinkNo][system_type] = D_RRH_ON;					/* 応答結果 ON 設定		*/
    }
    else
    {
        /* 立ち上げ対象外のREから応答があった場合(異常終了）	*/
        re_startupRSP[cprLinkNo][system_type] = D_RRH_OFF;		/* 明示的にOFFに設定	*/
#ifdef FHM_RCT_DEBUG_FOR_IT1
		printf("[%d]%s end [cpri:%d][sys:%d][result:%d]\n",__LINE__,__FUNCTION__,parm_p->link_num,parm_p->system_type,parm_p->result);
#endif
        return(D_RRH_MNG);
    }

	/* 立ち上げ対象REがすべて応答したか判定		*/
	for( cnt = 1 ; cnt < D_RRH_CPRINO_NUM ; cnt++ )
	{
		/* 3Gで要求していないけど応答がある場合(異常終了）	*/
		if( (re_startupREQ[cnt][CMD_SYS_3G] == D_RRH_OFF) &&
			(re_startupRSP[cnt][CMD_SYS_3G] == D_RRH_ON) )
		{
			re_startupRSP[cnt][CMD_SYS_3G] = D_RRH_OFF;
			ret_Val = D_RRH_MNG;
			break;
		}

		/* S3Gで要求していないけど応答がある場合(異常終了）	*/
		if( (re_startupREQ[cnt][CMD_SYS_S3G] == D_RRH_OFF) &&
			(re_startupRSP[cnt][CMD_SYS_S3G] == D_RRH_ON) )
		{
			re_startupRSP[cnt][CMD_SYS_S3G] = D_RRH_OFF;
			ret_Val = D_RRH_MNG;
			break;
		}

		/* 3GとS3Gとも要求なし(要求なしで応答ある場合は上記で異常終了処理している	*/
			/* この場合はあり得ない		*/
		if( (re_startupREQ[cnt][CMD_SYS_3G] == D_RRH_OFF ) &&
			(re_startupREQ[cnt][CMD_SYS_S3G] == D_RRH_OFF ) )
		{
			ret_Val = D_RRH_OK;
		}
		/* 3GとS3Gで要求と応答が一致	*/
		else if( (re_startupREQ[cnt][CMD_SYS_3G] == re_startupRSP[cnt][CMD_SYS_3G]) &&
			(re_startupREQ[cnt][CMD_SYS_LTE] == re_startupRSP[cnt][CMD_SYS_LTE]) ) 
		{
			/* 立ち上がった*/
			ret_Val = D_RRH_OK;
		}
		else
		{
			/* 立ち上がっていない	*/
			ret_Val = D_RRH_NG;
			break;
		}
	}

	/* すべて応答していたらタイマーは停止	*/
	if( ret_Val == D_RRH_OK )
	{
		cm_TStop( CMD_TIMID_STARTUPRE, &errcd );
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf("[%d]%s end [cpri:%d][sys:%d][result:%d]\n",__LINE__,__FUNCTION__,parm_p->link_num,parm_p->system_type,parm_p->result);
#endif
	return(ret_Val);

}

/* @} */
