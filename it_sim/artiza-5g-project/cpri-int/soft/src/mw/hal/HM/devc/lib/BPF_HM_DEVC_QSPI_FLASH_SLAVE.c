/* RRH-001 MD alpha)matsuhashi add start */
/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   BPF_HM_DEVC_QSPI_FLASH_SLAVE.c
 *  @brief
 *  @date   2014/12/03 ALPHA)Yokoyama create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2013-2014
 */
/********************************************************************************************************************/

#define UNIT_ID BPF_HM_DEVC /* Pre-define */

/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_hm_devc.h"


/** @addtogroup BPF_HM_DEVC
 * @{ */


volatile int			slave_flash_size;							/* サイズ                 */
volatile char*			slave_flash_map_desc;						/* マップディスクリプタ   */
volatile int			slave_flash_offset;							/* オフセット             */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  レジスタ情報取得
 *  @note   レジスタ情報を取得する
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2015/06/25 ALPHA)Yokoyama create
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_reg_info()
{
	volatile int ret;										/* 戻り値                 */
	volatile pthread_mutex_t  *mutex;						/* mutexオブジェクト      */
	volatile int access_type;								/* アクセスタイプ         */

	/* 初期化処理 */
	slave_flash_size = 0;
	ret = 0;
	
	/* オフセット情報から、取得対象のデータの情報を取得する 										*/
	/* 性能の都合からmutex lock等は取得しない														*/
	/* 既にSLAVE-FLASH用のmutexを取得していることや、SLAVE-FLASH系のレジスタを触るのはココしかない)	*/
	ret = bpf_hm_dev_reg_info(	BPF_HM_DEVC_PLQSPI_OFFSET_BASE,
								&slave_flash_offset,
								&slave_flash_size,
								&slave_flash_map_desc,
								&mutex,
								&access_type);
	if (ret != BPF_HM_DEVC_COMPLETE)
	{
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3, "Offset Range NG. offset = %08x", slave_flash_offset);
		/* データ取得失敗 */
		return BPF_HM_DEVC_NG;
	}
	
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
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_reg_read(unsigned int offset)
{
	unsigned int data;               /* レジスタデータ     */
	
	/* 同期する	*/
	msync( (void*)slave_flash_map_desc, slave_flash_size, MS_SYNC );
	/* 4byteアクセス	*/
	/* データを設定する */
	data = *((unsigned *)(slave_flash_map_desc + (offset - BPF_HM_DEVC_PLQSPI_OFFSET_BASE + slave_flash_offset) ) );
	
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
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_reg_write(unsigned int offset,unsigned int data)
{
	/* 4byteアクセス	*/
	/* データを設定する */
	*((unsigned *)(slave_flash_map_desc + (offset - BPF_HM_DEVC_PLQSPI_OFFSET_BASE + slave_flash_offset))) = data;
	
	/* 同期する	*/
	msync( (void*)slave_flash_map_desc, slave_flash_size, MS_SYNC );
	
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  データ読み出し
 *  @note   読み出しデータの有無を確認してからデータを読み出す
 *  @param  data_p  [in]   読み出しデータ格納バッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_data_read(unsigned char *data_p)
{
    unsigned int count;
    unsigned int buf;
    
    // Rx_Empty = "0"であることを確認
	count = 0;
	while (bpf_hm_devc_qspi_flash_slave_reg_read(BPF_HM_DEVC_PLQSPI_OFFSET_STATE) & 0x00000001) {
		// 待ち合わせ必要？
        bpf_hm_devc_delay(0, BPF_HM_DEVC_WAIT_TO_100US); // 100us wait
		if(count++ > 1000000) return BPF_HM_DEVC_NG;		// 一定回数確認してデータ無ければ抜ける
	}
    // FLASHからのリード値取得(1byte)
    buf = bpf_hm_devc_qspi_flash_slave_reg_read(BPF_HM_DEVC_PLQSPI_OFFSET_RX);				// 0xA002_006C
	*data_p = (unsigned char)buf;

	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  データ読み出し(回数分読み捨て)
 *  @note   RXレジスタから指定回数分データを読み捨てる
 *  @param  num     [in]   読み捨て回数
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_data_read_cycle(int num)
{
	int rcode = BPF_HM_DEVC_COMPLETE;
    unsigned int count;
    unsigned char data;
    
	for (count = 0; count < num; count++) {
		//ダミーリード
		if(bpf_hm_devc_qspi_flash_slave_data_read(&data) != 0) {
			printf(" RX FIFO timeout(Dummy read:%d/%d)(pl qspi) \n",count,num);
			rcode = BPF_HM_DEVC_NG;
		}
	}
	return rcode;
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
 */
/********************************************************************************************************************/
void bpf_hm_devc_qspi_flash_slave_qspi_csend()
{
	/* CS選択解除 */
	bpf_hm_devc_qspi_flash_slave_reg_write(	BPF_HM_DEVC_PLQSPI_OFFSET_CS,
											BPF_HM_DEVC_PLQSPI_CMD_CS_SEL_OFF);

	/* データ送信ディセーブル設定 */
	bpf_hm_devc_qspi_flash_slave_reg_write(	BPF_HM_DEVC_PLQSPI_OFFSET_CNT,
											BPF_HM_DEVC_PLQSPI_CMD_TRANS_DISABLE);

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  csイネーブル
 *  @note   csイネーブル処理を行う。
 *  @param  なし
 *  @return result code
 *  @retval なし
 *  @date   2014/04/02 HID)Iwabuchi create
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
void bpf_hm_devc_qspi_flash_slave_qspi_csstart()
{
	/* CS選択 */
	bpf_hm_devc_qspi_flash_slave_reg_write(	BPF_HM_DEVC_PLQSPI_OFFSET_CS,
											BPF_HM_DEVC_PLQSPI_CMD_CS_SEL_ON);

	/* データ送信イネーブル設定 */
	bpf_hm_devc_qspi_flash_slave_reg_write(	BPF_HM_DEVC_PLQSPI_OFFSET_CNT,
											BPF_HM_DEVC_PLQSPI_CMD_TRANS_ENABLE);

	return;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Write Enableコマンド設定
 *  @note   Write Enableコマンドを設定する
 *  @param  なし
 *  @return result code
 *  @retval なし
 */
/********************************************************************************************************************/
void bpf_hm_devc_qspi_flash_slave_write_en()
{
    unsigned char data;

    //SPIシステムイネーブルをイネーブル
    bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_TRANS_DISABLE);		// 0xA002_0060
    //WRITE ENABLEコマンド
    bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, BPF_HM_DEVC_PLQSPI_WRITE_ENABLE);		// 0xA002_0068

    //csイネーブル
    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定
	//ダミーリード
	if(bpf_hm_devc_qspi_flash_slave_data_read(&data) != 0) {
		printf(" RX FIFO timeout(write EN)(pl qspi) \n");
	}
	// CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_qspi_csend();
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  Flag Status Register確認
 *  @note   データ設定が完了しているかの確認を行う
 *  @param  なし
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_streg_chk()
{
    unsigned int count;
    unsigned char data, stReg;

    // Status Register(Bit0:write in progress) = "0"であることを確認
	stReg = 0x01; //初期値として"1"(in progress)を設定
	count = 0;
	while (1) {
		//Status register readコマンド
		bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, 0x00000005);		// 0xA002_0068
		//データリード用のダミー書き込み
		bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, 0x00000000);		// 0xA002_0068

		//csイネーブル
	    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定

		//ダミーリード
		if(bpf_hm_devc_qspi_flash_slave_data_read(&data) != 0) {
			printf(" RX FIFO timeout(Status register command read)(pl qspi) \n");
		}
		//Status registerデータリード
		if(bpf_hm_devc_qspi_flash_slave_data_read(&stReg) != 0) {
			printf(" RX FIFO timeout(Status register data read)(pl qspi) \n");
		}
		// CS ディセーブルに設定
	    bpf_hm_devc_qspi_flash_slave_qspi_csend();
		
		if ((stReg & 0x01) == 0x00) {
			break ;			// Status Register確認完了
		}
		// 待ち合わせ必要？
        bpf_hm_devc_delay(0, BPF_HM_DEVC_WAIT_TO_100US); // 100us wait
		if(count++ > 10000000) return BPF_HM_DEVC_NG;		// 一定回数確認して書き込み完了にならなければ抜ける
	}
	//SPIシステムイネーブルをディセーブル
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_SYS_DISABLE);		// 0xA002_0060

	return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続QSPIアクセス初期化コマンド
 *  @note   PL接続QSPIアクセス初期化コマンドを行う。
 *  @param  なし
 *  @return result code
 *  @retval なし
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
void bpf_hm_devc_qspi_flash_slave_qspi_ini(void)
{
static int BPF_HM_DEVC_QSPI_FLASH_SLAVE_INI_FLG = 0;

	if (BPF_HM_DEVC_QSPI_FLASH_SLAVE_INI_FLG != 0) return;		//初期化されていれば、処理しない

/* FIFOクリア */
	// SPIソフトリセット
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_RESET, 0x0000000A);		// 0xA002_0040
	// Wait 1us以上
    bpf_hm_devc_delay(0, BPF_HM_DEVC_WAIT_TO_2US); // 2us wait
	//SPIシステムイネーブルをディセーブル
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_SYS_DISABLE);		// 0xA002_0060
    // CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CS, BPF_HM_DEVC_PLQSPI_CMD_CS_SEL_OFF);		// 0xA002_0070
	//FIFOリセット
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, 0x000001E4);		// 0xA002_0060
    //SPIシステムイネーブルをイネーブル
