/*!
 * @skip 		$Id:$
 * @file 		f_eom_typ.h
 * @brief		EOM thread type header
 * @date		2018/11/23 FJT)H.Yoshida Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
*/

/** @addtogroup RRH_PF_EOM

 *  @{
 */
#ifndef F_EOM_TYP_H
#define F_EOM_TYP_H

/****************************************************************************************************************/
/*  Information of EOM Thread																					*/
/****************************************************************************************************************/
/*!
 * @name Information of EOM Thread
 * @note Information of EOM Thread
 * @{
 */
typedef struct{
	UINT							thdState;							/* Thread状態			*/
}T_EOM_INFO;
/* @} */

/****************************************************************************************************************/
/*  Matrix function of EOM Thread																				*/
/****************************************************************************************************************/
/*!
 * @name	Matrix function of EOM Thread
 * @note	Matrix function of EOM Thread
 * @{
 */
typedef struct{
	INT								(*eventProcess_addr)(VOID *msgPtr);	/* マトリクス関数		*/
}T_EOM_CALLSETRELL_MTXTBL;
/* @} */

/****************************************************************************************************************/
/*  Time information for SAA response																			*/
/****************************************************************************************************************/
/*!
 * @name	LBR/LBM Frame Format
 * @note	LBR/LBM Frame Format
 * @{
 */
typedef struct {
	char				dst_mac_addr[6];				/* Destination address					*/
	char				src_mac_addr[6];				/* Source address						*/
	char				eth_type[2];					/* EtherType (CFM :0x8902)				*/
} T_EOM_ETH_HEADER;

typedef struct {
	char				version;						/* version (MA-Level:3bit + Ver:5bit)	*/
	char				OpCode;							/* OpCode (LBM=0x03/LBR=2)				*/
	char				Flag;							/* Flag (0固定)							*/
	char				TLVoffset;						/* TLV_Offset (4固定)					*/
	char				TransactionID[4];				/* TransactionID						*/
	char				PatternData[2000];				/* パターンデータ						*/
} T_EOM_ETH_PAYLOAD;

typedef struct {
	T_EOM_ETH_HEADER	head;							/* Etherヘッダ							*/
	T_EOM_ETH_PAYLOAD	body;							/* Payload								*/
} T_EOM_LBX_FRAME;
/* @} */


#endif
/** @} */
