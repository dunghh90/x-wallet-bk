/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FSEEK.c
 *	@brief	File Access Control File Seek.
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
 *  @brief  File Access Control File Seek.
 *  @note   API function. 
 *  @param  file_descriptor [in]  directory path name
 *  @param  offset          [in]  offset
 *  @param  seek_mode       [in]  seek mode
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_FSEEK      file seek error
 *  @retval BPF_RU_FACC_ERR_SMODE      seek mode error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FSEEK(int file_descriptor, bpf_ru_facc_off_t offset, bpf_ru_facc_seekmode seek_mode, int *errcd)
{
    off_t off_result;                                 /* offset バイト数 */

    /* 入力パラメータチェック */
    switch(seek_mode){
        case FACC_SEEK_SET:
        case FACC_SEEK_CUR:
        case FACC_SEEK_END:
            /* 指定seek modeは有効 */
            break;
        default:
            /* 指定seek mode異常 */
            return BPF_RU_FACC_ERR_SMODE;
    }

    /* 初期化処理 */
    off_result = 0;	

    /************************************************
    * file seek processing.                         *
    ************************************************/
    /* システムコール file lseekを実行する */
    off_result = lseek(file_descriptor,  /* ファイルディスクリプタ (I) */ 
                       offset         ,  /* オフセット             (I) */
                       seek_mode);       /* シークモード           (I) */

    /* file seek結果を判定する */
    if (off_result == BPF_RU_FACC_SYS_NG){
        /* file seek失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FSEEK;
    }

    /* file seek成功 */

    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
