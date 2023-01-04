/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_DMOVE.c
 *	@brief	File Access Control Rename/Move Directory.
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
 *  @brief  File Access Control Rename/Move Directory.
 *  @note   API function. 
 *  @param  src_dir_path_name   [in]  source directory path name
 *  @param  dst_dir_path_name   [in]  destination directory path name
 *  @param  errcd               [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  parameter error
 *  @retval BPF_RU_FACC_ERR_DMOVE      failed
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_DMOVE (const bpf_ru_facc_dpathname *src_dir_path_name,
                       const bpf_ru_facc_dpathname *dst_dir_path_name, int *errcd)
{
    int ret;                                   /* リターンコード         */
    char src_pthname[BPF_RU_FACC_DPATHNAME_LEN];
                                               /* srcフルパスファイル名  */
    char dst_pthname[BPF_RU_FACC_DPATHNAME_LEN];
                                               /* dstフルパスファイル名  */
    char cmd_str[BPF_RU_CMD_LENGTH_MAX];       /* コマンド格納エリア     */
    int src_cmd_len;                           /* srcコマンドレングス    */
    int dst_cmd_len;                           /* dstコマンドレングス    */
    int status;

    /* 初期化をする */
    memset(src_pthname,0,sizeof(src_pthname));
    memset(dst_pthname,0,sizeof(dst_pthname));
    memset(cmd_str,0,sizeof(cmd_str));
    ret = 0;
    src_cmd_len = 0;
    dst_cmd_len = 0;
    status = 0;
	
    /************************************************
    * directory Rename/Move processing.             *
    ************************************************/
    /* srcフルパス生成を行う */
    ret = bpf_ru_facc_makepathname_dir(src_dir_path_name, /* ディレクトリパス名 (I) */
                                       src_pthname);      /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /* dstフルパス生成を行う */
    ret = bpf_ru_facc_makepathname_dir(dst_dir_path_name, /* ディレクトリパス名 (I) */
                                       dst_pthname);      /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /* srcコマンド用ファイルレングスを取得する */
    src_cmd_len = strnlen(src_pthname,BPF_RU_FACC_DPATHNAME_LEN);

    /* dstコマンド用ファイルレングスを取得する */
    dst_cmd_len = strnlen(dst_pthname,BPF_RU_FACC_DPATHNAME_LEN);

    /* mvコマンドイメージ作成 */
    /*=====================================================*/
    /* systemに渡すコマンドイメージは下記                  */
    /* LNAG=C;nice -n 20 mv  src_dir dst_dir               */
    /*=====================================================*/

    sprintf(cmd_str, "LANG=C;nice -n %d mv ",BPF_RU_CMD_NICE_ADJ);
    strncat(cmd_str, src_pthname, src_cmd_len);
    strcat(cmd_str, " ");
    strncat(cmd_str, dst_pthname, dst_cmd_len);
    strcat(cmd_str, " 2>/dev/null");

    /* シェルコマンド実行:system */
    ret = system(cmd_str);

    /* シェルコマンド実行結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* シェル実行失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_DMOVE;
    }
	
    if( WEXITSTATUS( status ) != 0 ){
        *errcd = errno;
        return BPF_RU_FACC_ERR_DMOVE;
    }

    /* コマンド正常終了 */

    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
