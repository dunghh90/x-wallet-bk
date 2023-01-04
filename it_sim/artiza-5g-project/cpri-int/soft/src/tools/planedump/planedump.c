#include "planedump.h"
#include <pcap.h>
#include <signal.h>
#include <sys/prctl.h>
#include <signal.h>

static pcap_t*			 rruplog_pcap_p = NULL;
static volatile int		 rrupf_stop_flg = 0;
static volatile int		 rrupf_clean_flg = 0;
int gmode = 0;

/* バイナリ表示関数 */
static void func_datadsp(
	FILE* fp,
	int   datalen,
	char* datap,
	int   maxlen,
	int   dspmode
)
{
	unsigned int count;
	unsigned int count2;
	unsigned char* tmpdatap_c = NULL;
	unsigned short* tmpdatap_s = NULL;
	unsigned int* tmpdatap_i = NULL;
	unsigned char* tmpdatap2 = NULL;
	unsigned char* tmpdatap3 = NULL;
	
	int len = datalen;
	int mode = dspmode;
	char str[6];
	unsigned int rtncount = 0;

	if(len <= 0)
	{
		return;
	}
	if(datalen > maxlen)
	{
		len = maxlen;
	}
	
	switch(dspmode)
	{
		case 1:
			strncpy(str,"%02X ",sizeof(str));
			tmpdatap_c = (unsigned char*)datap;
			break;
		case 2:
			strncpy(str,"%04X ",sizeof(str));
			tmpdatap_s = (unsigned short*)datap;
			break;
		case 4:
			strncpy(str,"%08X ",sizeof(str));
			tmpdatap_i = (unsigned int*)datap;
			break;
		default:
			mode = 1;
			strncpy(str,"%02X ",sizeof(str));
			tmpdatap_c = (unsigned char*)datap;
			break;
	}
	tmpdatap3 = (unsigned char*)datap;
	fprintf(fp,"    <%08X>:",0);
	rtncount = 0;
	for(count = 0; count < (len/mode) ; count++)
	{
		switch(mode)
		{
			case 1:
				fprintf(fp,str, *tmpdatap_c);
				tmpdatap_c++;
				tmpdatap2 = tmpdatap_c;
			break;
			case 2:
				fprintf(fp,str, (*tmpdatap_s));
				tmpdatap_s++;
				tmpdatap2 = (unsigned char*)tmpdatap_s;
				break;
			case 4:
				fprintf(fp,str, (*tmpdatap_i));
				tmpdatap_i++;
				tmpdatap2 = (unsigned char*)tmpdatap_i;
				break;
			default:
				fprintf(fp,str, *tmpdatap_c);
				tmpdatap_c++;
				tmpdatap2 = tmpdatap_c;
				break;
		}
		if(((count + 1) % (16/mode)) == 0)
		{
			fprintf(fp,"   ");
			for(count2 = 0; count2 < 16 ; count2++,tmpdatap3++)
			{
				if((*tmpdatap3 >= 0x21) && (*tmpdatap3 <= 0x7e))
				{
					fprintf(fp,"%c",*tmpdatap3);
				}
				else
				{
					fprintf(fp,".");
				}
			}
			fprintf(fp,"\n");
			rtncount = (unsigned int)((count + 1)*(mode));
			if((count + 1) < ( len/mode ))
			{
				fprintf(fp,"    <%08X>:",(unsigned int)((count + 1)*(mode)));
				rtncount = 0;
			}
		}
	}
	if((len % mode) > 0){
		if(rtncount > 0)
		{
			fprintf(fp,"    <%08X>:",rtncount);
		}
		for(count = 0; count < (len % mode) ; count++,tmpdatap2++)
		{
			fprintf(fp,"%02X", *tmpdatap2);
		}
		fprintf(fp," ");
		for(count = 0; count < (16 - (len % 16)) ; count++)
		{
			fprintf(fp,"  ");
			if(((count + 1) % mode) == 0)
			{
				fprintf(fp," ");
			}
		}
		fprintf(fp,"   ");
		for(count2 = 0; count2 < (len % mode) ; count2++,tmpdatap3++)
		{
			if((*tmpdatap3 >= 0x21) && (*tmpdatap3 <= 0x7e))
			{
				fprintf(fp,"%c",*tmpdatap3);
			}
			else
			{
				fprintf(fp,".");
			}
		}
		fprintf(fp,"\n");
	}
	else if((len % 16) > 0){
		for(count = 0; count < (16 - (len % 16)) ; count++)
		{
			fprintf(fp,"  ");
			if(((count + 1) % mode) == 0)
			{
				fprintf(fp," ");
			}
		}
		fprintf(fp,"   ");
		for(count2 = 0; count2 < (len % 16) ; count2++,tmpdatap3++)
		{
			if((*tmpdatap3 >= 0x21) && (*tmpdatap3 <= 0x7e))
			{
				fprintf(fp,"%c",*tmpdatap3);
			}
			else
			{
				fprintf(fp,".");
			}
		}
		fprintf(fp,"\n");
	}
	else{ ; }

	return;
};


