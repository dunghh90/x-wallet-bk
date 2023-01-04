/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_RU_FACC_FLIST.c
 *  @brief  File Access Control File List.
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_FACC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_ru_facc.h"

/** @addtogroup BPF_RU_FACC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control File List.
 *  @note   API function. 
 *  @param  dir_path_name     [in]  source directory path name
 *  @param  max_file_list_num [in]  max list number of files
 *  @param  file_list         [out] file list
 *  @param  act_file_list_num [out] real list number of files
 *  @param  errcd             [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_LISTNUM   max file list number error
 *  @retval BPF_RU_FACC_ERR_LIST      file list error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FLIST(const bpf_ru_facc_dpathname *dir_path_name, unsigned int max_file_list_num,
                      bpf_ru_facc_filelist *file_list, unsigned int *act_file_list_num, int *errcd)
{
    char droppath[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN] = ""; /* 必要な場合はベースdirを設定しておく */
                                           /* ディレクトリオープン用パス */
    char tmppath[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
                                           /* ファイルサイズ取得用       */
    DIR* dirfd;                            /* Dir file descriptor        */
    struct dirent *entry;                   /* entry                      */
    struct stat buf;                       /* ファイルサイズ確認用       */
    size_t file_len;                       /* ファイルLength             */
    size_t dir_len;                        /* Dir Length                 */
    size_t path_len;                       /* PATH Length                */
    int ret;                               /* リターンコード             */
    int cnt;                               /* 読み出しカウント           */
    int entry_loop;                        /* entry用loopカウンタ        */
    int count_loop;                        /* 読み出し用loopカウンタ     */


    /* 初期化を行う */
    memset(tmppath,0,sizeof(tmppath));
    memset(&buf,0,sizeof(buf));
    dirfd = 0;
    file_len = 0;
    dir_len = 0;
    path_len = 0;
    ret = 0;
    cnt = 1;
    entry_loop = 0;
    count_loop = 0;     
	memset(file_list,0,sizeof(bpf_ru_facc_filelist));

    /* 入力パラメータチェックを行う */
    /*==================================================*/
    /* フルパス生成時と重複チェックとなるが、仕方が無い */
    /*==================================================*/
    if (dir_path_name == NULL){
        /* ディレクトリパス名異常 */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    if (max_file_list_num == 0){
        /* 読み出し可能ファイル数指定異常 */
        return BPF_RU_FACC_ERR_LISTNUM;
    }

    /************************************************
    * file list processing.                         *
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
        return BPF_RU_FACC_ERR_LIST;
    }

    /* ディレクトリ内のファイルを読み出す */
    /* 1st loop */
    for(entry_loop=1;;entry_loop++){
        /* ディレクトリ内のエントリーを読み出す */
        entry = readdir(dirfd);

        /* 読み出し結果を判定する */
        if (entry == NULL){
            /* エントリーが無くなった */
            /* 読み出しファイル数を設定する */
            /*==============================================================*/
            /* ファイル読み出し数cntは初期値が1のため、実際の読み出し数設定 */
            /* 時はcnt-1を設定しないと1ファイル多いファイル数を設定しまう   */
            /*==============================================================*/
            file_list->file_num = cnt-1;
            *act_file_list_num = cnt-1;
            break;
        }
        /* ディレクトリ内エントリーがファイルかリンクか判定する */
        if((entry->d_type == DT_REG)||(entry->d_type==DT_LNK)){
            /* ファイル名先頭は"."か？ */
            if(entry->d_name[0]==0x2e){
                /* 次！ */
                continue;
            }else{
                /* 読み出し可能数は上限か？ */
                if ( cnt > max_file_list_num){
                    /* ここからはファイル数だけを計上する */
                    /* 2nd loop */
                    /* 読み出し設定済みファイル数を設定する */
                    /*=============================================================*/
                    /* このルートに入る条件は読み出し数がmax_file_list_numを超えた */
                    /* 時であり、初期値がcnt=1のため、実読み出し数はcnt-1で設定    */
                    /*=============================================================*/
                    file_list->file_num = cnt-1;
                    for(count_loop=1;;count_loop++){
                        /* ディレクトリ内エントリを読み出す */
                        entry = readdir(dirfd);

                        /* 読み出し結果を判定する */
                        if (entry == NULL){
                            /* エントリが無くなった */
                            *act_file_list_num = cnt;
                            /* break from 2nd loop */
                            break;
                        }else{
                            /* ディレクトリ内エントリがファイルかリンクか判定する */
                            if((entry->d_type == DT_REG) || (entry->d_type == DT_LNK)){
                                /* ファイル名先頭は"."か？ */
                                if (entry->d_name[0]==0x2e){
                                    /* 次！ */
                                    continue;
                                }else{
                                    /* エントリ数を加算する */
                                    cnt++;
                                }
                            }
                        }
                    }
                    /* 全てのファイル設定が完了 */
                    /* break from 1st loop */
                    break;
                }
            }
            /* ファイルサイズ読み出しようにフルパスを生成する */
            ret = bpf_ru_facc_makepathname(dir_path_name,          /* ディレクトリパス名 (I) */
                                        (bpf_ru_facc_fname*)entry->d_name, /* ファイル名         (I) */
                                        tmppath);                  /* pathname           (O) */
                                    
            /* 念の為に生成結果を判定する */
            if (ret != BPF_RU_FACC_OK){
                /* 生成失敗 */
                /* このファイルは読み飛ばして次のファイルの処理をする */
                continue;
            }else{
                /* 生成成功 */
                /* ファイル情報を取得する */
                ret = stat(tmppath,&buf);

                /* 取得結果を判定する */
                if (ret == BPF_RU_FACC_SYS_NG){
                    /* 初期化だけして、次のファイルの処理をする */
                    memset(tmppath,0,sizeof(tmppath));
                    memset(&buf,0,sizeof(buf));
                    continue;
                }

                /* 取得成功 */
                /* 読み出したファイル名を設定する */
                /* ファイルレングスを算出する */
                file_len = strnlen(entry->d_name, BPF_RU_FACC_FNAME_LEN+1);

                /* ファイル名を設定する */
                memcpy(&file_list->file_info[cnt-1],entry->d_name,file_len);

                /* ファイルサイズを設定する */
                file_list->file_info[cnt-1].file_size = buf.st_size;

                /* 次処理用に初期化する */
                memset(tmppath,0,sizeof(tmppath));
                memset(&buf,0,sizeof(buf));

                cnt++;
            }
        }
    }

    /* ディレクトリをクローズする */
    closedir(dirfd);

    return BPF_RU_FACC_COMPLETE; 
}
/* @} */

/* @} */
