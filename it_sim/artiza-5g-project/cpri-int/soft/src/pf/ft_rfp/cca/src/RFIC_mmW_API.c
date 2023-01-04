/**
 * \file RFIC_TALISE_API.c
 *
 */


#include "f_dpda_inc.h"
#include <math.h>

static UINT	 RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG1_MMW;
/********************************
 * brief  : mmW向けRF PLL 設定
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_RF_PLL(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_RF_PLL");
	
	unsigned int reg_addr[RFIC_MMW_RF_PLL_DATA_NUM];
	unsigned int data[RFIC_MMW_RF_PLL_DATA_NUM];
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int i;
	
	reg_addr[0] = 0x0000;
	reg_addr[1] = 0x0000;
	reg_addr[2] = 0x0070;
	reg_addr[3] = 0x006F;
	reg_addr[4] = 0x006E;
	reg_addr[5] = 0x006D;
	reg_addr[6] = 0x006C;
	reg_addr[7] = 0x006B;
	reg_addr[8] = 0x006A;
	reg_addr[9] = 0x0069;
	reg_addr[10] = 0x0068;
	reg_addr[11] = 0x0067;
	reg_addr[12] = 0x0066;
	reg_addr[13] = 0x0065;
	reg_addr[14] = 0x0064;
	reg_addr[15] = 0x0063;
	reg_addr[16] = 0x0062;
	reg_addr[17] = 0x0061;
	reg_addr[18] = 0x0060;
	reg_addr[19] = 0x005F;
	reg_addr[20] = 0x005E;
	reg_addr[21] = 0x005D;
	reg_addr[22] = 0x005C;
	reg_addr[23] = 0x005B;
	reg_addr[24] = 0x005A;
	reg_addr[25] = 0x0059;
	reg_addr[26] = 0x0058;
	reg_addr[27] = 0x0057;
	reg_addr[28] = 0x0056;
	reg_addr[29] = 0x0055;
	reg_addr[30] = 0x0054;
	reg_addr[31] = 0x0053;
	reg_addr[32] = 0x0052;
	reg_addr[33] = 0x0051;
	reg_addr[34] = 0x0050;
	reg_addr[35] = 0x004F;
	reg_addr[36] = 0x004E;
	reg_addr[37] = 0x004D;
	reg_addr[38] = 0x004C;
	reg_addr[39] = 0x004B;
	reg_addr[40] = 0x004A;
	reg_addr[41] = 0x0049;
	reg_addr[42] = 0x0048;
	reg_addr[43] = 0x0047;
	reg_addr[44] = 0x0046;
	reg_addr[45] = 0x0045;
	reg_addr[46] = 0x0044;
	reg_addr[47] = 0x0043;
	reg_addr[48] = 0x0042;
	reg_addr[49] = 0x0041;
	reg_addr[50] = 0x0040;
	reg_addr[51] = 0x003F;
	reg_addr[52] = 0x003E;
	reg_addr[53] = 0x003D;
	reg_addr[54] = 0x003C;
	reg_addr[55] = 0x003B;
	reg_addr[56] = 0x003A;
	reg_addr[57] = 0x0039;
	reg_addr[58] = 0x0038;
	reg_addr[59] = 0x0037;
	reg_addr[60] = 0x0036;
	reg_addr[61] = 0x0035;
	reg_addr[62] = 0x0034;
	reg_addr[63] = 0x0033;
	reg_addr[64] = 0x0032;
	reg_addr[65] = 0x0031;
	reg_addr[66] = 0x0030;
	reg_addr[67] = 0x002F;
	reg_addr[68] = 0x002E;
	reg_addr[69] = 0x002D;
	reg_addr[70] = 0x002C;
	reg_addr[71] = 0x002B;
	reg_addr[72] = 0x002A;
	reg_addr[73] = 0x0029;
	reg_addr[74] = 0x0028;
	reg_addr[75] = 0x0027;
	reg_addr[76] = 0x0026;
	reg_addr[77] = 0x0025;
	reg_addr[78] = 0x0024;
	reg_addr[79] = 0x0023;
	reg_addr[80] = 0x0022;
	reg_addr[81] = 0x0021;
	reg_addr[82] = 0x0020;
	reg_addr[83] = 0x001F;
	reg_addr[84] = 0x001E;
	reg_addr[85] = 0x001D;
	reg_addr[86] = 0x001C;
	reg_addr[87] = 0x001B;
	reg_addr[88] = 0x001A;
	reg_addr[89] = 0x0019;
	reg_addr[90] = 0x0018;
	reg_addr[91] = 0x0017;
	reg_addr[92] = 0x0016;
	reg_addr[93] = 0x0015;
	reg_addr[94] = 0x0014;
	reg_addr[95] = 0x0013;
	reg_addr[96] = 0x0012;
	reg_addr[97] = 0x0011;
	reg_addr[98] = 0x0010;
	reg_addr[99] = 0x000F;
	reg_addr[100] = 0x000E;
	reg_addr[101] = 0x000D;
	reg_addr[102] = 0x000C;
	reg_addr[103] = 0x000B;
	reg_addr[104] = 0x000A;
	reg_addr[105] = 0x0009;
	reg_addr[106] = 0x0008;
	reg_addr[107] = 0x0007;
	reg_addr[108] = 0x0006;
	reg_addr[109] = 0x0005;
	reg_addr[110] = 0x0004;
	reg_addr[111] = 0x0003;
	reg_addr[112] = 0x0002;
	reg_addr[113] = 0x0001;
	reg_addr[114] = 0x0000;
	reg_addr[115] = 0x0000;
	
	data[0] = 0x2512;
	data[1] = 0x2510;
	data[2] = 0x0000;
	data[3] = 0x0000;
	data[4] = 0x0000;
	data[5] = 0x0000;
	data[6] = 0x0000;
	data[7] = 0x0000;
	data[8] = 0x0000;
	data[9] = 0x0021;
	data[10] = 0x0000;
	data[11] = 0x0000;
	data[12] = 0x3F80;
	data[13] = 0x0011;
	data[14] = 0x0000;
	data[15] = 0x0000;
	data[16] = 0x0200;
	data[17] = 0x0888;
	data[18] = 0x0000;
	data[19] = 0x0000;
	data[20] = 0x0000;
	data[21] = 0x0000;
	data[22] = 0x0000;
	data[23] = 0x0000;
	data[24] = 0x0000;
	data[25] = 0x0000;
	data[26] = 0x0000;
	data[27] = 0x0000;
	data[28] = 0x0000;
	data[29] = 0xD300;
	data[30] = 0x0001;
	data[31] = 0x0000;
	data[32] = 0x1E00;
	data[33] = 0x0000;
	data[34] = 0x6666;
	data[35] = 0x0026;
	data[36] = 0x0003;
	data[37] = 0x0000;
	data[38] = 0x000C;
	data[39] = 0x0800;
	data[40] = 0x0000;
	data[41] = 0x003F;
	data[42] = 0x0001;
	data[43] = 0x0081;
	data[44] = 0xC350;
	data[45] = 0x0000;
	data[46] = 0x03E8;
	data[47] = 0x0000;
	data[48] = 0x01F4;
	data[49] = 0x0000;
	data[50] = 0x1388;
	data[51] = 0x0000;
	data[52] = 0x0322;
	data[53] = 0x00A8;
	data[54] = 0x0000;
	data[55] = 0x0001;
	data[56] = 0x8001;
	data[57] = 0x0020;
	data[58] = 0x0000;
	data[59] = 0x0000;
	data[60] = 0x0000;
	data[61] = 0x0000;
	data[62] = 0x0820;
	data[63] = 0x0080;
	data[64] = 0x0000;
	data[65] = 0x4180;
	data[66] = 0x0300;
	data[67] = 0x0300;
	data[68] = 0x07FD;
	data[69] = 0xC8DF;
	data[70] = 0x1F22;
	data[71] = 0x9428;	/* EEPROM :3FE,3FF */
	data[72] = 0xCD24;	/* EEPROM :3FC,3FD */
	data[73] = 0x0000;
	data[74] = 0x0000;
	data[75] = 0xFFFF;
	data[76] = 0xFFFF;
	data[77] = 0x0204;
	data[78] = 0x0021;	/* EEPOM : 3FB */
	data[79] = 0x0004;
	data[80] = 0x0000;
	data[81] = 0x1E21;
	data[82] = 0x0393;
	data[83] = 0x43EC;
	data[84] = 0x318C;
	data[85] = 0x318C;
	data[86] = 0x0488;
	data[87] = 0x0002;
	data[88] = 0x0DB0;
	data[89] = 0x0624;
	data[90] = 0x071A;
	data[91] = 0x007C;
	data[92] = 0x0001;
	data[93] = 0x0401;
	data[94] = 0xC848;
	data[95] = 0x27B7;
	data[96] = 0x0064;
	data[97] = 0x012C;
	data[98] = 0x0080;
	data[99] = 0x064F;
	data[100] = 0x1E50;
	data[101] = 0x4000;
	data[102] = 0x5001;
	data[103] = 0x0018;
	data[104] = 0x10D8;
	data[105] = 0x1604;
	data[106] = 0x2000;
	data[107] = 0x40B2;
	data[108] = 0xC802;
	data[109] = 0x00C8;
	data[110] = 0x0A43;
	data[111] = 0x0642;
	data[112] = 0x0500;
	data[113] = 0x0808;
	data[114] = 0x2598;
	data[115] = 0x2598;

	UCHAR	centerFreqMode;
	UCHAR	eep_val;
	BPF_HM_DEVC_EEPROM_READ(0x3F6, &centerFreqMode);

	if(centerFreqMode == D_RRH_ON){
		BPF_HM_DEVC_EEPROM_READ(0x3FE, &eep_val);
		data[71] = (((unsigned int)eep_val) << 8) & 0xFF00;
		BPF_HM_DEVC_EEPROM_READ(0x3FF, &eep_val);
		data[71] |= ((unsigned int)eep_val) & 0x00FF;

		BPF_HM_DEVC_EEPROM_READ(0x3FC, &eep_val);
		data[72] = (((unsigned int)eep_val) << 8) & 0xFF00;
		BPF_HM_DEVC_EEPROM_READ(0x3FD, &eep_val);
		data[72] |= ((unsigned int)eep_val) & 0x00FF;

		BPF_HM_DEVC_EEPROM_READ(0x3FB, &eep_val);
		data[78] = eep_val;

		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "RFIC_mmW_RF_PLL change value No.72:0x%x,73:0x%x,79:0x%x ",data[71],data[72],data[78]);
	}

	//RF_PLLへ初期設定値を書き込む
	for(i=0;i<RFIC_MMW_RF_PLL_DATA_NUM;i++){
		rtn_bpf = BPF_HM_DEVC_RFPLL_WRITE(reg_addr[i], data[i]);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if(i == 114) { usleep(10000); }	/*	手順115の後にWait(10ms)	*/
	}
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_RF_PLL finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けPA Gate Bias出力DAC設定
 * param  : in def_flg デフォルト設定フラグ（0:通常 / 1:Default初期値設定）
 *          in pw_mode 電力モード（0:通常電力モード / 1:低電力モード）
 * return :正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_RF_AMP_DAC(int def_flg, int pw_mode){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_RF_AMP_DAC, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
	
	//引数の設定値が範囲内か判定
	if((def_flg > 1) || (pw_mode > 0)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int traning_flg_v = 0;
	unsigned int traning_flg_h = 0;
	unsigned char set_data_v1 = 0x00; //V偏波 上位bit
	unsigned char set_data_v2 = 0x00; //V偏波 下位bit
	unsigned char set_data_h1 = 0x00; //H偏波 上位bit
	unsigned char set_data_h2 = 0x00; //H偏波 下位bit
	
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_RF_AMP_DAC TRACE Normal Power & Normal Start");
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_AMP_DAC_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_AMP_DAC_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		
		//V偏波トレーニング値設定
		if(traning_flg_v == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RF_AMP_DAC Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
			set_data_v1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_AMP_DAC_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_v2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_AMP_DAC_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		}
		//H偏波トレーニング値設定設定値
		if(traning_flg_h == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RF_AMP_DAC Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
			set_data_h1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_AMP_DAC_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_h2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_AMP_DAC_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		}
	}
	
	unsigned int set_data;
	
	//V偏波設定
	set_data = (int)(set_data_v1 << 8) + set_data_v2;
	rtn_bpf = BPF_HM_DEVC_PAVGDAC_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, set_data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//H偏波設定
	set_data = (int)(set_data_h1 << 8) + set_data_h2;
	rtn_bpf = BPF_HM_DEVC_PAVGDAC_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, set_data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_RF_AMP_DAC finished");
	
	return RFIC_MMW_COMPLETE;
}

