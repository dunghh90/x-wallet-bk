/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Fmemcpy.c
 *  @brief  copy memory to memory
 *  @date   20070316  FFCS)linjunhua create
 *  @date   2015/10/24 TDIPS)sasaki Warning対処
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2007-2015
 */

//#include "f_trp_com.h"				/*TRIFプロセス共通ヘッダ			*/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief copy memory to memory
 *  @note call BPF function
 *  @param  dstAdr_p [OUT] the target memory address
 *  @param  srcAdr_p [IN] the memmory to copy
 *  @param  cpyLen [IN] copy length
 *  @return result
 *  @retval CMD_OK success
 *  @retval CMD_NG copy failed
 *  @date 20070316 FFCS)linjunhua create
 *  @TBD_No 修正途中
 */

INT f_trp_com_Fmemcpy(VOID	*dstAdr_p, const VOID *srcAdr_p, UINT cpyLen)
{
	INT retv;

	if(dstAdr_p == NULL || srcAdr_p == NULL)
	{
//TODO
//		/*parameter error*/
//		F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_ALERT,
//			"Invalid para: NULL",
//			(UCHAR *)NULL,
//			CMD_NUM0);
		return CMD_NG;
	}

	/*  メモリコピー */
	retv = BPF_RU_MACC_BCOPY(dstAdr_p, srcAdr_p, cpyLen);
	if(retv != BPF_RU_MACC_OK)
	{
//TODO
//		/*  パラメータ異常 or メモリコピー失敗  */
//		F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_ALERT,
//			"memory copy NG.",
//			(UCHAR *)&retv,
//			sizeof(retv));
		return CMD_NG;
	}


	return CMD_OK;
}
/* @} */  /* group TRIF_TCM */

