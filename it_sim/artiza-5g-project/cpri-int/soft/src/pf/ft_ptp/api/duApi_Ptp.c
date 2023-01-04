/*!
 * @skip  $ld:$
 * @file  duApi_Ptp.c
 * @brief API : PTP����nAPI
 * @date  2018/04/26 FJT)H.Yoshida create
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
#include "duApi_Ptp.h"

/********************************************************************************************************************/
/**
 *  @brief	API : PTP�J�n�v��
 *  @note	PF EVENT ID : 0xA00E0001
 * 			Reponse  ID : 0xA00E0002
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_PtpStart( INT		qid,					/**< QID				*/
										INT		wtime,					/**< �����҂�����		*/
										VOID	*data_p )				/**< �擾�f�[�^			*/
{
	T_API_PTP_START_REQ				*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_START_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_START_REQ;		/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_START_REQ);		/* Length				*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP��~�v��
 *  @note	PF EVENT ID : 0xA00E0003
 * 			Reponse  ID : 0xA00E0004
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_PtpShutdown( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< �����҂�����		*/
										   VOID		*data_p )			/**< �擾�f�[�^			*/
{
	T_API_PTP_SHUTDOWN_REQ			*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_SHUTDOWN_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_SHUTDOWN_REQ;		/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_SHUTDOWN_REQ);	/* Length				*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP�g���[�X���O���x���ݒ�v��
 *  @note	PF EVENT ID : 0xA00E0005
 * 			Reponse  ID : -
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_SetTraceLogLevel( INT		qid,			/**< QID				*/
												INT		wtime,			/**< �����҂�����		*/
												VOID	*data_p,		/**< �擾�f�[�^			*/
												UINT	dbgMode,		/**< ���W���[���ԍ�		*/
												UINT	dbgLevel )		/**< ���O���x��			*/
{
	T_API_PTP_TLOG_SET_REQ			*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_TLOG_SET_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_TLOG_SET_REQ;		/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_TLOG_SET_REQ);	/* Length				*/

	/* API�F�v���f�[�^���ݒ�	*/
	apiReq_p->data.dbgMode			= dbgMode;							/* ���W���[���ԍ�		*/
	apiReq_p->data.dbgLevel			= dbgLevel;							/* ���O���x��			*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP APR���O���x���ݒ�v��
 *  @note	PF EVENT ID : 0xA00E0007
 * 			Reponse  ID : -
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_SetAprLogLevel( INT		qid,			/**< QID				*/
											  INT		wtime,			/**< �����҂�����		*/
											  VOID		*data_p,		/**< �擾�f�[�^			*/
											  UINT		dbgLevel )		/**< ���O���x��			*/
{
	T_API_PTP_APRLOG_SET_REQ		*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_APRLOG_SET_REQ),	/* Size				*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_APRLOG_SET_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_APRLOG_SET_REQ);	/* Length				*/

	/* API�F�v���f�[�^���ݒ�	*/
	apiReq_p->data.dbgLevel			= dbgLevel;							/* ���O���x��			*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP���v���擾�v��
 *  @note	PF EVENT ID : 0xA00E0009
 * 			Reponse  ID : -
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_GetTrafficLog( INT		qid,			/**< QID				*/
											 INT		wtime,			/**< �����҂�����		*/
											 VOID		*data_p )		/**< �擾�f�[�^			*/
{
	T_API_PTP_TRAFFIC_GET_REQ		*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_TRAFFIC_GET_REQ),	/* Size				*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_TRAFFIC_GET_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_TRAFFIC_GET_REQ);	/* Length			*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP�f�o�C�X��Ԏ擾�v��
 *  @note	PF EVENT ID : 0xA00E000B
 * 			Reponse  ID : -
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_GetDevState( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< �����҂�����		*/
										   VOID		*data_p )			/**< �擾�f�[�^			*/
{
	T_API_PTP_DEV_STATE_GET_REQ		*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_DEV_STATE_GET_REQ),/* Size				*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_DEV_STATE_GET_REQ;	/* Event ID			*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_DEV_STATE_GET_REQ);	/* Length			*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP clock�擾�v��
 *  @note	PF EVENT ID : 0xA00E000D
 * 			Reponse  ID : -
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_GetPtpClock( INT		qid,				/**< QID				*/
										   INT		wtime,				/**< �����҂�����		*/
										   VOID		*data_p )			/**< �擾�f�[�^			*/
{
	T_API_PTP_CLOCK_GET_REQ			*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_CLOCK_GET_REQ),/* Size				*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_CLOCK_GET_REQ;	/* Event ID			*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_CLOCK_GET_REQ);	/* Length			*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP Sync���擾�v��
 *  @note	PF EVENT ID : 0xA00E0013
 * 			Reponse  ID : 0xA00E0014
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @param	mode		: �擾���[�h
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mpr_SubscribeSyncInfo( INT		qid,		/**< QID				*/
												 INT		wtime,		/**< �����҂�����		*/
												 VOID		*data_p,	/**< �擾�f�[�^			*/
												 UINT		mode )		/**< �擾mode			*/
{
	T_API_PTP_SYNC_INFO_REQ			*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_SYNC_INFO_REQ),	/* Size				*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_SYNC_INFO_REQ;	/* Event ID				*/
	apiReq_p->header.uiSignalkind	= mode;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_SYNC_INFO_REQ);	/* Length				*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP�p�����[�^�擾�v��
 *  @note	PF EVENT ID : 0xA00E0015
 * 			Reponse  ID : 0xA00E0016
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE	duApi_Ptp_Mqr_GetPtpPram( INT		qid,				/**< QID				*/
										  INT		wtime,				/**< �����҂�����		*/
										  VOID		*data_p )			/**< �擾�f�[�^			*/
{
	T_API_PTP_PRAM_GET_REQ			*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_PRAM_GET_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_PRAM_GET_REQ;		/* Event ID			*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_PRAM_GET_REQ);	/* Length			*/

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	return E_API_RCD_OK;
}