/********************************
 * brief  : mmW向けPGC BIAS設定
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_PGC_BIAS(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_PGC_BIAS");
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用

	unsigned int device_data_val_pt1[RFIC_MMW_PGC_BIAS_DATA_NUM_PTN1];
	/* [14] = X;Write込みで値を設定 */
	device_data_val_pt1[0]  = 0x4003;
	device_data_val_pt1[1]  = 0x4103;
	device_data_val_pt1[2]  = 0x4203;
	device_data_val_pt1[3]  = 0x4303;
	device_data_val_pt1[4]  = 0x4404;
	device_data_val_pt1[5]  = 0x4504;
	device_data_val_pt1[6]  = 0x4604;
	device_data_val_pt1[7]  = 0x4704;
	device_data_val_pt1[8]  = 0x4806;
	device_data_val_pt1[9]  = 0x4900;
	device_data_val_pt1[10] = 0x4A45;

	unsigned int device_data_val_pt2[RFIC_MMW_PGC_BIAS_DATA_NUM_PTN2];
	device_data_val_pt2[0]  = 0x4806;
	device_data_val_pt2[1]  = 0x4900;
	device_data_val_pt2[2]  = 0x4A45;


	unsigned int* device_data_val_p;
	unsigned int  device_data_num;
	unsigned int i,num;
	unsigned int ptn;

	rtn_bpf = BPF_HM_DEVC_RFEEP_READ(0x50, &ptn);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	if(ptn == 1){
		device_data_val_p = &device_data_val_pt1[0];
		device_data_num = RFIC_MMW_PGC_BIAS_DATA_NUM_PTN1;
	}else{
		device_data_val_p = &device_data_val_pt2[0];
		device_data_num = RFIC_MMW_PGC_BIAS_DATA_NUM_PTN2;
	}

	for(num=0 ; num < device_data_num ; num++,device_data_val_p++){
		for(i=0; i < RFIC_MMW_PGC_BIAS_DATA_NUM_GRP ; i++){
			rtn_bpf = BPF_HM_DEVC_PGC_WRITE2(i+1,*device_data_val_p,*device_data_val_p,*device_data_val_p,*device_data_val_p);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE) {
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "RFIC_mmW_PGC_BIAS error(%d:0x%08x)",i,*device_data_val_p);
				 return RFIC_MMW_NG;
			}
		}
	}

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_PGC_BIAS finished");
	
	return RFIC_MMW_COMPLETE;
}

static unsigned char RFIC_mmW_PGC_LUT_Data[0x00010000];
/********************************
 * brief  : mmW向けPGC LUT設定のトレーニングデータに位相オフセット、利得オフセットを加算して新トレーニングデータを作成
 * param  : unsigned char *addr; トレーニングデータの先頭アドレス
 * return : void
 ******************************/
int RFIC_mmW_PGC_LUT_45TableToNew45Table (unsigned char *addr) {
	int		fileDsc, errcd, ret;
	unsigned int count;
	unsigned char beam23tra_data[RFIC_MMW_LUT_TRANING_DATA_NUM_23BEAM];
	unsigned int read_size;
	int  gain_tmp;
	char phase_offset;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_PGC_LUT_45TableToNew45Table");

	/* BeamID=23のテーブルを作成し、そのテーブルに位相オフセットを加算して新トレーニングデータを作成する */
	for(count=0; count<RFIC_MMW_LUT_TRANING_DATA_NUM_23BEAM;count++) {
		beam23tra_data[count] = addr[0x2C00 + count];
	}

	/* 位相オフセット、利得オフセットのテーブルをオープンし、トレーニングデータに加算する */
	ret = BPF_RU_FACC_FOPEN((bpf_ru_facc_dpathname *)RFIC_MMW_LUT_PHASE_OFFSET_TABLE_PATH,
							(bpf_ru_facc_fname *)RFIC_MMW_LUT_PHASE_OFFSET_TABLE_NAME,
							FACC_READ,
							BPF_RU_FACC_NO_CREATE,
							&fileDsc,
							&errcd);
	if( ret != BPF_RU_FACC_COMPLETE) {
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "RFIC_mmW_PGC_LUT_45TableToNew45Table file open NG ret:%d errcd:%d filename:%s%s",ret, errcd, RFIC_MMW_LUT_PHASE_OFFSET_TABLE_PATH, RFIC_MMW_LUT_PHASE_OFFSET_TABLE_NAME);
		return BPF_HM_DEVC_NG;
	}

	for(count=0; count<RFIC_MMW_LUT_TRANING_DATA_NUM*2; count++) {
		(VOID)BPF_RU_FACC_FREAD(fileDsc, 1, &phase_offset, &read_size, &errcd);

		if( ((count / 32) % 2) == 0 ) {	/* [0 - 31] : 位相	*/
			/* 位相はMOD((旧ID23の値+オフセット値), 256) */
			addr[count] = beam23tra_data[(count % RFIC_MMW_LUT_TRANING_DATA_NUM_23BEAM)] + (unsigned char)phase_offset;
		} else {	/* [32 - 63] : 利得	*/
			/* 利得は(旧ID23の値+オフセット値) ただし、80以上255以下の範囲に収める*/
			gain_tmp = (int)beam23tra_data[(count % RFIC_MMW_LUT_TRANING_DATA_NUM_23BEAM)] + (signed char)phase_offset;
			if(gain_tmp < 80) {
				gain_tmp = 80;
			} else if(gain_tmp > 255) {
				gain_tmp = 255;
			}
			addr[count] = (unsigned char)gain_tmp;
		}
	}
	(VOID)BPF_RU_FACC_FCLOSE(fileDsc, &errcd);
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_PGC_LUT_45TableToNew45Table finished");

	return BPF_HM_DEVC_COMPLETE;
}

static unsigned int RFIC_mmW_PGC_LUT_GAIN_V[D_DU_TXPOW_DETECT_VH_MAX] = 
{
	0x00000028,0x00000068,0x000000A8,0x000000E8,0x0000002C,0x0000006C,0x000000AC,0x000000EC,
	0x00000030,0x00000070,0x000000B0,0x000000F0,0x00000034,0x00000074,0x000000B4,0x000000F4
};

