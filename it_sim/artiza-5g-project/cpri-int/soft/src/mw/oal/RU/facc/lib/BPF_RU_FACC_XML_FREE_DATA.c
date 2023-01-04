/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_FREE_DATA.c
 *	@brief  xmlFree.
 *  @date   $Date:$ $Author:$
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2018
 */
/********************************************************************************************************************/

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "BPF_RU_FACC.h"
#include "bpf_l_com.h"
#include "bpf_i_ru_facc_xml.h"

/** @addtogroup BPF_RU_FACC
 * @{ */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  xmlFree.
 *  @note   API function. 
 *  @param  obj                   [in]  xml object info
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_FREE_DATA (char *data)
{
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_FREE_DATA START!! data=%p", data);
	
	if(data == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. data is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* XMLバッファ解放 */
	xmlFree(data);
	
    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_FREE_DATA END!!");
    return BPF_RU_FACC_COMPLETE;
}
/* @} */


/* @} */
