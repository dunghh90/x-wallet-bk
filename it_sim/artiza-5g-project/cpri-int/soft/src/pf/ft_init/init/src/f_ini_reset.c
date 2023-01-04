/*!
 * @skip  $ld:$
 * @file  f_ini_reset.c
 * @brief リセット処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/06/05 ALPHA) tomioka Create RRH-007-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_ini_inc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

/*!
 *	@brief	force reg write
 *	@param N/A
 *	@return N/A
 *	@date  2019/03/13 FJT)taniguchi create
*/
static VOID f_ini_force_regwrite( UINT addr , UINT data)
{
	int fd;
	void *ptr;
	unsigned page_addr;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	if( ( fd = open("/dev/mem",O_RDWR) ) == -1){
		return;
	}
	page_addr   = (addr & ~(page_size-1));
	if((ptr = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,page_addr)) == (void*)-1 ){	/* pgr0203 */
		close(fd);
		return;
	}
	*((unsigned *)(ptr + (addr - page_addr))) = data;
	msync( ptr , page_size , MS_SYNC );
	munmap(ptr,page_size);
	close(fd);
	return;
}

/*!
 *	@brief	force reg read
 *	@param N/A
 *	@return N/A
 *	@date  2019/09/05 FJT)taniguchi create
*/
static UINT f_ini_force_read( UINT addr )
{
	int fd;
	void *ptr;
	unsigned page_addr;
	unsigned page_size=sysconf(_SC_PAGESIZE);
	unsigned regval;
	UINT data;

	if( ( fd = open("/dev/mem",O_RDWR) ) == -1){
		return 0xFFFFFFFF;
	}
	page_addr   = (addr & ~(page_size-1));
	if((ptr = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,page_addr)) == (void*)-1 ){	/* pgr0203 */
		close(fd);
		return 0xFFFFFFFF;
	}
	msync( ptr , page_size , MS_SYNC );
	regval = *((unsigned *)(ptr + (addr - page_addr)));
	data = (UINT)regval;
	munmap(ptr,page_size);
	close(fd);
	return data;
}

/*!
 *	@brief	reset time save
 *	@param N/A
 *	@return N/A
 *	@date  2015/04/22 FJT)taniguchi create for TDD
*/
static VOID f_ini_saveSysTime( VOID* buf_p )
{
	T_RRH_SYSTEM_TIME		tDateTime;
	UCHAR*					time_p;
	UINT					count;
	UINT					offset;
	T_RRH_TRAINVTBL			*invTbl;
	int						errcd;

	(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
	if(invTbl == NULL){
		cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_CRITICAL, "Sharedmem get errcd=%x",errcd);
		return;
	}
	(VOID)BPF_HM_DEVC_GETTIME(&tDateTime);
	tDateTime.year = htons(tDateTime.year);	/* 2byteなのでendian変換 */
	time_p = (UCHAR*)&tDateTime;
	for( count = 0 , offset = D_DU_EEP_TIMESAVE_YEAR1 ; offset <= D_DU_EEP_TIMESAVE_10MSEC ; count++ , offset++ , time_p++ ){
		/* 必要な部分だけ更新する */
		if(invTbl->timesaveinfo[count] == *time_p){
			continue;
		}
		BPF_HM_DEVC_EEPROM_WRITE(offset,time_p);
		invTbl->timesaveinfo[count] = *time_p;
	}
	if(invTbl->timesave_flg != D_RRH_ON){
		invTbl->timesave_flg = D_RRH_ON;
		(VOID)BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_TIMESAVE_FLG, &(invTbl->timesave_flg));
	}
	return;
}
/* @} */

/*!
 *	@brief	all ant stop wave
 *	@param N/A
 *	@return N/A
 *	@date  2018/09/14 FJT)koshida create
*/
VOID f_ini_wvStp_allAnt(VOID)
{
	INT						rtn_bpf;
	UCHAR					device_kind0;
	UINT					regw_data = 0x01;
	
	rtn_bpf = BPF_HM_DEVC_EEPROM_READ(D_RRH_EEP_DEVICE_KIND0, &device_kind0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE)
	{
		cmn_com_assert_th_get(f_iniw_assert_main_p, D_RRH_LOG_AST_LV_ERROR, "Err<0x%08x>:rrhkind0 get form EEPROM", rtn_bpf);
		return;
	}
	switch(device_kind0)
	{
		case D_DU_EEP_DEV_KIND_5GDU_SUB6:
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_TCX_TDANTENA, &regw_data);
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_TCX_TDANTENB, &regw_data);
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_TCX_TDANTENC, &regw_data);
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_TCX_TDANTEND, &regw_data);
			break;
		case D_DU_EEP_DEV_KIND_5GDU_MMW:
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_TCX_TDANTENA, &regw_data);
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, D_DU_REG_TCX_TDANTENB, &regw_data);
			break;
		default:
			cmn_com_assert_th_get(f_iniw_assert_main_p, D_RRH_LOG_AST_LV_ERROR, ":rrhkind0 is unkown<0x%x>", device_kind0);
			return;
			break;
	}
	return;
}
/* @} */

