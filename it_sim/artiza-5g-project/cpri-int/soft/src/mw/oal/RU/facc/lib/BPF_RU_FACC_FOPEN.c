/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FOPEN.c
 *	@brief	File Access Control File Open.
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
 *  @brief  File Access Control File Open.
 *  @note   API function. 
 *  @param  dir_path_name   [in]  directory path name
 *  @param  file_name       [in]  file name
 *  @param  open_mode       [in]  open mode
 *  @param  create_mode     [in]  file create mode
 *  @param  file_descriptor [out] file descriptor
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FNAME      file name error
 *  @retval BPF_RU_FACC_ERR_FCOUNT     number of file over error
 *  @retval BPF_RU_FACC_ERR_OMODE      open mode error
 *  @retval BPF_RU_FACC_ERR_FOPEN      file open error
 *  @retval BPF_RU_FACC_ERR_EXC        exclusion error
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FOPEN(const bpf_ru_facc_dpathname *dir_path_name, const bpf_ru_facc_fname *file_name,
                      bpf_ru_facc_openmode open_mode, unsigned int  create_mode, int *file_descriptor, int *errcd)                           /* 詳細NGコード            (O) */
{
    char pathname[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
                                               /* フルパスファイル名     */
    int l_fdescriptor;                         /* ファイルディスクリプタ */
    int ret;                                   /* リターンコード         */
    int flag;                                  /* ファイルオープンフラグ */
    int mode;                                  /* ファイルオープンモード */
    int exc_mode;                              /* 排他制御モード         */
	mode_t c_mode;

    /* 初期化処理を行う */
    l_fdescriptor = 0;
    ret           = 0;
    flag          = 0;
    mode          = 0;
    memset(pathname,0,sizeof(pathname));


    /************************************************
    * file open processing.                         *
    ************************************************/
    /* フルパス生成を行う */
    ret = bpf_ru_facc_makepathname(dir_path_name, /* ディレクトリパス名 (I) */
                                    file_name,    /* ファイル名         (I) */
                                    pathname);    /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /*====================================================================*/
    /* ファイルオープン時に該当ファイルが無かった場合は、ユーザの指定に   */
    /* 従う。Defaultは作成無しとし、パラメータチェック対象外とする        */
    /*====================================================================*/
	if (create_mode & BPF_RU_FACC_CREATE){
		flag = create_mode;
	}

    /* flag&Permitionはユーザ指定値を設定する */
    switch(open_mode){
        case FACC_READ:
            /* read only指定 */
            flag = flag | O_RDONLY;
            mode = (S_IRUSR | S_IRGRP | S_IROTH); 
            break;
        case FACC_WRITE:
            /* write only指定 */
            flag = flag | O_WRONLY;
            mode = (S_IWUSR | S_IWGRP | S_IWOTH);
            break;
        case FACC_READWRITE:
            /* read+write指定 */
            flag = flag | O_RDWR;
            mode = ((S_IRUSR+S_IWUSR) | (S_IRGRP+S_IWGRP) | (S_IROTH+S_IWOTH));
            break;
        default:
            /* オープンモード異常 */
            return BPF_RU_FACC_ERR_OMODE;
    }

	c_mode = umask(0);

    /* システムコール file openを実行する */
    l_fdescriptor = open(pathname,           /* pathname       (I) */ 
                         flag    ,           /* オープンフラグ (I) */ 
                         mode);              /* Permition      (I) */
	umask(c_mode);

    /* file open結果を判定する */
    if(l_fdescriptor == BPF_RU_FACC_SYS_NG){
        /* file open失敗 */
        /* 詳細エラーコードを設定する */
        *errcd = errno;
        return BPF_RU_FACC_ERR_FOPEN;
    }
	
    /* file open成功。ファイルディスクリプタを設定する */
    *file_descriptor = l_fdescriptor;

	/* Enable close-on-exec flag */
	ret = fcntl(l_fdescriptor, F_SETFD, FD_CLOEXEC);
	if( ret == BPF_RU_FACC_SYS_NG ){
		*errcd = errno;
		ret = close(l_fdescriptor);
		return BPF_RU_FACC_ERR_FOPEN;
	}	
	
    /************************************************
    * file lock processing.                         *
    ************************************************/
    /*==================================================================*/
    /* ユーザ側に排他モードは指定させず、一律AdvisoryLockとする         */
    /*==================================================================*/
    /* 排他制御モード設定 */
    exc_mode = FACC_ADV;

    /* 排他制御を実施する */
    ret = bpf_ru_facc_filelock(l_fdescriptor, /* ファイルディスクリプタ (I) */
                               open_mode,     /* オープンモード         (I) */
                               exc_mode);     /* 排他制御モード         (I) */

    /* 排他制御結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* 排他制御失敗 */
        /* ファイルをクローズして、処理を終了する */
        ret = close(l_fdescriptor);          /* ファイルディスクリプタ (I) */

        /* ここも排他制御失敗を返しておくが、詳細は必要か？ */
        return BPF_RU_FACC_ERR_EXC;
    }

    /* ファイルオープン処理成功 */

    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
