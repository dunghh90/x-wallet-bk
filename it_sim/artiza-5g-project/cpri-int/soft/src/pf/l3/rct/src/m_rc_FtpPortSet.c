/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   m_rc_FtpPortSet.c
 *  @brief  FTPデータ送信用PORT番号設定関数
 *  @date   2015/08/10 TDIPS)Shingo.W
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-48)
 *
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited
 */
/********************************************************************************************************************/

/** @addtogroup L3_RCT
 * @{ */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/

#include "m_cm_header.h"
#include "m_rc_header.h"
#include "rrhApi_Svp.h"
#include "m_rc_def.h"
#include "m_rc_ftyp.h"


/********************************************************************************************************************/
/**
 *  @brief	FTPデータ送信用PORT番号設定応答関数
 *  @note
 *  @param	parm_p [in] the buffer address pointer of received message
 *  @return	None
 *  @date	2015/08/10 TDIPS)Shingo.W
 *  @date   2021/02/04 M&C)Tri.hn Update code based on spec no.68 (sequence 3-48)
 */
/********************************************************************************************************************/
VOID m_rc_FtpPortSetReq(CMT_TSKIF_CPRISND_PORTSETREQ *parm_p)
{
	UINT		lwSigchk_rslt;					/*  result of checking signal */
	USHORT		lwSigsys_rslt;					/*  system by checking signal */
	USHORT		lwRsp_rslt;						/* 応答結果 */
	UINT		ldwResult;						/*result of message sending*/

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->cpridat_portsetreq.signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");

		if( CMD_SYS_3G == lwSigsys_rslt)
		{
			/* 処理NG(RE)(51)を返却 */
			lwRsp_rslt = CMD_HDL_NG;
			/* RECにFTPデータ送信用PORT番号設定応答を送信 */
			m_rc_FtpPortSetResSnd( lwSigsys_rslt, lwRsp_rslt );
		}
	}
	parm_p->cprisnd_inf.link_num = 1;	/* 1-21-72-1_NG */
	/* Send to re/rec */
	ldwResult = l3_com_sendMsg( CMD_TSKID_RCT, D_RRH_PROCQUE_RE, 0,  parm_p, sizeof( CMT_TSKIF_CPRISND_PORTSETREQ ) );
	if( D_RRH_OK != ldwResult)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							ldwResult,
							"Sending msg NG");
		return;
	}

	/* 処理終了 */
	return;
}

/********************************************************************************************************************/
/**
 *  @brief	FTPデータ送信用PORT番号設定応答
 *  @note
 *  @param	awSigsys_rslt [in] the system type LTE/3G
 *  @param	awRsp_rslt    [in] 
 *  @return	None
 *  @date   2021/02/04 M&C)Tri.hn Create - based on spec no.68 (sequence 3-48)
 */
/********************************************************************************************************************/
VOID m_rc_FtpPortSetRes( CMT_CPRIF_FTPPORTSETRES* parm_p )
{
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start \n",__LINE__,__FUNCTION__);
#endif

	UINT			lwSigchk_rslt;						/*  result of checking signal */
	USHORT			lwSigsys_rslt;					/*  system by checking signal */
	INT				errcd;
	INT				ldwResult;
	USHORT			lwTimerId;
	USHORT			lwLoop;
	T_RRH_LAYER3	ltLayer3Sts;

	/*In case buffer is Null*/
	if( NULL == parm_p )
	{
		/*end processing*/
		return;
	}

	/* validate signal */
	lwSigchk_rslt = m_cm_chksig(parm_p->signal_kind, &lwSigsys_rslt);
	/* signalがNGの場合 */
#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s m_cm_chksig:%d, lwSigsys_rslt:%d \n",__LINE__,__FUNCTION__,lwSigchk_rslt,lwSigsys_rslt);
#endif
	if(lwSigchk_rslt != CMD_RES_OK)
	{
		/* アサート出力処理 */
		cm_Assert(	CMD_ASL_USELOW,
							lwSigsys_rslt,
							"validate signal NG");
	}

#ifdef FHM_RCT_DEBUG_FOR_IT1
	printf( "[%d]%s start(result:%d, Systerm:%d) \n",__LINE__,__FUNCTION__,awResult, awSystem);
#endif

	/* FTPデータ送信用PORT番号設定応答 */
	m_rc_FtpPortSetResSnd(lwSigsys_rslt, parm_p->result);

	return;
}
/** @} */

/********************************************************************************************************************/
/**
 *  @brief	FTPデータ送信用PORT番号設定応答
 *  @note
 *  @param	awSigsys_rslt [in] the system type LTE/3G
 *  @param	awRsp_rslt    [in] 
 *  @return	None
 *  @date	2015/08/10 TDIPS)Shingo.W
 */
/********************************************************************************************************************/
VOID m_rc_FtpPortSetResSnd( USHORT awSigsys_rslt, USHORT awRsp_rslt )
{
	CMT_TSKIF_CPRISND_PORTSETRES *rsp_p;
	UINT		bufget_rslt;
	UINT		Snd_ret;

	rsp_p = NULL;
	bufget_rslt  = cm_L2BfGet(	CMD_BUFCA_LAPSND,
								sizeof(CMT_TSKIF_CPRISND_PORTSETRES),
								CMD_NUM1,
								(VOID **)&rsp_p	);
	if(bufget_rslt != CMD_RES_OK)
	{
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW,
					bufget_rslt,
					"m_rc_FtpPortSetResSnd cm_L2BfGet NG");
		return;
	}

	rsp_p->head.uiLength = sizeof(CMT_TSKIF_HEAD) + sizeof(CMT_CPRISIGRCV_INF) + sizeof(CMT_CPRIF_FTPPORTSETRES);
	rsp_p->cpridat_portsetres.signal_kind = CMD_CPRID_FTPPORTSETRES + awSigsys_rslt;
	rsp_p->cpridat_portsetres.result = awRsp_rslt;

	Snd_ret = m_cm_L3MsgSend_REC( rsp_p );
	if( Snd_ret != CMD_L3_OK )
	{
		cm_Assert( CMD_ASL_DBGLOW, CMD_NUM1,
								"m_rc_FtpPortSetResSnd m_cm_L3MsgSend_REC NG" );
	}

	return;
}
/** @} */
