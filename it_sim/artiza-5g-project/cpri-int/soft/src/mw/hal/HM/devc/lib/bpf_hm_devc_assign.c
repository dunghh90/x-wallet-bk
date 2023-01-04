/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_hm_devc_assign.c
 *  @brief  hm devc Assgin.
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2016
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                 */
/********************************************************************************************************************/
#include "bpf_l_com.h"
#include "bpf_l_hm_devc.h"
#include "bpf_i_hm_devc.h"
#include <limits.h>

/** @addtogroup BPF_HM_DEVC
 * @{ */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Device Control Assign.
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_HM_DEVC_INTER_OK    normal end
 *  @retval BPF_HM_DEVC_INTER_ERR_SYS 
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 */
/********************************************************************************************************************/
int bpf_hm_devc_assign(
        unsigned int process_desc)
{   
    key_t ipc_key;
    int shmid;
    int result;
    
    /* 既にアタッチ済みの場合は処理終了 */
    if(di_devc_assign_flag == DEVC_FLG_ON){
        return BPF_HM_DEVC_INTER_OK;
    }
    
    /********************************************
    * IPC key create processing.                *
    ********************************************/
    /* IPCKEY 生成マクロ  */
    ipc_key  = BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_DEVC,0,0);
    
    /********************************************
    * shared memory get processing.             *
    ********************************************/
    shmid = shmget( ipc_key,
                    sizeof(t_bpf_hm_devc_data),
                    BPF_COM_SEMFLG);
    if( shmid == BPF_HM_DEVC_SYS_NG ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget ipckey=%X size=%d errno=%d",
                            ipc_key,sizeof(t_bpf_hm_devc_data),errno);
        return BPF_HM_DEVC_INTER_ERR_SYS;
    }
        
    /********************************************
    * shared memory attach processing.          *
    ********************************************/
    di_devc_data_p = (t_bpf_hm_devc_data *)shmat(shmid,0,0);
    if(di_devc_data_p == (void *)BPF_HM_DEVC_SYS_NG ){ /* pgr0203 */
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmat ipckey=%X size=%d errno=%d shmid=%d",
                            ipc_key,sizeof(t_bpf_hm_devc_data),errno,shmid);
        return BPF_HM_DEVC_INTER_ERR_SYS;
    }
	
	/* 本関数がCALLされる場合、既にmutexは取得されているので	*/
	/* bpf_hm_devc_open内でEEPROMの関数にアクセスしても問題なし	*/
	
    /* デバイスのファイルオープン */
    result = bpf_hm_devc_open();
   if(result != BPF_HM_DEVC_INTER_OK){
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"bpf_hm_devc_open NG");
        
       return BPF_HM_DEVC_INTER_ERR_SYS;
    }
                
    /* アサインフラグ */
    di_devc_assign_flag = DEVC_FLG_ON;

    
    return BPF_HM_DEVC_INTER_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Device Control Assign Init
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_HM_DEVC_INTER_OK      normal end
 *  @retval BPF_HM_DEVC_INTER_ERR_SYS 
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_init(
        unsigned int process_desc)
{   
    key_t ipc_key;
    int shmid;
    int result;
    pthread_mutexattr_t mattr;
    int loop_cnt;
        
    /********************************************
    * IPC key create processing.                *
    ********************************************/
    /* IPCKEY 生成マクロ  */
    ipc_key  = BPF_COM_CREATE_INTERNAL_IPCKEY(BPF_COM_IPCKEY_MASK_DEVC,0,0);

    
    /********************************************
    * shared memory get processing.             *
    ********************************************/
    shmid = shmget( ipc_key,
                    sizeof(t_bpf_hm_devc_data),
                    BPF_COM_SEMFLG);
    if( shmid == BPF_HM_DEVC_SYS_NG ){

        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget ipckey=%X size=%d errno=%d",
                            ipc_key,sizeof(t_bpf_hm_devc_data),errno);
        return BPF_HM_DEVC_INTER_ERR_SYS;
    }
        
    /********************************************
    * shared memory attach processing.          *
    ********************************************/
    di_devc_data_p = (t_bpf_hm_devc_data *)shmat(shmid,0,0);
    if(di_devc_data_p == (void *)BPF_HM_DEVC_SYS_NG ){ /* pgr0203 */
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmat ipckey=%X size=%d errno=%d shmid=%d",
                            ipc_key,sizeof(t_bpf_hm_devc_data),errno,shmid);
        return BPF_HM_DEVC_INTER_ERR_SYS;
    }
    
    /* mutexオブジェクトの初期化 */
	/* コメント追加 : タイプは設定しないこと													*/
	/* 理由：プロセス死亡時にmutex全解除を行いたいが、タイプを設定(特にErrCheck)していると		*/
	/*		 mutexの解除が行えなくなるため。(mutex lockを掛けたスレッドでしかunlockできない)	*/
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
	
	/* I2C-0(eeprom)のmutexだけ先に取得する(装置種別を取得してからデバイスのファイルオープン処理を行うため)	*/
	/* I2C0だけ取得		*/
	pthread_mutex_init( &(di_devc_data_p->info[BPF_HM_DEVC_I2C_0].mutex), &mattr);
	
    /* デバイスのファイルオープン */
    result = bpf_hm_devc_open();
    if(result != BPF_HM_DEVC_INTER_OK){
        /* LOG(ERROR) */
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"bpf_hm_devc_open NG");

        return BPF_HM_DEVC_INTER_ERR_SYS;
    }
    
	/* mutex取得	*/
    for(loop_cnt = 0; loop_cnt < (sizeof(di_devc_data_fd)/sizeof(t_bpf_hm_devc_fd_info)) ; loop_cnt++ )
    {
    	/* I2C-0のmutexは既に取得済みのためここでは取得しない	*/
    	/* また、無効なDeviceについても取得しない				*/
    	if( ( BPF_HM_DEVC_I2C_0 == loop_cnt ) ||
    		( 0 == di_devc_data_fd.info[loop_cnt].validflg ) )
    	{
    		continue;
    	}
        pthread_mutex_init( &(di_devc_data_p->info[loop_cnt].mutex), &mattr);
    }
    
    di_devc_data_p->force_access = 0;
    di_devc_data_p->force_pid = 0;
    di_devc_data_p->force_tid = 0;
    
    /* QSPI状態情報初期化 */
    di_devc_data_p->qspi_side = 0xFFFF;
    di_devc_data_p->qspi_uplw = 0xFF;
	
    /* SPI(RFIC)制御状態 */
    di_devc_data_p->spi_rfic_ctrl = 0xFFFFFFFF;
	
	/* FPGA状態/PL-CONFIG状態を設定する	*/
	/* FPGAアクセス可否はPCIe設定完了後に可にする		*/
	/* PL-CONFIGはSYSロジックリセット解除後に可能にする	*/
	di_devc_data_p->fpga_access = DEVC_FLG_ON;	/* 5Gでは常にON	*/
	di_devc_data_p->pl_config   = DEVC_FLG_OFF;	/* 5GではOFF	*/
	di_devc_data_p->wdt_set_axi  = DEVC_FLG_OFF;	/* WDT初期化フラグ(AXI)OFF					*/
	di_devc_data_p->wdt_set_swdt = DEVC_FLG_OFF;	/* WDT初期化フラグ(SWDT)OFF					*/

