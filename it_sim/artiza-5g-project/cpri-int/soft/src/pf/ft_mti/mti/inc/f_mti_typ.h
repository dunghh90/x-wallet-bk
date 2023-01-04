/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_mti_typ.h
 *  @brief  5GDU-LLS external IO supervision & cntrol common structure head file defination
 *  @date   2019/05/13 FJT)Taniguchi create
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2019-
 */
/*********************************************************************************/

/* ìÒèdéÊçûÇ›ñhé~ */
#ifndef		F_MTI_TYP
#define		F_MTI_TYP

/** @addtogroup RRH_PF_MTI
* @{ */


typedef struct{
	UINT				msgid;					/**< message ID  				*/
	UINT				srcFbNo;				/**< source FB number  		    */
	UINT				dstFbNo;				/**< destination FB number  	*/
	UINT				allMsgLen;				/**< total message length  		*/
	UINT				replyId;				/**< reply Id  				    */
	UINT				headSeqNo;				/**< head sequence number  	    */
	USHORT				next;					/**< next flag  				*/
	USHORT				msgLen;					/**< single message length  	*/
	UINT				nowSeqNo;				/**< current sequence number	*/
}T_MSGHEAD;

#endif	/* F_MTI_TYP */

