/*!
 *  @skip   $Id:$
 *  @file   f_trp_com_Fget_precisetim.c
 *  @brief  to get the current timer value
 *  @date   20070322  FFCS)linjunhua create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2007
 */
 
#include "f_trp_com.h"				/*TRIFプロセス共通ヘッダ			*/

/*! @addtogroup TRIF_TCM
 * @{ */

/*!
 *  @brief to get the current timer value
 *  @note firstly, get the current timer, secondly, convert the format
 *  @param  tim [OUT] the timer to return
 *  @param  ms [OUT] the millisecond to return
 *  @return result
 *  @retval CMD_NG failed to get timer
 *	@retval CMD_OK success
 *  @date 20070320 FFCS)linjunhua create
 */
  
INT f_trp_com_Fget_precisetim(struct bpf_tm* tim, INT* ms)
{
	struct		bpf_timeval tv;
	INT			retv;
	//time_t*		time_adr;
	
	if(tim == NULL || ms == NULL)
	{
		/*parameter error*/
		F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_ALERT, 
			"Invalid para: NULL",
			(UCHAR *)NULL,
			CMD_NUM0);
		return CMD_NG;
	}
	
	/*******現在時刻を取得******/
	retv = BPF_RU_UTTM_GETTIME(&tv);
	if(retv != BPF_RU_UTTM_OK)
	{
		/*  時刻取得失敗  */
		F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_ALERT, 
			"get time NG",  
			(UCHAR *)&retv,
			sizeof(retv));
		return CMD_NG;
	}
	
	//time_adr = &tv.tv_sec;
		
	/*******時刻西暦算出*********/
	retv = BPF_RU_UTTM_GETLOCALTIME(&tv.tv_sec, tim, ms);
	if(retv != BPF_RU_UTTM_OK)
	{
		/*  算出失敗 or パラメータ異常  */
		F_COMF_CMM_FINVDLOG(D_TCM_INVLOGLV_ALERT, 
			"convert time NG",  
			(UCHAR *)&retv,
			sizeof(retv));
		return CMD_NG;
	}
	
	// *ms = tv.tv_usec / 1000;
	
	return CMD_OK;
}
/* @} */  /* group TRIF_TCM */