/* 時刻差分計算関数 */
static void func_gettimeoffset(
	pcap_data_timeoffs_t* timeoffs,
	pcap_data_timeoffs_t* oldtimeoffs,
	pcap_data_timeoffs_t* getoffs
	)
{
	memset(getoffs,0,sizeof(*getoffs));
	if(((oldtimeoffs->sec) == 0) && ((oldtimeoffs->usec) == 0))
	{
		return;
	}
	if((timeoffs->usec) < (oldtimeoffs->usec))
	{
		getoffs->usec = 1000000;
		(getoffs->sec)--;
	}
	getoffs->usec += (timeoffs->usec) - (oldtimeoffs->usec);
	getoffs->sec += (timeoffs->sec) - (oldtimeoffs->sec);
	return ;
}

static void func_clean( int no_sig )
{	
	if(rruplog_pcap_p != NULL)
	{
		pcap_breakloop(rruplog_pcap_p);
		pcap_close(rruplog_pcap_p);
		rruplog_pcap_p = NULL;
	}
	if(no_sig != 0)
	{
		if(no_sig == SIGINT)
		{
			fprintf(stdout,"\ncommands terminated(%d)\n",no_sig);
			fflush(stdout);
		}
	}
	return;
}

/* debug message */
void func_gmsg(const char* format, ...)
{
	if(gmode == 0)
	{
		return;
	}
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
	return;
}

/* リアルタイム通信ログ表示関数(コマンド実行時に表示) */
unsigned int func_seqdump(FILE* fp,int len,char* datap,int orglen,int mode , int dspmode)
{
	unsigned int				index;
	int rtn = D_RRUPKTDUMP_TYPE_UNKNOWN;

	if(len < sizeof(pcap_rrupf_ethheader))
	{
		fprintf(fp,"ERROR len is %d  \n",len);
		return (rtn);
	}
	rtn = D_RRUPKTDUMP_TYPE_EXTERNL;
	switch(mode){
		case 0:
			if(len > sizeof(pcap_rrupf_ethheader)){
				index = func_head_dsp(fp,(pcap_data_timeoffs_t* )NULL,datap);
				rtn = func_userdata_dsp(fp,(pcap_rrupf_userheader* )datap,len,index);
			}else{
				return(D_RRUPKTDUMP_TYPE_UNKNOWN);
			}
			break;
		case 1:
			func_datadsp(fp,len,datap,D_RRUPKTDSP_MSIZE,dspmode);
			break;
		case 2:
			func_datadsp(fp,len,datap,len,dspmode);
			break;
		default:
			return(D_RRUPKTDUMP_TYPE_UNKNOWN);
			break;
	}
	fprintf(fp,"\n");
	return (rtn);
}