/* FLASH EOL対応 start */
	/* Flash種別情報初期化 */
	di_devc_data_p->flash_kind = 0;		/* device 0 default 0 (Micron設定) */

	/*DEBUG用LOG*/
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"flash_kind default Set");
/* FLASH EOL対応 end */
	
    /* アサインフラグ */
    di_devc_assign_flag = DEVC_FLG_ON;

    return BPF_HM_DEVC_INTER_OK;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Device Control Device open
 *  @note   BPF internal function. 
 *  @return result code
 *  @retval BPF_HM_DEVC_INTER_OK      normal end
 *  @retval BPF_HM_DEVC_INTER_ERR_SYS 
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Kuwamura   docomo SLC/1CHIP対応
 *  @date   2015/05/21 ALPHA)Murakami   M-RRU-ZSYS-01584対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_open()
{   

    int fd;       /* ファイルディスクリプタ   */
    void* map;     /* マップディスクリプタ     */
    int loop_cnt;
    int ret;
    
    /* EEPROMから装置種別を取得して、そのDeviceが有効か無効かを設定する	*/
    ret = bpf_hm_devc_data_ini();
    if( ret != BPF_HM_DEVC_INTER_OK )
    {
    	/* INITでETHだけ起動して終了する	*/
        return BPF_HM_DEVC_INTER_ERR_ASSIGN;
    }
    
    for(loop_cnt = 0;loop_cnt< (sizeof(di_devc_data_fd)/sizeof(t_bpf_hm_devc_fd_info)) ;loop_cnt++)
    {
        /* 本装置にとって無効なDeviceだった場合はOPENしない	*/
        /* また、Device名がNULLのものは開けないのでcontinue	*/
        if( ( 0 == di_devc_data_fd.info[loop_cnt].validflg ) ||
            ( NULL == di_devc_data_fd.info[loop_cnt].filename ) )
        {
            continue;
        }
        
        /* QSPIフラッシュの場合はファイルオープンはしない。アクセスの度に実施		*/
        /* I2C-0はbpf_hm_devc_data_iniで既にOPENしているのでここではOPENしない		*/
        if ( (loop_cnt == BPF_HM_DEVC_QSPI_FLASH ) || (loop_cnt == BPF_HM_DEVC_I2C_0) )
        {
            continue;
        }
        
        fd = open(di_devc_data_fd.info[loop_cnt].filename, O_RDWR);
        if (fd == BPF_HM_DEVC_SYS_NG)
        {
            /* LOG(ERROR) */
            bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"open NG Name = %s",di_devc_data_fd.info[loop_cnt].filename);
            /* エラーを出力し継続する */
            
        }
        
        di_devc_data_fd.info[loop_cnt].fd = fd;
        
        /* レジスタアクセス（GPIO含む）の場合はmappする必要があるので設定する */
        if (di_devc_data_fd.info[loop_cnt].filesize > 0)
        {
            /* mmap the UIO device */
            map = mmap(NULL, di_devc_data_fd.info[loop_cnt].filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

            /* mmap結果を判定する */
            if (map == (void*)BPF_HM_DEVC_SYS_NG)	/* pgr0203 */
            {

                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "mmap. NG Name = %s file_descriptor = %d, error = %d",
                    di_devc_data_fd.info[loop_cnt].filename ,di_devc_data_fd.info[loop_cnt].fd,errno);

                /* mmap失敗 */
                return BPF_HM_DEVC_INTER_OK;
            }

            di_devc_data_fd.info[loop_cnt].map = (char*)map;
        }
    }
    
    return BPF_HM_DEVC_INTER_OK;

}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  bpf_hm_devc_data_get_uioval
 *  @note   BPF internal function. 
 *  @return value
 *  @date   2018/3/20 Taniguchi create
 */
