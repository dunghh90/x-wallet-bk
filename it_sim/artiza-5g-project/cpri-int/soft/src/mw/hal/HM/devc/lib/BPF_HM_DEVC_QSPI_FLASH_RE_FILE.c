/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_QSPI_FLASH_RE_FILE.c
 *  @brief
 *  @date   2014/12/03 ALPHA)Yokoyama create
 *  @date   2015/08/12 FJT)harada modied for FHM
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2015
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"


/** @addtogroup BPF_HM_DEVC
 * @{ */


volatile int			re_file_flash_size;							/* サイズ                 */
volatile char*			re_file_flash_map_desc;						/* マップディスクリプタ   */



/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ情報取得
 *  @note   レジスタ情報を取得する
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/06/25 ALPHA)Yokoyama create
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_reg_info()
{
	volatile int ret;										/* 戻り値                 */
//	volatile pthread_mutex_t  *mutex;						/* mutexオブジェクト      */
	volatile int access_type;								/* アクセスタイプ         */
	volatile int offset;									/* オフセット             */

	/* 初期化処理 */
	re_file_flash_size = 0;
	ret = 0;
	
	/* オフセット情報から、取得対象のデータの情報を取得する 										*/
	/* 性能の都合からmutex lock等は取得しない														*/
	/* 既にSLAVE-FLASH用のmutexを取得していることや、SLAVE-FLASH系のレジスタを触るのはココしかない)	*/
#if 0
	ret = bpf_hm_dev_reg_info(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE,
								&offset,
								&re_file_flash_size,
								&re_file_flash_map_desc,
								&mutex,
								&access_type);
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Offset Range NG. offset = %08x", offset);
		/* データ取得失敗 */
		return BPF_HM_DEVC_NG;
	}
#else
	offset = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE - di_devc_data_fd.info[13].startoffset;
	re_file_flash_size = di_devc_data_fd.info[13].filesize;
	re_file_flash_map_desc = di_devc_data_fd.info[13].map;
//	mutex = di_devc_data_p->info[13].mutex;
	access_type = di_devc_data_fd.info[13].access_type;
