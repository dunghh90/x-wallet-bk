/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FWRITE.c
 *	@brief	File Access Control File Write.
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
 *  @brief  File Access Control File Write.
 *  @note   API function. 
 *  @param  file_descriptor [in]  directory path name
 *  @param  write_size      [in]  write size
 *  @param  write_data      [in]  write data
 *  @param  act_write_size  [out] real write size
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_WSIZE      write size error
 *  @retval BPF_RU_FACC_ERR_WDATA      write data space error
 *  @retval BPF_RU_FACC_ERR_FCLOSE     file write error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FWRITE(int file_descriptor, unsigned int write_size, const char *write_data, 
                       unsigned int *act_write_size, int *errcd)
{
    ssize_t count_result;                      /* 書き込みバッファサイズ */

    /* 初期化処理 */
    count_result = 0;

    /* 入力パラメータチェック */
    /* 書き込みサイズチェック */
    if (write_size < BPF_RU_FACC_WDATA_MIN ||
        write_size > BPF_RU_FACC_WDATA_MAX){
        /* 書き込みサイズ異常 */
        return BPF_RU_FACC_ERR_WSIZE;
    }

    /* 書き込みデータ格納領域チェック */
    if (write_data == NULL){
        /* 書き込みデータ無し */
        return BPF_RU_FACC_ERR_WDATA;
    }

    /************************************************
    * file write processing.                        *
    ************************************************/
    /* システムコール file writeを実行 */
    count_result = write(file_descriptor,        /* ファイルディスクリプタ (I) */
                         write_data     ,        /* 書き込みデータ         (I) */ 
                         write_size);            /* 書き込みサイズ         (I) */

    /* file writeの結果を判定する */
    if (count_result == BPF_RU_FACC_SYS_NG || count_result == BPF_RU_FACC_SIZE_ZERO){
        /* file write失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FWRITE;
    }

    /* CFへ同期する */
    fsync(file_descriptor);

    /* file write成功 */
    /* 実書き込みサイズを設定する */
    *act_write_size = count_result;
    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