/********************************************************************************************************************/
unsigned int bpf_hm_devc_data_get_uioval(char* uio_name)
{
	char	mapval[32];
	unsigned long val;
	FILE*	fp;
	size_t	size;

	if((fp = fopen(uio_name,"r")) == NULL){
		bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"UIO FileOpenERR:%s",uio_name);
		return 0xFFFFFFFF;
	}
	memset(mapval,0,sizeof(mapval));
	if(( size = fread(mapval,1,sizeof(mapval),fp)) < 1){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"UIO ReadErr:%ld",size);
		fclose(fp);
		return 0xFFFFFFFF;
	}
	
	val = strtoul(mapval, NULL, 16);
	if ((ULONG_MAX == val) && (ERANGE == errno)) {
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"UIO strtoulErr:%d",errno);
		fclose(fp);
		return 0xFFFFFFFF;
	}
	if((val & 0xFFFFFFFF00000000) != 0){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"UIO RangeOver 0x%lx",val);
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"File:%s",uio_name);
		val = 0xFFFFFFFF;
	}
	fclose(fp);
	return (unsigned int)val;
}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  bpf_hm_devc_data_get_dtsname
 *  @note   BPF internal function. 
 *  @return value
 *  @date   2018/3/20 Taniguchi create
 */
/********************************************************************************************************************/
unsigned int bpf_hm_devc_data_get_dtsname_check(char* uio_name,const char* check_name,int len)
{
	char	mapval[32];
	FILE*	fp;
	size_t	size;
	unsigned int check_result = 0xFFFFFFFF;

	if((fp = fopen(uio_name,"r")) == NULL){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"UIO FileOpenERR:%s",uio_name);
		return check_result;
	}
	memset(mapval,0,sizeof(mapval));
	if(( size = fread(mapval,1,sizeof(mapval),fp)) < 1){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"UIO ReadErr:%ld",size);
		fclose(fp);
		return check_result;
	}
	if(!strncmp((const char*)mapval,check_name,len)){
		check_result = 0;	/* OK */
	}
	else{
		check_result = 0xFFFFFFFF;
	}
	fclose(fp);
	return check_result;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  bpf_hm_devc_data_ini
 *  @note   BPF internal function. 
 *  @return result code
 *  @retval BPF_HM_DEVC_INTER_OK      normal end
 *  @retval BPF_HM_DEVC_INTER_ERR_SYS 
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/12 ALPHA)Yokoyama    docomo SLC/1CHIP対応
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 *  @date   2016/03/14 ALPHA)Fujiiy      modify for M-RRU-ZSYS-02088
 *  @date   2018/08/10 KCN)Suehiro       modify for #7379
 *  @date   2021/02/18 M&C)Tri.hn        Update for intermediate device
 */
