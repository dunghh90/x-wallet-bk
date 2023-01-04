/*!
 * @skip  $ld:$
 * @file  f_esmc_typ.h
 * @brief 構造体宣言
 * @date  2018/08/28
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2018
 */

/*!
 * @addtogroup PF_PTP
 * @{
 */
#ifndef F_ESMC_TYP_H
#define F_ESMC_TYP_H

/**
 * @brief	ESMC PDUフォーマット
 * 
 */
struct esmc_header{
	char				destAddr[6];					/* Destination address					*/
	char				sourceAddr[6];					/* Source address						*/
	char				slowprotocolEth[2]; 			/* Slow protocol Ethertype				*/
	char				slowprotocolSub; 				/* Slow protocol subtype				*/
	char				itu_oui[3];						/* itu oui 								*/
	char				itu_subtype[2];					/* itu subtype 							*/
	char				verflag;						/* Version + Eventflag 		 			*/
	char				reserve1[3];					/* Reserve								*/
	char				type;							/* Type									*/
	char				length[2];						/* Length								*/
	char				ssmcode;						/* 下位4ビットがSSMCODE					*/
	int					datapad[8];						/* Data and padding				 0固定 	*/
} __attribute__((packed));

#endif
/* @} */
