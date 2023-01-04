/*!
 * @skip  $ld:$
 * @file  duApi_Mpn.c
 * @brief API : M-Plane����nAPI
 * @date  2018/02/15 FJT)H.Yoshida create
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/** @addtogroup RRH_API_MPN
 *  @{
 */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "f_rrh_inc.h"
#include "f_sys_def.h"

#include "BPF_RU_IPCM.h"
#include "BPF_COM_LOG.h"

#include "rrhApi_Com.h"
#include "duApi_Mpn.h"

/********************************************************************************************************************/
/**
 *  @brief	API : M-Plane�ڑ������v��
 *  @note	PF EVENT ID : 0xA00F0001
 * 			Reponse  ID : -
 * 			TYPE		: MPN
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @param	lowVid		: ����VID�ŏ��l
 *  @param	highVid		: ����VID�ő�l
 *  @param	lastUsedVid	: VID�O��l
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/02/15	FJT)H.Yoshida	Create.
 *  @date	2018/02/23	FJT)H.Yoshida	delete input parameter "Method".
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
/***** 2018.02.14 H.Yoshida 5G-DU 3M�@�\�ǉ� Memo	pf_dhcp��M-Plane�ڑ������v���𑗐M����	*****/
E_RRHAPI_RCODE	duApi_Mpn_Mnr_MplaneReady( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< �����҂�����		*/
										   VOID		*data_p,			/**< �擾�f�[�^			*/
										   USHORT	Interface,			/**< Interface�w��		*/
										   USHORT	Flag,				/**< �ڑ��w��			*/
										   USHORT	lowVid,				/**< ����VID�ŏ��l		*/
										   USHORT	highVid,			/**< ����VID�ő�l		*/
										   USHORT	lastUsedVid)		/**< VID�O��l			*/
{
	T_API_MPLANE_READY_REQ			*apiReq_p = NULL;					/**< M-Plane�ڑ������v��*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*//* �� */
										sizeof(T_API_MPLANE_READY_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�FM-Plane�ڑ������v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_MPN_MPLANE_READY_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PF_DHC;				/* Destination T QID	*//* �� */
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_MPLANE_READY_REQ);	/* Length				*/

	/* API�FM-Plane�ڑ������v���f�[�^���ݒ�	*/
	apiReq_p->data.Interface		= Interface;
	apiReq_p->data.Flag				= Flag;
	apiReq_p->data.lowVid			= lowVid;							/* ����VID�ŏ��l		*/
	apiReq_p->data.highVid			= highVid;							/* ����VID�ő�l		*/
	apiReq_p->data.lastUsedVid		= lastUsedVid;						/* VID�O��l			*/

	/* PF_DHCP��M-Plane�ڑ������v���𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PF,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

#if 0
/********************************************************************************************************************/
/**
 *  @brief	API : M-Plane�ڑ��J�n�v��
 *  @note	PF EVENT ID : 0xA00F0003
 * 			Reponse  ID : -
 * 			TYPE		: MPN
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @param	remote_ipadr: CU��IP�A�h���X
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/02/15	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
/***** 2018.02.14 H.Yoshida 5G-DU 3M�@�\�ǉ� Memo	pf_mps��M-Plane�ڑ��J�n�v���𑗐M����	*****/
E_RRHAPI_RCODE	duApi_Mpn_Mnr_MplaneStart( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< �����҂�����		*/
										   VOID		*data_p,			/**< �擾�f�[�^			*/
										   UINT		local_ipadr,		/**< DU��IP�A�h���X		*/
										   UINT		remote_ipadr)		/**< CU��IP�A�h���X		*/
{
	T_API_MPLANE_START_REQ			*apiReq_p = NULL;					/**< M-Plane�ڑ������v��*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*//* �� */
										sizeof(T_API_MPLANE_START_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�FM-Plane�ڑ��J�n�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_MPN_MPLANE_START_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PF;					/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PF_MPS;				/* Destination T QID	*//* �� */
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_MPLANE_START_REQ);	/* Length				*/

	/* API�FM-Plane�ڑ��J�n�v���f�[�^���ݒ�	*/
	apiReq_p->data.local_ipadr		= local_ipadr;						/* DU��IP�A�h���X		*/
	apiReq_p->data.remote_ipadr		= remote_ipadr;						/* CU��IP�A�h���X		*/

	/* PF_MPS��M-Plane�ڑ��J�n�v���𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PF,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}
#endif
/** @} */

