
#include "f_dpda_inc.h"
#include "BPF_HM_DEVC.h"


/*!
 * @brief 関数機能概要:DPD ON設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_com_setDpdStart(unsigned int uiAntNum , unsigned int failSafeFlg )
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_com_setDpdStart called, uiAntNum = %d failSafeFlg = %d", uiAntNum,failSafeFlg);

    unsigned int uiSetAddr = 0xB1202000;
    unsigned int uiSetData = 0;
    unsigned int uiAutoCr;
    int iRtnBpf;

    iRtnBpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA read error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
        return D_DPDA_RTN_NG;
    }
    //はAnt間共通レジスタのため、オートクルーズ部分は自Ant以外の設定値は保持し自AntのみBit立てる
    uiAutoCr = (uiSetData & 0x0000FFF0) | (0x00000001 << (uiAntNum + 8));

	if( D_SYS_ON == f_dpdaw_info_all->taskInfo.flg.dbgmode ){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_com_setDpdStart skip, uiAntNum = %d", uiAntNum);
		uiSetData = uiAutoCr;
	}else {
		uiSetData = 0x00000001 | uiAutoCr;
	}
     //AntA～DのLUT初期化
     uiSetData = uiSetData | (0x00000001 << (uiAntNum + 24));

    //AntA～Dの高速モード移行
    uiSetData = uiSetData | (0x00000001 << (uiAntNum + 4));

    iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
        return D_DPDA_RTN_NG;
    }

	if(failSafeFlg == D_RRH_OFF){
		/* FSDORPT clear */
		uiSetAddr = 0xB1203020 + (uiAntNum * 4);

		uiSetData = 0;
		(void)BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, uiSetAddr, &uiSetData);

		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "FSDORPT Clear(add:0x%08x val:0x%08x -> 0) uiAntNum = %d",uiSetAddr,uiSetData , uiAntNum );

		uiSetData = 0;
		(void)BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr , &uiSetData);
	}

    return D_DPDA_RTN_COMPLETE;
}

/*!
 * @brief 関数機能概要:DPD停止設定
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~7:ANT.H)
 * @return int
 * @date 2018/09/10 KCN)吉木 Create
 */
int f_dpda_com_setDpdStop(unsigned int uiAntNum)
{
    BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_com_setDpdStop called, uiAntNum = %d", uiAntNum);

    unsigned int uiSetAddr = 0xB1202000;
    unsigned int uiSetData = 0;
    unsigned int uiSetBit;
    unsigned int uiAutoCrJdg;
    int iRtnBpf;

    iRtnBpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA read error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
        return D_DPDA_RTN_NG;
    }

    uiSetBit = 0x00000001 << ( 8 + uiAntNum);
    uiAutoCrJdg = (uiSetData & (~uiSetBit)) & 0x0000FF00;

//    if(uiAutoCrJdg == 0x00000000)
    if(uiAutoCrJdg == 0x00000000 || D_SYS_ON == f_dpdaw_info_all->taskInfo.flg.dbgmode)
    {
        uiSetData = (uiAutoCrJdg) | (uiSetData & 0x000000FE);
    }
    else
    {
        uiSetData = (uiAutoCrJdg | 0x00000001) | (uiSetData & 0x000000FE);
    }

    iRtnBpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, uiSetAddr, &uiSetData);
    if(iRtnBpf != BPF_HM_DEVC_COMPLETE)
    {
        BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "FPGA write error=0x%08x, addr = %08x", iRtnBpf, uiSetAddr);
        return D_DPDA_RTN_NG;
    }


    return D_DPDA_RTN_COMPLETE;
}

/*!
 * @brief 関数機能概要:DPD Fail Safe LUT log save
 * @note  関数処理内容:DPD Fail Safe LUT log save
 * @param  N/A
 * @return N/A
 * @date 2019/07/16 Taniguchi
 */
