/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_XPATH_CREATE_CTX.c
 *	@brief  xmlXPathNewContext.
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
 *  @brief  xmlXPathNewContext.
 *  @note   API function. 
 *  @param  doc                 [in]  xml document info
 *  @param  ctx                 [out] xml context info
 *  @param  errcd               [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 *  @retval BPF_RU_FACC_ERR_XML        xml call failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_XPATH_CREATE_CTX (bpf_xmlDocPtr doc,    			/* ドキュメント */
									bpf_xmlXPathContextPtr *ctx,		/* コンテキスト */
									int *errcd)							/* エラーコード */
{
	xmlXPathContextPtr tmp = NULL;
	
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_CREATE_CTX START!! doc=%p", doc);
		
	if(doc == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. doc is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(ctx == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. ctx is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(errcd == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. errcd is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* XPathコンテキスト生成  */
	tmp = xmlXPathNewContext(doc);
    if(tmp == NULL)
	{
		/* エラーログ */
		*errcd = errno;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"xmlXPathNewContext ng.doc=%p err=%d", doc, *errcd);
		return BPF_RU_FACC_ERR_XML;
	}
	
	/* コンテキストを設定する  */
	*ctx = tmp;
	
    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_CREATE_CTX END!! *ctx=%p", *ctx);
    return BPF_RU_FACC_COMPLETE;
}


/* @} */


/* @} */
