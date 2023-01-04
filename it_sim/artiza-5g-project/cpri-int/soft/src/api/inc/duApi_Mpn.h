/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Struct Definition(RRH API common part)
 *  @note	Struct Definition(RRH API common part)
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2018
 */
/******************************************************************************************************************************/

/*!
 * @addtogroup RRH_API
 * @{
 */
#ifndef RRHAPI_MPN_H
#define RRHAPI_MPN_H
/*!
 * @addtogroup RRH_API_MPN
 * @{
 */

#define D_API_MSGID_MPN_BASE						0xA00F0000
/** @} */

/******************************************************************************************************************************/
/** T_API_MPLANE_READY
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of M-Plane coonection ready request
 * @note	Structure of M-Plane coonection ready request is...
 */
typedef struct{
	USHORT							Interface;		/**< Interface			*/
	USHORT							Flag;			/**< Flag				*/
	USHORT							lowVid;			/**< 検索VID最小値		*/
	USHORT							highVid;		/**< 検索VID最大値		*/
	USHORT							lastUsedVid;	/**< VID前回値			*/
}T_API_MPLANE_READY_REQ_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_MPLANE_READY_REQ_DATA		data;
}T_API_MPLANE_READY_REQ;
/** @} */

/**
 * @brief	Structure of M-Plane coonection ready response
 * @note	Structure of M-Plane coonection ready response is...
 */
typedef struct{
	USHORT							dhcpFlag;		/**< DHCP状態			*/
	char							ifname[32];		/**< Interface name		*/
	USHORT							vidAcquiringMethod;	/**< VID前回値		*/
	UCHAR							remoteMacAddress[6];/**< MAC Addr (CU)	*/
	USHORT							vid;			/**< VID				*/
	USHORT							pcp;			/**< PCP				*/
	UINT							localIpAddress;	/**< IP Address (DU)	*/
	UINT							remoteIpAdress;	/**< IP Address (CU)	*/
	UINT							defaultGateway;	/**< Default Gateway	*/
	UINT							subnetMask;		/**< サブネットマスク	*/
}T_API_MPLANE_READY_RSP_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_MPLANE_READY_RSP_DATA		data;
}T_API_MPLANE_READY_RSP;
/** @} */

/**
 * @brief	Message id of M-Plane coonection ready
 * @note	Defined massage id of M-Plane coonection ready request is...
 */
#define D_API_MSGID_MPN_MPLANE_READY_REQ			0xA00F0001
#define D_API_MSGID_MPN_MPLANE_READY_RSP			0xA00F0002

#define	D_API_INTERFACE_MPN_MPLANE_READY_FHPORT0	0
#define	D_API_INTERFACE_MPN_MPLANE_READY_FHPORT1	1
#define	D_API_INTERFACE_MPN_MPLANE_READY_DPORT		2

#define	D_API_FLAG_MPN_MPLANE_READY_D				0x0001
#define	D_API_FLAG_MPN_MPLANE_READY_V				0x0002
#define	D_API_FLAG_MPN_MPLANE_READY_VD				0x0003
/** @} */

/**
 * @brief	API Function of M-Plane coonection ready
 * @note	API Function of M-Plane coonection ready is...
 */
extern E_RRHAPI_RCODE duApi_Mpn_Mnr_MplaneReady(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								USHORT	Interface,	/**< Interface指定		*/
								USHORT	Flag,		/**< 接続指定			*/
								USHORT	lowVid,		/**< 検索VID最小値		*/
								USHORT	highVid,	/**< 検索VID最大値		*/
								USHORT	lastUsedVid);	/**< VID前回値		*/
/** @} */

/******************************************************************************************************************************/
/** T_API_MPLANE_START
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of M-Plane coonection start request
 * @note	Structure of M-Plane coonection start request is...
 */
