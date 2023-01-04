/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	bpf_ru_facc_com.c
 *	@brief	File Access Control Common Library.
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
 *  @brief  File Access Control Full Path make.
 *  @note   FACC internal function. 
 *  @param  *dir_path_name   [in]  directory path name
 *  @param  *file_name       [in]  file name
 *  @param  *pathname        [out] full path file name
 *  @return result code
 *  @retval BPF_RU_FACC_OK             normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FNAME      file name error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_facc_makepathname(
	const bpf_ru_facc_dpathname *dir_path_name,  /* ディレクトリパス名    (I) */
	const bpf_ru_facc_fname *file_name        ,  /* ファイル名            (I) */
	char  *pathname)                             /* フルパス名            (O) */
{
    size_t dir_len;                              /* ディレクトリLength        */
    size_t file_len;                             /* ファイルLength            */
    char tmppath[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN] = ""; /* 必要な場合はベースdirを設定しておく */
    size_t path_len;                             /* 連結DirLength             */
                                                 /* 連結前Dir格納エリア       */
    int ret = BPF_RU_FACC_OK;                    /* return code設定用         */
    char *work_p;                                /* ワイルドカード指定判定用  */

    /************************************************
    * full path file name make processing.          *
    ************************************************/
    /* ディレクトリパス名チェックを行う */
    if (dir_path_name == NULL){
        /* ディレクトリパス名異常 */
        ret = BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* ファイル名チェックを行う */
    if (file_name == NULL){
        /* ファイル名異常 */
        ret = BPF_RU_FACC_ERR_FNAME;
    }

    /* 処理続行可能かどうかを判定する */
    if (ret != BPF_RU_FACC_OK){
        /* 入力情報に誤りがある */	
        return ret;
    }

    /*====================================================================*/
    /* strnlenは終端NULLを含まないバイト数を返却する。従ってMAXサイズが   */
    /* 返却された場合は終端にNULLが設定されていないことになる             */
    /*====================================================================*/ 

    /* 入力ディレクトリ長を取り出す */
    dir_len = strnlen(dir_path_name->dpathname, BPF_RU_FACC_DPATHNAME_LEN);

    /* ディレクトリ長がMaxか判定する */
    if (dir_len == BPF_RU_FACC_DPATHNAME_LEN){
        /* ディレクトリ異常 */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* 入力ファイル名長を取り出す */
    file_len = strnlen(file_name->fname,BPF_RU_FACC_FNAME_LEN+1); 	

    /* ファイル名長がMaxか判定する */
    if (file_len == BPF_RU_FACC_FNAME_LEN+1){
        /* ファイル名異常 */
        return BPF_RU_FACC_ERR_FNAME;
    } 

    /* ディレクトリ名にワイルドカード指定がないかチェックする */
    work_p = strchr(dir_path_name->dpathname, '*');
    if (work_p != NULL){
        /* ディレクトリ名に"*"が指定された */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    work_p = strchr(dir_path_name->dpathname, '?');
    if (work_p != NULL){
        /* ディレクトリ名に"?"が指定された */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* ファイル名にワイルドカード指定がないかチェックする */
    work_p = strchr(file_name->fname, '*');
    if (work_p != NULL){
        /* ファイル名に"*"が指定された */
        return BPF_RU_FACC_ERR_FNAME;
    }

    work_p = strchr(file_name->fname, '?');
    if (work_p != NULL){
        /* ファイル名に"?"が指定された */
        return BPF_RU_FACC_ERR_FNAME;
    }

    /* フルパスファイル名生成処理を実施する */
    path_len = dir_len;
    /* ディレクトリパスを連結する */
    if(dir_path_name->dpathname[0] != '/'){
       tmppath[0]='/'; /* fail safe */
       path_len ++;
    }
    strncat(tmppath,dir_path_name->dpathname,dir_len);

    /* 末尾に"/"を追加する */
    tmppath[path_len] = '/';

    /* ディレクトリパスとファイルネームを連結する */
    strncat(tmppath,file_name->fname,file_len);

    /* フルパスファイル名を設定する */
    memcpy(pathname,tmppath,sizeof(tmppath));

    /* 正常終了 */
    return BPF_RU_FACC_OK; 
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control File Lock.
 *  @note   FACC internal function. 
 *  @param  fdescriptor      [in]  file descriptor
 *  @param  open_mode        [in]  open mode
 *  @param  exc_mode         [in]  exclusion mode
 *  @return result code
 *  @retval BPF_RU_FACC_OK             normal end
 *  @retval BPF_RU_FACC_ERR_EXC        exclusion error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_facc_filelock(
        int fdescriptor        ,               /* ファイルディスクリプタ (I) */
        bpf_ru_facc_openmode open_mode,        /* オープンモード         (I) */
        bpf_ru_facc_excmode  exc_mode)         /* 排他制御モード         (I) */
{
    int ret;           /* リターンコード        */
    struct flock lock; /* ロック用I/F設定エリア */

    /* 初期化処理 */
    ret = 0;

    /* ロック処理についてはAdvisory LockとMandatory Lockがあるが、試作では */
    /* Advisory Lockのみを設定し、Mandatory Lockは非許容とする。尚、modeに */
    /* ついてはBPF_RU_FACC_FOPENが保証するため、パラメータチェックも行わない  */

    /************************************************
    * file lock type set processing.                *
    ************************************************/
    /* openモードは判定する */
    if (open_mode == FACC_READ){
        /* read only指定 */
        /* ロックタイプに共用ロックを設定する */
        lock.l_type = F_RDLCK;
    }else{
        /* ロックタイプに排他ロックを設定 */
        lock.l_type = F_WRLCK;
    }

    /************************************************
    * file lock processing.                         *
    ************************************************/
    /* Advisory Lock入力パラメータの設定 */
    lock.l_start  = 0;         /* Starting offset for lock */
    lock.l_whence = SEEK_SET;  /* How to interpret l_start SEEK_SET=top of buf */
    lock.l_len    = 0;         /*  */
    lock.l_pid    = (int)getpid();  /*  */

    /* Advisory Lockをかける */
    /* arg2は試作では固定でF_SETLKとし、待ち合わせは行わない。今後どうするか */
    /* は検討の余地あり                                                      */
    ret = fcntl(fdescriptor, F_SETLK, &lock);

    /* ロック結果を判定する */
    if (ret == BPF_RU_FACC_SYS_NG){
        /* ロック失敗 */
        return BPF_RU_FACC_ERR_EXC;
    }

    /* ロック成功。正常終了 */
    return BPF_RU_FACC_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control File Unlock.
 *  @note   FACC internal function. 
 *  @param  fdescriptor      [in]  file descriptor
 *  @param  open_mode        [in]  open mode
 *  @param  exc_mode         [in]  exclusion mode
 *  @return result code
 *  @retval BPF_RU_FACC_OK             normal end
 *  @retval BPF_RU_FACC_ERR_EXC        exclusion error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_facc_fileunlock(int fdescriptor)
{
    int ret;           /* リターンコード        */
    struct flock lock; /* ロック用I/F設定エリア */

    /************************************************
    * file unlock processing.                       *
    ************************************************/
    /* Advisory Lock入力パラメータの設定 */
    lock.l_type   = F_UNLCK; /* Unlockを設定                                 */
    lock.l_start  = 0;       /* Starting offset for lock                     */
    lock.l_whence = SEEK_SET;/* How to interpret l_start SEEK_SET=top of buf */
    lock.l_len    = 0;       /* Number of bytes to lock                      */
    lock.l_pid    = (int)getpid();/* PID of process blocking our lock             */

    /* Advisory Lockを解除する */
    ret = fcntl(fdescriptor, F_SETLK, &lock);

    /* ロック解除結果を判定する */
    if (ret == BPF_RU_FACC_SYS_NG){
        /* ロック解除失敗 */
        return BPF_RU_FACC_ERR_EXC; /* ロック解除が失敗したといわれても。。 */
    }

    return BPF_RU_FACC_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control Directory Path make.
 *  @note   FACC internal function. 
 *  @param  *dir_path_name   [in]  directory path name
 *  @param  *pathname        [out] full path file name
 *  @return result code
 *  @retval BPF_RU_FACC_OK             normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FNAME      file name error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int bpf_ru_facc_makepathname_dir(
	const bpf_ru_facc_dpathname *dir_path_name,  /* ディレクトリパス名    (I) */
	char  *pathname)                             /* フルパス名            (O) */
{
    size_t dir_len;                              /* ディレクトリLength        */
    char tmppath[BPF_RU_FACC_DPATHNAME_LEN] = ""; /* 必要な場合はベースdirを設定しておく */
                                                 /* 連結前Dir格納エリア       */
    int ret = BPF_RU_FACC_OK;                    /* return code設定用         */
    char *work_p;                                /* ワイルドカード指定判定用  */

    /************************************************
    * dir path file name make processing.          *
    ************************************************/
    /* ディレクトリパス名チェックを行う */
    if (dir_path_name == NULL){
        /* ディレクトリパス名異常 */
        ret = BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* 処理続行可能かどうかを判定する */
    if (ret != BPF_RU_FACC_OK){
        /* 入力情報に誤りがある */	
        return ret;
    }

    /*====================================================================*/
    /* strnlenは終端NULLを含まないバイト数を返却する。従ってMAXサイズが   */
    /* 返却された場合は終端にNULLが設定されていないことになる             */
    /*====================================================================*/ 

    /* 入力ディレクトリ長を取り出す */
    dir_len = strnlen(dir_path_name->dpathname, BPF_RU_FACC_DPATHNAME_LEN);

    /* ディレクトリ長がMaxか判定する */
    if (dir_len == BPF_RU_FACC_DPATHNAME_LEN){
        /* ディレクトリ異常 */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* ディレクトリ名にワイルドカード指定がないかチェックする */
    work_p = strchr(dir_path_name->dpathname, '*');
    if (work_p != NULL){
        /* ディレクトリ名に"*"が指定された */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    work_p = strchr(dir_path_name->dpathname, '?');
    if (work_p != NULL){
        /* ディレクトリ名に"?"が指定された */
        return BPF_RU_FACC_ERR_DPATHNAME;
    }

    /* フルパスファイル名生成処理を実施する */

    /* ディレクトリパスを連結する */
    if(dir_path_name->dpathname[0] != '/'){
        tmppath[0] = '/';
    }
    strncat(tmppath,dir_path_name->dpathname,dir_len);

    /* フルパスファイル名を設定する */
    memcpy(pathname,tmppath,sizeof(tmppath));

    /* 正常終了 */
    return BPF_RU_FACC_OK; 
}
/* @} */

/* @} */
