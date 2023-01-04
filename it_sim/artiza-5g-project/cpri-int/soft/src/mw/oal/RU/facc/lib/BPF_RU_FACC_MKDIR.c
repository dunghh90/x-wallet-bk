/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_MKDIR.c
 *	@brief	File Access Control Create Directory.
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
 *  @brief  File Access Control Create Directory.
 *  @note   API function. 
 *  @param  dir_path_name   [in]  directory path name
 *  @param  dir_mode        [in]  create mode
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  parameter error
 *  @retval BPF_RU_FACC_ERR_DMODE      parameter error
 *  @retval BPF_RU_FACC_ERR_MKDIR      failed
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_MKDIR (const bpf_ru_facc_dpathname *dir_path_name, int dir_mode, int *errcd)
{
    int ret;                                   /* リターンコード         */
    char dir_pthname[BPF_RU_FACC_DPATHNAME_LEN];
                                               /* dirフルパスファイル名  */
	mode_t c_mode;

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
    /* ディレクトリ許可属性 */
    if (dir_mode < 00001 ||
        dir_mode > 00777){
        return BPF_RU_FACC_ERR_DMODE;
    }

    /************************************************
    * directory create processing.                  *
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

	c_mode = umask(0);
    /* システムコール mkdirを実行 */
    ret = mkdir((const char*)dir_pthname, dir_mode);
	umask(c_mode);

    /* mkdirの結果を判定する */
    if (ret == BPF_RU_FACC_SYS_NG){
        /* mkdir失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_MKDIR;
    }

    return BPF_RU_FACC_COMPLETE;
}


/* @} */

/* @} */