VOID f_dpda_com_setDpdFailSafe_lutlogsave(UINT uiAntNum)
{
	UINT								uiSetData;
	UINT								uiAntCnt;
	INT									rtn_bpf;
	INT									errcd;
	struct bpf_timespec					dtime;	/* sem waittime */	
	T_SYS_NMA_LUTLOG_WRITE_NFT*			sendMsg;
	UINT								uiIndex;
	T_RRH_LUT_LOG_DATA*					data_p;

	dtime.tv_sec = 1;
	dtime.tv_nsec = 0;


	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s called",__func__);

	/* LUT取得 */
	rtn_bpf = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != rtn_bpf){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "SEM ERROR %d",errcd );
		return;
	}

	uiIndex = f_dpdaw_lut_log->head.index;
	if(uiIndex >= D_DU_LUT_LOG_MAX){
		uiIndex = 0;
	}

	/* FLASH書き込み前に4回以上発生したに違いない */
	if( f_dpdaw_lut_log->head.flash_head[uiIndex].flash_saving == D_RRH_ON ){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "Skip LUT Log save %d",uiIndex );
		(void)BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, &errcd);
		return;
	}

	f_dpdaw_lut_log->head.flash_head[uiIndex].flash_saved = D_RRH_OFF;

	data_p = &(f_dpdaw_lut_log->log[uiIndex]);
	for(uiAntCnt=0 ; uiAntCnt < f_dpdaw_ant_max_num ; uiAntCnt++){
	
		uiSetData = 0x3;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_PD_PDMEMKEY(uiAntCnt), &uiSetData);
		/* 0xB1220000-0xB122FFFF */
		(void)BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_LUT_BASE,sizeof(data_p->lut_data[uiAntCnt][0]), (UINT *)&(data_p->lut_data[uiAntCnt][0][0])) ;
		uiSetData = 0x0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_PD_PDMEMKEY(uiAntCnt), &uiSetData);
	
		uiSetData = 0x1;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_PD_PDMEMKEY(uiAntCnt), &uiSetData);
		/* 0xB1220000-0xB122FFFF */
		(void)BPF_HM_DEVC_REG_READ_BUFFER(0, D_DU_REG_LUT_BASE,sizeof(data_p->lut_data[uiAntCnt][1]), (UINT *)&(data_p->lut_data[uiAntCnt][1][0])) ;
		uiSetData = 0x0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_PD_PDMEMKEY(uiAntCnt), &uiSetData);
	
		/* 0xB1200XC0 */
		BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, M_DU_REG_PD_DLTOFSTMON(uiAntCnt), &(data_p->log_info[uiAntCnt]));
	}
	
	if( f_cmw_tra_inv_tbl->cmx_bootup_face == D_RRH_BOOT_ACT0 ){
		data_p->version = f_cmw_tra_inv_tbl->cmx_f0_sver[0];
	}
	else{
		data_p->version = f_cmw_tra_inv_tbl->cmx_f1_sver[0];
	}
	data_p->antOfs = uiAntNum;
	(VOID)BPF_HM_DEVC_GETTIME(&(data_p->date));

	f_dpdaw_lut_log->head.flash_head[uiIndex].flash_saved = f_comw_rrhVersion;

	rtn_bpf = BPF_RU_IPCM_PROCMSG_ADDRGET( E_BPF_RU_IPCM_BUFF_KIND_TASK,sizeof(T_SYS_NMA_LUTLOG_WRITE_NFT),(VOID **)&sendMsg,&errcd );
	if( rtn_bpf == D_SYS_OK ){
		sendMsg->thdIf.uiEventNo		= D_SYS_MSGID_NMAS_LUTLOG_WRITE_NFT;
		sendMsg->thdIf.uiDstPQueueID	= D_RRH_PROCQUE_PF;
		sendMsg->thdIf.uiDstTQueueID	= D_SYS_THDQID_PF_NMAS;
		sendMsg->thdIf.uiSrcPQueueID	= D_RRH_PROCQUE_PF;
		sendMsg->thdIf.uiSrcTQueueID	= D_SYS_THDQID_PF_CCA;
		sendMsg->thdIf.uiLength			= sizeof(T_SYS_NMA_LUTLOG_WRITE_NFT);
		sendMsg->uiIndex				= uiIndex;

		f_dpdaw_lut_log->head.flash_head[uiIndex].flash_saving = D_RRH_ON;

		rtn_bpf = BPF_RU_IPCM_MSGQ_SEND( D_SYS_THDQID_PF_NMAS ,(void *)sendMsg );
		if(rtn_bpf != D_SYS_OK){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Send Error Skip LUT Log save %d",uiIndex );

			f_dpdaw_lut_log->head.flash_head[uiIndex].flash_saving = D_RRH_OFF;

		}else{
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Send LUT Log save %d",uiIndex );

			/* index 更新 */
			f_dpdaw_lut_log->head.index = (uiIndex + 1) % D_DU_LUT_LOG_MAX;

		}
	}else{
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "Buff error LUT Log save %d",uiIndex );
	}

	(void)BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_LUT_LOG,BPF_RU_IPCM_LOCK_RW, &errcd);

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_RETURN , "%s return(%d)",__func__,errcd);
	return;
}

/*!
 * @brief 関数機能概要:DPD Fail Safe setting
 * @note  関数処理内容.
 * @param  uiAntNum     [in]unsigned int    アンテナ番号(0:ANT.A~3:ANT.D)
 * @return N/A
 * @date 2019/07/16 Taniguchi
 */
VOID f_dpda_com_setDpdFailSafe(UINT uiAntNum)
{

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s called ANT-%d",__func__, uiAntNum);

	f_dpda_com_setDpdStop(uiAntNum);

	f_comw_rf_setup_flg = D_RRH_OFF;	/* 1秒以上かかりそうなので1sec割り込み抑止 */
	/* LUT log save */
	f_dpda_com_setDpdFailSafe_lutlogsave(uiAntNum);
	f_comw_rf_setup_flg = D_RRH_ON;

	f_dpda_com_setDpdStart(uiAntNum,D_RRH_ON);

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_RETURN , "%s return ANT-%d",__func__, uiAntNum);

	return;
}


