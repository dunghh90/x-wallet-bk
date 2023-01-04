
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also �v���Z�X��IF�d�l��(xxx-API).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/******************************************************************************************************************************/

#include "rrhApi_Com.h"
/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_CPRI_H
#define RRHAPI_CPRI_H

/*!
 * @addtogroup RRH_API_CPRI
 * @{
 */
#define D_API_MSGID_CPRI_BASE						0xA0000000
#define D_API_MSGID_CPRI_LINK_STARTUP_IND			0xA0000001
#define D_API_MSGID_CPRI_LINK_ENABLE_IND			0xA0000005
#define D_API_MSGID_CPRI_LINK_DISABLE_IND			0xA0000007
#define D_API_MSGID_CPRI_STATE_SUB_REQ				0xA0000009
#define D_API_MSGID_CPRI_STATE_STATE_NTC			0xA000000A
#define D_API_MSGID_CPRI_STATE_UNSUB_IND			0xA000000B
#define D_API_MSGID_CPRI_PARAM_UPDATE_IND			0xA000000C
#define D_API_MSGID_CPRI_FIBEROUTPU_IND				0xA0000010
#define D_API_MSGID_CPRI_LINK_SETUP_DEBUG			0xA0000011
/* 2020/12/29 M&C) Merge 4G FHM src (start add) */
#define D_API_MSGID_CPRI_SET_REUSE_IND              0xA0000013      /**< RE use status set request                            */
#define D_API_MSGID_CPRI_LINK_STOP_IND              0xA0000015      /**< CPRI link stop indication                            */
#define D_API_MSGID_CPRI_DLY_ADJ_STEP3_REQ          0x40000017      /**< �x���␳�v��(STEP3)                                  */
#define D_API_MSGID_CPRI_DLY_ADJ_STEP7_REQ          0x40000019      /**< �x���␳�v��(STEP7)                                  */
#define D_API_MSGID_CPRI_DLY_ADJ_STEP7_RSP          0x4000001A
#define D_API_MSGID_CPRI_DLY_ADJ_STEP10_REQ         0x4000001B
#define D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSP         0x4000001C
#define D_API_MSGID_CPRI_CPMPOWERON_IND             0x4000001D		/**< ���݂ɂ��CPM#n�d��ON�v��                            */
#define D_API_MSGID_CPRI_DLY_ADJ_CLEAR_REQ			0x4000001E
/* 2020/12/29 M&C) Merge 4G FHM src (end add) */
/** @} */

/*!
 * @addtogroup RRH_API_CPRI
 * @{
 */
 /******************************************************************************************************************************/
/** T_API_CPRILINK_STARTUP_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of startup cpri link
 * @note	The request of startup cpri link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num', 'sfpLogTrigger')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT		link_num;
	UINT        sfpLogTrigger;
}T_API_CPRILINK_STARTUP_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRILINK_STOP_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of stop cpri link
 * @note	The request of stop cpri link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT link_num;
}T_API_CPRILINK_STOP_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRILINK_ENABLE_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of setup cpri link
 * @note	The request of setup cpri link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT	link_num;				/**< CPRI �ԍ� */
}T_API_CPRILINK_ENABLE_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRILINK_DISABLE_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of setup cpri link
 * @note	The request of setup cpri link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT	link_num;				/**< CPRI �ԍ� */
}T_API_CPRILINK_DISABLE_IND;
/** @} */
 /******************************************************************************************************************************/
/** T_API_CPRIMSG_SUB_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of the subscription of receving CPRI state
 * @note	The request of the subscription of receving CPRI state is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT	link_num;				/**< CPRI �ԍ� */
}T_API_CPRILINK_STATE_SUB_REQ;
/**
 * @brief	The request of the subscription of receving CPRI state
 * @note	The request of the subscription of receving CPRI state is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT	link_num;				/**< CPRI �ԍ� */
	UINT	cpriHDLCBitrate;
	UINT	cpriLineBitrate;
	UINT	cpriHdlc;
	UINT	cpriEDown;
	UINT	hfnSync;
	UINT	cpriState;
	UINT	cscdKind;
}T_API_CPRILINK_STATE_NTC;
/** @} */

/******************************************************************************************************************************/
/** T_API_CPRILINK_STATE_UNSUB_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of the unsubscription of receving CPRI state
 * @note	The request of the unsubscription of receving CPRI state is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_CPRILINK_STATE_UNSUB_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRILINK_PARAM_UPDATE_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notice of updating related cpri system parameter
 * @note	The notice of receving related cpri system parameter is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	stableValue;
	/*T.B.D*/
}T_API_CPRILINK_PARAM_UPDATE_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_CPRILINK_PARAM_UPDATE_DATA data;
	/*T.B.D*/
}T_API_CPRILINK_PARAM_UPDATE_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRILINK_PARAM_UPDATE_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The notice of cpri fiber ouptut
 * @note	The notice of rcpri fiber ouptut is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT	link_num;
	UINT	onOff; 
}T_API_CPRILINK_FIBEROUTPUT_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRILINK_SETERROR_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of HDLC Statistics Collection  
 * @note	The indication of HDLC Statistics Collection  is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	UINT	onOff;
}T_API_CPRILINK_SETERROR_IND;

/**
 * @brief   RE�g�p/���g�p�ݒ�ʒm
 * @date    2015/10/05 TDI)satou create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_API_COMMON_HEADER header;             /**< �w�b�_                             */
    USHORT              link_num;           /**< CPRI�����N�ԍ�                     */
    USHORT              onoff;              /**< RE�g�p(D_RRH_ON)/���g�p(D_RRH_OFF) */
} T_API_CPRILINK_SET_REUSE_IND;