#endif	
	
	return BPF_HM_DEVC_COMPLETE;
}

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ読み込み
 *  @note   レジスタのREADを行う
 *  @param  offset  [in]   レジスタ読み込みオフセット
 *  @return result code
 *  @retval レジスタ値
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/06/25 ALPHA)Yokoyama 
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_reg_read(unsigned int offset)
{
	unsigned int data;               /* レジスタデータ     */
	
	/* 同期する	*/
	msync( (void*)re_file_flash_map_desc, re_file_flash_size, MS_SYNC );
	/* 4byteアクセス	*/
	/* データを設定する */
	data = *((unsigned *)(re_file_flash_map_desc + (offset - BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE) ) );
	
	return data;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ書き込み
 *  @note   レジスタのWRITEを行う
 *  @param  offset  [in]   レジスタ書き込みオフセット
 *  @param  data    [in]   レジスタ書き込みデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_reg_write(unsigned int offset,unsigned int data)
{
	/* 4byteアクセス	*/
	/* データを設定する */
	*((unsigned *)(re_file_flash_map_desc + (offset - BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE))) = data;
	
	/* 同期する	*/
	msync( (void*)re_file_flash_map_desc, re_file_flash_size, MS_SYNC );
	
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  csディセーブル
 *  @note   csディセーブル処理を行う。
 *  @param  なし
 *  @return result code
 *  @retval なし
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
void bpf_hm_devc_qspi_flash_re_file_qspi_csend()
{
	/* CS選択解除 */
	bpf_hm_devc_qspi_flash_re_file_reg_write(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CS,
											BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_OFF);

	/* データ送信ディセーブル設定 */
	bpf_hm_devc_qspi_flash_re_file_reg_write(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT,
											BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_TRANS_DISABLE);

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  csイネーブル
 *  @note   csイネーブル処理を行う。
 *  @param  ss   [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval なし
 *  @date   2014/04/02 HID)Iwabuchi create
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
void bpf_hm_devc_qspi_flash_re_file_qspi_csstart( unsigned int ss )
{
	/* CS選択 */
	bpf_hm_devc_qspi_flash_re_file_reg_write(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CS,ss);

	/* データ送信イネーブル設定 */
	bpf_hm_devc_qspi_flash_re_file_reg_write(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT,
											BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_TRANS_ENABLE);

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続QSPIアクセス初期化コマンド
 *  @note   PL接続QSPIアクセス初期化コマンドを行う。
 *  @param  ss      [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_qspi_ini(unsigned int ss)
{
    int cnt,rcode;
    unsigned int data;
    unsigned int len_dat;
    unsigned int tx_dat[5];
    unsigned int rx_dat[5];

    rcode = 0;
    
	/* FLASH EOL対応 add start */
	if(di_devc_data_p->flash_kind_re[reQspiFlashIndx] == 1)	/* pf_mainで初期化済みのため、macronixの場合は初期化しない */
	{
		return rcode;
	}
	/* FLASH EOL対応 add end */
	
    // Software reset に 0x0000000A Write SPIソフトリセット
    bpf_hm_devc_qspi_flash_re_file_reg_write( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RESET, 0x0000000A);

    // Wait 1us以上
    rcode = bpf_hm_devc_delay(0, BPF_HM_DEVC_WAIT_TO_2US); // 2us wait
    if (rcode != BPF_HM_DEVC_COMPLETE)
    {
        // csディセーブル
        bpf_hm_devc_qspi_flash_re_file_qspi_csend();

        return rcode;
    }


	// CNT reg SPIディセーブル
	bpf_hm_devc_qspi_flash_re_file_reg_write(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT,
											BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_SYS_DISABLE);

	// CS選択解除
	bpf_hm_devc_qspi_flash_re_file_reg_write(	BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CS,
											BPF_HM_DEVC_PLQSPI_RE_FILE_CMD_CS_SEL_OFF);

    // CNT reg 初期設定設定(FIFO RESET)
	data = bpf_hm_devc_qspi_flash_re_file_reg_read( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT ) & 0xFFFFFC00;
	data |= 0x000001E4; 
	bpf_hm_devc_qspi_flash_re_file_reg_write( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT, data );

	// CNT reg FIFO RESET解除
	data = bpf_hm_devc_qspi_flash_re_file_reg_read( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT ) & 0xFFFFFF9F;
	bpf_hm_devc_qspi_flash_re_file_reg_write( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT, data );

	// FIFO empty 確認
	data = bpf_hm_devc_qspi_flash_re_file_reg_read( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_STATE );
    if((data & 0x00000001) == 0){
        printf("RX FIFO init err!(fifo rst)\n");

        // RX FIFOを空にする(FIFO RESETにする？)
        cnt = 0;
        while((bpf_hm_devc_qspi_flash_re_file_reg_read( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_STATE ) & 0x00000001) == 0) {

            data = bpf_hm_devc_qspi_flash_re_file_reg_read( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RX );
            cnt += 1;
            if (cnt >= 256) {
                printf("RX FIFO init err(ini)\n");
                rcode = 1;
            }
        }
    }
    if((data & 0x00000004) == 0){
        printf("TX FIFO init err!(fifo rst)\n");
        rcode = 1;
    }

    // CNT reg SPIイネーブル
    data = bpf_hm_devc_qspi_flash_re_file_reg_read( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT )
            | 0x00000002;
    bpf_hm_devc_qspi_flash_re_file_reg_write( BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_CNT, data );

    // Dummy cycle設定
    
    // write enable コマンド
    tx_dat[0] = 0x06;
    len_dat = 1;
    rcode = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rcode != BPF_HM_DEVC_COMPLETE)
    {
        return rcode;
    }

    // write volatile configuration register コマンド
    tx_dat[0] = 0x81;
    tx_dat[1] = 0x6B;
    len_dat = 2;
    rcode = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rcode != BPF_HM_DEVC_COMPLETE)
    {
        return rcode;
    }

    return rcode;
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続QSPIアクセスコマンド
 *  @note   PL接続QSPIアクセスコマンド を行う。
 *  @param  dat_len [in]   tx_datのデータ数(コマンド、アドレスも含む)
 *  @param  tx_dat  [in]   送信データの先頭アドレス
 *  @param  rx_dat  [in]   受信データの格納先頭アドレス(ulong)
 *  @param  cs_cnt  [in]   cs制御有り/1 cs制御最初無し/2 cs制御最後無し/3 cs制御無し
 *  @param  ss      [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/03 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_qspi(unsigned int dat_len, unsigned int *tx_dat, unsigned int *rx_dat, unsigned short cs_cnt, unsigned int ss)
{

    int          cnt,cnt1;
    unsigned int *tx_data,*rx_data;
    unsigned int data_offset,status_offset,rxdata_offset;

    data_offset   = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_TX;
    status_offset = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_STATE;
    rxdata_offset = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RX;

    tx_data = tx_dat;
    rx_data = rx_dat;

    /* データ送信 */
    for (cnt = 0;cnt < dat_len;cnt++) {
    	/* 関数CALLによるオーバヘッドで時間が掛かっていたのでここだけ直接レジスタアクセスを行う	*/
		*((unsigned *)(re_file_flash_map_desc + (data_offset - BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE))) = *tx_data;
		/* 同期する	*/
		msync( (void*)re_file_flash_map_desc, re_file_flash_size, MS_SYNC );
    	
        tx_data++;
    }

    /* FIFOにデータ格納後にCSイネーブル */
    if ((cs_cnt == 0) || (cs_cnt == 2)){
        /* csイネーブル */
        bpf_hm_devc_qspi_flash_re_file_qspi_csstart(ss);
    }

    /* データ受信 */
    for (cnt = 0;cnt < dat_len;cnt++) {
        cnt1 = 0;
        /* データ受信するまで待つ */
        while((bpf_hm_devc_qspi_flash_re_file_reg_read(status_offset) & 0x00000001) != 0) {
            if (cnt1 > BPF_HM_DEVC_TX_MAX_CNT_1000) {

                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "bpf_hm_devc_qspi_flash_re_file_qspi RX FIFO timeout");

                /* csディセーブル */
                bpf_hm_devc_qspi_flash_re_file_qspi_csend();

                return BPF_HM_DEVC_NG;
            }
            cnt1++;
        }
        /* 受信データ取り込み */
    	/* 関数CALLによるオーバヘッドで時間が掛かっていたのでここだけ直接レジスタアクセスを行う	*/
		/* 同期する	*/
		msync( (void*)re_file_flash_map_desc, re_file_flash_size, MS_SYNC );
		/* データを設定する */
		*rx_data = *((unsigned *)(re_file_flash_map_desc + (rxdata_offset - BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_BASE) ) );
    	
        rx_data++;
    }

    if ((cs_cnt == 0) || (cs_cnt == 1)){
        /* csディセーブル */
        bpf_hm_devc_qspi_flash_re_file_qspi_csend();
    }
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLASH起動面変更
 *  @note   FLASH起動面変更を行う。
 *  @param  upper  [in]   FLASH起動面
 *  @param  ss     [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_sect_set(unsigned int upper, unsigned int ss)
{
    unsigned int len_dat;
    unsigned int tx_dat[5];
    unsigned int rx_dat[5];
    int          rtn;                /* 戻り値             */


    /* write enable コマンド */
    tx_dat[0] = 0x06;
    len_dat = 1;
    rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        return rtn;
    }

    /* write extended address register コマンド */
    tx_dat[0] = 0xC5;
    tx_dat[1] = upper;
    len_dat = 2;
    rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        return rtn;
    }
    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLASH status 確認
 *  @note   FLASHアクセス前にstatusがReadyであることを確認する。
 *  @param  ss     [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/10/07 FJT)harada create for FHM
 */
/********************************************************************************************************************/
/* FLASH EOL対応 add start */
int bpf_hm_devc_qspi_flash_re_status_check_c2(unsigned int ss, unsigned int lpCnt, unsigned int dlytime)
{
    unsigned int len_dat;
    unsigned int tx_dat[5];
    unsigned int rx_dat[5];
    int          rtn;                /* 戻り値             */
    int          cnt;

	/* status情報確認。3秒+マージン = 5秒間チェック(1ms*5000 or 100us*50000)する	*/
    for(cnt = 0;cnt<lpCnt;cnt++) {
	    /* STATUS register READ コマンド */
	    /* FLASH EOL対応 chg start	*/
	    /* Flag Status register read コマンド(Micron):0x70 */
	    /* Status register read コマンド(Macronix):0x05 */
	    tx_dat[0] = plQspiFlash_stsReg[di_devc_data_p->flash_kind_re[reQspiFlashIndx]];
	    /* FLASH EOL対応 chg end	*/
	    tx_dat[1] = 0x00;
	    len_dat = 2;

        rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
		    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
		        "bpf_hm_devc_qspi_flash_re_status_check_c2 ng1");
            return rtn;
        }

	    /* FLASH EOL対応 chg start	*/
	    /* Status register確認値(Micron=bit7, Macronix=bit0) */ /* FHMハソに合わせて修正 */
        if((rx_dat[1] & plQspiFlash_stsReg_chkbit[di_devc_data_p->flash_kind_re[reQspiFlashIndx]]) == plQspiFlash_stsReg_rsltbit[di_devc_data_p->flash_kind_re[reQspiFlashIndx]])
        {
	    /* FLASH EOL対応 chg end	*/

            return BPF_HM_DEVC_COMPLETE;
        }

        rtn = bpf_hm_devc_delay( 0, dlytime ); /* 1ms wait */
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
		    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
		        "bpf_hm_devc_qspi_flash_re_status_check_c2 ng2");
            return BPF_HM_DEVC_NG;
        }
    }
    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "bpf_hm_devc_qspi_flash_re_status_check_c2 ng3");
    return BPF_HM_DEVC_NG;
}
/* FLASH EOL対応 add end */

