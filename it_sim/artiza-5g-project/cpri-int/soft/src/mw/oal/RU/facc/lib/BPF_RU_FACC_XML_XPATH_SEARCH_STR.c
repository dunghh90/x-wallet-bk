/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_XPATH_SEARCH_STR.c
 *	@brief  xmlXPathEvalExpression.
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
 *  @brief  xmlXPathEvalExpression.
 *  @note   API function. 
 *  @param  ctx                 [in]  xml context info
 *  @param  str                 [in]  xml object path
 *  @param  obj                 [out] xml object info
 *  @param  errcd               [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 *  @retval BPF_RU_FACC_ERR_XML        xml call failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_XPATH_SEARCH_STR (bpf_xmlXPathContextPtr ctx,			/* コンテキスト */
							const char * str, 							/* XPath文字列 */
							bpf_xmlXPathObjectPtr *obj, 					/* オブジェクト */
							int *errcd)									/* エラーコード */
{
	xmlXPathObjectPtr		objptr = NULL;
	
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_SEARCH_STR START!! ctx=%p str=%s", ctx, str);
	
	if(ctx == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. ctx is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
		
	if(str == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. str is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(obj == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. obj is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(errcd == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. errcd is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* XPath検索を実施する  */
	objptr = xmlXPathEvalExpression((const xmlChar *)str, ctx);
    if(objptr == NULL)
	{
		/* エラーログ */
		*errcd = errno;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"xml ng.ctx=%p str=%s err=%d", ctx, str, *errcd);
		return BPF_RU_FACC_ERR_XML;
	}
	
	if(objptr->nodesetval == NULL || objptr->nodesetval->nodeNr == 0)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. str:%s not found.", str);
		
		/* 異常のXPathオブジェクト削除 */ 
		xmlXPathFreeObject(objptr);
		
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* オブジェクトを設定する  */
	*obj = objptr;
	
    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_SEARCH_STR END!! *obj=%p", *obj);
    
    return BPF_RU_FACC_COMPLETE;
}


/* @} */


/* @} */
