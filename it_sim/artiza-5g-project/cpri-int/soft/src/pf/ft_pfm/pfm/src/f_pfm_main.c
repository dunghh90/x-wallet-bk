/*!
 * @skip 		$ld:$
 * @file 		f_pfm_main.c
 * @brief		pf_pfm?X???b?h ???C??????
 * @date		2018/02/19 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup RRH_PF_PFM
 * @{
 */

#include "f_pfm_inc.h"

/****************************************************************************************************************************/
/*!
 * @brief		f_pfm_main
 * @note		pf_pfm?X???b?h???C???B?X???b?h?N?????獓G?󐍑҂??̌oop?????{
 *				-# BPF?փX???b?h??o?^???鍊 *				-# PFM?X???b?h?󑔂𢏉???󑔢?ɐݒ肷?鍊 *				-# ???????֐??㡬l???騦_pfm_init())
 *				-# MSG?҂????oop?????ֈڍs???鍊 *					-# ?X???b?h?ԒʐM?L???[?󐍊֐??㡬l???騂PF_RU_IPCM_MSGQ_RECV())
 *					-# ?󐍃??b?Z?[?W?𐍊֐??㡬l???騦_pfm_anlz())
 *					-# ?ʐM?p???L???????̉𕺨BPF_RU_IPCM_PROCMSG_ADDRFREE())
 * @param		N/A
 * @retval		N/A
 * @return		N/A
 * @warning		N/A
 * @FeatureID	-
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date		2018/02/19 FJT)H.Yoshida Create
 */
/****************************************************************************************************************************/
VOID f_pfm_main(void)
{
	VOID							*msgP;								/* ?󐍭sg?|?C???^		*/
	UINT							rcvmsgsz;							/* ?󐍭sg?T?C?Y		*/
	INT								ret;								/* ?߂蒬				*/

	/* ?X???b?h???\??	*/
	prctl(PR_SET_NAME, "pf_pfm", 0, 0, 0);

	/* BPF?փX???b?h??o?^	*/
	BPF_RM_SVRM_THREAD_INFO_SET( D_RRH_PROCID_PF,						/* ProcessID			*/
								 D_SYS_THDID_PF_PFM,					/* ThreadID				*//*??*/
								 D_SYS_THDQID_PF_PFM,					/* ThreadQueueID		*//*??*/
								 (unsigned long int)pthread_self());	/* pThreadID			*/
	/* ???????????֐?Call	*/
	(VOID)f_pfm_InitReq( NULL );
	
	/************************************************************************************************************************/
	/* ???b?Z?[?W?󐍑҂?																									*/
	/************************************************************************************************************************/
	while(1)
	{
		/* ???b?Z?[?W?󐍉*/
		ret = BPF_RU_IPCM_MSGQ_RECV(	D_SYS_THDQID_PF_PFM,			/* Thread Qid			*/
										0,								/* ?????҂????ԉ		*/
										(void **)&msgP,					/* ?󐍭sg?|?C???^		*/
										&rcvmsgsz );					/* ?󐍭sg?T?C?Y		*/
		printf("\ninit f_pfm_main recv");//C9 add code debug
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}
		/* ?󐍃??b?Z?[?W?𐍊֐?Call	*/
		(VOID)f_pfm_anlz((VOID*)msgP);

		/* ?ʐM?p???L???????̉𕺉*/
		ret = BPF_RU_IPCM_PROCMSG_ADDRFREE( (VOID*)msgP );
		if( ret != BPF_RU_IPCM_OK )
		{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV failed %d", ret );
			break;
		}
	}
	return;
}

/* @} */
