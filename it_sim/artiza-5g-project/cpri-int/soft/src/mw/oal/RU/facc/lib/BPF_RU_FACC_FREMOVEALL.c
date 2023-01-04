/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FREMOVEALL.c
 *	@brief	File Access Control File Remove All.
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
 *  @brief  File Access Control File Remove All.
 *  @note   API function. 
 *  @param  dir_path_name   [in]  directory path name
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FREMOVEALL file remove all error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FREMOVEALL(const bpf_ru_facc_dpathname * dir_path_name, int *errcd)
{
    DIR* dirfd;                            /* ディレクトリディスクリプタ */
    struct dirent *entry;                   /* entry                      */
    int ret;                               /* リターンコード             */
    int loop;                              /* ループカウンター           */
    char pathname[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
                                           /* フルパスファイル名         */
    char droppath[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN] = ""; /* 必要な場合はベースdirを設定しておく */
                                           /* ディレクトリオープン用パス */
    size_t dir_len;                        /* ディレクトリLength         */
    size_t path_len;                       /* PATHLength                 */

    /* 入力パラメータチェック */
    if (dir_path_name == NULL){
        /* ポインターにNULLが設定された */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* 初期化を行う */
    dir_len = 0;
    path_len = 0;
    memset(pathname,0,sizeof(pathname));

    /************************************************
    * directory open processing.                    *
    ************************************************/
    /* ディレクトリオープンの為にフルパスを生成する */
    /* 入力ディレクトリ長を取り出す */
    dir_len = strnlen(dir_path_name->dpathname, BPF_RU_FACC_DPATHNAME_LEN);

    /* Max Length長チェック等は行わず、指定のDirのオープンの成否で判断する */
    /* ディレクトリパスを連結する */
    path_len = dir_len;
    if(dir_path_name->dpathname[0] != '/'){
        droppath[0]='/'; /* fail safe */
        path_len ++;
    } 
    strncat(droppath,dir_path_name->dpathname,dir_len);

    /* 末尾に"/"を追加する */
    droppath[path_len] = '/';

	/* ディレクトリをオープンする:opendir */
    dirfd = opendir(droppath);

    /* オープン結果を判定する */
    if (dirfd == NULL){
        /* ディレクトリオープン失敗 */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FREMOVEALL;
    }

    /************************************************
    * all file delete processing.                   *
    ************************************************/
    /* ディレクトリ内のエントリーを読みだし、削除する */
    for(loop=1;;loop++){
        /* ディレクトリ内エントリー読みだし */
        entry = readdir(dirfd);
        if(entry == NULL){
            /* ファイルは一つも無いが、正常終了 */
            ret = BPF_RU_FACC_COMPLETE;
            break;
        }
        /* ディレクトリ内エントリーがファイルかリンクか判定する */
        if((entry->d_type == DT_REG)||(entry->d_type==DT_LNK)){
            /* ファイル名先頭は"."か？ */
            if(entry->d_name[0]==0x2e){
                /* 次！ */
                continue;
            }else{
                /* 削除ファイルのフルパス生成 */
                ret = bpf_ru_facc_makepathname(dir_path_name,               /* ディレクトリパス名 (I) */
                                            (bpf_ru_facc_fname *)(entry->d_name), /* ファイル名         (I) */
                                            pathname);                   /* pathname           (O) */
                /* 生成結果を判定する */
                if (ret != BPF_RU_FACC_OK){
                    /* フルパス生成に失敗したが処理は継続する */
                    /* 失敗することは殆ど有り得ない */
                    continue;
                }
                /* ファイル削除起動 */
                ret = unlink(pathname);

                /* 削除結果を確認する */
                if (ret != BPF_RU_FACC_OK){
                    /* 削除失敗 */
                    /* errcdを設定し、処理を中断する */
                    *errcd = errno;
                    ret = BPF_RU_FACC_ERR_FREMOVEALL;
                    break;
                }
            }
        }
    }

    /************************************************
    * directory close processing.                   *
    ************************************************/
    /* 全削除が出来たので、ディレクトリをクローズする */
    closedir(dirfd);

    /* 終了 */
    /* ここにくるまでになんらかのエラーまた正常終了が設定されている */
    return ret;
}
/* @} */

/* @} */