static unsigned int RFIC_mmW_PGC_LUT_GAIN_H[D_DU_TXPOW_DETECT_VH_MAX] = 
{
	0x00000038,0x00000078,0x000000B8,0x000000F8,0x0000003C,0x0000007C,0x000000BC,0x000000FC,
	0x00000040,0x00000080,0x000000C0,0x00000100,0x00000044,0x00000084,0x000000C4,0x00000104
};

/********************************
 * brief  : mmW向けPGC LUT設定
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_PGC_LUT(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_PGC_LUT");
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned short crc_traning_data;
	unsigned short crc_data_cal = 0;
	unsigned short crc_data_flash;
	unsigned int i;

	T_RRH_TXPOW_DETECT_TBL* txpow_detect_p;
	INT			errcd;
	T_RRH_TXPOW_DETECT_GAIN_IN* gain_in_p;
	unsigned int	beam_index;
	unsigned int	gain_index;
	unsigned int	v_h_index;
	unsigned int	offset;
	DL				tmp_Y;
	DL				tmp_Y2;
	DL				tmp_Z;
	DL				tmp_avg;
	DL				tmp_avg_base[E_DU_TXPOW_DETECT_VH_NUM];
	DL				tmp_rate;
	DL				tmp_rvs;


	/* header, data, crc 込み */
	rtn_bpf = BPF_HM_DEVC_QSPI_FLASH_READ(0, 0x06A00000, sizeof(RFIC_mmW_PGC_LUT_Data), RFIC_mmW_PGC_LUT_Data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO , "Header Ver:%02X%02X date:%02X%02X %02X-%02X crc:%02X%02X",
				RFIC_mmW_PGC_LUT_Data[0],
				RFIC_mmW_PGC_LUT_Data[1],
				RFIC_mmW_PGC_LUT_Data[2],
				RFIC_mmW_PGC_LUT_Data[3],
				RFIC_mmW_PGC_LUT_Data[4],
				RFIC_mmW_PGC_LUT_Data[5],
				RFIC_mmW_PGC_LUT_Data[0x5A08],
				RFIC_mmW_PGC_LUT_Data[0x5A09]
				);

	crc_data_flash = ((((unsigned short)RFIC_mmW_PGC_LUT_Data[0x5A08]) << 8 ) & 0xFF00) | RFIC_mmW_PGC_LUT_Data[0x5A09];

	unsigned char* lut_data_p = &RFIC_mmW_PGC_LUT_Data[8];

	for(i=0; i< RFIC_MMW_LUT_TRANING_DATA_NUM ; i++,lut_data_p++){
		crc_traning_data = ((((unsigned short)(*lut_data_p)) << 8) & 0xFF00);
		lut_data_p++;
		crc_traning_data |= (unsigned short)*lut_data_p;
		f_com_dpda_init_addCrc(crc_traning_data, &crc_data_cal); 
	}

	(void)BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_TXPOW_DETECT_TBL,(VOID**)&txpow_detect_p,&errcd);
	memcpy(&(txpow_detect_p->head.pgclut_version[0]),&RFIC_mmW_PGC_LUT_Data[0],6);
	memcpy(&(txpow_detect_p->head.pgclut_version[6]),&RFIC_mmW_PGC_LUT_Data[0x5A08],2);

	/**** 45-BeamID PGC Flashデータ CRC OK?	*****/
	if(crc_data_flash != crc_data_cal){	/* No	*/

		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "RFIC_mmW_PGC_LUT CRC ERROR cal:0x%04X flash:0x%04X",crc_data_cal,crc_data_flash);
		f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_PGC_TRA_PRM_NG_MMW );
		rtn_bpf = BPF_HM_DEVC_PGCLUT_WRITE2(0,NULL);
		memset(RFIC_mmW_PGC_LUT_Data,0,sizeof(RFIC_mmW_PGC_LUT_Data));	/* 念のため初期化 */

		txpow_detect_p->head.pgclut_validFlg = D_RRH_OFF;
		return RFIC_MMW_NG;

	} else {	/* Yes	*/

		/**** BeamID新旧モード判定 EEP 0x385=0x01?	****/
		if( f_cmw_tra_inv_tbl->cmx_bf_envelope_mode == D_RRH_ON ) {	/* Yes (新Beam動作)	*/

			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO, "BeamID new mode execution");

			/**** ID-23 TX/RX トレーニング値取得	****/
			/**** 新45-BeamIDデータ算出				****/
			rtn_bpf = RFIC_mmW_PGC_LUT_45TableToNew45Table( &RFIC_mmW_PGC_LUT_Data[8] );

			if( rtn_bpf != BPF_HM_DEVC_COMPLETE ) {
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING, "BeamID Convert NG rtn_bpf=%d", rtn_bpf);
				f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_PGC_TRA_PRM_NG_MMW );
				rtn_bpf = BPF_HM_DEVC_PGCLUT_WRITE2(0,NULL);
				return rtn_bpf;
			}
			/**** ソフト算出値→FPGA LUTメモリ展開	****/
			rtn_bpf = BPF_HM_DEVC_PGCLUT_WRITE2( (sizeof(RFIC_mmW_PGC_LUT_Data) - 8), &RFIC_mmW_PGC_LUT_Data[8] );
			/**** 受信BeamID読み替え[無効]設定		****/
			BPF_HM_DEVC_REG_WRITE_BITON(D_RRH_LOG_REG_LV_READ_WRITE, 0xA2010050, 0x1);

		} else {	/* No (旧Beam動作)	*/

			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_INFO, "BeamID old mode execution");

			/**** Flash Readデータ→FPGA LUTメモリ展開	****/
			rtn_bpf = BPF_HM_DEVC_PGCLUT_WRITE2( (sizeof(RFIC_mmW_PGC_LUT_Data) - 8), &RFIC_mmW_PGC_LUT_Data[8] );
		}
	}

	tmp_avg_base[E_DU_TXPOW_DETECT_V] = 0.0;
	tmp_avg_base[E_DU_TXPOW_DETECT_H] = 0.0;

	for( beam_index=0; beam_index < D_DU_TXPOW_DETECT_MAX ; beam_index++){
		offset = 0x200 * beam_index;

		for( gain_index=0 ; gain_index < E_DU_TXPOW_DETECT_VH_NUM; gain_index++){
			gain_in_p = &(txpow_detect_p->gainIn[beam_index][gain_index]);
			tmp_avg = 0.0;

			for( v_h_index=0 ; v_h_index < D_DU_TXPOW_DETECT_VH_MAX; v_h_index++){
				if(gain_index == E_DU_TXPOW_DETECT_V){
					gain_in_p->FLASH_VH[v_h_index] = (DL)RFIC_mmW_PGC_LUT_Data[ RFIC_mmW_PGC_LUT_GAIN_V[v_h_index] + offset ];
				}else{
					gain_in_p->FLASH_VH[v_h_index] = (DL)RFIC_mmW_PGC_LUT_Data[ RFIC_mmW_PGC_LUT_GAIN_H[v_h_index] + offset ];
				}

				tmp_Y  = ( 0.0533 * gain_in_p->FLASH_VH[v_h_index] ) + 16.667;	/* Y(dB) = 0.0533 * Gain設定値[X] + 16.667 */
				tmp_Y2 = round( tmp_Y * 10.0 ) / 10.0;							/* 少数第2位四捨五入 */
				tmp_Z  = pow(10.0 ,tmp_Y2 / 10.0 );								/* Z = 10^(Y/10) */
				gain_in_p->YZ_VH[v_h_index] = round( tmp_Z * 100.0 ) / 100.0;	/* 少数第3位四捨五入 */
				tmp_avg += gain_in_p->YZ_VH[v_h_index];
			}

			gain_in_p->AVG_VH = tmp_avg / (DL)D_DU_TXPOW_DETECT_VH_MAX;
			if(beam_index == 22){
				tmp_avg_base[gain_index] = gain_in_p->AVG_VH;
			}
	
		}
	}


	for( beam_index=0; beam_index < D_DU_TXPOW_DETECT_MAX ; beam_index++){
		for( gain_index=0 ; gain_index < E_DU_TXPOW_DETECT_VH_NUM; gain_index++){
			gain_in_p = &(txpow_detect_p->gainIn[beam_index][gain_index]);

			tmp_rate = gain_in_p->AVG_VH / tmp_avg_base[gain_index];
			tmp_rate = round( tmp_rate * 100.0 ) / 100.0;						/* 少数第3位四捨五入(xlsから) */
			tmp_rvs = 1.0 / tmp_rate;
			gain_in_p->RVS_VH = round( tmp_rvs * 1000000.0 ) / 1000000.0;		/* 小数点第7位を四捨五入 */

		}
	}

	txpow_detect_p->head.validFlg = D_RRH_ON;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_PGC_LUT finished(cal:0x%04X)",crc_data_cal);
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けTX VATT DAC設定
 * param  : in def_flg デフォルト設定フラグ（0:通常 / 1:Default初期値設定）
 *          in pw_mode 電力モード（0:通常電力モード / 1:低電力モード）
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_TX_VATT_DAC(int def_flg, int pw_mode){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_TX_VATT_DAC, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
	
	//引数の設定値が範囲内か判定
	if((def_flg > 1) || (pw_mode > 0)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int traning_flg_v = 0;
	unsigned int traning_flg_h = 0;
	unsigned char set_data_v = 0x04; //V偏波
	unsigned char set_data_h = 0x04; //H偏波
	
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_TX_VATT_DAC TRACE Normal Power & Normal Start");
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_TXVATT_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_TXVATT_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

		
		//V偏波トレーニング値設定
		if(traning_flg_v == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "TX_VATT_DAC Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
			set_data_v = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_TXVATT_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
		}
		//H偏波トレーニング値設定設定値
		if(traning_flg_h == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "TX_VATT_DAC Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
			set_data_h = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_TXVATT_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
		}
	}

	//V偏波設定
	rtn_bpf = BPF_HM_DEVC_TXVATTDAC_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, set_data_v);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//H偏波設定
	rtn_bpf = BPF_HM_DEVC_TXVATTDAC_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, set_data_h);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_TX_VATT_DAC finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けRX VATT DAC設定
 * param  : in def_flg デフォルト設定フラグ（0:通常 / 1:Default初期値設定）
 *          in pw_mode 電力モード（0:通常電力モード / 1:低電力モード）
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_RX_VATT_DAC(int def_flg, int pw_mode){

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_RX_VATT_DAC, def_flg = %d, pw_mode = %d", def_flg, pw_mode);

	//引数の設定値が範囲内か判定
	if((def_flg > 1) || (pw_mode > 0)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
		return RFIC_MMW_NG;
	}

	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int traning_flg_v = 0;
	unsigned int traning_flg_h = 0;
	unsigned char set_data_v = 0x07; //V偏波
	unsigned char set_data_h = 0x07; //H偏波
	
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_RX_VATT_DAC TRACE Normal Power & Normal Start");
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_RXVATT_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_RXVATT_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		
		//V偏波トレーニング値設定
		if(traning_flg_v == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RX_VATT_DAC Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
			set_data_v = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_RXVATT_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			if( f_cmw_tra_inv_tbl->cmx_bf_envelope_mode == D_RRH_ON ) {
				/* RFIC_mmW_PGC_LUT()でケースB(新技適対応)の場合、-10する -10すると負になる場合は0にする */
				if(set_data_v < 10) {
					set_data_v = 0;
				} else {
					set_data_v -=10;
				}
			}
		}
		//H偏波トレーニング値設定設定値
		if(traning_flg_h == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RX_VATT_DAC Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
			set_data_h = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_RXVATT_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			if( f_cmw_tra_inv_tbl->cmx_bf_envelope_mode == D_RRH_ON ) {
				/* RFIC_mmW_PGC_LUT()でケースB(新技適対応)の場合、-10する -10すると負になる場合は0にする */
				if(set_data_h < 10) {
					set_data_h = 0;
				} else {
					set_data_h -=10;
				}
			}
		}
	}

	//V偏波設定
	rtn_bpf = BPF_HM_DEVC_RXVATTDAC_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, set_data_v);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	//H偏波設定
	rtn_bpf = BPF_HM_DEVC_RXVATTDAC_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, set_data_h);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_RX_VATT_DAC finished");

	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けQMOD/QDEM DAC設定
 * param  : in def_flg デフォルト設定フラグ（0:通常 / 1:Default初期値設定）
 *          in pw_mode 電力モード（0:通常電力モード / 1:低電力モード）
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_QMOD_QDEM_DAC(int def_flg, int pw_mode){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_QMOD_QDEM_DAC, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
	
	//引数の設定値が範囲内か判定
	if((def_flg > 1) || (pw_mode > 0)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int traning_flg_v = 0;
	unsigned int traning_flg_h = 0;
	unsigned char set_data_v1 = 0x00; //V偏波 上位bit
	unsigned char set_data_v2 = 0x00; //V偏波 下位bit
	unsigned char set_data_h1 = 0x00; //H偏波 上位bit
	unsigned char set_data_h2 = 0x00; //H偏波 下位bit
	unsigned int set_data;
	unsigned int addr = 0x00000000; //BPF_HM_DEVC_QMOD_WRITE用ダミーアドレス
	
	//QMOD DAC設定
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD_QDEM_DAC TRACE Normal Power & Normal Start");
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QMOD_DAC_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QMOD_DAC_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		
		//V偏波トレーニング値設定
		if(traning_flg_v == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD_DAC Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
			set_data_v1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QMOD_DAC_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_v2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QMOD_DAC_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		}
		//H偏波トレーニング値設定設定値
		if(traning_flg_h == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD_DAC Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
			set_data_h1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QMOD_DAC_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_h2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QMOD_DAC_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		}
	}
	
	//V偏波設定
	set_data = (int)(set_data_v1 << 8) + set_data_v2;
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL3, addr, set_data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//H偏波設定
	set_data = (int)(set_data_h1 << 8) + set_data_h2;
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL3, addr, set_data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	
	//QDEM DAC設定
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD_QDEM_DAC TRACE Normal Power & Normal Start");
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QDEM_DAC_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QDEM_DAC_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		
		//V偏波トレーニング値設定
		if(traning_flg_v == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM_DAC Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
			set_data_v1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_DAC_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_v2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_DAC_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		}
		//H偏波トレーニング値設定設定値
		if(traning_flg_h == 1){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM_DAC Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
			set_data_h1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_DAC_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_h2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_DAC_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		}
	}
	
	//V偏波設定
	set_data = (int)(set_data_v1 << 8) + set_data_v2;
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL3, addr, set_data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//H偏波設定
	set_data = (int)(set_data_h1 << 8) + set_data_h2;
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL3, addr, set_data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD_QDEM_DAC finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けQMOD設定
 * param  : in def_flg デフォルト設定フラグ（0:通常 / 1:Default初期値設定）
 *          in pw_mode 電力モード（0:通常電力モード / 1:低電力モード）
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_QMOD(int def_flg, int pw_mode){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_QMOD, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
	
	//引数の設定値が範囲内か判定
	if((def_flg > 1) || (pw_mode > 0)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int traning_flg_v = 0;
	unsigned int traning_flg_h = 0;
	unsigned char set_data_1;
	unsigned char set_data_2;
	unsigned int set_data;
	unsigned int base_mem_addr;
	unsigned int base_reg_addr;
	unsigned int i;
	unsigned char crc_result_1;
	unsigned char crc_result_2;
	
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD TRACE Normal Power & Normal Start");
		//トレーニングフラグチェック
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QMOD_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QMOD_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	}
	
	//V偏波設定
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0000, 0x00A1);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0001, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0002, 0xFFFF);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0003, 0x01D7);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	if(traning_flg_v == 1){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD TRACE Normal Power");
		crc_result_1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_TXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
		crc_result_2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_TXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD Traning Flag V CRC, crc_result_1 = %x", crc_result_1);
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD Traning Flag V CRC, crc_result_2 = %x", crc_result_2);
		
		if((crc_result_1 == 0xAB) && (crc_result_2 == 0xAB)){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD CRC OK");
			base_mem_addr = D_DU_RF_EEP_N_QMOD_IM_I_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM;
			base_reg_addr = 0x005;
			
			for(i=0;i<4;i++){
				set_data_1 = f_comw_rftrainingData ->traVal[base_mem_addr + (i*2)];
				set_data_2 = f_comw_rftrainingData ->traVal[base_mem_addr + (i*2+1)];
				set_data = (set_data_1 << 8) + set_data_2;
				rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, base_reg_addr + i, set_data);
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
			}
		}
	}
	
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0009, 0x5705);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x000A, 0x000F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//H偏波設定
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0000, 0x00A1);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0001, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0002, 0xFFFF);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0003, 0x01D7);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	if(traning_flg_h == 1){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
		crc_result_1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_TXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
		crc_result_2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_TXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD Traning Flag H CRC, crc_result_1 = %x", crc_result_1);
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QMOD Traning Flag H CRC, crc_result_2 = %x", crc_result_2);
		
		if((crc_result_1 == 0xAB) && (crc_result_2 == 0xAB)){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD CRC OK");
			base_mem_addr = D_DU_RF_EEP_N_QMOD_IM_I_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM;
			base_reg_addr = 0x005;
			
			for(i=0;i<4;i++){
				set_data_1 = f_comw_rftrainingData ->traVal[base_mem_addr + (i*2)];
				set_data_2 = f_comw_rftrainingData ->traVal[base_mem_addr + (i*2+1)];
				set_data = (set_data_1 << 8) + set_data_2;
				rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, base_reg_addr + i, set_data);
				if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
			}
		}
	}
	
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x0009, 0x5705);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QMOD_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL0, 0x000A, 0x000F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QMOD finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けQDEM設定
 * param  : in def_flg デフォルト設定フラグ（0:通常 / 1:Default初期値設定）
 *          in pw_mode 電力モード（0:通常電力モード / 1:低電力モード）
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_QDEM(int def_flg, int pw_mode){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_QDEM, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
	
	//引数の設定値が範囲内か判定
	if((def_flg > 1) || (pw_mode > RFIC_MMW_NORMAL_POWER_MODE)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, def_flg = %d, pw_mode = %d", def_flg, pw_mode);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int traning_flg_v = 0;
	unsigned int traning_flg_h = 0;
	unsigned char set_data_1;
	unsigned char set_data_2;
	unsigned int set_data;
	unsigned char crc_result_1;
	unsigned char crc_result_2;
	
	//通常電力モード時 トレーニングデータ利用可否判定
	if((def_flg == RFIC_MMW_SET_TRANING_DATA) && (pw_mode == RFIC_MMW_NORMAL_POWER_MODE)){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QDEM TRACE Normal Power & Normal Start");
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QDEM_V_FLAG, &traning_flg_v);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		rtn_bpf = BPF_HM_DEVC_RFEEP_READ(D_DU_RF_EEP_N_QDEM_H_FLAG, &traning_flg_h);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		
	}
	
	//V偏波設定
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0000, 0x0091);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0001, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0002, 0xFFFF);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0003, 0x3117);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0004, 0x5585);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	if(traning_flg_v == 1){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM Traning Flag V ON, traning_flg_v = %d", traning_flg_v);
		crc_result_1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_RXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
		crc_result_2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_RXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM Traning Flag V CRC, crc_result_1 = %x", crc_result_1);
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM Traning Flag V CRC, crc_result_2 = %x", crc_result_2);
		
		if((crc_result_1 == 0xAB) && (crc_result_2 == 0xAB)){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QDEM CRC OK");
			set_data_1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_IM_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_IM_V - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
			set_data = (set_data_1 << 8) + set_data_2;
			rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0005, set_data);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		}
	}
	
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0006, 0x55D5);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0007, 0xD808);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0008, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0009, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x000A, 0x2390);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTA, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x000B, 0x0254);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	
	//H偏波設定
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0000, 0x0091);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0001, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0002, 0xFFFF);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0003, 0x3117);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0004, 0x5585);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	if(traning_flg_h == 1){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM Traning Flag H ON, traning_flg_h = %d", traning_flg_h);
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QDEM TRACE Normal Power");
		crc_result_1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_RXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
		crc_result_2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_RXCRC16 - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM Traning Flag H CRC, crc_result_1 = %x", crc_result_1);
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "QDEM Traning Flag H CRC, crc_result_2 = %x", crc_result_2);
		
		if((crc_result_1 == 0xAB) && (crc_result_2 == 0xAB)){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QDEM CRC OK");
			set_data_1 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_IM_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM];
			set_data_2 = f_comw_rftrainingData ->traVal[D_DU_RF_EEP_N_QDEM_IM_H - RFIC_MMW_OFFSET_RFEEP_SHRMEM + 1];
			set_data = (set_data_1 << 8) + set_data_2;
			rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0005, set_data);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		}
	}
	
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0006, 0x55D5);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0007, 0xD808);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0008, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x0009, 0x0000);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x000A, 0x2390);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_QDEM_WRITE(RFIC_MMW_DEVC_TRXSPI_VATTDAC_ANTB, RFIC_MMW_DEVC_TRXSPI_SEL2, 0x000B, 0x0254);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_QDEM finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けJESDセットアップ
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_TX_DAC(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_TX_DAC");
	
	unsigned int rtn_mmw = RFIC_MMW_COMPLETE; //mmW設定の戻り値判定用
	unsigned int count_10;
	unsigned int fpga_addr;
	unsigned int data;
	unsigned int rtn_bpf;
	
	//Marconi及びFPGAへのSYSREF OFF要求(初期化)
    //SYSREF OFF
	rtn_mmw = RFIC_mmW_JC_PLL(RFIC_MMW_JC_PLL_SYSREF_OFF);
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_JC_PLL Failed");
		return RFIC_MMW_NG;
	}
	
	//TXDACリセット解除
	fpga_addr = 0xA0000040;
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_addr, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00000100);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &data);
	//wait 1s
	usleep(1000000);

	//SERDESパラメータ及び初期設定
	rtn_mmw = f_dpda_mmW_setTxDacSerdesParameter(RFIC_MMW_TRX_FPGA1);
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacSerdesParameter Failed");
		return RFIC_MMW_NG;
	}
	
	//FPGAへのSYSREF ON要求
	rtn_mmw = RFIC_mmW_JC_PLL(RFIC_MMW_JC_PLL_SYSREF_ON);
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_JC_PLL Failed");
		return RFIC_MMW_NG;
	}
	
	//JESD QPLL/JSFW PHY起動
	//TRX_FPGA1
	count_10 = 0;
	while(1){
		rtn_mmw = f_dpda_mmW_setTxDacJesdQpllJsfwPhy(RFIC_MMW_TRX_FPGA1);
		if(rtn_mmw == RFIC_MMW_COMPLETE){
			break;
		}
		else if(rtn_mmw == RFIC_MMW_RETRY_10){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacJesdQpllJsfwPhy Failed, rtn_mmw = %d", rtn_mmw);
			if(count_10 < 10){
				count_10++;
				rtn_mmw = f_dpda_mmW_retryTxDacJesdQpllJsfwPhy(RFIC_MMW_TRX_FPGA1);
				if(rtn_mmw != RFIC_MMW_COMPLETE) {
					BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_retryTxDacJesdQpllJsfwPhy Failed");
					return RFIC_MMW_NG;
				}
			}
			else{
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacJesdQpllJsfwPhy Failed, Retry %d times", count_10);
				//TRX-FPGA#1 JESD QPLL/JSFW PHY起動NG
				return RFIC_mmW_RF_API_LAST_ALMCD;
			}
		}
		else{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacJesdQpllJsfwPhy Failed, rtn_mmw = %d", rtn_mmw);
			return RFIC_MMW_NG;
		}
	}
	
	//DACのセットアップ
	rtn_mmw = f_dpda_mmW_setTxDac();
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDac Failed:0x%08x", rtn_mmw);
		//  TODO f_com_abort()
		return rtn_mmw;
	}
	
	//wait 100ms
	usleep(100000);
	
	//FW JESD起動
	//TRX_FPGA1
	while(1){
		rtn_mmw = f_dpda_mmW_setTxDacJesd(RFIC_MMW_TRX_FPGA1);
		if(rtn_mmw == RFIC_MMW_COMPLETE){
			break;
		}
		else if(rtn_mmw == RFIC_MMW_RETRY_10){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacJesd Failed, rtn_mmw = %d", rtn_mmw);
			//★ TRX-FPGA#1 SYNC受信確認リトライアウト
			return RFIC_mmW_RF_API_LAST_ALMCD;
		}
		else if(rtn_mmw == RFIC_MMW_RETRY_3){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacJesd Failed, rtn_mmw = %d", rtn_mmw);
			//★ TRX-FPGA#1 FW_JESD起動シーケンス設定リトライアウト
			return RFIC_mmW_RF_API_LAST_ALMCD;
		}
		else{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacJesd Failed, rtn_mmw = %d", rtn_mmw);
			return RFIC_MMW_NG;
		}
	}
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_TX_DAC finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けJESDリンクアップシーケンス設定
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_RX_ADC(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_RX_ADC");
	
	unsigned int rtn_mmw = RFIC_MMW_COMPLETE; //mmW設定の戻り値判定用
	unsigned int fpga_addr;
	unsigned int data;
	unsigned int rtn_bpf;
	
	//ADCのセットアップ
	//RXADC PDWN解除
	fpga_addr = 0xA0000040;
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_addr, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00000200);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_addr, &data);
	//wait 1s
	usleep(1000000);
	//ADC初期設定
	rtn_mmw = f_dpda_mmW_setRxAdc();
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setRxAdc Failed:0x%08x", rtn_mmw);
		//  TODO f_com_abort()
		return rtn_mmw;
	}
	
	//wait 100ms
	usleep(100000);
	
	//RX JESD起動
	//TRX_FPGA1
	while(1){
		rtn_mmw = f_dpda_mmW_setRxAdcJesd(RFIC_MMW_TRX_FPGA1);
		if(rtn_mmw == RFIC_MMW_COMPLETE){
			break;
		}
		else if(rtn_mmw == RFIC_MMW_RETRY_10){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setRxAdcJesd Failed, rtn_mmw = %d", rtn_mmw);
			//TRX-FPGA#1 RX_JESD起動シーケンス設定リトライアウト
			return RFIC_mmW_RF_API_LAST_ALMCD;
		}
		else{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setRxAdcJesd Failed, rtn_mmw = %d", rtn_mmw);
			return RFIC_MMW_NG;
		}
	}
	
	//RX ILAS確認
	//TRX_FPGA1
	while(1){
		rtn_mmw = f_dpda_mmW_checkRxAdcIlas(RFIC_MMW_TRX_FPGA1);
		if(rtn_mmw == RFIC_MMW_COMPLETE){
			break;
		}
		else if(rtn_mmw == RFIC_MMW_RETRY_10){
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_checkRxAdcIlas Failed, rtn_mmw = %d", rtn_mmw);
			//TRX-FPGA#1 FB/RX_JESD起動シーケンス設定リトライアウト
			return RFIC_mmW_RF_API_LAST_ALMCD;
		}
		else{
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_checkRxAdcIlas Failed, rtn_mmw = %d", rtn_mmw);
			return RFIC_MMW_NG;
		}
	}
	
	//SYSREF OFF要求
	rtn_mmw = RFIC_mmW_JC_PLL(RFIC_MMW_JC_PLL_SYSREF_OFF);
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_JC_PLL Failed");
		return RFIC_MMW_NG;
	}
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_RX_ADC finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : mmW向けJESD PLL 設定
 * param  : in on_off 0:SYSREF-OFF / 1:SYSREF-ON
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_JC_PLL(int on_off){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_JC_PLL, on_off = %d", on_off);
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int reg_addr = RFIC_MMW_JC_PLL_REG_ADDR;
	unsigned int data;
	
	if(on_off == RFIC_MMW_JC_PLL_SYSREF_OFF){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_JC_PLL OFF");
		rtn_bpf = BPF_HM_DEVC_EXT_PLL_READ(reg_addr, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		data = data & (~0x00000003);
		data |= 0x00000002;
		rtn_bpf = BPF_HM_DEVC_EXT_PLL_WRITE(reg_addr, data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		usleep(500000);
	}
	else if(on_off == RFIC_MMW_JC_PLL_SYSREF_ON){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_JC_PLL ON");
		rtn_bpf = BPF_HM_DEVC_EXT_PLL_READ(reg_addr, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		data = data | 0x00000003;
		rtn_bpf = BPF_HM_DEVC_EXT_PLL_WRITE(reg_addr, data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		usleep(500000);
	}
	else{
		return RFIC_MMW_NG;
	}
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_JC_PLL finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : TX_DACのSERDESパラメータ設定
 * param  : in ｆpga_no 0:TRX-FPGA#1 / 1:TRX-FPGA#2
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_setTxDacSerdesParameter(unsigned int fpga_no){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setTxDacSerdesParameter, fpga_no = %d", fpga_no);
	
	if(fpga_no != RFIC_MMW_TRX_FPGA1) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, fpga_no = %d", fpga_no);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int fpga_base_addr;
	unsigned int  data;
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacSerdesParameter FPGA1");
	
	// JSFW Swing voltage/Emphasis設定
	fpga_base_addr = RFIC_MMW_BASE_ADDR_CTX_FPGA1;
	data = 0x001F0014;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000940, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	data = 0x000A0000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000944, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	data = 0x000A0000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000948, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	data = 0x00080000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000094C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	// JSFW/RX LMFC Timing Offset設定
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSFW_FPGA1;
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004500, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = ((data & (~0x000000FF)) | 0x00000001);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004500, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSRX_FPGA1;
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001500, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = ((data & (~0x000000FF)) | 0x0000000A);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001500, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	// JSRXコンフィグパラメータ設定
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSRX_FPGA1;
	data = 0x83010000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001100, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x00004800;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000110C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x83000000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001110, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x00004700;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000111C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x83030000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001120, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x00004A00;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000112C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x83020000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001130, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = 0x00004900;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000113C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacSerdesParameter finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : TX_DACのJESD QPLL/JSFW PHY起動
 * param  : in fpga_no 0:TRX-FPGA#1 / 1:TRX-FPGA#2
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_setTxDacJesdQpllJsfwPhy(unsigned int fpga_no){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setTxDacJesdQpllJsfwPhy, fpga_no = %d", fpga_no);
	
	if(fpga_no != RFIC_MMW_TRX_FPGA1) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, fpga_no = %d", fpga_no);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int fpga_base_addr;
	unsigned int data;
	unsigned int count;
	
	
	//外部SYSREFマスク解除
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSFW_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004000, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00000100);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004000, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//wait 10ms
		usleep(10000);
		//FPGA外部SYSREF検出確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004004, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00000001) != 0) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE Retry 10 times, count = %d", count);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG1_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//外部SYSREFマスク
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004000, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000100;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004000, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD QPLL Power-up
	fpga_base_addr = RFIC_MMW_BASE_ADDR_CTX_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);

	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000050, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000010;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000050, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD GTY Power-up
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000058, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000005;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000058, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD QPLL/SERDES DONE表示マスク解除
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000204, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00050100);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000204, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD QPLLリセット
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000044, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000010;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000044, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//wait 1ms
		usleep(1000);
		//JESD QPLL UnLock確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000200, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00000100) == 0) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE Retry 10 times, count = %d", count);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG2_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//JSFW PHYリセット
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x0000004C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000001;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000004C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//wait 10ms
		usleep(10000);
		//JSFW SERDES NOT DONE確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000200, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00010000) == 0) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE Retry 10 times, count = %d", count);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG3_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//PHY Status確認
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSFW_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);

	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004304, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	if(data != 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy TRACE Retry 10 times, data = %x", data);
		RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG4_MMW;
		return RFIC_MMW_RETRY_10;
	}
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesdQpllJsfwPhy finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : TX_DACのJESD QPLL/JSFW PHY起動の設定を初期化しリトライ
 * param  : in fpga_no 0:TRX-FPGA#1 / 1:TRX-FPGA#2
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_retryTxDacJesdQpllJsfwPhy(unsigned int fpga_no){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_retryTxDacJesdQpllJsfwPhy, fpga_no = %d", fpga_no);
	
	if((fpga_no != RFIC_MMW_TRX_FPGA1) && (fpga_no != RFIC_MMW_TRX_FPGA2)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, fpga_no = %d", fpga_no);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int fpga_base_addr;
	unsigned int data;
	
	//外部SYSREFマスク
	if(fpga_no == RFIC_MMW_TRX_FPGA1){
		fpga_base_addr = RFIC_MMW_BASE_ADDR_JSFW_FPGA1;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_retryTxDacJesdQpllJsfwPhy TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	}
	else{
		fpga_base_addr = RFIC_MMW_BASE_ADDR_JSFW_FPGA2;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_retryTxDacJesdQpllJsfwPhy TRACE FPGA2, fpga_base_addr = %x", fpga_base_addr);
	}
	
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004000, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000100;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004000, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD QPLL Power-Down
	if(fpga_no == RFIC_MMW_TRX_FPGA1){
		fpga_base_addr = RFIC_MMW_BASE_ADDR_CTX_FPGA1;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_retryTxDacJesdQpllJsfwPhy TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	}
	else{
		fpga_base_addr = RFIC_MMW_BASE_ADDR_CTX_FPGA2;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_retryTxDacJesdQpllJsfwPhy TRACE FPGA2, fpga_base_addr = %x", fpga_base_addr);
	}
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000050, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00000010);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000050, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD GTY Power-Down
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000058, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00000007);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000058, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	//JESD QPLL/SERDES DONE表示マスク設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000204, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00070100;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00000204, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_retryTxDacJesdQpllJsfwPhy finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : TX_DACの初期設定
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_setTxDac(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setTxDac");
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int data;
	UINT count;

	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x000, 0x81);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x000, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x091, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x083, 0x82);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x085, 0x12);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x206, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x705, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x090, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x095, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x796, 0xE5);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x7A0, 0xBC);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x794, 0x08);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x797, 0x20);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x798, 0x10);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x7A2, 0x7F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	usleep(100000);
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x08F, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x799, 0x0C);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x793, 0x18);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x094, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x792, 0x02);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x792, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	usleep(100000);
	rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x7B5, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) {return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac 0x7B5 data = %x", data);
	data = data & 0x00000001;
	if(data == 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac TRACE bit check, data = %x", data);
		return RFIC_MMW_NG;
	}
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0C0, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0CC, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0CD, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0DB, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0DB, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0DB, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0C1, 0x68);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0C1, 0x69);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x0C7, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x0C3, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac 0x0C3 data = %x", data);
	data = data & 0x00000001;
	if(data == 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac TRACE bit check, data = %x", data);
		return RFIC_MMW_NG;
	}
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x050, 0x2A);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x061, 0x68);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x051, 0x82);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x051, 0x83);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	for(count=0;count < 10 ; count++){
		rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x052, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
		data = data & 0x00000004;
		if(data == 0){
			break;
		}
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%d 0x052 data = %x",count+1, data);
		f_com_taskDelay(2,0,0,0);	/* 念のため */
	}

	for(count=0;count < 10 ; count++){
		rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x052, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
		data = data & 0x00000001;
		if(data == 1){
			break;
		}
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%d 0x052 data = %x",count+1, data);
		f_com_taskDelay(2,0,0,0);	/* 念のため */
	}


	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x081, 0x03);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x100, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x110, 0x23);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x111, 0x83);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x084, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x312, 0x10);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x300, 0x0B);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x475, 0x09);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x453, 0x81);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x458, 0x2F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x475, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x300, 0x0C);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x475, 0x09);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x453, 0x81);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x458, 0x2F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x475, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x008, 0xC0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x112, 0x0C);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x114, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x115, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x116, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x117, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x118, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x119, 0x20);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x11C, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x11D, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x113, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	for(count=0;count < 10 ; count++){
		rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x113, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
		data = data & 0x00000002;
		if(data != 0){
			break;
		}
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%d 0x113 data = %x",count+1, data);
		f_com_taskDelay(2,0,0,0);	/* 念のため */
	}

	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x240, 0xAA);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x241, 0xAA);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x242, 0x55);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x243, 0x55);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x244, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x245, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x246, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x247, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x248, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x249, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x24A, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x24B, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x201, 0x0F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x203, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x253, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x254, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x210, 0x16);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x216, 0x05);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x212, 0xFF);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x212, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x210, 0x87);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x216, 0x11);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x200, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	usleep(100000);
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x210, 0x86);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x216, 0x40);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x210, 0x86);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x216, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x210, 0x87);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x216, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x213, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x280, 0x05);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x280, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x281, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac 0x281 data = %x", data);
	data = data & 0x00000001;
	if(data == 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac TRACE bit check, data = %x", data);
		return RFIC_MMW_NG;
	}
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x308, 0x25);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x309, 0x08);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x30A, 0x37);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x30B, 0x1A);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x306, 0x0C);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x307, 0x0C);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x304, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x305, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x03B, 0xF1);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x036, 0x0F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x03A, 0x02);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_READ(0x03A, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_TX_DAC_RSTSNG_MMW; }
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac 0x03A, data = %x", data);
	data = data & 0x00000010;
	if(data == 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac TRACE bit check, 0x03A data = %x", data);
		return RFIC_MMW_NG;
	}
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x300, 0x0B);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x083, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x085, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x1DE, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x008, 0xC0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(0x596, 0x0C);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDac finished");
	
	return RFIC_MMW_COMPLETE;
}

