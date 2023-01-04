/**************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  proc_logdump.c
 * @brief
 * @date  2013/08/05 FJT)Taniguchi Create\n
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2013
 */
/**************************************************************************************************/

/*!
 * @addtogroup RRH_CMN
 * @{
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "f_rrh_inc.h"
#include "BPF_RM_SVRM.h"

/**************************************************************************************************/
/*!
 *  @brief  main
 *  @note   coredump�\��
 *  @return None
 *  @date
***************************************************************************************************/
int main(int arg , char** argv)
{
	// FILE*	fp;
	// INT		len;
	// INT		size;
	// INT		rtn;
	// CHAR*	tbl_p;
	// CHAR*	buf_p = NULL;

	// if(arg < 2 ){
	// 	printf("Usage:%s\n",argv[0]);
	// 	printf(" %s <input file name> [ output file name] \n",argv[0]);
	// 	printf("   <input file name> e.g.)core file \n");
	// 	printf("   [output file ] output txt filename ,if not, stdout \n");
	// 	return(1);
	// }
	// fp = fopen(argv[1],"rb");
	// if(fp == NULL)
	// {
	// 	printf("File analyze file open error \n");
	// 	return(1);
	// }
	// rtn = fseek(fp,0, SEEK_END);
	// if(rtn != 0)
	// {
	// 	printf("file seek(seek end) error \n");
	// 	fclose(fp);
	// 	return(1);
	// }
	// size = ftell(fp);
	// if(size < 0)
	// {
	// 	printf("file tell error \n");
	// 	fclose(fp);
	// 	return(1);
	// }
	// rtn = fseek(fp, 0, SEEK_SET);
	// if(rtn != 0)
	// {
	// 	printf("file seek(seek end) error 2\n");
	// 	fclose(fp);
	// 	return(1);
	// }
	// buf_p = malloc(size);
	// if(buf_p == NULL)
	// {
	// 	printf("malloc error 2\n");
	// 	fclose(fp);
	// 	return(1);
	// }
	// len = fread(buf_p, 1, size , fp);
	// if(len != size)
	// {
	// 	printf("file read error \n");
	// 	free(buf_p);
	// 	fclose(fp);
	// 	return(1);
	// }
	// fclose(fp);

	// tbl_p = buf_p;
	// if(arg > 2){
	// 	fp = fopen(argv[2],"w");
	// 	if(fp == NULL){
	// 		printf("Dump file open error\n");
	// 		free(buf_p);
	// 		return(1);
	// 	}
	// 	(void)BPF_RM_SVRM_SIGNAL_COREDUMP(fp,tbl_p,size);
	// 	fflush(fp);
	// 	fclose(fp);
	// }
	// else{
	// 	(void)BPF_RM_SVRM_SIGNAL_COREDUMP(stdout,tbl_p,size);
	// 	fflush(stdout);
	// }
	// free(buf_p);
	return(0);
}

/* @} */
