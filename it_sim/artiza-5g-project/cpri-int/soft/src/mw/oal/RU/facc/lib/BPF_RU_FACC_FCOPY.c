/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FCOPY.c
 *	@brief	File Access Control File Copy.
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 *
 *	ALL Rights Reserved, Copyright c FUJITSU Limited 2013
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_RU_FACC /* Pre-define */

/********************************************************************************************************************/
/* include file 																									*/
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_i_ru_facc.h"

/** @addtogroup BPF_RU_FACC
 * @{ */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  File Access Control File Copy.
 *  @note   API function. 
 *  @param  src_dir_path_name [in]  source directory path name
 *  @param  dst_dir_path_name [in]  destination directory path name
 *  @param  src_file_name     [in]  source file name
 *  @param  dst_file_name     [in]  destination file name
 *  @param  errcd             [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FNAME      file name error
 *  @retval BPF_RU_FACC_ERR_FCOUNT     number of file over error
 *  @retval BPF_RU_FACC_ERR_FCOPY      file copy error
 *  @date   2013/04/30 ALPHA)matsuhashi    modiry for BS3001
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FCOPY(const bpf_ru_facc_dpathname *src_dir_path_name, const bpf_ru_facc_dpathname *dst_dir_path_name,
                      const bpf_ru_facc_fname *src_file_name, const bpf_ru_facc_fname *dst_file_name, int *errcd)
{
    int ret;                                   /* リターンコード         */
    char src_pthname[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
                                               /* srcフルパスファイル名  */
    char dst_pthname[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
                                               /* dstフルパスファイル名  */
    char cmd_str[BPF_RU_CMD_LENGTH_MAX];          /* コマンド格納エリア     */
    struct stat buf;                           /* ファイル確認用エリア   */
    int status;

    char *exec_param[6];
    int exetbl_set_ret;
    unsigned int exetbl_id;
    pid_t pid;


    /* 初期化を行う */
    memset(src_pthname,0,sizeof(src_pthname));
    memset(dst_pthname,0,sizeof(dst_pthname));
    memset(cmd_str,0,sizeof(cmd_str));
    ret = 0;
    status = 0;
	
    /************************************************
    * file copy processing.                         *
    ************************************************/
    /* srcフルパス生成を行う */
    ret = bpf_ru_facc_makepathname(src_dir_path_name, /* ディレクトリパス名 (I) */
                                   src_file_name,     /* ファイル名         (I) */
                                   src_pthname);      /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /* src側対象ファイル確認 */
    ret = stat(src_pthname, &buf);

    if (ret != BPF_RU_FACC_OK){
        /* 対象ファイルは存在しない */
        *errcd = ENOENT;
        return BPF_RU_FACC_ERR_FCOPY;
    }

    /* dstフルパス生成を行う */
    ret = bpf_ru_facc_makepathname(dst_dir_path_name, /* ディレクトリパス名 (I) */
                                   dst_file_name,     /* ファイル名         (I) */
                                   dst_pthname);      /* pathname           (O) */

    /* 生成結果を判定する */
    if (ret != BPF_RU_FACC_OK){
        /* フルパス生成失敗 */
        /* フルパス生成NG要因を設定して、return */
        return ret;
    }

    /* cpコマンドイメージ作成(exe_cmdから起動) */

/* RRH-001 MD alpha)matsuhashi add start */
    exec_param[0] = "/bin/cp";
    exec_param[1] = "-pf";
    exec_param[2] = src_pthname;
    exec_param[3] = dst_pthname;
    exec_param[4] = NULL;
/* RRH-001 MD alpha)matsuhashi add end */

    pid = fork();
    if( pid < 0 ){
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,
                          "Child process fork ng. ret=%d errno=%d", pid, errno);
        return BPF_RU_FACC_ERR_FCOPY;
    }
    if( pid == 0){
        /* child process */
        ret = nice(BPF_RU_CMD_NICE_ADJ);
        ret = execv(exec_param[0], exec_param);
        if(ret < 0){
            /* LOG(ERROR) */
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,
                          "Execute cp command  ng. ret=%d errno=%d src=%s dst=%s", 
                           ret, errno, src_pthname, dst_pthname);
            exit(50);
        }
        /* ここにはこない */
    }else{
        /* parent process */

        /* set pid on execute table */
        exetbl_set_ret = bpf_ru_facc_cmn_set_exetbl(pid, &exetbl_id);
        if( exetbl_set_ret != BPF_RU_FACC_OK ){
            /* LOG(ERROR) */
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,
                          "cannot set execute table. ret=%d", ret);
            /* Continue */
        }

        ret = waitpid(pid, &status, 0);

        /* delete pid on execute table */
        if( exetbl_set_ret == BPF_RU_FACC_OK ){
            bpf_ru_facc_cmn_delete_exetbl(exetbl_id);
        }

        if( ret < 0 ){
            /* LOG(ERROR) */
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L1,
                          "Wait pid  ng. ret=%d errno=%d", ret, errno);
            *errcd = errno;
            return BPF_RU_FACC_ERR_FCOPY;
        }
        if( WIFEXITED( status )){
            if( WEXITSTATUS( status ) != 0 ){

                 /* LOG(TRACE) */
                bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,
                          "exit status ng. status=%d", WEXITSTATUS(status));

                *errcd = status;
                return BPF_RU_FACC_ERR_FCOPY;
            }
        }else{

	        if( WIFSIGNALED( status )){
	            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
	                          "signal recieve. signal=%d", WTERMSIG(status));
	        }
            *errcd = status;
            return BPF_RU_FACC_ERR_FCOPY;

        }
    }

    /* コマンド正常終了 */

    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