int bpf_hm_devc_qspi_flash_re_status_check(unsigned int ss, unsigned int lpCnt, unsigned int dlytime)
{
    unsigned int len_dat;
    unsigned int tx_dat[5];
    unsigned int rx_dat[5];
    int          rtn;                /* 戻り値             */
    int          cnt;

	/* FLASH EOL対応 chg start */
	if(di_devc_data_p->flash_kind_re[reQspiFlashIndx] == 1)
	{
		/*	macronix用のStatus register readを行う	*/
		rtn = bpf_hm_devc_qspi_flash_re_status_check_c2(ss, lpCnt, dlytime);
		return rtn;
	}
    /* STATUS register READ コマンド */
    /* FLASH EOL対応 chg start	*/
    /* Flag Status register read コマンド(Micron):0x70 */
    /* Status register read コマンド(Macronix):0x05 */
    tx_dat[0] = plQspiFlash_stsReg[di_devc_data_p->flash_kind_re[reQspiFlashIndx]];
    /* FLASH EOL対応 chg end	*/
    tx_dat[1] = 0x00;
    len_dat = 2;

	/* status情報確認。3秒+マージン = 5秒間チェックする	*/
    for(cnt = 0;cnt<lpCnt;cnt++) {
        rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
		    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
		        "bpf_hm_devc_qspi_flash_re_status_check ng1");
            return rtn;
        }

	    /* FLASH EOL対応 chg start	*/
	    /* Status register確認値(Micron=bit7, Macronix=bit0) */ /* FHMハソに合わせて修正 */
        if((rx_dat[1] & plQspiFlash_stsReg_chkbit[di_devc_data_p->flash_kind_re[reQspiFlashIndx]]) == plQspiFlash_stsReg_rsltbit[di_devc_data_p->flash_kind_re[reQspiFlashIndx]])
        {
	    /* FLASH EOL対応 chg end	*/

            return BPF_HM_DEVC_COMPLETE;
        }

        rtn = bpf_hm_devc_delay( 0, dlytime ); /* 1ms wait */
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
		    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
		        "bpf_hm_devc_qspi_flash_re_status_check ng2");
            return BPF_HM_DEVC_NG;
        }
    }
	
    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
        "bpf_hm_devc_qspi_flash_re_status_check ng3");
    return BPF_HM_DEVC_NG;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  spi flash 読み込み（面単位）
 *  @note   spi flash 読み込みを面単位で行う
 *  @param  buffer         [in]  書き込みバッファ
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @param  ss             [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi create
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_read_half(unsigned char *buffer, unsigned int flash_address, unsigned int len, unsigned int ss)
{
    unsigned int len_dat;
    unsigned int tx_dat[8];
    unsigned int rx_dat[8];
    unsigned int address1,address2,prn_chk;
    unsigned int l_add,h_add;
    int          rtn;                /* 戻り値             */

    unsigned int data_offset,status_offset,rxdata_offset;
    unsigned int cnt,cnt0,cnt1;

    data_offset   = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_TX;
    status_offset = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_STATE;
    rxdata_offset = BPF_HM_DEVC_PLQSPI_RE_FILE_OFFSET_RX;

    /* quad output fast read コマンド */
    tx_dat[0] = 0x6B;
    /* アドレス */
    address1 = flash_address >> 8;
    address2 = address1 >> 8;
    tx_dat[1] = address2 & 0x000000FF;
    tx_dat[2] = address1 & 0x000000FF;
    tx_dat[3] = flash_address & 0x000000FF;

    /* Dummy cycle用データ */
    tx_dat[4] = 0x0000;
    tx_dat[5] = 0x0000;
    tx_dat[6] = 0x0000;

    len_dat = 7;

    rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 2, ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        return rtn;
    }

    l_add = len & 0x0000000F;
    h_add = len >> 4;

    /* 16Byte毎の受信データ取り込み */
    for(cnt0 = 0; cnt0 < h_add; cnt0++) {

        for(cnt = 0; cnt < 0x10; cnt++) {

            /* データ送信 */
            bpf_hm_devc_qspi_flash_re_file_reg_write(data_offset,0);
        }

        for(cnt = 0; cnt < 0x10; cnt++) {
            cnt1 = 0;
            /* データ受信するまで待つ */
            while((bpf_hm_devc_qspi_flash_re_file_reg_read(status_offset) & 0x00000001) != 0) {
                if (cnt1 > BPF_HM_DEVC_TX_MAX_CNT_1000) {

                    bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                        "bpf_hm_devc_qspi_flash_re_file_read_half RX FIFO timeout");

                    /* csディセーブル */
                    bpf_hm_devc_qspi_flash_re_file_qspi_csend();

                    return BPF_HM_DEVC_NG;
               }
               cnt1++;
            }
            /* 受信データ取り込み */
            *buffer = (unsigned char)(bpf_hm_devc_qspi_flash_re_file_reg_read(rxdata_offset));
            prn_chk = (unsigned int)buffer & 0x000FFFFF;
            if(prn_chk == 0) {
                /* WTDタイマのクリア */
                BPF_HM_DEVC_WDT_CLEAR();
            }
            buffer++;
        }
    }

    /* 16Byteで割り切れなかった分の受信データ取り込み */
    for(cnt = 0;cnt<l_add;cnt++) {
        /* データ送信 */
        bpf_hm_devc_qspi_flash_re_file_reg_write(data_offset,0);
    }

    for(cnt = 0;cnt<l_add;cnt++) {
        cnt1 = 0;
        /* データ受信するまで待つ */
        while((bpf_hm_devc_qspi_flash_re_file_reg_read(status_offset) & 0x00000001) != 0) {

            if (cnt1 > BPF_HM_DEVC_TX_MAX_CNT_1000) {

                bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
                    "bpf_hm_devc_qspi_flash_re_file_read_half RX FIFO timeout");

                /* csディセーブル */
                bpf_hm_devc_qspi_flash_re_file_qspi_csend();

                return BPF_HM_DEVC_NG;
            }
            cnt1++;
        }
        /* 受信データ取り込み */
        *buffer = (unsigned char)(bpf_hm_devc_qspi_flash_re_file_reg_read(rxdata_offset));
        prn_chk = (unsigned int)buffer & 0x000FFFFF;
        if(prn_chk == 0){
            /* WTDタイマのクリア */
            BPF_HM_DEVC_WDT_CLEAR();
        }

        buffer++;
    }

    /* csディセーブル */
    bpf_hm_devc_qspi_flash_re_file_qspi_csend();

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 書き込み（面単位）
 *  @note   qspi flash 書き込みを面単位で行う
 *  @param  buffer         [in]  書き込みバッファ
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @param  ss             [in]  イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_write_half(unsigned char *buffer,unsigned int flash_address, unsigned int len, unsigned int ss)
{
    unsigned int len_dat;
    unsigned int tx_dat[5];
    unsigned int rx_dat[5];
    unsigned int address1,address2,add_chk,prn_chk;
    int          rtn;                /* 戻り値             */

    unsigned int cnt,cnt2;

    /* write enable コマンド */
    tx_dat[0] = 0x06;
    len_dat = 1;
    rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        return rtn;
    }
    /* FLASH EOL対応 chg start	*/
    /* quad input fast program コマンド(Micron):0x32 */
    /* page program コマンド(Macronix):0x02 */
    tx_dat[0] = plQspiFlash_write_cmd[di_devc_data_p->flash_kind_re[reQspiFlashIndx]];
    /* FLASH EOL対応 chg end	*/
    
    /* アドレス */
    address1 = flash_address >> 8;
    address2 = address1 >> 8;
    tx_dat[1] = address2 & 0x000000FF;
    tx_dat[2] = address1 & 0x000000FF;
    tx_dat[3] = flash_address & 0x000000FF;
    len_dat = 4;

    len_dat = 1;

    add_chk = 1;
    prn_chk = 1;
    for(cnt = 0; cnt < len; cnt++) {
        if(add_chk == 0) {
            /* csディセーブル */
            bpf_hm_devc_qspi_flash_re_file_qspi_csend();

/* FLASH EOL対応 chg start */
            rtn = bpf_hm_devc_qspi_flash_re_status_check(ss, 50000, BPF_HM_DEVC_WAIT_TO_100US);
            if (rtn != BPF_HM_DEVC_COMPLETE)
            {
                printf("Status register read cmd NG\n");
                return rtn;
            }
/* FLASH EOL対応 chg end */

            /* write enable コマンド */
            tx_dat[0] = 0x06;
            len_dat = 1;
            rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
            if (rtn != BPF_HM_DEVC_COMPLETE)
            {
                printf("write enable cmd2 NG\n");

                return rtn;
            }
		    /* FLASH EOL対応 chg start	*/
		    /* quad input fast program コマンド(Micron):0x32 */
		    /* page program コマンド(Macronix):0x02 */
		    tx_dat[0] = plQspiFlash_write_cmd[di_devc_data_p->flash_kind_re[reQspiFlashIndx]];
		    /* FLASH EOL対応 chg end	*/
		    
            /* アドレス */
            address1 = flash_address >> 8;
            address2 = address1 >> 8;
            tx_dat[1] = address2 & 0x000000FF;
            tx_dat[2] = address1 & 0x000000FF;
            tx_dat[3] = flash_address & 0x000000FF;
        }

        if((add_chk==0)||(cnt==0)){
            //CS お試し
            tx_dat[4] = *buffer;
            len_dat = 5;
            rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 2, ss);
            if (rtn != BPF_HM_DEVC_COMPLETE){
                printf("write2 cmd NG\n");
                return rtn;
            }
        }else{
            tx_dat[0] = *buffer;
            len_dat = 1;
            rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 3, ss);
            if (rtn != BPF_HM_DEVC_COMPLETE){
                printf("write3 cmd NG\n");
                return rtn;
            }
        }

        buffer++;
        flash_address++;
        add_chk = flash_address & 0x000000FF;
        prn_chk = flash_address & 0x0000FFFF;
        if(prn_chk == 0){
            /* WTDタイマのクリア */
            BPF_HM_DEVC_WDT_CLEAR();
        }
    }

    /* csディセーブル */
    bpf_hm_devc_qspi_flash_re_file_qspi_csend();