/********************************
 * brief  : TX_DACのJESD起動
 * param  : in fpga_no 0:TRX-FPGA#1 / 1:TRX-FPGA#2
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_setTxDacJesd(unsigned int fpga_no){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setTxDacJesd, fpga_no = %d", fpga_no);
	
	if(fpga_no != RFIC_MMW_TRX_FPGA1) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, fpga_no = %d", fpga_no);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int fpga_base_addr;
	unsigned int data;
	unsigned int count;
	
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSFW_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesd TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	
	count = 0;
	while(1){
		//FPGA SYNC受信確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004034, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00000003) == 0) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesd TRACE, data= %x", count,data);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG5_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//JSFW内部SYSREFマスク解除設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00030000);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//wait 10ms
		usleep(10000);
		//FPGA SYNC受信確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004034, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00000003) == 0x00000003 ){ break; }
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesd TRACE, count = %x data= %x", count,data);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG6_MMW;
			return RFIC_MMW_RETRY_3;
		}
	}
	
	//JSFW SYNC設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004030, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000003;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004030, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//wait 10ms
		usleep(10000);
		//FPGA Link確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004300, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x0033000F) == 0x0033000F) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesd TRACE, count = %x data= %x", count,data);
			if((data & 0xF) != 0xF){
				RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG7_1_MMW;
			}else if((data & 0x00300000) != 0x00300000){
				RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG7_2_MMW;
			}else{
				RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG7_3_MMW;
			}
			return RFIC_MMW_RETRY_3;
		}
	}
	
	//JSFW内部SYSREFマスク設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00030000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00004020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//エラー確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00004404, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if(data == 0x00000000) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesd TRACE, count = %x data= %x", count,data);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG8_MMW;
			return RFIC_MMW_RETRY_3;
		}
	}

	// TxDacLinkPage 0
	rtn_bpf = f_dpda_mmW_setTxDacLinkPage(0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacLinkPage0 Failed, rtn_mmw = %d", rtn_bpf);
		return RFIC_MMW_RETRY_3;
	}
	// TxDacLinkPage 1
	rtn_bpf = f_dpda_mmW_setTxDacLinkPage(1);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE){
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "f_dpda_mmW_setTxDacLinkPage1 Failed, rtn_mmw = %d", rtn_bpf);
	    RFIC_mmW_RF_API_LAST_ALMCD += 4;
		return RFIC_MMW_RETRY_3;
	}
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setTxDacJesd finished");
	
	return RFIC_MMW_COMPLETE;
}

/********************************
 * brief  : TX_DACのLINK PAGE設定
 * param  : NONE
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
unsigned int f_dpda_mmW_setTxDacLinkPage(unsigned int pageno){

	int rtn_bpf = BPF_HM_DEVC_COMPLETE;
	unsigned int data;
	unsigned int address;
	unsigned int count;
	unsigned int count2;

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setTxDacLinkPage");

	address = 0x300;
	data = 0;
	rtn_bpf = BPF_HM_DEVC_TXDAC_READ(address, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	if(0 == pageno){
		data &= ~0x00000004;
	}else{
		data |= 0x00000004;
	}

	rtn_bpf = BPF_HM_DEVC_TXDAC_WRITE(address, data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }

	address = 0x470;	/* 0x470~473 */
	for( count=0 ; count < 4 ;count++ ,address++){
		count2 = 0;
		while(1){
			data = 0;
			rtn_bpf = BPF_HM_DEVC_TXDAC_READ(address, &data);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
			if((data & 0x3) == 0x3){
				break;
			}
			count2++;
			if(count2 >= 10){
				BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_WARNING , "f_dpda_mmW_setTxDacLinkPage TRACE, addr:0x%08x count = %x",address, count);
				if(address == 0x470){
					RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_1_MMW;
				}else if(address == 0x471){
					RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_2_MMW;
				}else if(address == 0x472){
					RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_3_MMW;
				}else{
					RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_TX_DAC_NG09_4_MMW;
				}
				return RFIC_MMW_NG;

			}
		}
	}

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_RETURN , "f_dpda_mmW_setTxDacLinkPage finished");

	return RFIC_MMW_COMPLETE;
		
}