/* パケットダンプコールバッグ関数 */
static int func_rrupacketcap_callback(
	 unsigned char* drlp ,
	 const struct  pcap_pkthdr* rph ,
	 const unsigned char* rpmsg,
	 unsigned int cons
)
{ 
	unsigned int			index;
	int						len;

	if(rpmsg == NULL)
	{
		func_gmsg("addr error \n");
		return -1;
	}
	/* レングス異常 */
	len = rph->caplen;
	if(len < sizeof(pcap_rrupf_ethheader))
	{
		func_gmsg("length mismatch \n");
		return 0;
	}
	/* 出力先がファイルの場合は、pcapで出力する		*/
	/* コンソールの場合は標準出力(stdout)に出力する	*/
	if(drlp != NULL)
	{
		pcap_dump( drlp, rph, rpmsg);
		pcap_dump_flush((pcap_dumper_t*)drlp);
	}
	else
	{
		if(cons == 2){
			func_mirror_send( len ,(char*)rpmsg );
		}else{
			index  = func_head_dsp(stdout,(pcap_data_timeoffs_t*)&(rph->ts),(char*)rpmsg);
			if(len > sizeof(pcap_rrupf_ethheader))
			{
				(void)func_userdata_dsp(stdout,(pcap_rrupf_userheader*)rpmsg,len,index);
				fprintf(stdout,"\n");
			}
			fflush(stdout);
		}
	}
	return 1;
}

