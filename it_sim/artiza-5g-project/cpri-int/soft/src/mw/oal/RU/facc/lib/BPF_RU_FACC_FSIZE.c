/********************************************************************************************************************/
/**
 *	@skip	$Id:$
 *	@file	BPF_RU_FACC_FSIZE.c
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
 *  @param  file_sz         [out] file size(byte)
 *  @param  errcd           [out] detail error code
 *  @return result code
 *  @retval BPF_RU_FACC_COMPLETE       normal end
 *  @retval BPF_RU_FACC_ERR_DPATHNAME  directory path name error
 *  @retval BPF_RU_FACC_ERR_FNAME      file name error
 *  @retval BPF_RU_FACC_ERR_FSIZE      file size get error
 *  @retval BPF_RU_FACC_ERR_ENOENT     No such file
 *  @date   2013/04/30 ALPHA)matsuhashi BS3001 ORIGINAL(COPY)
 */
/********************************************************************************************************************/
int BPF_RU_FACC_FSIZE(
	const bpf_ru_facc_dpathname *dir_path_name,
	const bpf_ru_facc_fname *file_name,
	long *file_sz, 
	int *errcd)
{
	char pathname[BPF_RU_FACC_DPATHNAME_LEN+BPF_RU_FACC_FNAME_LEN];
    int ret=0;
	struct stat stat_buf;

	memset(pathname,0,sizeof(pathname));
	memset(&stat_buf,0,sizeof(stat_buf));
	*errcd = 0;
	*file_sz = 0;
	
	/* Cat dir name and file name */
	ret = bpf_ru_facc_makepathname(
				dir_path_name,
				file_name,
				pathname);    /* pathname	(O) */

	if (ret != BPF_RU_FACC_OK){
		return ret;
	}
	
	
	ret = stat(pathname,&stat_buf);
	if( ret == BPF_RU_FACC_SYS_NG ){
		*errcd = errno;
		if( *errcd == ENOENT ){
			return BPF_RU_FACC_ERR_ENOENT;
		}else{
			return BPF_RU_FACC_ERR_FSIZE;
		}
	}
	if( (!S_ISREG(stat_buf.st_mode)) &&
		(!S_ISLNK(stat_buf.st_mode)) ){
		/* regular file  or symbolic link */
		*errcd = stat_buf.st_mode;
		return BPF_RU_FACC_ERR_FSIZE;
	}
	
	*file_sz = stat_buf.st_size;
	
	
    return BPF_RU_FACC_COMPLETE;
}
/* @} */

/* @} */
