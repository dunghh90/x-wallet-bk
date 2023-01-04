/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_GET_ATTRIBUTE.c
 *	@brief  xmlParseFile.
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
 *  @brief  xmlGetProp.
 *  @note   API function. 
 *  @param  obj                   [in]  xml object info
 *  @param  index                 [in]  index of node
 *  @param  att_name              [in]  attribute name of node
 *  @param  att_value             [out] attribute value of node
 *  @param  errcd                 [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 *  @retval BPF_RU_FACC_ERR_XML        xml call failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_GET_ATTRIBUTE (bpf_xmlXPathObjectPtr obj, 				/* オブジェクト */
									unsigned int index, 					/* インデックス */
									const char *att_name, 					/* アトリビュート名 */
									char **att_value, 						/* アトリビュート情報 */
									int *errcd)								/* エラーコード */
{
	xmlChar *pchar = NULL;
	int nodeNum = 0;
	xmlNode *nodeptr = NULL;
	
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,
				"BPF_RU_FACC_XML_GET_ATTRIBUTE START!obj=%p index=%d att_name=%s", 
				obj, index, att_name);
		
	if(obj == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. obj is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(obj->nodesetval == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. obj->nodesetval is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	nodeNum = obj->nodesetval->nodeNr;
	if(index >= nodeNum)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. node invalid. nodeNum=%d index=%d", nodeNum, index);
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(att_name == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. att_name is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(att_value == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. att_value is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(errcd == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. errcd is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* Nodeを取得 */ 
	nodeptr = obj->nodesetval->nodeTab[index];
	
	/* XML アトリビュート取得 */ 
	pchar = xmlGetProp(nodeptr, (const xmlChar *)att_name);
	if(pchar == NULL)
	{
		/* エラーログ */
		*errcd = errno;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,
						"xmlGetProp ng.obj=%p index=%d att_name=%s err=%d", 
						obj, index, att_name, *errcd);
		return BPF_RU_FACC_ERR_XML;
	}
	
	/* attributeのバッファーを設定 */ 
	*att_value = (char *)pchar;
	
    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_GET_ATTRIBUTE END!! *att_value=%p", *att_value);
    return BPF_RU_FACC_COMPLETE;
}

/* @} */


/* @} */