/* FLASH EOL対応 chg start */
    for (cnt2 = 0;cnt2 < 10;cnt2++) {
	    rtn = bpf_hm_devc_qspi_flash_re_status_check(ss, 50000, BPF_HM_DEVC_WAIT_TO_100US);
	    if (rtn == BPF_HM_DEVC_COMPLETE)
	    {
	        break;
	    }
	}
	if (rtn != BPF_HM_DEVC_COMPLETE)
	{
		printf("Status register read cmd NG\n");
	}
    return rtn;
/* FLASH EOL対応 chg end */
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash セクタ消去(セクタ単位)
 *  @note   qspi flash セクタ消去を行う。
 *  @param  sect_add  [in]  FLASHアドレス
 *  @param  ss        [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada FHM対応
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_sect_erase(unsigned int  sect_add, unsigned int ss)
{
    unsigned int len_dat;
    unsigned int tx_dat[5];
    unsigned int rx_dat[5];
    unsigned int address0,address1,address2;
    int          rtn;

    /* write enable コマンド */
    tx_dat[0] = 0x06;
    len_dat = 1;

    rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        return rtn;
    }

    /* FLAG STATUS register CLEAR コマンド */
    address2 = 0;
    address1 = 0;
    address0 = sect_add & 0x000000FF;

    tx_dat[0] = 0xD8;
    tx_dat[1] = address0;
    tx_dat[2] = address1;
    tx_dat[3] = address2;

    len_dat = 4;

    rtn = bpf_hm_devc_qspi_flash_re_file_qspi (len_dat, tx_dat, rx_dat, 0, ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        return rtn;
    }

    /* FLASH EOL対応 chg start	*/
    rtn = bpf_hm_devc_qspi_flash_re_status_check(ss, 5000, BPF_HM_DEVC_WAIT_TO_1MS);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        printf("Status register read cmd NG\n");
        return rtn;
    }
	
    return rtn;
    /* FLASH EOL対応 chg end	*/
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 消去(面単位)
 *  @note   qspi flash 消去を行う。
 *  @param  sect_add  [in]  FLASHアドレス
 *  @param  len       [in]  データ長
 *  @param  ss        [in]   イネーブルにするcsのwriteデータ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada modified for FHM
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_re_file_erase_half(unsigned int add, unsigned int len, unsigned int ss)
{
    unsigned int address1,address2;
    unsigned int prn_chk;
    int          rtn;

    unsigned int cnt;

    address1 = add >> 16;
    address2 = (add + len - 1) >> 16;

    prn_chk = 1;
    for(cnt=address1;cnt<=address2;cnt++) {
        rtn = bpf_hm_devc_qspi_flash_re_file_sect_erase (cnt, ss);
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
            return rtn;
        }

        prn_chk = cnt & 0x000F;
        if(prn_chk == 0){
            /* WTDタイマのクリア */
            BPF_HM_DEVC_WDT_CLEAR();
        }
    }

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RE file用 qspi flash 読み込み(API-IF)
 *  @note   RE file用 qspi flash 読み込みを行う(API-IF)
 *  @param  face           [in]  FLASH面
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @param  buffer         [in]  書き込みバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/05/23 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada FHM対応 flash64MB
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ(unsigned int face, unsigned int flash_address, unsigned int len,unsigned char *buffer)
{
    int          rtn;
    pthread_mutex_t  *mutex;         /* mutexオブジェクト    */
	unsigned int access_side,ss;     /* 面情報 */

	/**********************/
	/* 装置が未対応の場合 */
	/**********************/
	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_FHM)
	{
		return BPF_HM_DEVC_UNSUPPORT;
	}

    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ flash_address = %08x , len = %08x" ,
                        flash_address,len);
	/* face check */
	if( face > BPF_HM_DEVC_RE_FILE_FLASH_FACE_15 ){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "face is invalid:0x%08x",face);
        return BPF_HM_DEVC_NG;
	}
	ss=flash_re_file_face2data[face][0];
	access_side=flash_re_file_face2data[face][1];