/*!
 *	@brief	reset message
 *	@param N/A
 *	@return N/A
 *	@date  2019/11/29 FJT)Taniguchi create
*/
VOID f_ini_resetMsg(UINT resetReason)
{
	UINT	regw_data;
	CHAR*	reason_str;

	BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, D_DU_REG_CTB_SWWORK_DPMODE, &regw_data );
	if((regw_data & 0x00010001) != 0){
		f_com_dumpMsgInit();
		switch(resetReason){
			case D_RRH_RST_RSN_REQ:
				reason_str = "MPLANE-TRIGGERED-RESTART";
				break;
			case D_RRH_RST_RSN_LINK_TIMEOUT:
				reason_str = "SUPERVISION-WATCHDOG";
				break;
			case D_RRH_RST_RSN_CTLLINK:
				reason_str = "MPLANE-RECOVERY-RESTART BY Fronthaul L1 DOWN";
				break;
			case D_RRH_RST_RSN_MMI:
				reason_str = "MAINTENANCE-RESTART";
				break;
			case D_RRH_RST_RSN_WDT:
				reason_str = "OTHER-WATCHDOG-TIMER";
				break;
			case D_RRH_RST_RSN_SFT_FAIL:
			case D_RRH_RST_RSN_PROCESS_DEAD:
			case D_RRH_RST_RSN_SWRST1:
				reason_str = "SOFTWARE-FAILURE";
				break;
			case D_RRH_RST_RSN_UNKNWN:
			case D_RRH_RST_RSN_POWER:
				reason_str = "POWER-ON";
				break;
			default:
				reason_str = "UNKNOWN";
				break;
		}
		f_com_dumpMsgEventSend("Shut-down by Reset(%s:0x%x)",reason_str,resetReason);
	}
	return;
}
/* @} */

/*!
 * @brief f_ini_reset
 * @note  関数処理内容.
 *		-# ResetReasonの設定を行う。
 *		-# ログのFLASH退避を行う。
 *		-# リセット制御を行う。
 * @param		resetReason			[in] リセット要因
 * @param		addInfo				[in] リセット要因付加情報
 * @param		uiCompelResetFlg	[in] 強制リセットフラグ
 * @param		uiResetFlg			[in] 実際にRESETが必要か否かのフラグ
 * @warning		N/A
 * @FeatureID	PF-Init-001-002-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2013/07/03 ALPHA)fujii Create
 * @date 2013/11/07 ALPHA)nakamura modify for ZYNQ
 * @date 2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date 2015/04/22 FJT) Taniguchi Create RRH-001-000 TDD対応
 * @date 2015/06/05 ALPHA) tomioka Create RRH-007-000 TDD対応
 */
