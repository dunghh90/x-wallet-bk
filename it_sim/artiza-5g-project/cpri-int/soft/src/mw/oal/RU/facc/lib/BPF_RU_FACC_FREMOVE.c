/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FREMOVE.c
 *	@brief	File Access Control File Remove.
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
 *  @brief  File Access Control File Remove.
 *  @note   API function. 
 *  @param  dir_path_name   [in]  directory path name
 *  @param  file_name       [in]  file name
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FNAME      file name error
 *  @retval BPF_RU_FACC_ERR_FOPEN      file remove error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FREMOVE(const bpf_ru_facc_dpathname *dir_path_name, const bpf_ru_facc_fname *file_name, int *errcd)
{
    char pathname[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
                                               /* フルパスファイル名     */
    int ret;                                   /* リターンコード         */

    /* 初期化を行う */
    ret = 0;
    memset(pathname,0,sizeof(pathname));

    /************************************************
    * file remove processing.                       *
    ************************************************/
    /* フルパス生成を行う */
    ret = bpf_ru_facc_makepathname(dir_path_name, /* ディレクトリパス名 (I) */
                                   file_name,     /* ファイル名         (I) */
                                   pathname);     /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /* システムコール unlinkを実行する */
    ret = unlink(pathname);

    /* unlink結果を判定する */
    if (ret == BPF_RU_FACC_SYS_NG){
        /* unlink 失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FREMOVE;
    }

    /* unlink成功 */
    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