/********************************
 * brief  : RX_ADCの初期設定
 * param  : void
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_setRxAdc(void){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setRxAdc");
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int data;
	
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0000, 0x81);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	usleep(10000);
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0121, 0x0F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0200, 0x02);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0201, 0x02);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0310, 0x40);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0311, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0314, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0316, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0317, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0318, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0319, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x031a, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x031b, 0xC0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0330, 0x40);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0331, 0x05);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0334, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0336, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0337, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0338, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0339, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x033a, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x033b, 0xC0);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0300, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0571, 0x15);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0572, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0573, 0x40);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x056E, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x058B, 0x83);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x058C, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x058D, 0x1F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x058E, 0x03);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x058F, 0x0F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0590, 0x2F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0591, 0x20);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0592, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05B0, 0x0F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05B2, 0x54);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05B3, 0x76);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05B5, 0x01);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05B6, 0x23);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C0, 0x11);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C1, 0x11);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C2, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C3, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C4, 0x80);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C5, 0x80);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C6, 0x80);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C7, 0x80);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C8, 0x90);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05C9, 0x90);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05CA, 0x90);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x05CB, 0x90);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0571, 0x14);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	usleep(100000);
	rtn_bpf = BPF_HM_DEVC_RXADC_READ(0x056F, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return D_DU_ALMCD_RFIC_INIT_RX_ADC_RSTSNG_MMW; }
	data = data & 0x00000080;
	if(data == 0){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdc TRACE bit check, data = %x", data);
		return RFIC_MMW_NG;
	}
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1228, 0x4F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1228, 0x0F);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1222, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1222, 0x04);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1222, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1262, 0x08);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x1262, 0x00);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	usleep(1000000);
	rtn_bpf = BPF_HM_DEVC_RXADC_WRITE(0x0120, 0x04);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdc finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : RX_ADCのJESD起動
 * param  : in fpga_no 0:TRX-FPGA#1 / 1:TRX-FPGA#2
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_setRxAdcJesd(unsigned int fpga_no){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_setRxAdcJesd, fpga_no = %d", fpga_no);
	
	if(fpga_no != RFIC_MMW_TRX_FPGA1) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, fpga_no = %d", fpga_no);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int fpga_base_addr;
	unsigned int data;
	unsigned int count;
	
	fpga_base_addr = RFIC_MMW_BASE_ADDR_CTX_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdcJesd TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	
	//JSRX PHYリセット
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x0000004C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000004;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x0000004C, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//wait 10ms
		usleep(10000);
		//JSFB/RX SERDES NOT DONE確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00000200, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00040000) == 0x00000000) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdcJesd TRACE, count = %x", count);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG1_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSRX_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdcJesd TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	
	count = 0;
	while(1){
		//PHY Status確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001308, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if(data == 0x00000000) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdcJesd TRACE, count = %x", count);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG2_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//JSRX 内部SYSREFマスク解除設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data & (~0x00010000);
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//JSRX SYNC確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001034, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00000001) != 0) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdcJesd TRACE, count = %x", count);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG3_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//JSRX SYNC出力設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001030, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00000001;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001030, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_setRxAdcJesd finished");
	
	return RFIC_MMW_COMPLETE;
}


/********************************
 * brief  : RX_ADCのILAS確認
 * param  : in fpga_no 0:TRX-FPGA#1 / 1:TRX-FPGA#2
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int f_dpda_mmW_checkRxAdcIlas(unsigned int fpga_no){
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called f_dpda_mmW_checkRxAdcIlas, fpga_no = %d", fpga_no);
	
	if((fpga_no != RFIC_MMW_TRX_FPGA1) && (fpga_no != RFIC_MMW_TRX_FPGA2)) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "Invalid arguents, fpga_no = %d", fpga_no);
		return RFIC_MMW_NG;
	}
	
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	unsigned int fpga_base_addr;
	unsigned int data;
	unsigned int count;
	
	fpga_base_addr = RFIC_MMW_BASE_ADDR_JSRX_FPGA1;
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_checkRxAdcIlas TRACE FPGA1, fpga_base_addr = %x", fpga_base_addr);
	
	count = 0;
	while(1){
		//JSRX SYNC確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001300, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if((data & 0x00110001) == 0x00110001) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "f_dpda_mmW_checkRxAdcIlas TRACE, addr = %x data = %x", fpga_base_addr + 0x00001300,data);
			if((data & 0x00100000) != 0x00100000){
				RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG4_1_MMW;
			}else if((data & 0x00010000) != 0x00010000){
				RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG4_2_MMW;
			}else{
				RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG4_3_MMW;
			}
			return RFIC_MMW_RETRY_10;
		}
	}
	
	//JSRX Error Status確認
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001310, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	if(data != 0x00000000){
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "f_dpda_mmW_checkRxAdcIlas TRACE, addr = %x data = %x", fpga_base_addr + 0x00001310,data);
		RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG5_MMW;
		return RFIC_MMW_RETRY_10;
	}

	//JSRX内部SYSREFマスク設定
	rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	data = data | 0x00010000;
	rtn_bpf = BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, fpga_base_addr + 0x00001020, &data);
	if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
	
	count = 0;
	while(1){
		//JSRX エラー確認
		rtn_bpf = BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, fpga_base_addr + 0x00001404, &data);
		if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
		if(data == 0x00000000) break;
		count++;
		if(count >= 10){
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ERROR , "f_dpda_mmW_checkRxAdcIlas TRACE, addr = %x data = %x", fpga_base_addr + 0x00001404,data);
			RFIC_mmW_RF_API_LAST_ALMCD = D_DU_ALMCD_RFIC_INIT_RX_ADC_NG6_MMW;
			return RFIC_MMW_RETRY_10;
		}
	}
	
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "f_dpda_mmW_checkRxAdcIlas finished");
	
	return RFIC_MMW_COMPLETE;
}

/********************************
 * brief  : TXEQ設定
 * param  : アンテナ番号(0/1)
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_TXEQ(unsigned int antno){
	unsigned int regval;
	unsigned int regaddr;
	unsigned int data;
	unsigned int count1;
	unsigned int count2;

	regaddr = 0x40+antno;
	BPF_HM_DEVC_RFEEP_READ(regaddr, &data);

	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called %s ant:%d rfeep:0x%x val:0x%x",__func__,antno,regaddr,data);

	if(data != 0x1){
		regval = 0;
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_CC_AEQFEN(antno), &regval );
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s finished",__func__);
		return RFIC_MMW_COMPLETE;
	}

	/* RF-EEPからリードして、ひたすら0xB11001F0 or 0xB11005F0に書く */
	regaddr = 0x440 + (0x20*antno);
	for(count1=0 ; count1 < 8 ; count1++ ){
		regval = 0;
		for(count2=0;count2 < 4;count2++){
			BPF_HM_DEVC_RFEEP_READ(regaddr, &data);
			data &= 0xFF;
			regval |= (unsigned int)(data << (24 - (count2*8)));
			regaddr++;
		}
		BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_CC_AEQFCOEF(antno), &regval );
	}
	regval = 1;
	BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_CC_AEQFEN(antno), &regval );
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "%s finished",__func__);

	return RFIC_MMW_COMPLETE;
}