/**
 * @brief   The request of D_API_MSGID_CPRI_DLY_ADJ_STEP7_REQ
 * @date    2015/11/05 TDI)satou ���̑�������-008 create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_API_COMMON_HEADER header;
    USHORT tgt_re[D_RRH_CPRINO_RE_MAX];
    INT param_check_en;
} T_API_CPRILINK_ADJUST_DELAY_STEP7;

/**
 * @brief   The request of D_API_MSGID_CPRI_DLY_ADJ_STEP10_REQ
 * @date    2015/11/09 TDI)satou ���̑�������-017 create
 * @date	2016/04/20 TDI)satou forceStopEn�ǉ�
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_API_COMMON_HEADER header;
    USHORT cpr_num;
    USHORT isStartup;
    /*
     * ������~�ւ̑J�ڗL��(D_RRH_ON)/����(D_RRH_OFF).
     *
     * LTE-REC�����Toffset�l�񍐗v����M���ACPRI�����N�f��������̒x���␳�̎�����
     * �ő�x���ʒ��߂̋�����~�̊Ď�(�ی�܂߂�)���s��. 3G-REC�����Toffset�l�񍐗v��
     * ��M���̒x���␳�ł́A�ő�x���ʒ��߂����Ă��邩�ǂ����������Ď����A������~�ւ̑J�ڂ�
     * �ی�i���̍X�V�͍s��Ȃ�. �ڍׂ́A f_cpr_DelayAdjProc.c ��Step10���Q��.
     */
    USHORT forceStopEn;
} T_API_CPRILINK_ADJUST_DELAY_STEP10;

/**
 * @brief   The response of D_API_MSGID_CPRI_DLY_ADJ_STEP7_RSP
 * @date    2015/11/-7 TDI)satou ���̑�������-017
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_API_COMMON_HEADER header;
    USHORT result_re[D_RRH_CPRINO_RE_MAX];
} T_API_CPRILINK_ADJUST_DELAY_STEP7_RSP;

/**
 * @brief   The response of D_API_MSGID_CPRI_DLY_ADJ_STEP10_RSP
 * @date    2016/01/29 TDI)satou eNB-F-15B-3201-00115
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
	T_API_COMMON_HEADER header;
	USHORT cpr_num;
	USHORT result;
} T_API_CPRILINK_ADJUST_DELAY_STEP10_RSP;

/**
 * @brief   RE�g�p����SFP�d��ON�ʒm
 * @date    2015/12/25 FJT)koshida create
 * @date    2020/12/28 M&C) Merge 4G FHM src
 */
typedef struct {
    T_API_COMMON_HEADER header;             /**< �w�b�_                             */
    USHORT              link_num;           /**< CPRI�����N�ԍ�                     */
}T_API_MSGID_CPRI_CPMPOWERON_IND;

/* 2020/12/28 M&C) Merge 4G FHM src (start add param 'linkno', 'sfpLogTrigger') */
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_StartupLink(USHORT linkno, UINT sfpLogTrigger,
								INT		qid, INT		wtime,VOID *data_p);
/* 2020/12/28 M&C) Merge 4G FHM src (end add param 'linkno', 'sfpLogTrigger') */

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_StopLink(INT qid, USHORT linkno);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/* 2020/12/28 M&C) Merge 4G FHM src (start add param 'linkno') */
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_EnableLink(USHORT linkno,
								INT		qid, 	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_DisableLink(USHORT linkno,
								INT		qid,	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mpr_SubscribeLinkState(USHORT linkno,
								INT		qid,	INT		wtime,VOID *data_p);
/* 2020/12/28 M&C) Merge 4G FHM src (end add param 'linkno') */

extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_UnsubscribeLinkState(
								INT		qid, 	INT		wtime,VOID *data_p);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_UpdateSystemPara(
								INT		qid, 	INT		wtime,VOID *data_p,
								UINT	value);
/* 2020/12/28 M&C) Merge 4G FHM src (start add param 'linkno') */
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_FiberOutput(USHORT linkno,
								INT		qid, 	INT		wtime,VOID *data_p,UINT	onOff);
/* 2020/12/28 M&C) Merge 4G FHM src (end add param 'linkno') */

extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_SetupLink_Dbg(
								INT		qid,	INT		wtime,VOID	*data_p,
								UINT	cpriHDLCBitrate,	UINT	cpriLineBitrate, UINT	setcpriState,
								UINT    linkno);

/* 2020/12/28 M&C) Merge 4G FHM src (start add) */
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_SetReUse(INT qid, USHORT linkno, UINT onOff);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_DelayAdjustStep3(INT qid);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_DelayAdjustStep7(INT qid, INT delayPrmChkEn, USHORT *tgt_re, T_API_CPRILINK_ADJUST_DELAY_STEP7_RSP *apiRsp);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_DelayAdjustStep10(INT qid, USHORT cpr_num, USHORT isStartup, USHORT forceStopEn);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_DelayAdjustClear(INT qid);
extern E_RRHAPI_RCODE rrhApi_Cpri_Mnt_SetSfpPwr(INT qid, USHORT linkno);
/* 2020/12/28 M&C) Merge 4G FHM src (end add) */

/***************************************************************************************************************************//**
 *
 * @} addtogroup RRH_API_CPRI
 *
 ******************************************************************************************************************************/

#endif
/** @} */