/********************************************************************************************************************/
/**
 *  @brief	API : PTP�f�o�C�X����v��
 *  @note	PF EVENT ID : 0xA00E0017
 * 			Reponse  ID : 0xA00E0018
 * 			TYPE		: PTP
 *  @param	qid			: response queue id (�������M��QueueID)
 *  @param	wtime		: wait time(msec) for response (�����҂����ԁB���������^�ł̂ݗL��)
 *  @param	*data_p		: �擾�f�[�^�|�C���^
 *  @return	E_RRHAPI_RCODE
 *  @retval	ret
 *  @date	2018/04/26	FJT)H.Yoshida	Create.
 *  @FeatureID	-
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
/********************************************************************************************************************/
E_RRHAPI_RCODE duApi_Ptp_Mqr_CtlPtpDevice(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< �����҂�����		*/
								VOID	*data_p,	/**< �擾�f�[�^			*/
								UINT	mode,
								UINT	addr,
								UINT	value)
{
	T_API_PTP_DEV_CTL_REQ			*apiReq_p = NULL;					/* �v���o�b�t�@�|�C���^	*/
	INT								errcd;
	INT								ret;
	UINT	rcvmsgsz = 0;
	T_API_PTP_DEV_CTL_RSP*			resp = NULL;

	/* �ʐM�p���L���������擾����	*/
	ret = BPF_RU_IPCM_PROCMSG_ADDRGET(	E_BPF_RU_IPCM_BUFF_KIND_TASK,	/* Buffer���			*/
										sizeof(T_API_PTP_DEV_CTL_REQ),	/* Size					*/
										(VOID **)&apiReq_p,				/* msgP					*/
										&errcd );						/* �G���[�R�[�h			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}

	/* API�F�v���w�b�_�ݒ�	*/
	apiReq_p->header.uiEventNo		= D_API_MSGID_PTP_DEV_CTL_REQ;		/* Event ID			*/
	apiReq_p->header.uiSignalkind	= 0;								/* Signal Kind			*/
	apiReq_p->header.uiDstPQueueID	= D_RRH_PROCQUE_PTP;				/* Destination P QID	*/
	apiReq_p->header.uiDstTQueueID	= D_SYS_THDQID_PTP_MAIN;				/* Destination T QID	*/
	apiReq_p->header.uiSrcPQueueID	= qid;								/* Source P QID			*/
	apiReq_p->header.uiSrcTQueueID	= 0;								/* Source T QID			*/
	apiReq_p->header.uiLength		= sizeof(T_API_PTP_DEV_CTL_REQ);	/* Length			*/


	apiReq_p->mode = mode;
	apiReq_p->addr = addr;
	apiReq_p->value = value;

	/* PTP_MAIN�֗v�����b�Z�[�W�𑗐M����	*/
	ret = BPF_RU_IPCM_MSGQ_SEND( D_RRH_PROCQUE_PTP,						/* ���M��Proc Queue ID	*/
								 (void *)apiReq_p );					/* ���MMessage			*/

	/* OAL�������ʃ`�F�b�N	*/
	if( ret != E_API_RCD_OK )
	{
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_SEND NG %d", ret);
		return E_API_RCD_ERR_BPF_CALL;
	}
	
	/* ��������M���� */
	ret = BPF_RU_IPCM_MSGQ_RECV( qid, wtime, (void**)&resp, &rcvmsgsz );
	if( ret != E_API_RCD_OK )
	{
		if( resp != NULL ){
			/* ��M�o�b�t�@��������� */
			BPF_RU_IPCM_PROCMSG_ADDRFREE(resp);
		}
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_MSGQ_RECV NG %d", ret);
		
		/* BPF�̌��ʂɂ��AAPI ErrorCode�𕪂��� */
		if( ret == BPF_RU_IPCM_ENOMSG )
		{
			return E_API_RCD_ENOMSG;
		}
		else
		{
			return E_API_RCD_ERR_BPF_CALL;
		}
	}
	
	if( resp != NULL )
	{
		/* ���������Ăяo�����֕ԋp���� */
		if(data_p != NULL){
			memcpy(data_p, resp,rcvmsgsz);
		}
		/* ��M�o�b�t�@��������� */
		BPF_RU_IPCM_PROCMSG_ADDRFREE(resp);
	}

	return E_API_RCD_OK;
}



/** @} */