VOID f_ini_reset( UINT resetReason,  UINT addInfo,  VOID* buf_p )
{
	UCHAR reset_ctlflag = D_RRH_SOFT_RESET_NORMAL;
	UINT  reset_val     = 0x0000a55a;
	UINT  regw_addr = D_DU_REG_CTB_SRSTREQ;
	UCHAR	reset_msk;
	UINT	resetValHis;

	T_RRH_TRAINVTBL			*invTbl;
	int						errcd;


	cmn_com_assert_th_get(buf_p, D_RRH_LOG_AST_LV_CRITICAL, "ENTER resetReason=%d addInfo=%08x ",resetReason,addInfo);

	BPF_COM_LOG_DMESG("Starting SW Reset(0x%x) ...\n",resetReason);
	f_ini_resetMsg(resetReason);

	switch(resetReason){
		case D_RRH_RST_RSN_PROCESS_DEAD:
		case D_RRH_RST_RSN_WDT:
		case D_RRH_RST_RSN_SFT_FAIL:
		case D_RRH_RST_RSN_SWRST1:
		case D_RRH_RST_RSN_SWRST2:
		case D_RRH_RST_RSN_ALMRST:
		case D_RRH_RST_RSN_IMG_FAIL:
		case D_RRH_RST_RSN_HRD_FAIL:
			(VOID)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_INVENTORY, (VOID **)&invTbl, &errcd);
			reset_ctlflag = D_RRH_SOFT_RESET_ALM;
			regw_addr = D_DU_REG_OTH_RESET_SOFT;
			reset_val = D_DU_REG_OTH_RESET_SOFT_CTL;
			/* リセット履歴更新前に死亡 */
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, D_DU_REG_OTH_RESET_REASON, &resetValHis);
			if((resetValHis & D_DU_REG_OTH_RESET_BIT_SOFT) != 0 ){
				BPF_COM_LOG_DMESG("SW Reset(0x%x) SKIP1 ...\n",resetValHis);
				reset_val = 0;
			}else{
				if(invTbl->almreset_flag[0] == D_RRH_ON){
					BPF_COM_LOG_DMESG("SW Reset(0x%x) SKIP2 ...\n",resetValHis);
					reset_val = 0;
				}
			}
			break;
		case D_RRH_RST_RSN_CTLLINK:
			reset_ctlflag = D_RRH_SOFT_RESET_MPLINKDOWN;
			break;
		case D_RRH_RST_RSN_REQ:
			reset_ctlflag = D_RRH_SOFT_RESET_CU;
			break;
		case D_RRH_RST_RSN_LINK_TIMEOUT:
			reset_ctlflag = D_RRH_SOFT_RESET_MPWDT;
			break;
		case D_RRH_RST_RSN_MMI:
		default:
			reset_ctlflag = D_RRH_SOFT_RESET_NORMAL;
			break;
	}

	/********************************************/
	/* リセット要因保持							*/
	/********************************************/
	BPF_COM_LOG_RST_REASON(f_comw_rrhVersion, resetReason, addInfo );
	/* RESET認識有りに設定する	*/
	BPF_HM_DEVC_EEPROM_WRITE(D_DU_EEP_SOFT_RESET_FLG, &reset_ctlflag);

	/* サイクリックログ退避	*/
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "CYCLIC LOG SAVE START");
	f_com_logSaveFlash( D_RRH_OFF );
	cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_INFO, "CYCLIC LOG SAVE END");

	/********************************************/
	/* リセット時刻保存							*/
	/********************************************/	
	f_ini_saveSysTime(buf_p);

	BPF_HM_DEVC_EEPROM_READ(D_DU_EEP_SOFT_RESET_MSK, &reset_msk);
	if(reset_msk != D_RRH_ON){
		if(reset_val != 0){
			f_ini_force_regwrite(regw_addr,reset_val);
		}
		else{
			cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_WARNING, "Skip SW Reset(0x%x)",resetReason);
			f_ini_led_alm(buf_p);
			f_ini_wvStp_allAnt();
			BPF_HM_DEVC_WDT_SET( D_SYS_SWDT_TIMER_CLR, D_SYS_SWDT_TIMER_CLR );
		}
	}else{
		BPF_COM_LOG_DMESG("SW Reset Mask(0x%x) ...\n",resetReason);
		BPF_HM_DEVC_WDT_SET( D_SYS_SWDT_TIMER_CLR, D_SYS_SWDT_TIMER_CLR );
	}

	/* 念のためやっておく */
	if((errcd = system("/opt/sh/cmd_iptables 1>/dev/null 2>/dev/null")) != D_RRH_OK ){
		cmn_com_assert_th_get(buf_p,D_RRH_LOG_AST_LV_WARNING, "iptables setting error(%d)",errcd);
	}

	/* リセットされない場合は強制的にether起動する */
	BPF_COM_LOG_DMESG("ifconfig force start(%d)  ...\n",reset_msk);
	errcd = system("ifconfig eth0 192.168.1.131 up");
	if(errcd != D_RRH_OK){
		BPF_COM_LOG_DMESG("ifconfig error(0x%x) ...\n",errcd);
	}

	if( reset_msk != D_RRH_ON ){
		/* 5GではInband Resetがないため、連続２回でALM保持し続けると、永久にリセットされず鉄塔で漬物石が出来上がる可能性がある。*/
		/* そのため、最大48時間のガード時間をもち、それを超える場合は自律リセットとすることで漬物石を回避する                   */
		/* 48時間という時間はLTEの考え方を踏襲している                                                                          */
		INT guard_count = D_RRH_ALM_SELFRESET_TIMECOUNT * 3600;	/* 48h */
		UINT	lnk_regval_cur;
		UINT	lnk_regval_pre;

		lnk_regval_pre = f_ini_force_read(D_DU_REG_ETH_IRQMACSTI);
		BPF_COM_LOG_DMESG("Starting Self-Reset guard timer(%dhour) fhl1:0x%x ...\n",guard_count/3600,lnk_regval_pre);
		while(1){
			sleep(1);
			guard_count--;
			lnk_regval_cur = f_ini_force_read(D_DU_REG_ETH_IRQMACSTI);
			/* FH LINK状態がdownからupに変化があったら即リセット(CU側再開後、Linkupした場合を想定)する */
			if(((lnk_regval_pre & D_RRH_ON) != 0) && ((lnk_regval_cur & D_RRH_ON) == 0)){
				/* LINK Down -> LINK up */
				BPF_COM_LOG_DMESG("...Force Time out(%dh.%ds) fhl1:0x%x -> 0x%x and Reset ...\n",guard_count / 3600,guard_count % 3600,lnk_regval_pre,lnk_regval_cur);
				break;
			}else{
				/* LINK up -> LINK down   : 交換入ったのかな? */
				/* LINK up -> LINK up     : ALM継続中 */
				/* LINK Down -> LINK Down : 交換中かな? */
				if(guard_count <= 0 ){
					BPF_COM_LOG_DMESG("...Time out(%d) fhl1:0x%x , 0x%x and Reset ...\n",guard_count,lnk_regval_pre,lnk_regval_cur);
					break;
				}
			}
			lnk_regval_pre = lnk_regval_cur;
		}
		f_ini_force_regwrite(D_DU_REG_OTH_RESET_SOFT,D_DU_REG_OTH_RESET_SOFT_CTL);
	}

	return;
}
/* @} */