/********************************
 * brief  : rf_ccaからRF初期設定時にCallしてもらう関数
 * param  : なし
 * return : 正常終了 RFIC_MMW_COMPLETE
 *          異常終了 RFIC_MMW_NG
 ******************************/
int RFIC_mmW_START(void)
{
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "called RFIC_mmW_START");
	
	INT pwr_mode;
	unsigned int rtn_bpf = BPF_HM_DEVC_COMPLETE; //HALの戻り値判定用
	int rtn_mmw = RFIC_MMW_COMPLETE; //mmW設定の戻り値判定用
	unsigned char rf_board_flg = 0x01;
	unsigned char ps_serial[8];
	unsigned char rf_serial[8];
	unsigned int rf_serial_tmp;
	unsigned int i;
	unsigned short ps_addr;
	unsigned int rf_addr;
	unsigned int data;
	
	//RFボード接続判定	0xFF0A_0068
	BPF_HM_DEVC_REG_READ_BIT( D_RRH_LOG_REG_LV_HDL_READ, D_DU_REG_OTH_GPIO_DATA_2_RO, D_DU_VAL_SHIFT_L(1,21) , &data ) ;
	rf_board_flg = (UCHAR)D_DU_VAL_SHIFT_R_M(data,21,1);

	if(rf_board_flg != 0x01)
	{
	
		//RFボードシリアルNO取得
		ps_addr = D_DU_EEP_BORDINFO_MMW;
		rf_addr = D_DU_RF_EEP_MAKE_DATE1;
		for(i=0; i<8; i++){
			//PS-EEPROM
			rtn_bpf = BPF_HM_DEVC_EEPROM_READ(ps_addr, &ps_serial[i]);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
			ps_addr++;
			//RF-EEPROM
			rtn_bpf = BPF_HM_DEVC_RFEEP_READ(rf_addr, &rf_serial_tmp);
			if(rtn_bpf != BPF_HM_DEVC_COMPLETE) { return RFIC_MMW_NG; }
			rf_serial[i] = (unsigned char)(rf_serial_tmp & 0x000000FF);
			rf_addr++;
		}

		// RFボードあり
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "MMW RF Board is connected");
		
		// 通常モード
		pwr_mode = RFIC_MMW_NORMAL_POWER_MODE;
		BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "mmW Start Normal Power Mode");
		
		// (2)RF-PLL
		rtn_mmw = RFIC_mmW_RF_PLL();
		if(rtn_mmw != RFIC_MMW_COMPLETE) {
			BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_RF_PLL Failed");
			return rtn_mmw;
		}
		
		if(memcmp(ps_serial, rf_serial, (size_t)8) == 0)
		{	// シリアルNo一致の時
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "Serial No is matched");
			
			// (3)RF-AMP DAC
			rtn_mmw = RFIC_mmW_RF_AMP_DAC(0, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_RF_AMP_DAC Failed");
				return rtn_mmw;
			}
			
			// (4-1)PGC(BIAS)
			rtn_mmw = RFIC_mmW_PGC_BIAS();
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_PGC_BIAS Failed");
				return rtn_mmw;
			}
			
			// (4-2)PGC(LUT)
			rtn_mmw = RFIC_mmW_PGC_LUT();
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_PGC_LUT Failed");
				return rtn_mmw;
			}
			

			// PGC初期設定完了
			data = 1;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2000A04, &data);

			
			// (5-1)TX_VATT
			rtn_mmw = RFIC_mmW_TX_VATT_DAC(0, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_TX_VATT_DAC Failed");
				return rtn_mmw;
			}
			
			// (5-2)RX_VATT
			rtn_mmw = RFIC_mmW_RX_VATT_DAC(0, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_RX_VATT_DAC Failed");
				return rtn_mmw;
			}
			
			// (6)QMOD/QDEM DAC
			rtn_mmw = RFIC_mmW_QMOD_QDEM_DAC(0, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_QMOD_QDEM_DAC Failed");
				return rtn_mmw;
			}
			
			// QMODリセット解除[2][0]
			BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTX_RST, 0x5 );
			
			// (7)QMOD
			rtn_mmw = RFIC_mmW_QMOD(0, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_QMOD Failed");
				return rtn_mmw;
			}

			// QDEMリセット解除[3][1]
			BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTX_RST, 0xA );

			// (8)QDEM
			rtn_mmw = RFIC_mmW_QDEM(0, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_QDEM Failed");
				return rtn_mmw;
			}

			// (11)TX EQ
			rtn_mmw = RFIC_mmW_TXEQ(0);
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_TXEQ(0) Failed");
				return rtn_mmw;
			}
			rtn_mmw = RFIC_mmW_TXEQ(1);
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_TXEQ(1) Failed");
				return rtn_mmw;
			}

			
		}else
		{	//シリアルNo不一致一致の時
			BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "Serial No is not matched");
			f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_RF_UNIT_SN_MIS );

			// (3)RF-AMP DAC
			rtn_mmw = RFIC_mmW_RF_AMP_DAC(1, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_RF_AMP_DAC Failed");
				return rtn_mmw;
			}

			// (4-1)PGC(BIAS)
			rtn_mmw = RFIC_mmW_PGC_BIAS();
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_PGC_BIAS Failed");
				return rtn_mmw;
			}
			
			// (4-2)PGC(LUT)
			BPF_HM_DEVC_PGCLUT_WRITE2(0,NULL);

			// PGC初期設定完了
			data = 1;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, 0xA2000A04, &data);


			// (5-1)TX_VATT
			rtn_mmw = RFIC_mmW_TX_VATT_DAC(1, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_TX_VATT_DAC Failed");
				return rtn_mmw;
			}
			
			// (5-2)RX_VATT
			rtn_mmw = RFIC_mmW_RX_VATT_DAC(1, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_RX_VATT_DAC Failed");
				return rtn_mmw;
			}
			
			// (6)QMOD/QDEM DAC
			rtn_mmw = RFIC_mmW_QMOD_QDEM_DAC(1, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_QMOD_QDEM_DAC Failed");
				return rtn_mmw;
			}

			// QMODリセット解除[2][0]
			BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTX_RST, 0x5 );
			
			// (7)QMOD
			rtn_mmw = RFIC_mmW_QMOD(1, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_QMOD Failed");
				return rtn_mmw;
			}
			
			// QDEMリセット解除[3][1]
			BPF_HM_DEVC_REG_WRITE_BITOFF(D_RRH_LOG_REG_LV_READ_WRITE, D_DU_REG_CTX_RST, 0xA );

			// (8)QDEM
			rtn_mmw = RFIC_mmW_QDEM(1, pwr_mode);
			// 0:通常 , 1:デフォルト値
			// 0:RFIC_MMW_NORMAL_POWER_MODE(通常) , 1:RFIC_MMW_LOW_POWER_MODE(低電力)
			if(rtn_mmw != RFIC_MMW_COMPLETE) {
				BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_QDEM Failed");
				return rtn_mmw;
			}

			// (11)TX EQ
			data = 0;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_CC_AEQFEN(0), &data );
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, M_DU_REG_CC_AEQFEN(1), &data );


		}

	}
	else{
		f_com_abort( D_SYS_THDID_PF_CCA, D_DU_ALMCD_RF_UINIT_UNMOUNT );
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "MMW RF Board is not connected");
	}

	BPF_COM_LOG_DMESG("Setting TXDAC ...\n");

	// (9)TX_DAC
	rtn_mmw = RFIC_mmW_TX_DAC();
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_TX_DAC Failed:0x%08x", rtn_mmw);
		return rtn_mmw;
	}

	BPF_COM_LOG_DMESG("Setting RXDAC ...\n");
	
	// (10)RX_ADC
	rtn_mmw = RFIC_mmW_RX_ADC();
	if(rtn_mmw != RFIC_MMW_COMPLETE) {
		BPF_COM_LOG_ASSERT( D_RRH_LOG_AST_LV_ERROR, "RFIC_mmW_RX_ADC Failed:0x%08x", rtn_mmw);
		return rtn_mmw;
	}
	BPF_COM_PLOG_TRACE_RFIC( D_RRH_LOG_AST_LV_ENTER , "RFIC_mmW_START finished");
	
	return RFIC_MMW_COMPLETE;
}



