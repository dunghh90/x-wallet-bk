/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(message I/F)
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also プロセス間IF仕様書(xxx-API).xls
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2013-2015
 */
/******************************************************************************************************************************/
#include "rrhApi_Com.h"
/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_L2_H
#define RRHAPI_L2_H

/*!
 * @addtogroup RRH_API_L2
 * @{
 */
#define D_API_MSGID_L2_BASE							0xA00B0000
#define D_API_MSGID_L2_LINK_SETUP_REQ				0xA00B0001
#define D_API_MSGID_L2_LINK_SETUP_RSP				0xA00B0002
#define D_API_MSGID_L2_LINK_REL_REQ					0xA00B0003
#define D_API_MSGID_L2_LINK_REL_RSP					0xA00B0004
#define D_API_MSGID_L2_LINK_DEACT_IND				0xA00B0005
#define D_API_MSGID_L2_CPRIMSG_SUB_REQ				0xA00B0007
#define D_API_MSGID_L2_CPRIMSG_RCV_NTC				0xA00B0008
#define D_API_MSGID_L2_CPRIMSG_UNSUB_IND			0xA00B0009
#define D_API_MSGID_L2_CPRIMSG_SEND_IND				0xA00B000B
#define D_API_MSGID_L2_LINKSTATE_SUB_REQ			0xA00B000D
#define D_API_MSGID_L2_LINKSTATE_NTC				0xA00B000E
#define D_API_MSGID_L2_LINKSTATE_UNSUB_IND			0xA00B000F
#define D_API_MSGID_L2_CPRIMSG_STASTIC_IND			0xA00B0011
#define D_API_MSGID_L2_LINK_STOP_IND				0xA00B0013

/*!
 * @addtogroup RRH_API_PL
 * @{
 */
#define D_API_MSGID_PL_LINK_SETUP_REQ				0xA00D0001
#define D_API_MSGID_PL_LINK_REL_REQ					0xA00D0003
#define D_API_MSGID_PL_LINK_DEACT_IND				0xA00D0005
#define D_API_MSGID_PL_CPRIMSG_SUB_REQ				0xA00D0007
#define D_API_MSGID_PL_CPRIMSG_RCV_NTC				0xA00D0008
#define D_API_MSGID_PL_CPRIMSG_UNSUB_IND			0xA00D0009
#define D_API_MSGID_PL_CPRIMSG_SEND_IND				0xA00D000B
#define D_API_MSGID_PL_LINKSTATE_SUB_REQ			0xA00D000D
#define D_API_MSGID_PL_LINKSTATE_NTC				0xA00D000E
#define D_API_MSGID_PL_LINKSTATE_UNSUB_IND			0xA00D000F
#define D_API_MSGID_PL_CPRIMSG_STASTIC_IND			0xA00D0011
#define D_API_MSGID_PL_LINK_STOP_IND				0xA00D0013


#define D_API_L2_STACHG_EST		1
#define D_API_L2_STACHG_REL		2
#define D_API_L2_STACHG_ABN		3
/** @} */

/*!
 * @addtogroup RRH_API_L2
 * @{
 */
 /******************************************************************************************************************************/
/** T_API_L2LINK_SETUP_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of setup lapb link
 * @note	The request of setup lapb link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT link_num;
}T_API_L2LINK_SETUP_REQ;
/** @} */

/**
 * @brief	The response of setup lapb link
 * @note	The response of setup lapb link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA	data;
}T_API_L2LINK_SETUP_RSP;
/** @} */

 /******************************************************************************************************************************/
/** T_API_L2LINK_REL_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of setup lapb link
 * @note	The request of setup lapb link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT link_num;
}T_API_L2LINK_REL_REQ;
/** @} */

/**
 * @brief	The response of releasing lapb link
 * @note	The response of releasing lapb link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	T_API_COM_RSP_DATA data;
}T_API_L2LINK_REL_RSP;
/** @} */

 /******************************************************************************************************************************/
/** T_API_L2LINK_DEACT_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of deacting lapb link
 * @note	The indication of deacting lapb link is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT link_num;
}T_API_L2LINK_DEACT_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRIMSG_SUB_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The request of the subscription of receving CPRI message
 * @note	The request of the subscription of receving CPRI message is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	UINT	            msgCount;
	UINT	            msgs[256];
}T_API_CPRIMSG_SUB_DATA;
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT  link_num;
	T_API_CPRIMSG_SUB_DATA data;
}T_API_CPRIMSG_SUB_REQ;

/**
 * @brief	The notice of receving cpri message
 * @note	The notice of receving cpri message is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
	VOID *data;
	/*T.B.D*/
}T_API_CPRIMSG_RCV_NTC;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRIMSG_UNSUB_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of cancelling subscription of receving CPRI 
 * @note	The indication of the subscription of receving CPRI message is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num', 'system_type')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT link_num;
	USHORT system_type;
}T_API_CPRIMSG_UNSUB_IND;
/** @} */


