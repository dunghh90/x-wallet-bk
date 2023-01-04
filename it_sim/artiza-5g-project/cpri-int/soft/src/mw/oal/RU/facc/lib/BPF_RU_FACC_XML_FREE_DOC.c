/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_FREE_DOC.c
 *	@brief  xmlFreeDoc.
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
 *  @brief  xmlFreeDoc.
 *  @note   API function. 
 *  @param  doc                   [in]  xml document info
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_FREE_DOC (bpf_xmlDocPtr doc)
{
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_FREE_DOC START!! doc=%p", doc);
			
	if(doc == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"para ng. doc is null.");
		
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* XMLファイル解放 */ 
	xmlFreeDoc(doc);

    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_FREE_DOC END!!");
    return BPF_RU_FACC_COMPLETE;
}
/* @} */


/* @} */
