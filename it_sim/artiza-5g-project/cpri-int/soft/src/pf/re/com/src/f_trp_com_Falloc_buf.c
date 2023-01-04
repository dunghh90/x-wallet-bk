/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Falloc_buf.c
 *  @brief  alloc buffer
 *  @date   2007/03/19  FFCS)linjunhua create
 *	@date	2008/07/29	FFCS)Tangj	S3G PRC eNB-008-002
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */
 

#include "m_cm_header.h"

/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief to alloc buffer
 *  @note step:
 *		1.change buffer size to buffer type
 *		2.call BPF_RU_MBLM_BUFFERALLOC function to alloc buffer from system
 *		3.check the allocated buffer is busy or not. 
 *			If busy over threshold value, send 自受信ビジー発生通知 to LAPB.
 *			If busy under threshold value, send 自受信ビジー解除通知 to LAPB.
 *  @param  buf_size [IN] the buffer size, can not larger than 16384
 *  @param  buf_addr [OUT] the buffer address that return to caller
 *  @return result
 *  @retval CMD_NG failed to allocate buffer
 *  @retval CMD_OK success
 *  @date 20070319 FFCS)linjunhua create
 *  @date 2015/08/12 FPT)Yen update
 */
  

INT f_trp_com_Falloc_buf(UINT buf_size, VOID** buf_addr)
{
	UINT	ldwRtn;

	ldwRtn = cm_L2BfGet( CMD_BUFCA_LAPSND,
						   buf_size,
						   CMD_NUM1,
						   buf_addr );
	/* 戻り値判定 */
	if( ldwRtn != CMD_RES_OK ){
		/* ASSERT */
		cm_Assert(	CMD_ASL_USELOW, ldwRtn,"f_trp_com_Falloc_buf cm_L2BfGet NG");
	}

	return ldwRtn;
}
/* @} */  /* group TRIF_TCM */

