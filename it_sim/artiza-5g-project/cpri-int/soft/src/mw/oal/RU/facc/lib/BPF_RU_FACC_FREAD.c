/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FREAD.c
 *	@brief	File Access Control File Read.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_FACC /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_i_ru_facc.h"

/** @addtogroup BPF_RU_FACC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control File Read.
 *  @note   API function. 
 *  @param  file_descriptor [in]  directory path name
 *  @param  read_size       [in]  read size
 *  @param  read_data       [out] read data
 *  @param  act_read_size   [out] real read size
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_RSIZE      read size error
 *  @retval BPF_RU_FACC_ERR_FREAD      file read error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FREAD(int file_descriptor, unsigned int read_size, char *read_data, 
                      unsigned int *act_read_size, int *errcd)
{
    ssize_t count_result;                                /* 返却バイト数 */

    /* 初期化処理 */
    count_result = 0;

    /* 読み出しサイズをチェックする */
    if (read_size < BPF_RU_FACC_RDATA_MIN ||
        read_size > BPF_RU_FACC_RDATA_MAX){
        /* 読み出しサイズオーバー */
        return BPF_RU_FACC_ERR_RSIZE;
    }

    /************************************************
    * file read processing.                         *
    ************************************************/
    /* システムコール file readを実行する。*/
    count_result = read(file_descriptor,   /* ファイルディスクリプタ (I) */ 
                        read_data,         /* 読み出しデータ格納先 (I/O) */ 
                        read_size);        /* 読み出しサイズ         (I) */

    /* file readの結果を判定する */
    if (count_result == BPF_RU_FACC_SYS_NG){
        /* file read失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FREAD;
    }else{
        /* file read成功 */
        /* 実読みだしサイズを設定する */
        *act_read_size = count_result;
        return BPF_RU_FACC_COMPLETE;
    }
}
/* @} */

/* @} */