/* Flash Dummy cycle設定のWRITE ENABLE コマンドで実施
 *	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_TRANS_DISABLE);		// 0xA002_0060
 */
	// Wait そのまま走らせるとWriteENコマンドのダミーリードが出来ないので、待ち合わせ
    bpf_hm_devc_delay(0, BPF_HM_DEVC_WAIT_TO_1MS);  // 1ms wait

/* Flash Dummy cycle設定 */
	// WRITE ENABLE コマンド
	bpf_hm_devc_qspi_flash_slave_write_en();
	//write status/configuration register コマンド
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, 0x00000001);		// 0xA002_0068
	//Status Register設定値
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, 0x00000040);		// 0xA002_0068
	//Configuration Register設定値
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, 0x00000047);		// 0xA002_0068
	//csイネーブル
    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定
	//ダミーリード 3回
	if(bpf_hm_devc_qspi_flash_slave_data_read_cycle(3) != 0) {
		printf(" RX FIFO timeout(Dummy cycle set)(pl qspi) \n");
	}
	// CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_qspi_csend();
	// Flag Status Register確認
	if (bpf_hm_devc_qspi_flash_slave_streg_chk() != 0) {
		printf(" RX FIFO timeout(Dummy cycle set)(pl qspi) \n");
	}
	BPF_HM_DEVC_QSPI_FLASH_SLAVE_INI_FLG = 1;		//初期化処理完了
}
/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続QSPIアクセスコマンド
 *  @note   PL接続QSPIアクセスコマンド を行う。
 *  @param  dat_len [in]   tx_datのデータ数(コマンド、アドレスも含む)
 *  @param  tx_dat  [in]   送信データの先頭アドレス
 *  @param  rx_dat  [in]   受信データの格納先頭アドレス
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/03 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_pl_qspi(unsigned int dat_len, unsigned char *tx_dat, unsigned char *rx_dat)
{

    int          cnt,rcode;
    unsigned char *tx_data,*rx_data;
    unsigned char data;

    tx_data = tx_dat;
    rx_data = rx_dat;
    rcode = BPF_HM_DEVC_COMPLETE;

    /* データ送信 */
    for (cnt = 0;cnt < dat_len;cnt++) {
    	if (tx_dat != 0) {
		    //データ書き込み
		    bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, (unsigned int)*tx_data);		// 0xA002_0068
    	    tx_data++;
    	} else
    	{
		    //空データ書き込み
		    bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, 0x00000000);		// 0xA002_0068
    	}
    	if (rx_dat == 0) {
    		rx_data = &data;		// ダミーバッファ設定
    	}
	    //データ読み出し
		if(bpf_hm_devc_qspi_flash_slave_data_read(rx_data) != 0) {
			printf(" RX FIFO timeout(Data read:%d/%d)(pl qspi) \n",cnt,(UINT)dat_len);
			rcode = BPF_HM_DEVC_NG;
		}
		rx_data++;
    }
    return rcode;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  FLASH起動面変更
 *  @note   FLASH起動面変更を行う。
 *  @param  upper  [in]   FLASH起動面
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_sect_set(unsigned int upper)
{
	// WRITE ENABLE コマンド
	bpf_hm_devc_qspi_flash_slave_write_en();

    // write expanded address register コマンド
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, BPF_HM_DEVC_PLQSPI_WRITE_EXT_ADR);		// 0xA002_0068
	//Configuration Register設定値
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, upper);				// 0xA002_0068
	//csイネーブル
    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定
	//ダミーリード 2回
	if(bpf_hm_devc_qspi_flash_slave_data_read_cycle(2) != 0) {
		printf(" RX FIFO timeout(sect set)(pl qspi) \n");
	}
	// CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_qspi_csend();
	//SPIシステムイネーブルをディセーブル
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_SYS_DISABLE);		// 0xA002_0060

    return BPF_HM_DEVC_COMPLETE;
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
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi create
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_read_part(unsigned char *buffer, unsigned int flash_address, unsigned int len)
{
    unsigned int tx_dat[8];
    unsigned int address1,address2;
    unsigned int cnt;

    /* quad output fast read コマンド */
    tx_dat[0] = 0x03;	//spi
    /* アドレス */
    address1 = flash_address >> 8;
    address2 = address1 >> 8;
    tx_dat[1] = address2 & 0x000000FF;
    tx_dat[2] = address1 & 0x000000FF;
    tx_dat[3] = flash_address & 0x000000FF;

    //SPIシステムイネーブルをイネーブル
    bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_TRANS_DISABLE);		// 0xA002_0060
	for (cnt = 0; cnt < 4; cnt++) {
		//readコマンド+アドレス用送信
		bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, tx_dat[cnt]);		// 0xA002_0068
	}
	//csイネーブル
    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定
	//ダミーリード 4回
	if(bpf_hm_devc_qspi_flash_slave_data_read_cycle(4) != 0) {
		printf(" RX FIFO timeout(Read command set)(pl qspi) \n");
	}

	//データリード
	if(bpf_hm_devc_qspi_flash_slave_pl_qspi(len, 0, buffer) != 0) {
		printf(" RX FIFO timeout(Data Read)(pl qspi) \n");
	}

	// CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_qspi_csend();
	//SPIシステムイネーブルをディセーブル
	bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_CNT, BPF_HM_DEVC_PLQSPI_CMD_SYS_DISABLE);		// 0xA002_0060

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  spi flash 読み込み（256単位）
 *  @note   spi flash 読み込みを256byteに分割して行う
 *  @param  buffer         [in]  書き込みバッファ
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_read_half(unsigned char *buffer, unsigned int flash_address, unsigned int len)
{
    unsigned long s_len,c_len;
    int rtn = BPF_HM_DEVC_COMPLETE;

	s_len = 0x100-(unsigned long)(flash_address % 0x100);	//最初の受信データ長設定（フラッシュの0x100番地まで）
	if (s_len > len) {
		s_len = len;				//全体データ長が短ければ、全体データ長分設定
	}
	for ( c_len =0; c_len < len; ) {
		if(bpf_hm_devc_qspi_flash_slave_read_part(buffer+c_len, flash_address+c_len, s_len) != 0) {
			rtn = BPF_HM_DEVC_NG;
		}
		c_len += s_len;				//受信済みデータ長更新
		if (len >= (c_len + 0x100)) {
			s_len = 0x100;			//受信データ最大256byte
		} else
		{
			s_len = len - c_len;	//残りサイズ
		}
	}

    return rtn;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 読み込み(MDL内部処理)
 *  @note   qspi flash 読み込みを行う(MDL内部処理)
 *  @param  buffer         [in]  書き込みバッファ
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_read_data(unsigned char *buffer,unsigned int flash_address, unsigned int len)
{
    unsigned int st_fadd,end_fadd,seg;
    unsigned int dat_len;
    unsigned char *st_buffer;

    st_fadd  = flash_address;
    end_fadd = flash_address + len - 1;
    st_buffer = buffer;

    /* 初期設定 */
    bpf_hm_devc_qspi_flash_slave_qspi_ini();

	for(seg = (flash_address >> 24);seg <= (end_fadd >> 24); seg++) {
		if(seg == (end_fadd >> 24)) {
			// 最後のブロックは残りサイズ
			dat_len = end_fadd - st_fadd + 1;
		} else
		{
			// 最初/途中のブロックは境界から先頭までのサイズ
			dat_len = 0x01000000 - (st_fadd & 0x00FFFFFF);
		}
        /* WDTタイマのクリア */
//        BPF_HM_DEVC_WDT_CLEAR();
		//アクセスエリア設定
        bpf_hm_devc_qspi_flash_slave_sect_set(seg);
		if(bpf_hm_devc_qspi_flash_slave_read_half(st_buffer,st_fadd,dat_len) != 0) {
			printf("PL qspi frash read NG segment:%d\n",(UINT)seg);
		}
		st_fadd += dat_len;		// フラッシュ読み出し先更新
		st_buffer += dat_len;		// バッファ書き込み先更新
	}

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
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_write_part(unsigned char *buffer,unsigned int flash_address, unsigned int len)
{
    unsigned int tx_dat[5];
    unsigned int address1,address2;
    unsigned int cnt;

    /* quad input fast program コマンド */
	tx_dat[0] = 0x02;  //spi

    /* アドレス */
    address1 = flash_address >> 8;
    address2 = address1 >> 8;
    tx_dat[1] = address2 & 0x000000FF;
    tx_dat[2] = address1 & 0x000000FF;
    tx_dat[3] = flash_address & 0x000000FF;
	tx_dat[4] = *(buffer) & 0x000000FF;		//FLASHへのWriteデータ(1byte目)
	buffer++;		//1byte目取り出したので更新
	len--;		//サイズも減算

	// WRITE ENABLE コマンド
	bpf_hm_devc_qspi_flash_slave_write_en();
    for(cnt = 0; cnt < 5; cnt++) {
		//writeコマンド+アドレス+データ1byte目送信
		bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, tx_dat[cnt]);		// 0xA002_0068
    }
	//csイネーブル
    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定
	//ダミーリード 5回
	if(bpf_hm_devc_qspi_flash_slave_data_read_cycle(5) != 0) {
		printf(" RX FIFO timeout(Write command set)(pl qspi) \n");
	}

	//データライト
	if(bpf_hm_devc_qspi_flash_slave_pl_qspi(len, buffer, 0) != 0) {
		printf(" RX FIFO timeout(Data Write)(pl qspi) \n");
	}

	// CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_qspi_csend();
	// Flag Status Register確認
	if (bpf_hm_devc_qspi_flash_slave_streg_chk() != 0) {
		printf(" RX FIFO timeout(Write command)(pl qspi) \n");
	}

    return BPF_HM_DEVC_COMPLETE;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 書き込み（256面単位）
 *  @note   qspi flash 書き込みを256byteに分割して行う
 *  @param  buffer         [in]  書き込みバッファ
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_write_half(unsigned char *buffer,unsigned int flash_address, unsigned int len)
{
    unsigned long s_len,c_len;
    int rtn = BPF_HM_DEVC_COMPLETE;

	s_len = 0x100-(unsigned long)(flash_address % 0x100);	//最初の送信データ長設定（フラッシュの0x100番地まで）
	if (s_len > len) {
		s_len = len;				//全体データ長が短ければ、全体データ長分設定
	}
	for ( c_len =0; c_len < len; ) {
		if(bpf_hm_devc_qspi_flash_slave_write_part(buffer+c_len, flash_address+c_len, s_len) != 0) {
			rtn = BPF_HM_DEVC_NG;
		}
		c_len += s_len;				//送信済みデータ長更新
		if (len >= (c_len + 0x100)) {
			s_len = 0x100;			//送信データ最大256byte
		} else
		{
			s_len = len - c_len;	//残りサイズ
		}
	}

    return rtn;
}
/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 書き込み(MDL内部処理)
 *  @note   qspi flash 書き込みを行う(MDL内部処理)
 *  @param  buffer         [in]  書き込みバッファ
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_write_data(unsigned char *buffer,unsigned int flash_address, unsigned int len)
{
    unsigned int st_flash_address,end_flash_address,seg;
    unsigned int dat_len;
    unsigned char *st_buffer;

    st_flash_address  = flash_address;
    end_flash_address = flash_address + len - 1;
	st_buffer = buffer;

    /* 初期設定 */
    bpf_hm_devc_qspi_flash_slave_qspi_ini();

	for(seg = (flash_address >> 24);seg <= (end_flash_address >> 24); seg++) {
		if(seg == (end_flash_address >> 24)) {
			// 最後のブロックは残りサイズ
			dat_len = end_flash_address - st_flash_address + 1;
		} else
		{
			// 最初/途中のブロックは境界から先頭までのサイズ
			dat_len = 0x01000000 - (st_flash_address & 0x00FFFFFF);
		}
        /* WDTタイマのクリア */
//        BPF_HM_DEVC_WDT_CLEAR();
		//アクセスエリア設定
        bpf_hm_devc_qspi_flash_slave_sect_set(seg);
		if(bpf_hm_devc_qspi_flash_slave_write_half(st_buffer,st_flash_address,dat_len) != 0) {
			printf("PL qspi frash write NG segment:%d\n",(UINT)seg);
		}
		st_flash_address += dat_len;		// フラッシュ書き込み先更新
		st_buffer += dat_len;		// バッファ読み出し先更新
	}

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash セクタ消去(セクタ単位)
 *  @note   qspi flash セクタ消去を行う。
 *  @param  sect_add  [in]  FLASHアドレス
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_sect_erase(unsigned int  sect_add)
{
    unsigned int tx_dat[5];
    unsigned int address0,address1,address2;
    unsigned int cnt;
    unsigned char data;

    /* FLAG STATUS register CLEAR コマンド */
    address2 = 0;
    address1 = 0;
    address0 = sect_add & 0x000000FF;

    tx_dat[0] = 0xD8;
    tx_dat[1] = address0;
    tx_dat[2] = address1;
    tx_dat[3] = address2;

	//フェールセーフ目的でダミーリード
	bpf_hm_devc_qspi_flash_slave_read_half(&data, (sect_add << 16),  1);

	// WRITE ENABLE コマンド
	bpf_hm_devc_qspi_flash_slave_write_en();
    for(cnt = 0; cnt < 4; cnt++) {
		//SECTOR ERASEコマンド+アドレス送信
		bpf_hm_devc_qspi_flash_slave_reg_write(BPF_HM_DEVC_PLQSPI_OFFSET_TX, tx_dat[cnt]);		// 0xA002_0068
    }
	//csイネーブル
    bpf_hm_devc_qspi_flash_slave_qspi_csstart();	// SS0固定
	//ダミーリード 4回
	if(bpf_hm_devc_qspi_flash_slave_data_read_cycle(4) != 0) {
		printf(" RX FIFO timeout(Erase command set)(pl qspi) \n");
	}

	// CS ディセーブルに設定
    bpf_hm_devc_qspi_flash_slave_qspi_csend();
	// Flag Status Register確認
	if (bpf_hm_devc_qspi_flash_slave_streg_chk() != 0) {
		printf(" RX FIFO timeout(Erase command)(pl qspi) \n");
	}

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 消去(面単位)
 *  @note   qspi flash 消去を行う。
 *  @param  sect_add  [in]  FLASHアドレス
 *  @param  len       [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_erase_half(unsigned int add, unsigned int len)
{
    unsigned int address1,address2;
    int          rtn;
    unsigned int cnt;

    address1 = add >> 16;
    address2 = (add + len - 1) >> 16;

    for(cnt=address1;cnt<=address2;cnt++) {
        rtn = bpf_hm_devc_qspi_flash_slave_sect_erase (cnt);
        if (rtn != BPF_HM_DEVC_COMPLETE)
        {
            return rtn;
        }
    }

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  qspi flash 消去(MDL内部処理)
 *  @note   qspi flash 消去を行う。(MDL内部処理)
 *  @param  sect_add  [in]  FLASHアドレス
 *  @param  len       [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/04/02 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 */
/********************************************************************************************************************/
int bpf_hm_devc_qspi_flash_slave_erase_data(unsigned int add, unsigned int len)
{
    unsigned int st_flash_address,end_flash_address,seg;
    unsigned int dat_len;

    st_flash_address = add;
    end_flash_address = add + len - 1;

    /* 初期設定 */
    bpf_hm_devc_qspi_flash_slave_qspi_ini();

	for(seg = (add >> 24);seg <= (end_flash_address >> 24); seg++) {
		if(seg == (end_flash_address >> 24)) {
			// 最後のブロックは残りサイズ
			dat_len = end_flash_address - st_flash_address + 1;
		} else
		{
			// 最初/途中のブロックは境界から先頭までのサイズ
			dat_len = 0x01000000 - (st_flash_address & 0x00FFFFFF);
		}
        /* WDTタイマのクリア */
//        BPF_HM_DEVC_WDT_CLEAR();
		//アクセスエリア設定
        bpf_hm_devc_qspi_flash_slave_sect_set(seg);
		if(bpf_hm_devc_qspi_flash_slave_erase_half(st_flash_address,dat_len) != 0) {
			printf("PL qspi frash erase NG segment:%d\n",(UINT)seg);
		}
		st_flash_address += dat_len;		// フラッシュ消去先更新
	}

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続 qspi flash 読み込み(API-IF)
 *  @note   PL接続 qspi flash 読み込みを行う(API-IF)
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @param  buffer         [in]  書き込みバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/05/23 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ(unsigned int flash_address, unsigned int len,unsigned char *buffer)
{
    int          rtn;
    pthread_mutex_t  *mutex;         /* mutexオブジェクト    */

    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ flash_address = %08x , len = %08x" ,
                        flash_address,len);

    /* pointer check */
    if( buffer == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "buffer == NULL");
        return BPF_HM_DEVC_NG;
    }

    mutex = &di_devc_data_p->info[BPF_HM_DEVC_SLAVE_FLASH].mutex;                  /* mutexオブジェクト      */

    /* Mutex lock */
    rtn = bpf_hm_devc_mutex_lock(mutex);
    if( rtn != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }

	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_slave_reg_info();
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ_S,flash_address,len,1,0);

    /* spi flash 書き込み処理を行う */
    rtn = bpf_hm_devc_qspi_flash_slave_read_data(buffer,flash_address,len);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_qspi_flash_slave_READ NG rtn = %d",rtn);

        return rtn;
    }

	BPF_COM_PLOG_QSPI(E_QSPI_LV_READ,flash_address,len,1,0);
	
    pthread_mutex_unlock(mutex);
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_READ COMPLETE");

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続 qspi flash 書き込み(API-IF)
 *  @note   PL接続 qspi flash 書き込みを行う(API-IF)
 *  @param  flash_address  [in]  FLASHアドレス
 *  @param  len            [in]  データ長
 *  @param  buffer         [in]  書き込みバッファ
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/05/23 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE(unsigned int flash_address, unsigned int len,unsigned char *buffer)
{
    int          rtn;
    pthread_mutex_t  *mutex;         /* mutexオブジェクト    */

    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE flash_address = %08x , len = %08x" ,
                        flash_address,len);

    /* pointer check */
    if( buffer == NULL )
    {
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "buffer == NULL");
        return BPF_HM_DEVC_NG;
    }

    mutex = &di_devc_data_p->info[BPF_HM_DEVC_SLAVE_FLASH].mutex;                  /* mutexオブジェクト      */

    /* Mutex lock */
    rtn = bpf_hm_devc_mutex_lock(mutex);
    if( rtn != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }

	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_slave_reg_info();
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE_S,flash_address,len,1,0);

    /* spi flash 書き込み処理を行う */
    rtn = bpf_hm_devc_qspi_flash_slave_write_data(buffer,flash_address,len);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE NG rtn = %d",rtn);

        return rtn;
    }

	BPF_COM_PLOG_QSPI(E_QSPI_LV_WRITE,flash_address,len,1,0);

    pthread_mutex_unlock(mutex);
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_WRITE COMPLETE");

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */

/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続 qspi flash 消去(API-IF)
 *  @note   PL接続 qspi flash 消去を行う。
 *  @param  sect_add  [in]  FLASHアドレス
 *  @param  len       [in]  データ長
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2013/04/30 ALPHA)Matsuhashi create
 *  @date   2014/05/23 HID)Iwabuchi modify for Dual
 *  @date   2014/12/03 ALPHA)Yokoyama Dual/Tri から docomoへ移植
 *  @date   2015/08/25 ALPHA)Miyazaki    TDD_RRE対応
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE(unsigned int add, unsigned int len)
{
    int          rtn;
    pthread_mutex_t  *mutex;         /* mutexオブジェクト    */

    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE flash_address = %08x , len = %08x" ,
                        add,len);

    mutex = &di_devc_data_p->info[BPF_HM_DEVC_SLAVE_FLASH].mutex;                  /* mutexオブジェクト      */

    /* Mutex lock */
    rtn = bpf_hm_devc_mutex_lock(mutex);
    if( rtn != 0 ){

        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "pthread_mutex_lock mutex = %08x, error = %d",
            mutex,errno);

        return BPF_HM_DEVC_MUTEX_NG;
    }
	
	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_slave_reg_info();
	
	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE_S,add,len,1,0);

    /*  spi flash 消去(処理を行う */
    rtn = bpf_hm_devc_qspi_flash_slave_erase_data(add,len);
    if (rtn != BPF_HM_DEVC_COMPLETE)
    {
        pthread_mutex_unlock(mutex);
        bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
            "BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE NG rtn = %d",rtn);

        return rtn;
    }

	BPF_COM_PLOG_QSPI(E_QSPI_LV_ERASE,add,len,1,0);

    pthread_mutex_unlock(mutex);
    bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_ERASE COMPLETE");

    return BPF_HM_DEVC_COMPLETE;
}

