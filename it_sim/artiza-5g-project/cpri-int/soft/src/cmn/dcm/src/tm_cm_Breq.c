/***************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   tm_cm_Breq.c
 *  @brief  common function - buffer hunt function
 *  @date   2008/07/18 FFCS)Tangj create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2008-2013
 */
/***************************************************************************/

/** @addtogroup TRA_COM
 *  @{
 */

/****************************************************************************************************************************/
/* ヘッダインクルード																										*/
/****************************************************************************************************************************/
#include "m_cm_header.h"		/* TRA共通ヘッダファイル			*/

/**************************************************************************/
/**
 *  @brief  buffer hunt processing
 *  @note   select the correct buffer kind according to buffer category  \n
 *          and buffer size \n
 *          if hunt buffer sucessfully, clear 0 for hunted buffer \n
 *          and set the buffer head address to output parameter  \n
 *  @param  buf_Category		[in]	buffer category
 *  @param  bufsize				[in]	buffer size
 *  @param  bufAdr_p			[out]	buffer pointer address
 *  @param  FileNamep			[in]	calling function file name
 *  @param  LineNo				[in]	calling function executing line
 *  @return RETURN CODE
 *  @retval CMD_RES_OK						0: Normal end
 *  @retval CMD_RES_CATEGORY		0xF000000F: buffer category specify error
 *  @retval CMD_RES_SIZEERR			0xF0000002: size specify error	
 *  @retval CMD_RES_EMPTY			0xF0000003: resource empty
 *  @warning   N/A
 *  @FeatureID N/A
 *  @Bug_No    N/A
 *  @CR_No     N/A
 *  @TBD_No    N/A
 *  @date   2006/01/13	FFCS)Xiax 新規作成
 *  @date   2006/04/12		FJT)S.Takayama	 WCDMA119-1273,1274 \n
 *                   	バッファ種別見直しに伴う修正 \n
 *  @date   2013/11/26 alpha)Kondou	zynq化対応
 */
/**************************************************************************/

UINT cmR_BReq(UINT buf_Category, UINT bufsize, VOID ** bufAdr_p, VOID * FileNamep, UINT LineNo)
{
	INT		ret;
	INT		errcd;

	ret = BPF_RU_IPCM_PROCMSG_ADDRGET( buf_Category, bufsize, bufAdr_p, &errcd );
	
	if(ret != 0){
		BPF_COM_LOG_ASSERT ( D_RRH_LOG_AST_LV_ERROR, "BPF_RU_IPCM_PROCMSG_ADDRGET ret=%d, error code=%d", ret,  errcd);
	}

	return((UINT)ret);

}
/* @} */
