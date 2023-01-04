/*!
 * @skip  $ld:$
 * @file  f_nma_VLANDataWriteReq.c
 * @brief Reception of Trouble Log Flash Write request
 * @date  2019/02/21 KCN)kamimoto create\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2019
 */

 /*!
 * @addtogroup RRH_PF_NMA
 * @{
 */

#include "f_nma_inc.h"


/*!
 *  @brief  Process of Trouble Log Flash Write request
 *  @note   This function is processed as follows.\n
 *          - Call function to write the flash.\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2014/01/30 ALPHA)yokoyama create\n
 *  @date  2019/02/21 KCN)kamimoto  Create\n
 *  @warning	N/A
 *  @FeatureID	PF_File-002-001-001
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */

VOID f_nma_VLANDataWriteReq(void *pRcvMsg)
{

	T_SYS_NMA_VLAN_WRITE_REQ *			RCVMsg;
	T_SYS_NMA_VLAN_WRITE_RSP *			sndMsg;

	int									ret ;
	int									errcd ;
	unsigned char						wData ;

	/* �����ݒ�(�ԋp�o�b�t�@�̊m��) */
	RCVMsg = (T_SYS_NMA_VLAN_WRITE_REQ *)pRcvMsg ;
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,			/* Buffer���						*/
										sizeof(T_SYS_NMA_VLAN_WRITE_RSP),		/* Size								*/
										(VOID **)&sndMsg,						/* sndMsg							*/
										&errcd );

	if( ret != D_SYS_OK )
	{
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_VLANDataWriteReq buffer error=0x%08x", errcd);
		return ;
	}
	else
	{
		sndMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMA_VLAN_WRITE_RSP ;		/* serchable-vlans�����݉���		*/
		sndMsg->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;						/* ���M��Process Queue ID			*/
		sndMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_DHC;					/* ���M��Thread Queue ID			*/
		sndMsg->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;						/* ���M��Process Queue ID			*/
		sndMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_NMA;					/* ���M��Thread Queue ID			*/
		sndMsg->requestID			= RCVMsg->requestID;					/* ���N�G�X�gID						*/
		
	}

	/* VLAN���FLASH�����݃t���O = �J�n */
	wData = D_RRH_EEP_WRITTING ;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_DU_EEP_FLASH_WRITE_FLAG_VLAN , &wData ) ;

	/* Flash�C���[�X */
	ret = f_nma_cm_EraseFlash(D_DU_QSPI_FLASH_FACE0 , D_RRH_QSPI_FLASH_OFFSET_SEARCHABLE_VLAN_INFO , D_DU_QSPI_FLASH_SIZE_SOFT_PARA) ;
	if( ret != D_SYS_OK )
	{
		/* Assert */
		cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "f_nma_NetConfDataWriteReq Flash Erase NG ret=%08x", ret);
		sndMsg->result = D_SYS_SFPRSLT_ERSERR ;
		ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_DHC ,			/* ���M��Thread Queue ID			*/
										(void*)sndMsg );				/* ���MMessage						*/
		return ;
	}

	/* Serch VLANId�����Flash������ */
	ret = BPF_HM_DEVC_QSPI_FLASH_WRITE( D_DU_QSPI_FLASH_FACE0 , D_RRH_QSPI_FLASH_OFFSET_SEARCHABLE_VLAN_INFO , sizeof(RCVMsg->searchable_vlans) , (UCHAR *)&RCVMsg->searchable_vlans );
	if( ret != BPF_HM_DEVC_COMPLETE )
	{
		/* �����ݎ��s�Ȃ� */
			cmn_com_assert_th_get(f_nmaw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_HM_DEVC_QSPI_FLASH_WRITE Error ret = 0x%08x", ret);
			sndMsg->result = D_SYS_SFPRSLT_WRTERR ;
			ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_DHC ,			/* ���M��Thread Queue ID			*/
											(void*)sndMsg );				/* ���MMessage						*/
			return ;
	}

	/* VLAN���FLASH�����݃t���O = OK */
	wData = D_RRH_EEP_WRITE_OK ;
	(VOID)BPF_HM_DEVC_EEPROM_WRITE( D_DU_EEP_FLASH_WRITE_FLAG_VLAN , &wData ) ;

	/* �����ݏI�������̑��M */
	sndMsg->result = D_SYS_SFPRSLT_OK ;
	ret = BPF_RU_IPCM_MSGQ_SEND(	D_SYS_THDQID_PF_DHC ,			/* ���M��Thread Queue ID			*/
									(void*)sndMsg );				/* ���MMessage						*/
	return ;
}

/* @} */