/* FLASH EOL対応 add start */
	reQspiFlashIndx = flash_re_file_face2data[face][2];
/* FLASH EOL対応 add end */
	
	/* size check */
	if( (flash_address + len) > BPF_HM_DEVC_PLQSPI_RE_FILE_MAXSIZE )
	{
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "flash_address + len is over RE flash size:0x%08x",flash_address + len);
        return BPF_HM_DEVC_NG;
	}
	
    /* pointer check */
    if( buffer == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "buffer == NULL");
        return BPF_HM_DEVC_NG;
    }

    mutex = &di_devc_data_p->info[BPF_HM_DEVC_RE_FILE_FLASH].mutex;                  /* mutexオブジェクト      */

    /* Mutex lock */
    rtn = bpf_hm_devc_mutex_lock(mutex);
    if( rtn != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }

	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_re_file_reg_info();
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ_S,flash_address,len,2,face);
    
	/* 初期設定 */
    rtn = bpf_hm_devc_qspi_flash_re_file_qspi_ini(ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ NG rtn = %d",rtn);
        return rtn;
        return rtn;
    }
	
	/* flash status確認 */
	rtn = bpf_hm_devc_qspi_flash_re_status_check(ss, 5000, BPF_HM_DEVC_WAIT_TO_1MS);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE NG  flash status strange");
        return rtn;
    }

    /* mem 選択 */
    rtn = bpf_hm_devc_qspi_flash_re_file_sect_set(access_side,ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ NG rtn = %d",rtn);
        return rtn;
    }
    /* リード処理 */
    rtn = bpf_hm_devc_qspi_flash_re_file_read_half(buffer,flash_address,len,ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ NG rtn = %d",rtn);
        return rtn;
    }

	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ,flash_address,len,2,face);
	
    pthread_mutex_unlock(mutex);
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_RE_FILE_READ COMPLETE");

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  RE file用 qspi flash 書き込み(API-IF)
 *  @note   RE file用 qspi flash 書き込みを行う(API-IF)
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @param  buffer         [in]  書き込みバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/05/23 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada FHM対応 flash64MB
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE(unsigned int face, unsigned int flash_address, unsigned int len,unsigned char *buffer)
{
    int          rtn;
    pthread_mutex_t  *mutex;         /* mutexオブジェクト    */
	unsigned int access_side,ss;     /* 面情報 */

	/**********************/
	/* 装置が未対応の場合 */
	/**********************/
	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_FHM)
	{
		return BPF_HM_DEVC_UNSUPPORT;
	}

    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE flash_address = %08x , len = %08x" ,
                        flash_address,len);
	/* face check */
	if( face > BPF_HM_DEVC_RE_FILE_FLASH_FACE_15 ){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "face is invalid:0x%08x",face);
        return BPF_HM_DEVC_NG;
	}
	ss=flash_re_file_face2data[face][0];
	access_side=flash_re_file_face2data[face][1];