/* パケットダンプ関数 */
void func_rrupacketcap(
	char* ifname,
	const char* str,
	const int str_len,
	int pmaxcount,
	int pmaxsize,
	int pfmaxcount,
	char* ipfilt
	)
{
	pcap_dumper_t* drlp;
	unsigned char* butp;
	struct pcap_pkthdr* rph;
	char rpbut[65536];	/* max 4K */
	char  str_old[384];
	int rtn;
	unsigned int count;
	unsigned int fcount;
	unsigned int gcount;
	unsigned int tcount;
	unsigned int rtcount;
	int len_old = 0;
	FILE* fp;
	FILE* tmpfp = NULL;
	int filelen;
	char* buff_p = NULL;
	unsigned int cons;
	struct bpf_program ffp;
	int    filesize;

	if(pmaxcount <= 0)
	{
		pmaxcount = 1024;
	}
	if(
		(pmaxsize < sizeof(pcap_rrupf_msg_t)) ||
		(pmaxsize >= sizeof(rpbut))
	)
	{
		pmaxsize = sizeof(rpbut) - 1;
	}
	if(pfmaxcount <= 0)
	{
		pfmaxcount = 1;
	}
	if((str_len <= 0) || (str_len > (sizeof(str_old) - 13)))
	{
		printf("file name too long %d\n",str_len);
		return;
	}
	func_gmsg("pmaxcount %u , pmaxsize %u , pfmaxcount %u str_len %u \n",
		pmaxcount,pmaxsize,pfmaxcount,str_len
	);
	rruplog_pcap_p = pcap_open_live(
		ifname,			/* IF name */
		pmaxsize,		/* max capture size */
		0,				/* promisscas mode */
		0,				/* buffer time out msec */
		rpbut);			/* buffer address */

	if(rruplog_pcap_p == NULL)
	{
		printf("%s open error\n",ifname);
		return;
	}
	if(ipfilt != NULL)
	{
		if (pcap_compile(rruplog_pcap_p, &ffp, ipfilt, 0, (bpf_u_int32)0xFF000000) == -1)
		{
			printf("filter compile error %s\n",ipfilt);
			func_clean(0);
			return;
		}
		if (pcap_setfilter(rruplog_pcap_p, &ffp) == -1)
		{
			pcap_freecode(&ffp);
			printf("filter setting error\n");
			func_clean(0);
			return;
		}
		pcap_freecode(&ffp);
	}
	/* console mode */
	if((str[0] == '-')&&(str[1] == '\0'))
	{
		func_gmsg("console dump \n");
		cons = 1;
	}
	else
	{
		if((rtn = strncmp(str,"mirror",strlen(str))) == 0){
			func_gmsg("mirror dump \n");
			cons = 2;
		}
		else{
			len_old = snprintf(str_old,sizeof(str_old),"%s.old",str);
			cons = 0;
		}
	}
	rtcount = 0;
	for(count = 0 ; ; count++)
	{
		for(fcount = 0 ; fcount < pfmaxcount ; fcount++)
		{
			tcount = 0;
			filesize = 0;

			if((cons == 1) || (cons == 2))
			{
				drlp = NULL;
			}
			else
			{
				snprintf(&str_old[len_old],(int)(sizeof(str_old) - len_old),"%u",fcount);
				func_gmsg("%s dump open \n",str);
				/* すでにファイルが存在するかどうかチェックする */
				if (( tmpfp != NULL ) || (( tmpfp = fopen(str, "rb") ) == NULL ))
				{
					tmpfp = (FILE*)-1;
					func_gmsg("Noting %s file \n",str);
					if((drlp = pcap_dump_open(rruplog_pcap_p, str)) == NULL)
					{
						printf("packet cap file open error \n");
						rtn = -1;
						break;
					}
					/* ヘッダ分加算 */
					filesize = sizeof(pcap_header_t);
				}
				else
				{
					fclose(tmpfp);
					func_gmsg("%s file \n",str);
					if( ( tmpfp = fopen(str,"ab") ) == NULL )
					{
						tmpfp = (FILE*)-1;
						printf("packet cap file reopen error1 \n");
						rtn = -1;
						break;
					}
					filesize = ftell(tmpfp);
					if( ( drlp = pcap_dump_fopen(rruplog_pcap_p,tmpfp) ) == NULL )
					{
						fclose(tmpfp);
						printf("packet cap file reopen error2 \n");
						rtn = -1;
						break;
					}
				}
			}
			/* capture start */
			while((rtn = pcap_next_ex(rruplog_pcap_p,&rph,(const unsigned char **)&butp)) >= 0)
			{
				if( rrupf_clean_flg > 0)
				{
					func_gmsg("packet file clean \n");
					rtn = 0;
					break;
				}
				if( ( rtn == 0) || ( rrupf_stop_flg > 0 ))
				{
					/* time out */
					func_gmsg("Time out %d\n",rrupf_stop_flg);
					continue;
				}
				rtcount += rtn;
				if((gcount = func_rrupacketcap_callback((unsigned char*)drlp,rph,butp,cons)) < 0)
				{
					/* ng */
					func_gmsg("cap_callback error %08x \n",rtn);
					rtn = gcount;
					break;
				}
				tcount += gcount;
				if(gcount > 0)
				{
					if(drlp != NULL)
					{
						filesize += rph->caplen;
					}
				}
				if((tcount >= pmaxcount) || ( filesize >= ((pmaxcount - 1) * D_RRUPKTDUMP_MAXFILESIZE)))
				{
					/* full */
					func_gmsg("packet count full %u %u %u \n",tcount,rtcount,filesize);
					rtn = 0;
					break;
				}
			}
			if(drlp != NULL)
			{
				func_gmsg("%s dump close %u \n",str,tcount);
				pcap_dump_flush(drlp);
				pcap_dump_close(drlp);
			}
			if(rtn < 0)
			{
				func_gmsg("return is %d \n",rtn);
				break;
			}
			if((cons == 1) || (cons == 2))
			{
				continue;
			}
			if((fp = fopen(str,"rb")) == NULL)
			{
				rtn = -10;
				break;
			}
			fseek(fp, 0, SEEK_END); 
			filelen = ftell(fp); 
			fseek(fp, 0, SEEK_SET); 
			if((buff_p = malloc(filelen )) == NULL)
			{
				rtn = -11;
				fclose(fp);
				break;
			}
			if((fread(buff_p,filelen,1,fp)) < 1)
			{
				rtn = -12;
				fclose(fp);
				free(buff_p);
				break;
			}
			fclose(fp);
			if((fp = fopen(str_old,"wb")) == NULL)
			{
				rtn = -13;
				free(buff_p);
				break;
			}
			if((fwrite(buff_p,filelen,1,fp)) < 1)
			{
				rtn = -14;
				fclose(fp);
				free(buff_p);
				break;
			}
			fclose(fp);
			free(buff_p);
			if( rrupf_clean_flg > 0 )
			{
				rrupf_clean_flg = 0;
				remove(str_old);
				func_gmsg("%s clean\n",str_old);
				break;
			}
		}
		if(rtn < 0)
		{
			func_gmsg("capture end %d \n",rtn);
			break;
		}
	}
	func_clean(0);
	func_gmsg("total count %u / last packet cap count %u \n" ,rtcount, tcount);
	return;
}

static void func_signal_handler(int no_sig)
{
	switch(no_sig)
	{
		case SIGUSR1:
			if( rrupf_stop_flg > 0 )
			{
				rrupf_stop_flg = 0;	/* start */
			}
			else
			{
				rrupf_stop_flg = 1;	/* stop */
			}
			break;
		case SIGUSR2:
			rrupf_clean_flg = 1;
			break;
		default:
			func_clean(no_sig);
			exit(EXIT_FAILURE);	/* 強制終了*/
			break;
	}
	return;
}

