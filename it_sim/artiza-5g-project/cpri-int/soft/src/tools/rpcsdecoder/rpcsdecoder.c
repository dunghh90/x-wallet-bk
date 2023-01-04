#define _GNU_SOURCE
#include <sys/types.h> 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <time.h>
#include <dirent.h>

#define D_RPCS_INFO_SIZE		65536
#define D_RPCS_OPE_MAX			8

typedef	unsigned char					UCHAR;
typedef	unsigned short int				USHORT;
typedef	unsigned int					UINT;
typedef	char							CHAR;
typedef	short int						SHORT;
typedef	int								INT;
typedef	unsigned long long int			ULLINT;

typedef struct {
	time_t	ofs_time_s;
	UINT	ofs_time_us;
	INT		rpc_size_pre;
	INT		rpc_size;
	pid_t	rpc_pid;
}T_RPCS_TIMES;

static UINT rpcs_time_info_index[D_RPCS_OPE_MAX];
static T_RPCS_TIMES rpcs_time_info[D_RPCS_OPE_MAX][D_RPCS_INFO_SIZE];
static 	ULLINT	rpcs_time_temp[D_RPCS_INFO_SIZE];
static 	ULLINT	rpcs_time_avg[D_RPCS_INFO_SIZE];
static 	ULLINT	rpcs_time_max[D_RPCS_INFO_SIZE];
static 	ULLINT	rpcs_time_min[D_RPCS_INFO_SIZE];
static 	INT		rpcs_time_size[D_RPCS_INFO_SIZE];
static 	INT		rpcs_time_size_pre[D_RPCS_INFO_SIZE];
static 	pid_t	rpcs_time_pid[D_RPCS_INFO_SIZE];
static 	INT		rpcs_time_index_count[D_RPCS_INFO_SIZE];