/* FLASH EOL対応 add start */
	reQspiFlashIndx = flash_re_file_face2data[face][2];
/* FLASH EOL対応 add end */
	
	/* size check */
	if( (flash_address + len) > BPF_HM_DEVC_PLQSPI_RE_FILE_MAXSIZE )
	{
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "flash_address + len is over RE flash size:0x%08x",flash_address + len);
        return BPF_HM_DEVC_NG;
	}

    /* pointer check */
    if( buffer == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "buffer == NULL");
        return BPF_HM_DEVC_NG;
    }

    mutex = &di_devc_data_p->info[BPF_HM_DEVC_RE_FILE_FLASH].mutex;                  /* mutexオブジェクト      */

    /* Mutex lock */
    rtn = bpf_hm_devc_mutex_lock(mutex);
    if( rtn != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }

	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_re_file_reg_info();
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE_S,flash_address,len,2,face);

    /* spi flash 書き込み処理を行う */
    /* 初期設定 */
    rtn = bpf_hm_devc_qspi_flash_re_file_qspi_ini(ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE NG rtn = %d",rtn);
        return rtn;
    }
	
	/* flash status確認 */
	rtn = bpf_hm_devc_qspi_flash_re_status_check(ss, 5000, BPF_HM_DEVC_WAIT_TO_1MS);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE NG  flash status strange");
        return rtn;
    }
    /* upper mem 選択 */
    rtn = bpf_hm_devc_qspi_flash_re_file_sect_set(access_side,ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE NG rtn = %d",rtn);
        return rtn;
    }

    /* 書き込み処理 */
    rtn = bpf_hm_devc_qspi_flash_re_file_write_half(buffer,flash_address,len,ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE NG rtn = %d",rtn);
        return rtn;
    }

	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE,flash_address,len,2,face);

    pthread_mutex_unlock(mutex);
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_RE_FILE_WRITE COMPLETE");

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  re file用 qspi flash 消去(API-IF)
 *  @note   re file用 qspi flash 消去を行う。
 *  @param  face      [in]  対象面
 *  @param  sect_add  [in]  FLASHアドレス
 *  @param  len       [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/05/23 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/12 FJT)harada FHM対応 flash64MB
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE( unsigned int face, unsigned int add, unsigned int len)
{
    int          rtn;
    pthread_mutex_t  *mutex;         /* mutexオブジェクト    */
	unsigned int access_side,ss;     /* 面情報 */

	/**********************/
	/* 装置が未対応の場合 */
	/**********************/
	if(di_devc_data_p->device_kind != D_RRH_EEP_DEV_KIND_FHM)
	{
		return BPF_HM_DEVC_UNSUPPORT;
	}

    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE add = %08x , len = %08x" ,
                        add,len);

    mutex = &di_devc_data_p->info[BPF_HM_DEVC_RE_FILE_FLASH].mutex;                  /* mutexオブジェクト      */
	
	/* face check */
	if( face > BPF_HM_DEVC_RE_FILE_FLASH_FACE_15 ){
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "face is invalid:0x%08x",face);
        return BPF_HM_DEVC_NG;
	}
	ss=flash_re_file_face2data[face][0];
	access_side=flash_re_file_face2data[face][1];
