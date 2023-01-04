/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_XML_OPEN_FILE.c
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
 *  @brief  xmlParseFile.
 *  @note   API function. 
 *  @param  filename            [in]  xml filename
 *  @param  doc                 [out] xml document info
 *  @param  errcd               [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_PARAM      parameter check failed
 *  @retval BPF_RU_FACC_ERR_XML        xml call failed
 */
/********************************************************************************************************************/
int BPF_RU_FACC_XML_OPEN_FILE (const char *filename,        /* XMLファイル */
								bpf_xmlDocPtr *doc,             /* ドキュメント */
								int *errcd)					/* エラーコード */
{
	xmlDocPtr tmp = NULL;
	
	/* トレースログ */
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_OPEN_FILE START!! filename=%s", filename);
		
	if(filename == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. filename is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(doc == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. doc is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	if(errcd == NULL)
	{
		/* エラーログ */
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"param ng. errcd is null.");
		return BPF_RU_FACC_ERR_PARAM;
	}
	
	/* スペースでのインデントフォーマットを指定する  */
	xmlKeepBlanksDefault(0);
	
	/* XMLファイルをOpenする  */
	tmp = xmlParseFile(filename);
    if(tmp == NULL)
	{
		/* エラーログ */
		*errcd = errno;
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L2,"xmlParseFile ng.filename=%s err=%d", filename, errno);
		return BPF_RU_FACC_ERR_XML;
	}
	
	/* ドキュメントを設定する  */
	*doc = tmp;
	
    /* トレースログ */
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_RU_FACC_XML_OPEN_FILE END!! *doc=%p", *doc);
    return BPF_RU_FACC_COMPLETE;
}


/* @} */


/* @} */