static void func_signal( void )
{
	signal(SIGSEGV,func_signal_handler);
	signal(SIGBUS,func_signal_handler);
	signal(SIGFPE,func_signal_handler);
	signal(SIGHUP,func_signal_handler);
	signal(SIGILL,func_signal_handler);
	signal(SIGINT ,func_signal_handler);
	signal(SIGTERM,func_signal_handler);
	signal(SIGABRT,func_signal_handler);
	signal(SIGTSTP,func_signal_handler);
	signal(SIGQUIT,func_signal_handler);
	signal(SIGURG ,func_signal_handler);
	signal(SIGPIPE,func_signal_handler);
	signal(SIGUSR1,func_signal_handler);
	signal(SIGUSR2,func_signal_handler);
	return;
}

void func_usage(char* argv[])
{
	printf("Usage: %s [-n count] [-N name ] [-f count] [-e ifname] [-F ipfilt] [-p size ] [-g] [ -c | -d ] [packet file name] \n",argv[0]);
	printf("\n");
	printf("RE Packet file dump command \n");
	printf("\n");
	printf(" -n dump count \n");
	printf(" -f dump file count. This option can be used in combination with \"-c\" option\n");
	printf(" -F IP filter. This option can be used in combination with \"-c\" option\n");
	printf("    e.g.)If you want to get assert log only, please add -I \"dst 127.1.1\".\n");
	func_help_fil_list();
	printf(" -e Network interface name (e.g. lo). This option can be used in combination with \"-c\" option\n");
	printf("\n");
	printf(" -d packet file dump to console \n");
	printf(" -b data disp byte size (1 or 2 or 4 or ...). This option can be used in combination with \"-d\" option\n");
	printf(" -p packet max size. This option can be used in combination with \"-c\" option\n");
	printf(" -c lo packet capture file dump .\n *** \"-c -\" : all packets is displayed to console *** \n *** \"-c mirror\" : all packets is mirrored to eth0 *** \n");
	printf(" -l long data dump and .seq file create \n");
	printf(" -m middle data dump and .seq file create < default > \n");
	printf(" -s short data dump and .seq file create \n");
	printf(" ** If you didn't set option (-c or -d ) , dump out seqfile to same path \n");
	printf("\n");
	printf(" -N command name set \n");
	printf(" -t bpf log test \n");
	printf(" -h help \n");
	printf(" -g debug information display mode \n");
	printf(" -v command version \n");
	return;
}

