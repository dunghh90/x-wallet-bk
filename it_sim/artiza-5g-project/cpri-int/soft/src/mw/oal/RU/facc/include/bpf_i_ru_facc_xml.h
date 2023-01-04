/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_i_ru_facc.h
 *  @brief  Definitions for BPF FACC Local header
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013-
 */
/********************************************************************************************************************/

#ifndef _BPF_I_RU_FACC_H_
#define _BPF_I_RU_FACC_H_

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "BPF_RU_FACC.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <libxml/xmlversion.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include "bpf_i_ru_facc.h"

/********************************************************************************************************************/
/* Internal Definition                                                                                              */
/********************************************************************************************************************/

/********************************************************************************************************************/
/* Internal Structure                                                                                              */
/********************************************************************************************************************/

typedef	xmlXPathContextPtr		bpf_xmlXPathContextPtr ;
typedef	xmlXPathObjectPtr		bpf_xmlXPathObjectPtr ;
typedef	xmlDocPtr				bpf_xmlDocPtr ;


/********************************************************************************************************************/
/* Internal enum                                                                                              */
/********************************************************************************************************************/


/********************************************************************************************************************/
/* Internal Function Routine Prototype                                                                                              */
/********************************************************************************************************************/
/* XMLファイルオープン */
int BPF_RU_FACC_XML_OPEN_FILE (const char *filename,				/* XMLファイル */
								bpf_xmlDocPtr *doc,					/* ドキュメント */
								int *errcd);						/* エラーコード */

/* xmlデータフリー */
int BPF_RU_FACC_XML_FREE_DATA (char *data) ;

/* xmlドキュメントデータフリー */
int BPF_RU_FACC_XML_FREE_DOC (bpf_xmlDocPtr doc) ;						/* ドキュメント */

/* xml属性データ取得 */
int BPF_RU_FACC_XML_GET_ATTRIBUTE (bpf_xmlXPathObjectPtr obj, 		/* オブジェクト */
									unsigned int index, 			/* インデックス */
									const char *att_name, 			/* アトリビュート名 */
									char **att_value, 				/* アトリビュート情報 */
									int *errcd);					/* エラーコード */
/* xmlコンテキスト生成 */
int BPF_RU_FACC_XML_XPATH_CREATE_CTX (bpf_xmlDocPtr doc,			/* ドキュメント */
							bpf_xmlXPathContextPtr *ctx,        	/* コンテキスト */
							int *errcd) ;                   		/* エラーコード */

/* xmlコンテキスト解放 */
int BPF_RU_FACC_XML_XPATH_FREE_CTX (bpf_xmlXPathContextPtr ctx) ;	/* コンテキスト */

/* xml オブジェクト解放 */
int BPF_RU_FACC_XML_XPATH_FREE_OBJ (bpf_xmlXPathObjectPtr obj) ;	/* オブジェクト */

/* xml xPath検索 */
int BPF_RU_FACC_XML_XPATH_SEARCH_STR (bpf_xmlXPathContextPtr ctx,	/* コンテキスト */
							const char * str, 						/* XPath文字列 */
							bpf_xmlXPathObjectPtr *obj, 			/* オブジェクト */
							int *errcd) ;							/* エラーコード */


/********************************************* EOF ******************************************************************/
#endif /* _BPF_I_RU_FACC_H_ */