/* FLASH EOL対応 add start */
	reQspiFlashIndx = flash_re_file_face2data[face][2];
/* FLASH EOL対応 add end */
	
	/* size check */
	if( (add + len) > BPF_HM_DEVC_PLQSPI_RE_FILE_MAXSIZE )
	{
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "add + len is over RE flash size:0x%08x",add + len);
        return BPF_HM_DEVC_NG;
	}
	
    /* Mutex lock */
    rtn = bpf_hm_devc_mutex_lock(mutex);
    if( rtn != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }
	
	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_re_file_reg_info();
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE_S,add,len,2,face);

    /* 初期設定 */
    rtn = bpf_hm_devc_qspi_flash_re_file_qspi_ini(ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE NG rtn = %d",rtn);
        return rtn;
    }
	
	/* flash status確認 */
	rtn = bpf_hm_devc_qspi_flash_re_status_check(ss, 5000, BPF_HM_DEVC_WAIT_TO_1MS);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE NG  flash status strange");
        return rtn;
    }

    /* upper mem 選択 */
    rtn = bpf_hm_devc_qspi_flash_re_file_sect_set(access_side,ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE NG rtn = %d",rtn);
        return rtn;
    }

    /* イレース処理 */
    rtn = bpf_hm_devc_qspi_flash_re_file_erase_half(add,len,ss);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE NG rtn = %d",rtn);
        return rtn;
    }

	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE,add,len,2,face);

    pthread_mutex_unlock(mutex);
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_RE_FILE_ERASE COMPLETE");

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */
/* @} */

/* RRH-001 MD alpha)matsuhashi add end */


