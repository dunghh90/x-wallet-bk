/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_XPATH_FREE_OBJ.c
 *	@brief  xmlXPathFreeObject.
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
 *  @brief  xmlXPathFreeObject.
 *  @note   API function. 
 *  @param  obj                   [in]  xml object info
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_XPATH_FREE_OBJ (bpf_xmlXPathObjectPtr obj)
{
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_FREE_OBJ START!! obj=%p", obj);
		
	if(obj == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. obj is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* XPathオブジェクト削除 */ 
	xmlXPathFreeObject(obj);

    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_XPATH_FREE_OBJ END!!");
    return BPF_RU_FACC_COMPLETE;
}

/* @} */


/* @} */