/********************************************************************************************************************/
int bpf_hm_devc_data_ini()
{
	int loop_count;
	int loop_index;
	char uio_file[64];
	unsigned int addr;
	unsigned int size;
	unsigned int irq_no = 1;
    int ret;                    /* 戻り値               */
    int setpage;                /* デバイスアドレス     */
    unsigned char kind_data;    /* 装置種別             */
	int tmp_fd;					/* 退避用FD定義         */
    unsigned char pgclog;		/* PGCLOG               */

	/* I2C0のファイルディスクリプタをオープン	*/
	di_devc_data_fd.info[BPF_HM_DEVC_I2C_0].fd = open( BPF_HM_DEVC_I2C0, O_RDWR );	
	/* ディスクリプタ情報を退避 */
	tmp_fd = di_devc_data_fd.info[BPF_HM_DEVC_I2C_0].fd;

	/* 装置種別取得	*/
    setpage = ((D_RRH_EEP_DEVICE_KIND0 >> 8) & 0x07) | BPF_HM_DEVC_I2C_BASE_ADD;

/* M&C)Tri.hn Modify start */
	// ret = bpf_hm_i2c_read(	BPF_HM_DEVC_I2C_0,				/* 種別				*/
	// 						setpage,						/* setpage			*/
	// 						D_RRH_EEP_DEVICE_KIND0, 		/* オフセット		*/
	// 						1,								/* 読み込みサイズ	*/
	// 						BPF_HM_DEVC_ACCESS_1BYTE,		/* アクセスタイプ	*/
	// 						&kind_data );					/* 読み込みデータ	*/
    // if (ret != BPF_HM_DEVC_COMPLETE)
	// {
	// 	bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"EEPROM access ERROR 0x%x 0x%x",setpage,ret);
	//     di_devc_data_p->device_kind = D_RRH_EEP_DEV_KIND_5GDU;
	// 	/* ioctl失敗 */
	// 	return BPF_HM_DEVC_INTER_ERR_SYS;
	// }

    // di_devc_data_p->device_kind = kind_data;
	// //di_devc_data_p->device_kind = D_RRH_EEP_DEV_KIND_5GDU ;
	// /* ZYNQ用のDevice情報をコピー	*/

	// /* PGCログ取得	*/
	// setpage = ((D_DU_EEP_PGCLOG >> 8) & 0x07) | BPF_HM_DEVC_I2C_BASE_ADD;

	// ret = bpf_hm_i2c_read(	BPF_HM_DEVC_I2C_0,				/* 種別				*/
	// 						setpage,						/* setpage			*/
	// 						D_DU_EEP_PGCLOG, 				/* オフセット		*/
	// 						1,								/* 読み込みサイズ	*/
	// 						BPF_HM_DEVC_ACCESS_1BYTE,		/* アクセスタイプ	*/
	// 						&pgclog );					/* 読み込みデータ	*/
	   
	t_bpf_hm_devc_data di_devc_data;
	di_devc_data_p = &di_devc_data;
/* M&C)Tri.hn Modify end */

	if (ret == BPF_HM_DEVC_COMPLETE)
	{
		di_devc_data_p->mmw_pgclog = pgclog;
	}else{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"EEPROM access ERROR 0x%x 0x%x",setpage,ret);
	}

	memcpy( &di_devc_data_fd, &di_devc_data_fd_data_zynqU, sizeof(di_devc_data_fd) );

	/* 退避したI2C0のファイルディスクリプタを戻す	*/
	di_devc_data_fd.info[BPF_HM_DEVC_I2C_0].fd = tmp_fd;
	/* di_devc_data_fd.info[BPF_HM_DEVC_I2C_0].fd = open( BPF_HM_DEVC_I2C0, O_RDWR ); */

	/* 有効なUIOのみvalidflgを立て、アドレス、サイズを設定する */
	for(loop_index = BPF_HM_DEVC_COM_LAST , loop_count=0 ; loop_index < BPF_HM_DEVC_ZYNQ_LAST ; loop_index++ , loop_count++){
		memset(uio_file,0,sizeof(uio_file));
		snprintf(uio_file,sizeof(uio_file) ,"/sys/class/uio/uio%d/maps/map0/addr",loop_count);
		if((addr =  bpf_hm_devc_data_get_uioval(uio_file)) == 0xFFFFFFFF){
			bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"UIO%d Error addr 0x%x",loop_count,addr);
			break;
		}
		memset(uio_file,0,sizeof(uio_file));
		snprintf(uio_file,sizeof(uio_file) ,"/sys/class/uio/uio%d/maps/map0/size",loop_count);
		if((size =  bpf_hm_devc_data_get_uioval(uio_file)) == 0xFFFFFFFF){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"UIO%d Error size 0x%x",loop_count,size);
			break;
		}
		memset(uio_file,0,sizeof(uio_file));
		snprintf(uio_file,sizeof(uio_file) ,"/sys/class/uio/uio%d/name",loop_count);
		if(!bpf_hm_devc_data_get_dtsname_check(uio_file,(const char*)"uio_irq",7)){
			di_devc_data_fd.info[loop_index].irq_no = irq_no;
			bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"IRQ entry UIO%d IRQNo:%d",loop_count,irq_no);
			irq_no++;
		}
		di_devc_data_fd.info[loop_index].validflg = DEVC_FLG_ON;
		di_devc_data_fd.info[loop_index].startoffset = addr;
		di_devc_data_fd.info[loop_index].filesize = size;
	}

	return BPF_HM_DEVC_INTER_OK;

}
/* @} */
/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Device Control UNAssign.
 *  @note   BPF internal function. 
 *  @param  process_desc       [in]  process descriptor
 *  @return result code
 *  @retval BPF_HM_DEVC_INTER_OK    normal end
 *  @retval BPF_HM_DEVC_INTER_ERR_SYS 
 *  @date   2014/01/30 FJT)Taniguchi create
 *  @date   2014/12/12 ALPHA)Kuwamura    docomo SLC/1CHIP対応
 */