typedef struct{
	UINT							local_ipadr;	/**< Local  IP Address	*/
	UINT							remote_ipadr;	/**< Remote IP Address	*/
}T_API_MPLANE_START_REQ_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_MPLANE_START_REQ_DATA		data;
}T_API_MPLANE_START_REQ;
/** @} */

/**
 * @brief	Structure of M-Plane coonection start response
 * @note	Structure of M-Plane coonection start response is...
 */
typedef struct{
	INT								result;			/**< 処理結果			*/
}T_API_MPLANE_START_RSP_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_MPLANE_START_RSP_DATA		data;
}T_API_MPLANE_START_RSP;
/** @} */

/**
 * @brief	Message id of M-Plane coonection start
 * @note	Defined massage id of M-Plane coonection start is...
 */
#define D_API_MSGID_MPN_MPLANE_START_REQ			0xA00F0003
#define D_API_MSGID_MPN_MPLANE_START_RSP			0xA00F0004
/** @} */

/**
 * @brief	API Function of M-Plane coonection start
 * @note	API Function of M-Plane coonection start is...
 */
extern E_RRHAPI_RCODE duApi_Mpn_Mnr_MplaneStart(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								UINT	local_ipadr,	/**< DU側IPアドレス	*/
								UINT	remote_ipadr);	/**< CU側IPアドレス	*/
/** @} */


/******************************************************************************************************************************/
/** T_API_MPLANE_RECV
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of M-Plane message reception ready request
 * @note	Structure of M-Plane message reception ready request is...
 */
typedef struct{
	T_API_COMMON_HEADER				header;
}T_API_MPLANE_RECV_REQ;
/** @} */

/**
 * @brief	Structure of M-Plane message reception ready notification
 * @note	Structure of M-Plane message reception ready notification is...
 */
typedef struct{
	UINT							msg_size;		/**< メッセージサイズ	*/
	UCHAR							msg_data[4];	/**< 受信メッセージ		*/
}T_API_MPLANE_RECV_NTF_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_MPLANE_RECV_NTF_DATA		data;
}T_API_MPLANE_RECV_NTF;
/** @} */

/**
 * @brief	Message id of M-Plane message reception ready
 * @note	Defined massage id of M-Plane message reception ready is...
 */
#define D_API_MSGID_MPN_MPLANE_RECV_REQ				0xA00F0005
#define D_API_MSGID_MPN_MPLANE_RECV_NTF				0xA00F0006

/**
 * @brief	M-Plane message reception ready API
 * @note	Function of M-Plane message reception ready is...
 */
extern E_RRHAPI_RCODE duApi_Mpn_Mpr_SubscribeMplaneRecv(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p);	/**< 取得データ			*/
/** @} */


/******************************************************************************************************************************/
/** T_API_MPLANE_SEND
 *  @addtogroup
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Structure of M-Plane message send request
 * @note	Structure of M-Plane message send request is...
 */
typedef struct{
	UINT							msg_size;		/**< メッセージサイズ	*/
	UCHAR							msg_data[4];	/**< 送信メッセージ		*/
}T_API_MPLANE_SEND_REQ_DATA;

typedef struct{
	T_API_COMMON_HEADER				header;
	T_API_MPLANE_RECV_NTF_DATA		data;
}T_API_MPLANE_SEND_REQ;
/** @} */

/**
 * @brief	Message id of M-Plane message send
 * @note	Defined massage id of M-Plane message sendt is...
 */
#define D_API_MSGID_MPN_MPLANE_SEND_REQ				0xA00F0007

/**
 * @brief	M-Plane message send API
 * @note	Function of M-Plane message send is...
 */
extern E_RRHAPI_RCODE duApi_Mpn_Mqr_MplaneSend(
								INT		qid,		/**< QID				*/
								INT		wtime,		/**< 応答待ち時間		*/
								VOID	*data_p,	/**< 取得データ			*/
								UINT	msg_size,	/**< メッセージサイズ	*/
								UCHAR*	msg_data_p);	/**< 送信メッセージ	*/
/** @} */

#endif
/** @} */
