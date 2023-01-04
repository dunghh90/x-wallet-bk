/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_DREMOVE.c
 *	@brief	File Access Control deleate Directory.
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
 *  @brief  File Access Control Delete Directory.
 *  @note   API function. 
 *  @param  dir_path_name   [in]  directory path name
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  parameter error
 *  @retval BPF_RU_FACC_ERR_DREMOVE    failed
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_DREMOVE(const bpf_ru_facc_dpathname *dir_path_name, int *errcd)
{
    int ret;                                   /* リターンコード         */
    char dir_pthname[BPF_RU_FACC_DPATHNAME_LEN];
                                               /* dirフルパスファイル名  */

    /* 初期化をする */
    memset(dir_pthname,0,sizeof(dir_pthname));
    ret = 0;

    /************************************************
    * parameter check processing.                   *
    ************************************************/
    /* ディレクトリパス名 */
    if (dir_path_name == NULL){
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    /************************************************
    * directory delete processing.                  *
    ************************************************/
    /* dirフルパス生成を行う */
    ret = bpf_ru_facc_makepathname_dir(dir_path_name,     /* ディレクトリパス名 (I) */
                                       dir_pthname);      /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /* システムコール rmdirを実行 */
    ret = rmdir((const char*)dir_pthname);

    /* rmdirの結果を判定する */
    if (ret == BPF_RU_FACC_SYS_NG){
        /* rmdir失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_DREMOVE;
    }

    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