/********************************************************************************************************************/
void bpf_hm_devc_unassign( void )
{   
    unsigned char buf[D_RRH_BUFSIZE_4096];			/* PAGE_SIZE */

    /* 既にunassing済みの場合は処理終了 */
    if(di_devc_assign_flag != DEVC_FLG_ON){
        return;
    }
    /* sharedmem attached ? */
    if( mincore(di_devc_data_p,1,buf) )
    {
        /* already sharedmem dettach */
        di_devc_assign_flag = DEVC_FLG_OFF;	
        return;
    }

    /* shmdt */
    shmdt(di_devc_data_p);
    /* アサインフラグ */
    di_devc_assign_flag = DEVC_FLG_OFF;
    return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  mutex lock設定
 *  @note   mutex lock設定
 *  @param  force_access  [out]   0:off 1:on
 *  @return 設定値
 *  @date   2014/01/30 FJT)Tanigchi create
 *  @date   2014/03/05 FJT)Tanigchi modify from pthread_mutex_lock to pthread_mutex_trylock
 */
/********************************************************************************************************************/
int bpf_hm_devc_mutex_lock( pthread_mutex_t* mutex )
{
	int rtn = BPF_HM_DEVC_COMPLETE;
	int count;
	struct bpf_timespec		req;	/* waittime */	
	struct bpf_timespec		rem;	/* canceltime */
	
	req.tv_sec = 0;
	req.tv_nsec = BPF_HM_DEVC_WAIT_TO_100US;	/* 100usec */
	if( di_devc_data_p->force_access <= 0 ){
		/* max 2sec(100usec*20000count ) */
		for( count = 0 ; count < BPF_HM_DEVC_WAIT_TO_MUTEX ; count++){
			/* when locked by other thread */
			if((rtn = pthread_mutex_trylock(mutex)) == EBUSY){
				memset(&rem,0,sizeof(rem));
				/* 100usec sleep */
				if(( rtn = nanosleep(&req,&rem)) != 0 ){
					if ( errno == EINTR ){
						(void)nanosleep(&rem,NULL);
					}
					else
					{
						return rtn;
					}
				}
				continue;
			}
			break;
		}
		if(count >= BPF_HM_DEVC_WAIT_TO_MUTEX){
			/* last try */
			if((rtn = pthread_mutex_trylock(mutex)) == EBUSY){
				unsigned int* wrk_p = (unsigned int*)mutex;
				if( sizeof(pthread_mutex_t) >= 24 ){
					bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Couldn't Lock(%d),Lck:%d,Cnt:%d,Tid:%d,Kid:%d,Nus:%d,Spn:%d",
							rtn,(*wrk_p),*(wrk_p+1),*(wrk_p+2),*(wrk_p+3),*(wrk_p+4),*(wrk_p+5));
				}
				else{
					unsigned char str[64];
					int	len = 0;
					int	alllen = 0;
					memset(str,0,sizeof(str));
					for(count = 0 ; count < ( sizeof(pthread_mutex_t)/4 ); count++ , wrk_p++ )
					{
						len = snprintf((char*)&str[alllen],(sizeof(str) - alllen),"0x%x ",(*wrk_p));
						if(len < 0 )
						{
							break;
						}
						alllen += len;
					}
					bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"Couldn't Lock(%d):%s",rtn,str);
				}
				pthread_mutex_unlock(mutex);
				if((rtn = pthread_mutex_lock(mutex)) == EINVAL){
					bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"%d Couldn't Lock, maybe BUG.",(int)getpid());
				}
			}
		}
	}
	return rtn;
}
/* @} */

/* @} */
/* RRH-001 MD alpha)matsuhashi add end */