/* @} */


/* @{ */
/********************************************************************************************************************/
/**
 *  @brief  PL接続 qspi flash 初期化
 *  @note   PL接続 qspi flash 初期化を行う。
 *  @param  N/A
 *  @return result code
 *  @retval BPF_HM_DEVC_COMPLETE
 *  @retval BPF_HM_DEVC_NG
 *  @date   2018/5/18 Taniguchi create
 */
/********************************************************************************************************************/
int BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT(void)
{
	int          rtn;
	pthread_mutex_t  *mutex;         /* mutexオブジェクト    */

	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT ");

	mutex = &di_devc_data_p->info[BPF_HM_DEVC_SLAVE_FLASH].mutex;                  /* mutexオブジェクト      */

	/* Mutex lock */
	if( (rtn = bpf_hm_devc_mutex_lock(mutex)) != 0 ){
	
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,
		    "pthread_mutex_lock mutex = %08x, error = %d",
		    mutex,errno);
		
		return BPF_HM_DEVC_MUTEX_NG;
	}
	
	/* SLAVE-FLASHのレジスタ情報を取得する	*/
	bpf_hm_devc_qspi_flash_slave_reg_info();
	/* 初期設定 */
	bpf_hm_devc_qspi_flash_slave_qspi_ini();

	pthread_mutex_unlock(mutex);
	bpf_com_dbglog_fmt(BPF_COM_LOG_TRACE,"BPF_HM_DEVC_QSPI_FLASH_SLAVE_INIT COMPLETE");
	return BPF_HM_DEVC_COMPLETE;
}
/* @} */
/* @} */

/* RRH-001 MD alpha)matsuhashi add end */