/******************************************************************************************************************************/
/** T_API_L2_LINKSTATE_SUB_REQ
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/**
 * @brief	The request of the subscription of LAPB link state
 * @note	The request of the subscription of LAPB link state is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT				link_num;
}T_API_L2_LINKSTATE_SUB_REQ;
/** @} */

/**
 * @brief	The notice of lapb link state
 * @note	The notice of lapb link state is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	UINT                state;
	USHORT				link_num;
}T_API_L2_LINKSTATE_CHG_DATA;

typedef struct{
	T_API_COMMON_HEADER header;
	T_API_L2_LINKSTATE_CHG_DATA                data;
}T_API_L2_LINKSTATE_CHG_NTC;
/** @} */

/******************************************************************************************************************************/
/** T_API_L2_LINKSTATE_UNSUB_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/******************************************************************************************************************************/
/**
 * @brief	The indication of the unsubscription of LAPB link state 
 * @note	The indication of the unsubscription of LAPB link state is ..
 * @date    2020/12/28 M&C) Merge 4G FHM src (add 'link_num')
 */
typedef struct{
	T_API_COMMON_HEADER header;
	USHORT link_num;
}T_API_L2_LINKSTATE_UNSUB_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRIMSG_SND_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of sending CPRI message 
 * @note	The indication of sending CPRI message is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_CPRIMSG_SND_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_CPRIMSG_SND_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of sending CPRI message 
 * @note	The indication of sending CPRI message is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_L2_CPRIMSG_STASTICS_IND;
/** @} */

 /******************************************************************************************************************************/
/** T_API_L2LINK_STOP_IND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	The indication of stopping lapb link
 * @note	The indication of stopping lapb link is ..
 */
typedef struct{
	T_API_COMMON_HEADER header;
}T_API_L2LINK_STOP_IND;
/** @} */

/* 2020/12/28 M&C) Merge 4G FHM src (start add param 'linkno') */
extern E_RRHAPI_RCODE rrhApi_L2_Mqr_SetupLink(
		USHORT linkno, INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_L2_Mqr_ReleaseLink(	
		USHORT linkno, INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_L2_Mnt_DeactLapbLink(
		USHORT linkno, INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_L2_Mpr_SubscribeCpriMsg(	USHORT linkno,
								INT		qid, INT		wtime,VOID	*data_p,
								UINT*   msgs,UINT    nMsg );
extern E_RRHAPI_RCODE rrhApi_L2_Mnt_UnSubscribeCpriMsg( USHORT linkno,
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_L2_Mpr_SubscribeLinkState( USHORT linkno,
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_L2_Mnt_UnsubscribeLinkState(	USHORT linkno,
								INT		qid, INT		wtime,VOID	*data_p);
/* 2020/12/28 M&C) Merge 4G FHM src (end add param 'linkno') */

/* 2020/12/28 M&C) Merge 4G FHM src (start add param 'linkno', 'system') */
extern E_RRHAPI_RCODE rrhApi_L2_Mnt_CpriMsgStatistic(USHORT linkno,		USHORT system,
								INT		qid, INT		wtime,VOID	*data_p);
/* 2020/12/28 M&C) Merge 4G FHM src (end add param 'linkno', 'system') */

extern E_RRHAPI_RCODE rrhApi_L2_Mnt_SendCpriMsg(	INT		qid, 
								INT		wtime,VOID	*data_p,VOID	*msg_p  );
extern E_RRHAPI_RCODE rrhApi_L2_Mnt_StopLapbLink(
								INT		qid, INT		wtime,VOID	*data_p);


extern E_RRHAPI_RCODE rrhApi_PL_Mqr_SetupLink(
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mqr_ReleaseLink(	
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mnt_DeactLapbLink(
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mpr_SubscribeCpriMsg(	
								INT		qid, INT		wtime,VOID	*data_p,
								UINT*   msgs,UINT    nMsg );
extern E_RRHAPI_RCODE rrhApi_PL_Mnt_UnSubscribeCpriMsg(	
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mpr_SubscribeLinkState(	
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mnt_UnsubscribeLinkState(	
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mnt_CpriMsgStatistic(	
								INT		qid, INT		wtime,VOID	*data_p);
extern E_RRHAPI_RCODE rrhApi_PL_Mnt_SendCpriMsg(	INT		qid, 
								INT		wtime,VOID	*data_p,VOID	*msg_p  );
extern E_RRHAPI_RCODE rrhApi_PL_Mnt_StopLapbLink(
								INT		qid, INT		wtime,VOID	*data_p);


/** @} */
#endif

