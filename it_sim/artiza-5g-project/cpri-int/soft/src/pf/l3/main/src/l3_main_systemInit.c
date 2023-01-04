/*!
 *  @skip   $Id:$
 *  @file   l3_main_systemInit.c
 *  @brief  system init 
 *  @date   2013/11/15 FFCS)linb Create 
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L3_MAIN
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "m_cm_header.h"
#include "l3_com_inc.h"
#include "l3_main_inc.h"
#include "f_com_aplCom.h"

#include "rrhApi_L2.h"
#include "rrhApi_Inv.h"

/********************************************************************************************************************/
/*!
 *  @brief  system init.
 *  @note   send system init complete notice to init process and init system
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)linb Create 
 *  @date   2015/09/03 TDIPS)ikeda 17リンク対応.
 */
/********************************************************************************************************************/
VOID l3_main_systemInit(VOID)
{
	INT 					ret;
	INT						errcd;							/* error code				*/
	UINT					rcvmsgsz;						/* receivied message size	*/
	UCHAR               	rcvBuf[512];
	CMT_TSKIF_HEAD			*head;							/**<  ?^?X?N?�衯��?w?b?_		 */
	USHORT linkno;
	UINT subTbl = CMD_CPRID_HCRES;
// 擬似環境
#if 0
	/* receive system init from init process*/
	ret = apl_com_msgQReceive(
			CMD_TSKID_INI,					/* Process queue ID		*/
			sizeof(rcvBuf),		 				/* received buffer size    */
			(VOID *)rcvBuf, 					/* received buffer         */
			(INT*)&rcvmsgsz, 							/* received message length */
			&errcd								/* Error code              */
		);
	/* checking parameter */
	if( ret != D_RRH_OK ) {
		cm_Assert( CMD_ASL_DBGLOW, ret,
		    "[l3_main] Receive system init from init process NG" );

		return;
	}
#endif
// 擬似環境

	/* get version information */
	l3_main_subGetVersion();

	/* make subscription of cpri message notice */
    rrhApi_L2_Mpr_SubscribeCpriMsg(D_RRH_CPRINO_REC,  D_RRH_PROCQUE_L3, 0, D_RRH_NULL, (UINT *)l3_comr_cprimsg_subTbl, D_L3_CPRIMSGNUM);

    /* ヘルスチェック応答はL3プロセスで処理する */
    for (linkno = D_RRH_CPRINO_RE1; linkno < D_RRH_CPRINO_NUM; linkno++) {
        rrhApi_L2_Mpr_SubscribeCpriMsg(linkno, D_RRH_PROCQUE_L3, 0, D_RRH_NULL, &subTbl, 1);
    }
	
	if(( ret = cm_BReq( CMD_BUFCA_TSKIF, sizeof( CMT_TSKIF_HEAD ), (VOID**)&head )) != CMD_RES_OK )
	{
		cm_Assert( CMD_ASL_DBGHIGH, ret,"Buffer get NG" );
		return;
	}

	head->uiEventNo = CMD_TSKIF_ALLTSKININTC;
	head->uiSrcTQueueID = CMD_TSKID_INI;
	head->uiDstTQueueID = CMD_TSKID_RCT;
	head->uiLength = sizeof(CMT_TSKIF_HEAD);

	if((ret = cm_Enter(	CMD_TSKID_RCT, CMD_QRB_NORMAL, CMD_TSKIF_ALLTSKININTC,head)) != CMD_RES_OK )
	{
		cm_Assert(CMD_ASL_DBGHIGH, ret, "cm_Enter NG" );
		return;
	}

	l3_mainw_stateno = L3_MAIN_ST_USE;
	return;
}


extern UINT f_recv_delMsgMap(UINT, UINT, UINT);

VOID l3_main_subGetVersion(VOID)
{
	INT						ret = CMD_NUM0;
	INT						errcd = CMD_NUM0;				/* error code				*/
	UINT					rcvmsgsz;						/* receivied message size	*/
	UCHAR               	rcvBuf[512];

	/* get version information */
	ret = rrhApi_File_Mpr_SubGetFirmVersion(D_RRH_PROCQUE_L3,100, NULL);
	if(ret != E_API_RCD_OK)
	{
		cm_Assert( CMD_ASL_DBGHIGH, ret, "rrhApi_File_Mpr_SubGetFirmVersion NG" ); 	
		return;
	}
	
	/* sync with l3_rct */
	ret = apl_com_msgQReceive(
			CMD_TSKID_INI,						/* Process queue ID		*/
			sizeof(rcvBuf),		 				/* received buffer size    */
			(VOID *)rcvBuf, 					/* received buffer         */
			(INT*)&rcvmsgsz, 					/* received message length */
			&errcd								/* Error code              */
		);
	/* checking parameter */
	if( ret != D_RRH_OK ) {
		cm_Assert( CMD_ASL_DBGHIGH, ret, "[l3_main_subGetVersion] Sync NG" );
		return;
	}

	/* remove message map dynamicly */
	f_recv_delMsgMap(D_API_MSGID_FILE_FIRM_VERSION_RSP,0,CMD_TSKID_INI);

	return;
}
/* @} */