static int check_wildcard(size_t len , char* check_string ,int* pos){
	UINT	count;
	for(count=0;count < len;count++){
		if(check_string[count] == '*'){
			*pos = count;
			return 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	FILE*	wfp=NULL;
	FILE*	rfp=NULL;
	CHAR	str2[64*1024];
	INT		str2_len = 0;
	INT		check_len = 0;
	INT		rtn_flg = 0;
	INT		insert_rtn_flg = 0;
	UINT	logs_ope_flg = 0;
	CHAR*	logs_ope_name[D_RPCS_OPE_MAX] = { "NotOpe",
								 "RPC  ", 
								 "REPLY",
								 "NOTI ",
								 "HELLO",
								 "NotOpe",
								 "NotOpe",
								 "NotOpe" };
	CHAR	logs_dir_flg = 0;
	CHAR	logs_times[4096];
	CHAR	logs_timeflg = 0;
	UINT	logs_count = 1;
	struct tm cur_time;
	INT		pre_time_usec = 0;
	time_t	pre_sec = 0;
	INT		cur_time_usec = 0;
	time_t	cur_sec = 0;
	time_t	ofs_time_s = 0;
	UINT	ofs_time_us = 0;
	UINT	rpcs_time_count = 0;
	UINT	rpcs_time_size_count = 0;
	UINT	rpcs_time_drop_count;
	CHAR*	rpcs_time_name;
	INT		rpc_msgid_size;
	INT		rpc_msgid;
	UINT	msg_count;

	if(argc < 2){
		printf("\n");
		printf("Usage: %s <full-path-rpcs-log-filename > <opt>\n",argv[0]);
		printf("\n");
		printf("  e.g.)\n");
		printf("    %s /var/log/netconfd-pro.log.rpcs.stup\n",argv[0]);
		printf("    %s /var/log/netconfd-pro.log.rpcs.log.1234\n",argv[0]);
		printf("    %s /var/log/netconfd-pro.log.rpcs.old.1234\n",argv[0]);
		printf("\n");
		printf(" <opt> \n");
		printf("  -t : timestamp only \n");
		printf("  -td : timestamp only and detail time \n");
		printf("\n");
		fflush(stdout);
		return(-1);
	}


	UINT	opt_time_mode = 0;

	if(argc > 2){
		if((strlen(argv[argc - 1]) > 1) && (argv[argc - 1][0] == '-')){
			switch(argv[argc - 1][1]){
				case 't':
					if((strlen(argv[argc - 1]) > 2) && (argv[argc - 1][2] == 'd')){
						opt_time_mode = 2;
					}else{
						opt_time_mode = 1;
					}
					break;

				default:
					break;
			}
		}
	}

	CHAR			log_file_full_name[512];
	CHAR			log_file_dir_name[256];
	UINT			dirflg;
	DIR*			log_file_dir = NULL;
	struct dirent	*log_file_dent;
	const char*		wname;
	CHAR			log_file_cmp_name[256];
	INT				pos;
	
	memset(log_file_full_name,0,sizeof(log_file_full_name));
	memset(log_file_dir_name,0,sizeof(log_file_dir_name));
	memset(log_file_cmp_name,0,sizeof(log_file_cmp_name));

	if( argv[1][strlen(argv[1]) - 1] == '/'){
		dirflg = 1;
		if((log_file_dir = opendir(argv[1])) == NULL){
			perror(argv[1]);
			fflush(stdout);
			return(-1);
		}
		snprintf(log_file_dir_name,sizeof(log_file_dir_name),"%s",argv[1]);
		strcpy(log_file_cmp_name,"netconfd-pro.log.rpcs");
	}
	else if(( check_wildcard( strlen(argv[1]) - 1 , argv[1] , &pos)) && ((wname = strrchr(argv[1], '/')) != NULL)){
		wname++;
		dirflg = 1;
		strncpy(log_file_dir_name,argv[1],(size_t)(wname - argv[1]));
		if((log_file_dir = opendir(log_file_dir_name)) == NULL){
			perror(argv[1]);
			fflush(stdout);
			return(-1);
		}
		if(pos > 0){
			check_wildcard( strlen(wname) , (char*)wname , &pos);
			if(pos > 0){
				strncpy(log_file_cmp_name,wname,pos);
			}else{
				strcpy(log_file_cmp_name,"netconfd-pro.log.rpcs");
			}
		}else{
			strcpy(log_file_cmp_name,"netconfd-pro.log.rpcs");
		}
	} else{
		dirflg = 0;
		snprintf(log_file_full_name,sizeof(log_file_full_name),"%s",argv[1]);
	}


	UINT search_count;
	UINT rpcs_time_size_index;
	UINT summary_flg;
	int rpc_size;
	unsigned long dummy2,dummy3;
	int rpc_size_pre;
	pid_t rpc_pid;

	wfp = stdout;
	while(1){

		if(dirflg != 0){
			if((log_file_dent = readdir(log_file_dir)) == NULL){
				break;
			}
			if( strncmp((log_file_dent->d_name),log_file_cmp_name,strlen(log_file_cmp_name)) ){
				continue;
			}
			memset(log_file_full_name,0,sizeof(log_file_full_name));
			snprintf(log_file_full_name,sizeof(log_file_full_name),"%s%s",log_file_dir_name,log_file_dent->d_name);
		}

		if(argc > 2){
			fprintf(wfp,"LOG FILE NAME:%s argc:%d Opt:%d ArgV:%s \n\n",log_file_full_name,argc,opt_time_mode,argv[argc - 1]);
		}else{
			fprintf(wfp,"LOG FILE NAME:%s\n\n",log_file_full_name);
		}

		rfp = fopen(log_file_full_name,"r");
		if(rfp == NULL){
			printf("%s File open error\n",log_file_full_name);
			fflush(stdout);
			return(-1);
		}

		logs_dir_flg = 0;
		logs_timeflg = 0;
		logs_count = 1;
		pre_time_usec = 0;
		pre_sec = 0;
		cur_time_usec = 0;
		cur_sec = 0;
		ofs_time_s = 0;
		ofs_time_us = 0;
		rpcs_time_count = 0;
		rpc_size_pre = 0;
		rpc_msgid_size = 0;
		rpc_msgid = 0;

		if(opt_time_mode != 0){
			memset(rpcs_time_info_index,0,sizeof(rpcs_time_info_index));
			memset(rpcs_time_info,0,sizeof(rpcs_time_info));
		}
		while(fgets(str2, sizeof(str2), rfp) != NULL) {
			switch(str2[0]){
				case '#':
				case '\n':
					continue;
					break;
				case ']':
					str2_len = strlen(str2);
					check_len = 0;
					while(check_len != str2_len){
						if(!strncmp(&str2[check_len],"<hello ",strlen("<hello "))){
							logs_ope_flg = 4;
							logs_timeflg = 1;
							break;
						}
						check_len++;
					}
					break;
				case '>':
				case '<':
					if(str2[1] == ' '){
						memset(logs_times,0,sizeof(logs_times));
						if(str2[0] == '>'){
							logs_dir_flg = 0;
						}else{
							logs_dir_flg = 1;
						}
						snprintf(logs_times,sizeof(logs_times),"%s",(char*)&str2[2]);
						continue;
					}
					else if (str2[1] == '?'){
						str2_len = strlen(str2) - strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
						if(str2_len > strlen("<rpc ")){
							str2_len = strlen("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
							if(!strncmp(str2 + str2_len,"<rpc ",strlen("<rpc "))){
								logs_ope_flg = 1;
								logs_timeflg = 1;
								sscanf(str2 + str2_len,"<rpc message-id=\"%d\"",&rpc_msgid);
								rpc_msgid_size = 1;
								for(msg_count=0 ; msg_count < 11 ; msg_count++){
									if(( rpc_msgid / 10) > 0){
										rpc_msgid_size++;
										rpc_msgid /= 10;
										if(rpc_msgid == 0){
											break;
										}
									}else{
										break;
									}
								}
								break;
							}
						}
						continue;
					}
					else{
						if(!strncmp(str2,"<rpc ",strlen("<rpc "))){
							logs_ope_flg = 1;
							logs_timeflg = 1;
							sscanf(str2,"<rpc message-id=\"%d\"",&rpc_msgid);
							rpc_msgid_size = 1;
							for(msg_count=0 ; msg_count < 11 ; msg_count++){
								if(( rpc_msgid / 10) > 0){
									rpc_msgid_size++;
									rpc_msgid /= 10;
									if(rpc_msgid == 0){
										break;
									}
								}else{
									break;
								}
							}
						}
						else if(!strncmp(str2,"<rpc-",strlen("<rpc-"))){
							if(opt_time_mode == 0){
								logs_ope_flg = 2;
								logs_timeflg = 1;
							}else{
								logs_ope_flg = 5;
							}
						}
						else if(!strncmp(str2,"<hello ",strlen("<hello "))){
							logs_ope_flg = 4;
							logs_timeflg = 1;
						}
						else if(!strncmp(str2,"</rpc-",strlen("</rpc-"))){
							if(opt_time_mode == 0){
								logs_ope_flg = 5;
							}else{
								logs_ope_flg = 2;
								logs_timeflg = 1;
							}
						}
						else if(!strncmp(str2,"</rpc>",strlen("</rpc>"))){
							logs_ope_flg = 5;
						}
						else if(!strncmp(str2,"</hello>",strlen("</hello>"))){
							logs_ope_flg = 6;
						}
						else if(!strncmp(str2,"<ycontrol",strlen("<ycontrol"))){
							logs_ope_flg = 0;
						}
						else{
						}
					}
					break;
				case ' ':
					if(!strncmp(str2," <notification ",strlen(" <notification "))){
						if(opt_time_mode == 0){
							logs_ope_flg = 3;
							logs_timeflg = 1;
						}else{
							logs_ope_flg = 7;
						}
					}
					else if(!strncmp(str2," </notification>",strlen(" </notification>"))){
						if(opt_time_mode == 0){
							logs_ope_flg = 7;
						}else{
							logs_ope_flg = 3;
							logs_timeflg = 1;
						}
					}
					else{
						str2_len = strlen(str2);
						check_len = 0;
						while(check_len != str2_len){
							if(( str2[check_len] != ' ') && ( str2[check_len] != '\n')){
								break;
							}
							check_len++;
						}
						if(check_len == str2_len){
							continue;
						}
					}
					break;
				default:
					break;
			}

			if(logs_ope_flg == 0){
				continue;
			}

			if(logs_timeflg == 1){
				pre_time_usec = cur_time_usec;
				pre_sec = cur_sec;

				memset(&cur_time,0,sizeof(cur_time));
				sscanf(logs_times,"%04d/%02d/%02d %02d:%02d:%02d.%06d  length=%d from=%ld to=%ld pid=%d",
					&(cur_time.tm_year),
					&(cur_time.tm_mon),
					&(cur_time.tm_mday),
					&(cur_time.tm_hour),
					&(cur_time.tm_min),
					&(cur_time.tm_sec),
					&cur_time_usec,
					&rpc_size,
					&dummy2,
					&dummy3,
					&rpc_pid
					);

				cur_time.tm_year -= 1900;
				cur_sec = mktime(&cur_time);

				if(pre_sec > cur_sec){
					ofs_time_s = 0;
					ofs_time_us = 0;
				}else{
					ofs_time_s = cur_sec - pre_sec;
					ofs_time_us = 0;
					if((pre_time_usec > cur_time_usec) && (ofs_time_s > 0))
					{
						ofs_time_s--;
						ofs_time_us = 1000000;
					}
					ofs_time_us = ofs_time_us + cur_time_usec - pre_time_usec;
				}
				if(logs_dir_flg == 0){
					if(opt_time_mode != 0){
						if(opt_time_mode == 2){
							fprintf(wfp,"[No.%04d] <CU> --- %s --> <RU> (%lds%u.%03ums) %s",logs_count,logs_ope_name[(int)logs_ope_flg],ofs_time_s,ofs_time_us/1000,ofs_time_us%1000,logs_times);
						}
					}else{
						fprintf(wfp,"\n[No.%04d] <CU> --- %s --> <RU> %s",logs_count,logs_ope_name[(int)logs_ope_flg],logs_times);
					}
				}else{
					if(opt_time_mode != 0){
						if(opt_time_mode == 2){
							fprintf(wfp,"[No.%04d] <CU> <-- %s --- <RU> (%lds%u.%03ums) %s",logs_count,logs_ope_name[(int)logs_ope_flg],ofs_time_s,ofs_time_us/1000,ofs_time_us%1000,logs_times);
						}
					}else{
						fprintf(wfp,"\n[No.%04d] <CU> <-- %s --- <RU> %s",logs_count,logs_ope_name[(int)logs_ope_flg],logs_times);
					}
				}

				if(opt_time_mode != 0){
					if(rpcs_time_info_index[logs_ope_flg] < D_RPCS_INFO_SIZE){
						rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].ofs_time_s  = ofs_time_s;
						rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].ofs_time_us = ofs_time_us;
						rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].rpc_pid = rpc_pid;
						if((logs_ope_flg == 1) || (logs_ope_flg == 2)){
							rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].rpc_size_pre = rpc_size_pre - rpc_msgid_size;
							rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].rpc_size = rpc_size - rpc_msgid_size;
							if(logs_ope_flg == 1){
								rpc_size_pre = rpc_size;
							}
						}else{
							rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].rpc_size_pre = 0;
							rpcs_time_info[logs_ope_flg][ rpcs_time_info_index[logs_ope_flg] ].rpc_size = rpc_size;
						}
						rpcs_time_info_index[logs_ope_flg]++;

					}
				}

				logs_timeflg = 0;
				logs_count++;
				rtn_flg = 0;
			}
			if(opt_time_mode == 0){
				str2_len = strlen(str2);
				check_len = 0;
				insert_rtn_flg = 0;
				while(check_len != str2_len){
					if((check_len !=0) && (str2[check_len] == '<') && (str2[check_len - 1] == '>')){
						insert_rtn_flg = 1;
					}
					check_len++;
				}

				if(rtn_flg == 1){
					if(str2[str2_len - 2] != '>'){
						str2[str2_len - 1] = '\0';
					}else{
						rtn_flg = 0;
					}
				}else{
					check_len = 0;
					while(check_len != str2_len){
						if(str2[check_len] == '<'){
							if(str2[str2_len - 2] != '>'){
								str2[str2_len - 1] = '\0';
								rtn_flg = 1;
								break;
							}
						}
						check_len++;
					}
				}
				if(insert_rtn_flg != 0){
					check_len = 0;
					while(check_len != str2_len){
						if(check_len !=0 ){
							if((str2[check_len] == '<') && (str2[check_len - 1] == '>')){
								fprintf(wfp,"\n");
							}
							else if((str2[check_len] == ' ') && (str2[check_len - 1] == '"') && ((str2[check_len + 1] != 'm') && (str2[check_len + 1] != 'e') && (str2[check_len + 1] != 'x'))){
								fprintf(wfp,"\n");
							}
							else if((str2[check_len] == ' ') && (str2[check_len - 1] == '|')){
								fprintf(wfp,"\n");
							}
						}

						fprintf(wfp,"%c", str2[check_len]);
						check_len++;
					}
				}else{
					fprintf(wfp,"%s", str2);
				}
			}
			if(logs_ope_flg >= 5){
				logs_ope_flg = 0;
			}
		}
		
		if(opt_time_mode != 0){
			summary_flg = 0;

			for(logs_ope_flg=0 ; logs_ope_flg < D_RPCS_OPE_MAX ; logs_ope_flg++){
				if(rpcs_time_info_index[logs_ope_flg] == 0){
					continue;
				}

				if(summary_flg == 0){
					fprintf(wfp,"\nTimestamp Summary ------------------------------------------------------------------------------------------------------------------------------------------------- \n\n");
					summary_flg++;
				}

				switch(logs_ope_flg){
					case 1:
						rpcs_time_name = "Rpc         ";
						break;
					case 2:
						rpcs_time_name = "Rpc-Reply   ";
						break;
					case 3:
						rpcs_time_name = "Notification";
						break;
					case 4:
						rpcs_time_name = "Hello       ";
						break;
					default:
						rpcs_time_name = "Unknown     ";
						break;
				}

				memset(rpcs_time_avg,0,sizeof(rpcs_time_avg));
				memset(rpcs_time_max,0,sizeof(rpcs_time_max));
				memset(rpcs_time_min,0xFF,sizeof(rpcs_time_min));
				memset(rpcs_time_temp,0,sizeof(rpcs_time_temp));
				memset(rpcs_time_size,0,sizeof(rpcs_time_size));
				memset(rpcs_time_size_pre,0,sizeof(rpcs_time_size_pre));
				memset(rpcs_time_pid,0,sizeof(rpcs_time_pid));
				memset(rpcs_time_index_count,0,sizeof(rpcs_time_index_count));
				rpcs_time_drop_count = 0;

				for(rpcs_time_count = 0,rpcs_time_size_count = 0 ;rpcs_time_count < rpcs_time_info_index[logs_ope_flg] ;rpcs_time_count++){

					if(rpcs_time_info[logs_ope_flg][ rpcs_time_count ].ofs_time_s >= 3600){
						rpcs_time_drop_count++;
//						fprintf(wfp,"Skip!!! [%04d]%s(%8d bytes) Guardtime Over(1hour) : %lds %u.%03um\n",
//							rpcs_time_count + 1,
//							rpcs_time_name,
//							rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_size,
//							rpcs_time_info[logs_ope_flg][ rpcs_time_count ].ofs_time_s,
//							rpcs_time_info[logs_ope_flg][ rpcs_time_count ].ofs_time_us / 1000,
//							rpcs_time_info[logs_ope_flg][ rpcs_time_count ].ofs_time_us % 1000
//							);
						continue;
					}

					for(search_count = 0; search_count < rpcs_time_size_count ;search_count++){
						if( ( rpcs_time_size[search_count] == rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_size)&&
							( rpcs_time_pid[search_count] == rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_pid)){
							if((logs_ope_flg == 2) && (rpcs_time_size_pre[search_count] != rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_size_pre)) {
								continue;
							}
							rpcs_time_size_index = search_count;
							break;
						}
					}
					if((rpcs_time_size_count == 0) || (search_count >= rpcs_time_size_count )){
						rpcs_time_size[rpcs_time_size_count]     = rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_size;
						rpcs_time_pid[rpcs_time_size_count]      = rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_pid;
						if(logs_ope_flg == 2){
							rpcs_time_size_pre[rpcs_time_size_count] = rpcs_time_info[logs_ope_flg][ rpcs_time_count ].rpc_size_pre;
						}
						rpcs_time_size_index                     = rpcs_time_size_count;
						rpcs_time_size_count++;
					}
					rpcs_time_temp[rpcs_time_size_index] = (rpcs_time_info[logs_ope_flg][ rpcs_time_count ].ofs_time_s * 1000000) + rpcs_time_info[logs_ope_flg][ rpcs_time_count ].ofs_time_us;
					if(rpcs_time_temp[rpcs_time_size_index] >= rpcs_time_max[rpcs_time_size_index]){
						rpcs_time_max[rpcs_time_size_index] = rpcs_time_temp[rpcs_time_size_index];
					}
					if(rpcs_time_min[rpcs_time_size_index] >= rpcs_time_temp[rpcs_time_size_index]){
						rpcs_time_min[rpcs_time_size_index] = rpcs_time_temp[rpcs_time_size_index];
					}
					rpcs_time_avg[rpcs_time_size_index] += rpcs_time_temp[rpcs_time_size_index];
					rpcs_time_index_count[rpcs_time_size_index]++;
				}

				fprintf(wfp,"%s Total Iteration:%u counts VALID:%u counts / INVALID:%u counts (1hour over))\n",
						rpcs_time_name,rpcs_time_info_index[logs_ope_flg],rpcs_time_info_index[logs_ope_flg] - rpcs_time_drop_count,rpcs_time_drop_count);
				fprintf(wfp,"NO.               (          Size.      Rpc Size)\n");
				for(search_count = 0; search_count < rpcs_time_size_count ;search_count++){
					rpcs_time_avg[search_count] = rpcs_time_avg[search_count] / rpcs_time_index_count[search_count];
					if(rpcs_time_avg[search_count] % rpcs_time_index_count[search_count] > 0){
						rpcs_time_avg[search_count]++;
					}
					fprintf(wfp,"[%04d]%s(%8d bytes.%8d bytes) Iteration:%4u counts Maximum:%10llds%4lld.%03lldm / Minimum:%10llds%4lld.%03lldm / Average:%10llds%4lld.%03lldm / Pid:%d\n",
							search_count + 1,
							rpcs_time_name,
							rpcs_time_size[search_count],
							rpcs_time_size_pre[search_count],
							rpcs_time_index_count[search_count],
							rpcs_time_max[search_count] / 1000000,
							( rpcs_time_max[search_count] % 1000000 ) / 1000,
							( rpcs_time_max[search_count] % 1000000 ) % 1000,	
							rpcs_time_min[search_count] / 1000000,
							( rpcs_time_min[search_count] % 1000000 ) / 1000,
							( rpcs_time_min[search_count] % 1000000 ) % 1000,	
							rpcs_time_avg[search_count] / 1000000,
							( rpcs_time_avg[search_count] % 1000000 ) / 1000,
							( rpcs_time_avg[search_count] % 1000000 ) % 1000,
							rpcs_time_pid[search_count]
					);
				}
				fprintf(wfp,"\n");
			}
			if(summary_flg > 0){
				fprintf(wfp,"------------------------------------------------------------------------------------------------------------------------------------------------------------------- \n");
			}
		}

		fprintf(wfp,"\n");
		fflush(wfp);
		fclose(rfp);

		if(dirflg == 0){
			break;
		}
	}
	if(dirflg != 0){
		closedir(log_file_dir);
	}
	return(0);
}
