/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_XPATH_FREE_CTX.c
 *	@brief  xmlXPathFreeContext.
 *  @date   $Date:$ $Author:$
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2018
 */
/********************************************************************************************************************/
/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_facc_xml.h"

/** @addtogroup BPF_RU_FACC
 * @{ */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  xmlXPathFreeContext.
 *  @note   API function. 
 *  @param  ctx                   [in]  xml context info
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_XPATH_FREE_CTX (bpf_xmlXPathContextPtr ctx)
{
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_FREE_CTX START!! ctx=%p", ctx);
		
	if(ctx == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. ctx is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* XPathコンテキスト削除 */ 
	xmlXPathFreeContext(ctx);

    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_FREE_CTX END!!");
    return BPF_RU_FACC_COMPLETE;
}
/* @} */


/* @} */