/* rrupfdumpコマンドのメイン処理 */
int main (int argc , char* argv[])
{
	FILE* fp;
	FILE* seqfp;
	char  str[256];
	char  str_tmp[256];
	int  str_len = 0;
	char* bufp;
	pcap_header_t fhead;
	pcap_data_header_t fdatahead;
	int filelen;
	int headlen;
	int dataheadlen;
	int datalen;
	int len;
	struct tm *gmtimep;
	pcap_data_timeoffs_t oldoffset;
	pcap_data_timeoffs_t offset;
	pcap_data_timeoffs_t getoffset;
	unsigned int count;
	unsigned int type;
	int rtn;
	int mode;
	int maxcount = -1;
	int dspmode = 4;
	int packetsize = 0;
	unsigned int typecount[3];
	int rmode = 0;
	char ipfilt[512];
	int  ipfilt_len = 0;
	mode = 1;
	int fmaxcount = 0;
	char ifname[16];
	int iflen;
	char procname[16];
	time_t gtime_ofs;
	FILE*	runfd;
	char	pidNo[256];
	int		killpid;
	char*	runProcName;

	union sigval	value;

	if( argc < 2)
	{
		func_usage(argv);
		return 1;
	}

	func_signal();

	memset(str,0,sizeof(str));
	memset(str_tmp,0,sizeof(str_tmp));
	memset(ipfilt,0,sizeof(ipfilt));

	strncpy(ifname,"lo",sizeof(ifname) - 1);

	/* コマンドoptionを解析開始									 */
	/* オプションが*:のものは、パラメータを期待するoptionである	 */
	/* e.g)-cオプションの場合 -c ./test.pcap のようになる		 */
	while((rtn = getopt(argc,argv,"hdgvtrkRKn:c:e:f:F:N:C:s:l:m:p:b:")) != -1)
	{
		switch(rtn)
		{
			case 's':			/* リアルタイムログのダンプ時に小サイズでダンプ */
				if((str_len = snprintf(str,sizeof(str),"%s",optarg)) <= 0)
				{
					printf("%c option input file error \n",rtn);
					func_usage(argv);
					return 1;
				}
				mode = 0;
				break;
			case 'm':			/* リアルタイムログのダンプ時に中サイズでダンプ */
				if((str_len = snprintf(str,sizeof(str),"%s",optarg)) <= 0)
				{
					printf("%c option input file error \n",rtn);
					func_usage(argv);
					return 1;
				}
				mode = 1;
				break;
			case 'l':			/* リアルタイムログのダンプ時に大サイズでダンプ */
				if((str_len = snprintf(str,sizeof(str),"%s",optarg)) <= 0)
				{
					printf("%c option input file error \n",rtn);
					func_usage(argv);
					return 1;
				}
				mode = 2;
				break;
			case 'd':			/* pcapファイルのダンプをコンソールに出力する */
				rmode = 1;
				break;
			case 'c':			/* キャプチャーモード開始 */
				if((str_len = snprintf(str,sizeof(str),"%s",optarg)) <= 0)
				{
					printf("%c option input file error \n",rtn);
					func_usage(argv);
					return 1;
				}
				mode = 3;
				break;
			case 'f':			/* キャプチャ時に何ファイルでローテーションするか */
				if((fmaxcount = strtol(optarg, NULL, 10)) == 0)
				{
					printf("%c option input param error \n",rtn);
					func_usage(argv);
					return 1;
				}
				fmaxcount = strtol(optarg, NULL, 10);
				break;
			case 'F':
				if((ipfilt_len = snprintf(ipfilt,sizeof(ipfilt),"%s",optarg)) <= 0)
				{
					printf("%c option input file error \n",rtn);
					func_usage(argv);
					return 1;
				}
				break;
			case 'e':			/* ネットワークIF名を指定 */
				if((iflen = strlen(optarg)) > (sizeof(ifname) - 1))
				{
					printf("%c option input param size error \n",rtn);
					func_usage(argv);
					return 1;
				}
				if((iflen = snprintf(ifname,sizeof(ifname) - 1,"%s",optarg)) <= 0)
				{
					printf("%c option input param error \n",rtn);
					func_usage(argv);
					return 1;
				}
				break;
			case 'n':
				if((maxcount = strtol(optarg, NULL, 10)) == 0)
				{
					printf("%c option input param error \n",rtn);
					func_usage(argv);
					return 1;
				}
				maxcount = strtol(optarg, NULL, 10);
				break;
			case 'b':
				if((dspmode = strtol(optarg, NULL, 10)) == 0)
				{
					printf("%c option input param error \n",rtn);
					func_usage(argv);
					return 1;
				}
				break;
			case 'p':
				if((packetsize = strtol(optarg, NULL, 10)) == 0)
				{
					printf("%c option input param error \n",rtn);
					func_usage(argv);
					return 1;
				}
				break;
			case 'v':
				/* omake */
				printf("command version is v5.0.T.T \n");
				return 0;
				break;
			case 'r':
			case 'k':
			case 'R':
			case 'K':
				if((rtn == 'r')||(rtn == 'k')){
					runProcName = "/var/run/cmd_planedump_m.pid";
				}else{
					runProcName = "/var/run/cmd_planedump_s.pid";
				}
				runfd = fopen(runProcName,"r");
				if(runfd != NULL){
					memset(pidNo,0,sizeof(pidNo));
					if( fgets(pidNo,sizeof(pidNo) - 1,runfd) != NULL){

						pidNo[strlen(pidNo) - 1] = '\0';	/* 改行コード削除 */
						sscanf(pidNo,"%d",&killpid);

						value.sival_int = 0;
						if ( sigqueue(killpid,0,value) == 0){
							sigqueue(killpid,SIGKILL,value);
							sleep(1);
						}
					}
					fclose(runfd);
				}
				if((rtn == 'r')||(rtn == 'R')){
					runfd = fopen(runProcName,"w");
					if(runfd != NULL){
						fprintf(runfd,"%d\n",getpid());
						fflush(runfd);
						fclose(runfd);
					}
				}else if((rtn == 'k')||(rtn == 'K')){
					return 0;
				}
				break;
			case 'g':
				gmode = 1;
				break;
			case 'h':
				func_usage(argv);
				return 0;
				break;
			case 'N':
				memset(procname,0,sizeof(procname));
				if( snprintf(procname,sizeof(procname),"%s",optarg) <= 0)
				{
					printf("%c option input error \n",rtn);
					func_usage(argv);
					return 1;
				}
				prctl(PR_SET_NAME, procname , 0, 0, 0);
				break;
			default:
				printf("found unsupported option \n");
				func_usage(argv);
				return 0;
				break;
		}
	}
	if(optind < argc)
	{
		if((str_len = snprintf(str,sizeof(str),"%s",argv[optind])) <= 0)
		{
			func_usage(argv);
			return 1;
		}
		mode = 1;
	}
	if(strlen(str) <= 0)
	{
		printf("not found packet file name \n");
		func_usage(argv);
		return 1;
	}
	if(mode == 3)
	{
		func_gmsg("RRU Packet log capture start \n");
		if( ipfilt_len > 0)
		{
			func_gmsg(" CapFilename:%s IF:\"%s\" MaxPacket count:%d,size:%d,filenum:%d ipfilt:%s\n",str,ifname,maxcount,packetsize,fmaxcount,ipfilt);
			func_rrupacketcap(ifname,str,str_len,maxcount,packetsize,fmaxcount,ipfilt);
		}
		else
		{
			func_gmsg(" CapFilename:%s IF:\"%s\" MaxPacket count:%d,size:%d,filenum:%d \n",str,ifname,maxcount,packetsize,fmaxcount);
			func_rrupacketcap(ifname,str,str_len,maxcount,packetsize,fmaxcount,NULL);
		}
		func_gmsg("RRU Packet capture complete \n");
		return 0;
	}

	func_gmsg("RRU Packet dump start\n");
	func_gmsg(" Mode %d dumpcount %d console %d dspmode %d \n",mode,maxcount,rmode,dspmode);
	snprintf(str_tmp,sizeof(str_tmp),"%s",str);
	func_gmsg("%s open \"rb\" \n",str);
	fp = fopen(str,"rb");
	if(fp == NULL)
	{
		func_gmsg("packet file open error\n");
		return 1;
	}
	
	fseek(fp, 0, SEEK_END); 
	filelen = ftell(fp); 
	fseek(fp, 0, SEEK_SET); 
	func_gmsg("%s file size is %d(%08X) bytes \n",str,filelen,filelen);
	if(filelen < sizeof(fhead))
	{
		func_gmsg("packet file size error %d \n",filelen);
		fclose(fp);
		return 1;
	}
	func_gmsg("%s header read \n",str);
	headlen = fread(&fhead , sizeof(fhead), 1, fp); 
	if(headlen < 1)
	{
		printf("Not packet head file \n");
		fclose(fp);
		return 1;
	}
	func_gmsg("magic_no is 0x%08X \n",fhead.magic_no);
	func_gmsg("linktype is 0x%08X \n",fhead.linktype);
	func_gmsg("Ver is      0x%04X%04X \n",fhead.version_major_no,fhead.version_minor_no);
	func_gmsg("thiszone is 0x%08X \n",fhead.thiszone);
	func_gmsg("sigfigs is  0x%08X \n",fhead.sigfigs);
	func_gmsg("snaplen is  0x%08X(%u) \n",fhead.snaplen,fhead.snaplen);

	if(fhead.magic_no != 0xA1B2C3D4)
	{
		printf("this packet file is unsupported magic_no %08X \n",fhead.magic_no);
		fclose(fp);
		return 1;
	}
	/* linx or ether */
	if((fhead.linktype != 113) && (fhead.linktype != 147) && (fhead.linktype != 1))
	{
		printf("this packet file is unsupported linktype %08X \n",fhead.linktype);
		fclose(fp);
		return 1;
	}
	snprintf(&str[str_len],(int)(sizeof(str) - str_len) ,".%u.seq",getpid());
	func_gmsg("%s open \"w\" \n",str);
	if(rmode != 0)
	{
		seqfp = stdout;
	}
	else
	{
		seqfp = fopen(str,"w");
		if(seqfp == NULL)
		{
			printf("packet seq file open error\n");
			fclose(fp);
			return 1;
		}
	}
	filelen -= sizeof(fhead);
	if(filelen < sizeof(fdatahead))
	{
		func_gmsg("Not packet data file %d \n",filelen);
		fclose(fp);
		if(rmode == 0)
		{
			fclose(seqfp);
		}
		return 1;
	}
	memset(&oldoffset,0,sizeof(oldoffset));
	count = 0;
	memset(typecount,0,sizeof(typecount));

	func_gmsg("last length %d(0x%08X) byte \n",filelen,filelen);

	while(filelen > 0)
	{
		dataheadlen = fread(&fdatahead , sizeof(fdatahead), 1, fp);
		if(dataheadlen < 1)
		{
			func_gmsg("packet no.%u datahead read error \n",count);
			break;
		}
		offset.sec = fdatahead.ts.tv_sec;
		offset.usec = fdatahead.ts.tv_usec;
		func_gettimeoffset(&offset,&oldoffset,&getoffset);
		oldoffset.sec = fdatahead.ts.tv_sec;
		oldoffset.usec = fdatahead.ts.tv_usec;
		filelen -= sizeof(fdatahead);
		datalen = fdatahead.incl_len;
		if(filelen < datalen)
		{
			func_gmsg("packet no.%u datalen error \n",count);
			break;
		}
		if(datalen == 0)
		{
			filelen += sizeof(fdatahead);
			if( filelen > sizeof(fhead) )
			{
				func_gmsg("packet no.%u recovery \n");
				if( fseek(fp, - sizeof(fdatahead), SEEK_CUR) != 0)
				{
					func_gmsg("packet no.%u seel error \n");
					break;
				}
				if((headlen = fread(&fhead , sizeof(fhead), 1, fp)) < 1)
				{
					func_gmsg("packet no.%u read error \n");
					break;
				}
				filelen -= sizeof(fhead);
				memset(&offset,0,sizeof(offset));
				memset(&oldoffset,0,sizeof(oldoffset));
				fprintf(seqfp,"LOG Recovery ******************************************************************************************************************** \n");
				continue;
			}
			func_gmsg("packet no.%u datalen is 0 \n",count);
			break;
		}
		bufp = malloc(datalen);
		if(bufp == NULL)
		{
			func_gmsg("packet no.%u malloc null \n",count);
			break;
		}
		len = fread(bufp , datalen, 1, fp);
		if(len < 1)
		{
			free(bufp);
			func_gmsg("packet no.%u data read error \n",count);
			break;
		}
		gtime_ofs = (time_t)fdatahead.ts.tv_sec;
		gmtimep = gmtime(&gtime_ofs);
		fprintf(seqfp,"%04d/%02d/%02d %02d:%02d:%02d.%06d(%u.%06d):",
			gmtimep->tm_year + 1900,
			gmtimep->tm_mon+1,
			gmtimep->tm_mday,
			gmtimep->tm_hour,
			gmtimep->tm_min,
			gmtimep->tm_sec,
			fdatahead.ts.tv_usec,
			getoffset.sec,
			getoffset.usec
		);
		func_gmsg("packet no.%u size %u func_seqdump call->",count + 1 ,datalen);
		type = func_seqdump(seqfp,datalen,bufp,fdatahead.orig_len,mode,dspmode);
		func_gmsg("packet type was %u \n",type);
		free(bufp);
		filelen -= datalen;
		count++;
		typecount[type]++;
		if(maxcount <= count)
		{
			func_gmsg("packet count over %u %u \n",maxcount,count);
			break;
		}
	}
	fclose(fp);
	func_gmsg("Packet counter total : %u / external(1) : %u / internal(2) : %u / unknown(0) %u \n",
			count,
			typecount[1],
			typecount[2],
			typecount[0]);

	fflush(seqfp);
	if(rmode == 0)
	{
		fclose(seqfp);
		printf("Created packet dump file. File name is %s \n",str);
	}
	func_gmsg("RRU Packet dump complete \n");
	return 0;
}
