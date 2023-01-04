/*!
 *  @skip   $Id:$
 *  @file   l2_main_systemInit.c
 *  @brief  system init 
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */


 /*!
 * @addtogroup RRH_L2_MAIN
 * @{
 */
 

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "l2_com_inc.h"
#include "l2_main_inc.h"

/********************************************************************************************************************/
/*!
 *  @brief  system init.
 *  @note   send system init complete notice to init process and init system
 *  @param  -      [in]  -
 *  @param  -      [out] -
 *  @return Result code is returned
 *  @retval -
 *  @date   2013/11/15 FFCS)hongj Create for zynq
 */
/********************************************************************************************************************/
VOID l2_main_systemInit(void)
{
	INT 				ret;
	INT					errcd;							/* error code				*/
	UINT				rcvmsgsz;						/* receivied message size	*/
	L2_SUPERVISION_NOTI supervision_noti;
	UCHAR				rcvBuf[512];

	/* receive system init from init process*/
	ret = l2_com_comMsgRcv(
			CMD_TSKID_INI,				/* Process queue ID		*/
			sizeof(rcvBuf),					/* received buffer size    */
			(VOID *)rcvBuf, 				/* received buffer		   */
			&rcvmsgsz,						/* received message length */
			&errcd							/* Error code			   */
		);
	/* checking parameter */
	if( ret != CMD_OK ) {
		cm_Assert( CMD_ASL_DBGLOW, ret,
			(CHAR*)"[l2_main] Receive system init from init process NG" );

		return;
	}

	/* send l2 link supervision notice to pol thread */
	memset(&supervision_noti, 0, sizeof(supervision_noti));
	supervision_noti.header.uiEventNo		= CMD_TSKIF_POLINTRNTC;			/**< Event	ID		   */
	supervision_noti.header.uiSignalkind	= 0;							/**< Signal Kind	   */
	supervision_noti.header.uiDstPQueueID	= D_RRH_PROCQUE_L2;				/**< Destination P QID */
	supervision_noti.header.uiDstTQueueID	= CMD_TSKID_POL;				/**< Destination T QID */
	supervision_noti.header.uiSrcPQueueID	= D_RRH_PROCQUE_L2;				/**< Source P QID	   */
	supervision_noti.header.uiSrcTQueueID	= CMD_TSKID_INI;				/**< Source T QID	   */
	supervision_noti.header.uiLength		= sizeof(supervision_noti);		/**< Length 		   */

	l2_com_comMsgSnd(
			CMD_TSKID_POL,					/* Thread queue ID		   */
			CMD_TSKIF_POLINTRNTC,			/* message event		   */
			(VOID *)&supervision_noti,		/* The message to transmit */
			sizeof(supervision_noti)		/* The size of message	   */
		);

	return;	
}


/* @} */

