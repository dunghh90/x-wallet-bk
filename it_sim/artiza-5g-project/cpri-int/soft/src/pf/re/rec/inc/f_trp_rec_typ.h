/******************************************************************************************************************************/
/**
 *  @skip	$Id:$
 *  @file
 *  @brief	Table Define
 *  @note	NOTE THAT YOU MUST NOT EDIT THIS FILE MANUALLY, which is automatically generated with VBA macro. \n
 * 			See also TableSpecifications(TRIF_REC)_com.xls
 * 
 *  @date   2011/02/17 FJT)Koshida modify for M-S3G-eNBSYS-02253(TS-111)対応.
 *  @date   2011/05/24 FJT)Koshida modify for M-S3G-eNBSYS-02443(TS042)対応.
 *  @date   2016/10/04 KCN)hfuku 16B機能追加
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2016
 */
/******************************************************************************************************************************/

#ifndef	_F_TRP_REC_TYP_h
#define	_F_TRP_REC_TYP_h

/******************************************************************************************************************************/
/** common structure definition
 *  @addtogroup TRIF_REC
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	RE equipment composition information
 * @note	
 * 
 */
typedef struct{
	USHORT				mk_name;							/**< Maker name  */
	USHORT				model_no;							/**< BTS model no  */
	USHORT				eqp_no;								/**< Equipment no  */
	USHORT				re_kind;							/**< RE Kind  */
	USHORT				model_3g;							/**< 3G BTS share model  */
	USHORT				dmy06;								/**< Dummy6  */
	USHORT				pow_type;							/**< Power type  */
	USHORT				rcvant;								/**< Receving Ant number  */
	USHORT				sndant;								/**< Sending Ant number  */
	USHORT				pow_cls_w;							/**< Power class Unit  */
	USHORT				pow_cls_0;							/**< Power class(0)  */
	USHORT				pow_cls_1;							/**< Power class(1)  */
	USHORT				pow_cls_2;							/**< Power class(2)  */
	USHORT				pow_cls_3;							/**< Power class(3)  */
	USHORT				dmy15;								/**< Dummy15  */
	USHORT				dmy16;								/**< Dummy16  */
	USHORT				frqband;							/**< Frquence band  */
	USHORT				max_bnd;							/**< Max band  */
	USHORT				crr_num_20g;						/**< 2.0GHz band carrier number  */
	USHORT				dmy20;								/**< Dummy20  */
	USHORT				dmy21;								/**< Dummy21  */
	USHORT				dmy22;								/**< Dummy22  */
	USHORT				dmy23;								/**< Dummy23  */
	USHORT				dmy24;								/**< Dummy24  */
	USHORT				trxinf_type;						/**< TRX-INF type  */
	USHORT				dmy26;								/**< Dummy26  */
	USHORT				dmy27;								/**< Dummy27  */
	USHORT				dmy28;								/**< Dummy28  */
	USHORT				dmy29;								/**< Dummy29  */
	USHORT				dmy30;								/**< Dummy30  */
	USHORT				dmy31;								/**< Dummy31  */
	USHORT				dmy32;								/**< Dummy32  */
	USHORT				dmy33;								/**< Dummy33  */
	USHORT				dmy34;								/**< Dummy34  */
	USHORT				dmy35;								/**< Dummy35  */
	USHORT				dmy36;								/**< Dummy36  */
	USHORT				dmy37;								/**< Dummy37  */
	USHORT				dmy38;								/**< Dummy38  */
	USHORT				dmy39;								/**< Dummy39  */
	USHORT				dmy40;								/**< Dummy40  */
	USHORT				dmy41;								/**< Dummy41  */
	USHORT				dmy42;								/**< Dummy42  */
	USHORT				dmy43;								/**< Dummy43  */
	USHORT				dmy44;								/**< Dummy44  */
	USHORT				dmy45;								/**< Dummy45  */
	USHORT				dmy46;								/**< Dummy46  */
	USHORT				dmy47;								/**< Dummy47  */
	USHORT				dmy48;								/**< Dummy48  */
	USHORT				dmy49;								/**< Dummy49  */
	USHORT				dmy50;								/**< Dummy50  */
	USHORT				dmy51;								/**< Dummy51  */
	USHORT				dmy52;								/**< Dummy52  */
	USHORT				dmy53;								/**< Dummy53  */
	USHORT				dmy54;								/**< Dummy54  */
	USHORT				dmy55;								/**< Dummy55  */
	USHORT				dmy56;								/**< Dummy56  */
	USHORT				dmy57;								/**< Dummy57  */
	USHORT				dmy58;								/**< Dummy58  */
	USHORT				dmy59;								/**< Dummy59  */
	USHORT				dmy60;								/**< Dummy60  */
	USHORT				dmy61;								/**< Dummy61  */
	USHORT				dmy62;								/**< Dummy62  */
	USHORT				dmy63;								/**< Dummy63  */
	USHORT				dmy64;								/**< Dummy64  */
}T_REC_REEQPINF;

/**
 * @brief	RE slot information
 * @note	
 * 
 */
typedef struct{
	USHORT				signal_kind;						/**< signal_kind  */
	USHORT				result;								/**< result  */
	USHORT				slt_num;							/**< Slot number 16B KCN add */
	USHORT				slt_no[CMD_NUM16];					/**< Slot no     16B KCN chg */
	USHORT				crdcap[CMD_NUM16];					/**< RE Card function part information 16B KCN chg */
	USHORT				slt_idx[CMD_SLOTNO_MAX];			/**< RE Card function part information 16B KCN chg */
}T_REC_RESLTINF;

/**
 * @brief	セクタ部毎機能部状態
 * @note	
 * 
 */
typedef struct{
	USHORT				trb_sts;
	USHORT				fb_bsvtrm;
	USHORT				fb_esvtrm;
	USHORT				sec_num;
}T_REC_SEC_FBSV;

/**
 * @brief   セクタ部スロット情報
 */
typedef struct{
	USHORT				set_flag;
	USHORT				sector;
	T_REC_SEC_FBSV		fbsts[CMD_NUM29];
}T_REC_SEC_SLOT_SV;

/**
 * @brief   セクタ部毎機能部状態(OFTRX) 
 */
typedef struct{
	USHORT				trb_sts;
	USHORT				dmy;
}T_REC_SEC_FBSV_OFTRX;

/**
 * @brief   セクタ部スロット情報(OFTRX)
 */
typedef struct{
	T_REC_SEC_FBSV_OFTRX		fbsts[CMD_NUM29];
}T_REC_SEC_SLOT_SV_OFTRX;

/**
 * @brief   sector function block status table
 */
typedef struct{
	T_REC_SEC_SLOT_SV			slot_sv[CMD_MAXSLOTNO];
	T_REC_SEC_SLOT_SV_OFTRX		slot_sv_oftrx[CMD_CPRINO_MAX][CMD_SLOTINF_MAX];
}T_REC_SEC_FBSTS_TBL;

/**
 * @brief	RE card status -> Function Block informtion -> Carrier supervision information -> Carrier&Branch supervision information
 * @note	
 * 
 */
typedef struct{
	USHORT				brc_no;								/**< Branch No  */
	USHORT				brc_bsvtrm;							/**< Branch basic supervision item  */
	USHORT				brc_esvtrm;							/**< Branch extend supervision item  */
//	USHORT				dmy;								/**< Dummy  */
}T_REC_BRC_SV;

/**
 * @brief	RE card status -> Function Block informtion -> Carrier supervision information
 * @note	
 * 
 */
typedef struct{
	USHORT				crr_no;								/**< Carrier No  */
	USHORT				crr_bnd;							/**< Carrier system band  */
	USHORT				crr_bsvtrm;							/**< Carrier function basic supervision item  */
	USHORT				crr_esvtrm;							/**< Carrier function extend supervision item  */
	USHORT				brc_num;							/**< Branch number  */
	USHORT				sct_no;								/**< Sector No  */
	T_REC_BRC_SV		brc_sv[D_REC_BRCNUM_MAX];			/**< Carrier&Branch supervision information  */
}T_REC_CRR_SV;

/**
 * @brief	RE card status -> Function Block informtion
 * @note	
 * 
 */
typedef struct{
	USHORT				fb_typ;								/**< Function type  */
	USHORT				trb_sts;							/**< Function Trouble status  */
	USHORT				fb_bsvtrm;							/**< Function Block basic supervision item  */
	USHORT				fb_esvtrm;							/**< Function Block extend supervision item  */
	USHORT				crr_num;							/**< Carrier number  */
	USHORT				dmy;								/**< Dummy  */
	T_REC_CRR_SV		crr_sv[CMD_CARNO_MAX];				/**< Carrier supervision information  */
}T_REC_FBSTS;

/**
 * @brief	RE card status
 * @note	
 * @date 2015/10/22 FPT)Tan Add カード制御項目, fix bug BUG_122_IT2_20-7-1
 */
typedef struct{
	USHORT				signal_kind;						/** 信号種別 */
	USHORT				result;								/** 応答結果 */
	USHORT				slt_no;								/**< Slot number  */
	USHORT				y;									/**< Year  */
	USHORT				md;									/**< Month Day  */
	USHORT				h;									/**< Hour  */
	USHORT				ms;									/**< Minutes Second  */
	USHORT				card_svitem;						/**< カード基本監視項目  */
	USHORT				card_extsvitem;						/**< カード拡張監視項目  */
	USHORT				card_cntitem;						/**< カード制御項目	*/  /* FPT)Tan Add */
	USHORT				cnt_code;							/**< 制御コード  */
	USHORT				func_item;							/**< 機能部固有制御項目  */
	USHORT				car_inf;							/**< キャリア情報		*/
	USHORT				alm_num;							/**< Alm information number  */
	USHORT				alm_inf[CMD_CRDALM_INFNUMMAX];				/**< ALM information   */
	USHORT				err_num;							/**< ERR information number  */
	USHORT				err_inf[CMD_CRDERR_INFNUMMAX];				/**< ERR information  */
}T_REC_CRDSTS_3G;

/**
 * @brief	RE card status
 * @note	
 * 
 */
typedef struct{
	USHORT				signal_kind;						/** 信号種別 */
	USHORT				result;								/** 応答結果 */
	USHORT				slt_no;								/**< Slot number  */
	UCHAR				crd_name[CMD_NUM8];					/**< Card name  */
	USHORT				y;									/**< Year  */
	USHORT				md;									/**< Month Day  */
	USHORT				h;									/**< Hour  */
	USHORT				ms;									/**< Minutes Second  */
	USHORT				crd_bsvtrm;							/**< Card basic supervision item  */
	USHORT				crd_esvtrm;							/**< Card extend supervision item  */
	USHORT				crd_bctltrm;						/**< Card basic control item  */
	USHORT				crd_ectltrm;						/**< Card extend control item  */
	USHORT				fb_num;								/**< Function Block number  */
	T_REC_FBSTS			fbsts[D_REC_FBNUM_MAX];				/**< Function Block informtion  */
	USHORT				alminf_num;							/**< Alm information number  */
	USHORT				alminf[CMD_CRDALM_INFNUMMAX_HIG];				/**< ALM information   */
	USHORT				errinf_num;							/**< ERR information number  */
	USHORT				errinf[CMD_CRDERR_INFNUMMAX_HIG];				/**< ERR information  */
}T_REC_CRDSTS;

/**
 * @brief	RE card status information for 3G
 * @note	
 * 
 */
typedef struct{
	T_REC_CRDSTS_3G		sts[CMD_SLOTINF_MAX];				/**< RE carrier status change report  */
}T_REC_RECRDSTS_3G;

/**
 * @brief	RE card status information
 * @note	
 * 
 */
typedef struct{
	T_REC_CRDSTS		sts[CMD_SLOTINF_MAX];				/**< RE carrier status change report  */
}T_REC_RECRDSTS;

/**
 * @brief	RE carrier status change report -> Branch status information
 * @note	
 * 
 */
typedef struct{
	USHORT				brc_no;								/**< Branch No  */
	USHORT				brcsts;								/**< Branch status  */
	USHORT				brcsts_ul;							/**< Branch status(UL)  */
	USHORT				brcsts_dl;							/**< Branch status(DL)  */
}T_REC_BRCSTS;

/**
 * @brief	RE carrier status change report -> Card slot information of the carrier
 * @note	
 * 
 */
typedef struct{
	USHORT				re_kind;							/**< RE kind  */
	USHORT				am_no;								/**< AMP/OF-TRX number  */
	USHORT				slf_no;								/**< Shlef No  */
	USHORT				slt_no;								/**< Slot No  */
}T_REC_CRRCRD_INF;

/**
 * @brief	RE carrier status change report for 3G
 * @note	
 * 
 */
typedef struct{
	USHORT				crr_no;								/**< Carrier number  */
	USHORT				crrsts;								/**< Carrier status  */
	USHORT				crreqp;								/**< キャリア構成状態  */
	USHORT				alminf_num;							/**< Alarm information number  */
	USHORT				alminf[CMD_ALM_NUM];				/**< Alarm information  */
	USHORT				errinf_num;							/**< Error information number */
	USHORT				errinf[CMD_ERR_NUM];				/**< Error information  */	
	USHORT				crd_num;							/**< Card number   */
	USHORT				slt_no[CMD_CARCRDMAX];				/**< Slot No  */
}T_REC_CRRSTS_3G;


/**
 * @brief	RE carrier status change report
 * @note	
 * 
 */
typedef struct{
	USHORT				crr_no;								/**< Carrier number  */
	USHORT				crrsts;								/**< Carrier status  */
	USHORT				crrsts_ul;							/**< Carrier status(UL)  */
	USHORT				crrsts_dl;							/**< Carrier status(DL)  */
	USHORT				brc_num;							/**< Branch number  */
	USHORT				dmy1;								/**< Dummy  */
	T_REC_BRCSTS		brcsts_inf[D_REC_BRCNUM_MAX];		/**< Branch status information  */
}T_REC_CRRSTS;

/**
 * @brief	RE carrier status 
 * @note	
 * 
 */
typedef struct{
	USHORT				signal_kind;						/**< signal */
	USHORT				result;								/**< response result */
	USHORT				num;								/**< Carrior information number  */
	USHORT				dmy;								/**< Dummy  */
	T_REC_CRRSTS_3G		sts[CMD_CARNO_MAX_3G];					/**< RE carrier status change report  */
}T_REC_RECRRSTS_3G;

/**
 * @brief	RE carrier status 
 * @note	
 * 
 */
typedef struct{
	USHORT				signal_kind;						/**< signal */
	USHORT				result;								/**< response result */
	USHORT				num;								/**< Carrior information number  */
	USHORT				dmy;								/**< Dummy  */
	T_REC_CRRSTS		sts[CMD_CARNO_MAX];					/**< RE carrier status change report  */
	USHORT				alminf_num;							/**< Alarm information number  */
	USHORT				alminf[CMD_ALM_NUM];				/**< Alarm information  */
	USHORT				crd_num;							/**< Card number   */
	USHORT				slt_no[CMD_CARCRDMAX];				/**< Slot No  */
}T_REC_RECRRSTS;

/**
 * @brief	Toffset value information
 * @note	
 * 
 */
typedef struct{
	USHORT				signal_kind;						/**< 信号種別  */
	USHORT				result;								/**< 応答結果  */
	USHORT				tofval;								/**< Toffset value  */
	USHORT				ueq_dly;							/**< Up equipment delay value  */
	USHORT				deq_dly;							/**< Dowm equipment delay value  */
	USHORT				cbl_dly;							/**< cable delay value  */
}T_REC_TOFVAL;

/**
 * @brief	RE management table area (Sector)
 * @note	
 * 
 */
typedef struct{
	T_REC_REEQPINF		reeqpinf;							/**< RE equipment composition information  */
	T_REC_RESLTINF		resltinf;							/**< RE slot information  */
	T_REC_RECRDSTS		recrdsts;							/**< RE card status information  */
	T_REC_RECRRSTS		recrrsts;							/**< RE carrier status   */
	T_REC_TOFVAL		tofval;								/**< Toffset value information  */
}T_REC_REMSC;

/**
 * @brief	RE management table area (Sector)
 * @note	
 * 
 */
typedef struct{
	T_REC_REEQPINF		reeqpinf;							/**< RE equipment composition information  */
	T_REC_RESLTINF		resltinf;							/**< RE slot information  */
	T_REC_RECRDSTS_3G	recrdsts;							/**< RE card status information  */
	T_REC_RECRRSTS_3G	recrrsts;							/**< RE carrier status 3G  */
	T_REC_TOFVAL		tofval;								/**< Toffset value information  */
}T_REC_REMSC_3G;

/**
 * @brief	RE management table(Sector)
 * @note	
 * 
 */
typedef struct{
	T_REC_REMSC			remsc[CMD_CPRINO_MAX];				/**< RE management table area (Sector)  */
}T_REC_REMSC_TBL;

/**
 * @brief	RE management table(Sector) for 3G
 * @note	
 * 
 */
typedef struct{
	T_REC_REMSC_3G			remsc[CMD_CPRINO_MAX];				/**< RE management table area (Sector)  */
}T_REC_REMSC_TBL_3G;

/**
 * @brief	Active link status
 * @note	
 * 
 */
typedef struct{
	USHORT				cpr_no;								/**< CPRI number  */
	USHORT				sts;								/**< status  */
}T_REC_ANSTS;

/**
 * @brief	RE status information
 * @note	
 * 
 */
typedef struct{
	USHORT				mk_name;							/**< Maker name  */
	USHORT				faninf_num;							/**< FAN information number  */
	USHORT				faninf[CMD_FAN_NUM];				/**< FAN information   */
	USHORT				mkreinf_num;						/**< Maker RE information number  */
	USHORT				mkreinf[CMD_MAKER_NUM];				/**< Maker RE information  */
	USHORT				re_no;								/**< RE number  */
}T_REC_RESTS;

/**
 * @brief	RE PORT status change report information 
 * @note	
 * 
 */
typedef struct{
	USHORT				prt_out;							/**< PORT output status  */
	USHORT				prt_in;								/**< PORT input status  */
	USHORT				re_no;								/**< RE number  */
	USHORT				dmy;								/**< Dummy  */
}T_REC_REPRTSTS;

/**
 * @brief	RE Function Block Information -> Function Block informtion
 * @note	
 * 
 */
typedef struct{
	USHORT				fb_typ;								/**< Function type  */
	USHORT				trb_sts;							/**< Function Trouble status  */
	USHORT				fb_bsvtrm;							/**< Function Block basic supervision item  */
	USHORT				fb_esvtrm;							/**< Function Block extend supervision item  */
}T_REC_REFBSTS_INF;

/**
 * @brief	RE Function Block Information
 * @note	
 * 
 */
typedef struct{
	USHORT				fb_num;								/**< Function Block number  */
	USHORT				dmy;								/**< Dummy  */
	T_REC_REFBSTS_INF	refbsts_inf[28];					/**< Function Block informtion  */
}T_REC_REFBSTS;

/**
 * @brief	management table area
 * @note	
 * 
 */
typedef struct{
	T_REC_RESTS			rests;								/**< RE status change repot information  */
	T_REC_REPRTSTS		reprtsts;							/**< RE PORT status change report information   */
	T_REC_RESLTINF		resltinf;							/**< RE slot information  */
	T_REC_RECRDSTS		recrdsts;							/**< RE card status information  */
	T_REC_REFBSTS		refbsts;							/**< RE Function Block information  */
}T_REC_REMCM;

/**
 * @brief	T_REC_RECRDSTS
 * @note	
 * 
 */
typedef struct{
	T_REC_ANSTS			actsts[D_TCM_ACTIVE_LINK_GROUP_NUM];/**< Active link status for AMP  */
	T_REC_REMCM			remcm[CMD_AMPNO_MAX+CMD_CPRINO_MAX];/**< management table area  */
}T_REC_REMCM_TBL;

/**
 * @brief	C01 status management table -> management table area
 * @note	
 * 
 */
typedef struct{
	ET_REC_C01_STN		stn;								/**< status  */
	ET_REC_C01_EVT		evt;								/**< event  */
}T_REC_C01_STS;

/**
 * @brief	C01 status management table
 * @note	
 * 
 */
typedef struct{
	T_REC_C01_STS		sts;								/**< management table area  */
}T_REC_C01_STSMNG_TBL;

/**
 * @brief	C02 status management table -> management table area -> retry time
 * @note	
 * 
 */
typedef struct{
	SHORT				refrcrstsnd;						/**< RE force reset sending  */
	SHORT				refrcrst;							/**< RE force reset  */
	SHORT				rerst;								/**< RE reset  */
	SHORT				tofvalreq;							/**< Toffset value request  */
	SHORT				reeqpinfreq;						/**< RE equipment information request  */
	SHORT				resltinfreq;						/**< RE slot information request  */
	SHORT				restsreq;							/**< RE status request  */
	SHORT				reprtstsreq;						/**< RE port status request  */
	SHORT				recrdstsreq[CMD_SLTNO_MAX];			/**< RE card status request  */
	SHORT				recrrstsreq;						/**< RE carrier status request  */
	SHORT				m_recrdstsreq[CMD_SLTNO_MAX];		/**< RE card status request  */
	SHORT				m_recrrstsreq;						/**< RE carrier status request  */
	SHORT				m_restsreq;							/**< USE RE status request  */
	SHORT				m_reprtstsreq;						/**< USE RE port status request  */
	SHORT				m_reeqpinfreq;						/**< USE RE equipment information request  */
	SHORT				dlyoft;								/**< Delay offset retry flag  */
	SHORT				mtaddsetcnt;						/**< MT address set retry  */
	SHORT				mtaddsetscnt;						/**< MT address set retry(SV-MT)  */
}T_REC_C02_TC;

/**
 * @brief	C02 status management table -> management table area
 * @note	
 * 
 */
typedef struct{
	ET_REC_C02_STN		stn;								/**< status  */
	ET_REC_C02_EVT		evt;								/**< event  */
	ET_REC_C02_STN		bf_stn;								/**< status befer N/E Switch  */
	T_REC_C02_TC		tc;									/**< retry time  */
	USHORT				crdssc_num;							/**< RE card status get number(sector)  */
	USHORT				crdscm_num;							/**< RE card status get number(common)  */
	USHORT				mtsetaddfactor;						/**< MTアドレス設定開始要因  */
	USHORT				mtsetaddsts;						/**< MTアドレス設定状態  */
}T_REC_C02_STS;

/**
 * @brief	C02 status management table
 * @note	
 * 
 */
typedef struct{
	T_REC_C02_STS		sts[CMD_CPRINO_MAX];				/**< management table area  */
	USHORT				reslt_sta[D_TCM_ACTIVE_LINK_GROUP_NUM];
	USHORT				mtsetf_sta[D_TCM_ACTIVE_LINK_GROUP_NUM];
	USHORT				mtsets_sta[D_TCM_ACTIVE_LINK_GROUP_NUM];
	USHORT				dummy;
}T_REC_C02_STSMNG_TBL;

/**
 * @brief	C02 paramter table
 * @note	
 * 
 */
typedef struct{
	USHORT				cpr_no;								/**< CPRI number  */
	USHORT				dmy;								/**< dummy  */
}T_REC_C02_PRM_TBL;

/**
 * @brief	C03 status management table -> management table area -> Retry time
 * @note	
 * 
 */
typedef struct{
	SHORT				txs;								/**< TRX Set retry time  */
	SHORT				txr;								/**< TRX Release retry time  */
}T_REC_C03_TC;

/**
 * @brief	C03 status management table -> management table area
 * @note	
 * 
 */
typedef struct{
	ET_REC_C03_STN		stn;								/**< status  */
	ET_REC_C03_EVT		evt;								/**< event  */
	ET_REC_C03_STN		bf_stn;								/**< status in N/E Switch  */
	T_REC_C03_TC		tc;									/**< Retry time  */
	UINT				replyId;							/**< replyID  */
//	T_DATA_COMMONID		cmnId;								/**< Common Id  */
	USHORT				sct_no;								/**< sector number  */
	USHORT				frq;								/**< frequence  */
	USHORT				bnd;								/**< system band  */
	USHORT				brc;								/**< branch specification  */
	USHORT				sndant;								/**< send ant number  */
	USHORT				tdd_up_dwn_config;					/**< TDD uplink/downlink configuration  */
	USHORT				tdd_spe_sf_config;					/**< TDD special subframe configuration  */
	USHORT				tdd_dwnlk_cp_len;					/**< TDD downlink cyclic prefix length  */
}T_REC_C03_STS;

/**
 * @brief	C03 status management table
 * @note	
 * 
 */
typedef struct{
	T_REC_C03_STS		sts[CMD_CPRINO_MAX][CMD_CARNO_MAX];	/**< management table area  */
}T_REC_C03_STSMNG_TBL;

/**
 * @brief   C03 status management table
 * @note
 * @date    2015/10/20 TDI)satou 問処番号No.116水平展開
 */
typedef struct{
    T_REC_C03_STS sts[CMD_CPRINO_MAX][CMD_CARNO_MAX_3G];    /**< management table area  */
}T_REC_C03_STSMNG_TBL_3G;

/**
 * @brief	C03 paramter table
 * @note	
 * 
 */
typedef struct{
	USHORT				cpr_no;								/**< CPRI number  */
	USHORT				crr_no;								/**< carrier number  */
}T_REC_C03_PRM_TBL;

///**
// * @brief	C04 status management table -> management table area
// * @note	
// * 
// */
//typedef struct{
//	ET_REC_C04_STN		stn;								/**< status  */
//	ET_REC_C04_EVT		evt;								/**< event  */
//	ET_REC_C04_STN		bf_stn;								/**< status in N/E Switch  */
//	UINT				replyId;							/**< replyID  */
////	T_DATA_COMMONID		cmnId;								/**< Common Id  */
//	USHORT				tc;									/**< Retry counter  */
//	USHORT				sct_no;								/**< Sector No  */
//	USHORT				brc_no;								/**< brance number  */
//	USHORT				pow;								/**< power  */
//	ET_REC_C04_STN		stnrdy;								/**< Old status  */
//	USHORT				brcrdy;								/**< Old brance number  */
//	USHORT				powrdy;								/**< Old power  */
//}T_REC_C04_STS;
//
///**
// * @brief	C04 status management table
// * @note	
// * 
// */
//typedef struct{
//	T_REC_C04_STS		sts[CMD_CPRINO_MAX][CMD_CARNO_MAX];	/**< management table area  */
//}T_REC_C04_STSMNG_TBL;
//
///**
// * @brief	C04 paramter table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				cpr_no;								/**< CPRI number  */
//	USHORT				crr_no;								/**< carrier number  */
//}T_REC_C04_PRM_TBL;
//
///**
// * @brief	C05 status management table -> management table area
// * @note	
// * 
// */
//typedef struct{
//	ET_REC_C05_STN		stn;								/**< status  */
//	ET_REC_C05_EVT		evt;								/**< event  */
//	ET_REC_C05_STN		stnrdy;								/**< state prapare to change  */
//}T_REC_C05_STS;
//
///**
// * @brief	C05 status management table
// * @note	
// * 
// */
//typedef struct{
//	T_REC_C05_STS		sts[CMD_CPRINO_MAX];				/**< management table area  */
//}T_REC_C05_STSMNG_TBL;
//
///**
// * @brief	C05 paramter table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				cpr_no;								/**< CPRI number  */
//	USHORT				dmy;								/**< dummy  */
//}T_REC_C05_PRM_TBL;
//
///**
// * @brief	C06 status management table -> management table area
// * @note	
// * 
// */
//typedef struct{
//	ET_REC_C06_STN		stn;								/**< status  */
//	ET_REC_C06_EVT		evt;								/**< event  */
//	USHORT				result;								/**< result  */
//	USHORT				dmy;								/**< Dummy  */
//}T_REC_C06_STS;
//
///**
// * @brief	C06 status management table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				flag;								/**< Download flag  */
//	USHORT				dmy;								/**< Dummy  */
//	T_REC_C06_STS		sts[CMD_CPRINO_MAX];				/**< management table area  */
//}T_REC_C06_STSMNG_TBL;
//
///**
// * @brief	C06 paramter table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				cpr_no;								/**< CPRI number  */
//	USHORT				dmy;								/**< dummy  */
//}T_REC_C06_PRM_TBL;
//
///**
// * @brief	C07 status management table -> management table area
// * @note	
// * 
// */
//typedef struct{
//	ET_REC_C07_STN		stn;								/**< status  */
//	ET_REC_C07_EVT		evt;								/**< event  */
//	USHORT				cprsts;								/**< CPRI link status  */
//	USHORT				dmy;								/**< Dummy  */
//}T_REC_C07_STS;
//
///**
// * @brief	C07 status management table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				flag;								/**< flag  */
//	USHORT				dmy;								/**< Dummy  */
//	T_REC_C07_STS		sts[CMD_CPRINO_MAX];				/**< management table area  */
//}T_REC_C07_STSMNG_TBL;
//
///**
// * @brief	C07 paramter table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				cpr_no;								/**< CPRI number  */
//	USHORT				dmy;								/**< dummy  */
//}T_REC_C07_PRM_TBL;
//
///**
// * @brief	C08 status management table -> management table area
// * @note	
// * 
// */
//typedef struct{
//	ET_REC_C08_STN		stn;								/**< status  */
//	ET_REC_C08_EVT		evt;								/**< event  */
//	USHORT				flag;								/**< RE trouble get request send flag  */
//	USHORT				comsec;								/**< Common/Sector  */
//	USHORT				cpr_no;								/**< Cpri No in Request message  */
//	USHORT				dmy;								/**< dummy  */
//}T_REC_C08_STS;
//
///**
// * @brief	C08 status management table
// * @note	
// * 
// */
//typedef struct{
//	T_REC_C08_STS		sts[CMD_CPRINO_MAX];				/**< management table area  */
//}T_REC_C08_STSMNG_TBL;
//
///**
// * @brief	C08 paramter table
// * @note	
// * 
// */
//typedef struct{
//	USHORT				cpr_no;								/**< CPRI number  */
//	USHORT				dmy;								/**< dummy  */
//}T_REC_C08_PRM_TBL;
//
///**
// * @brief	CFF status management table -> management table area
// * @note	
// * 
// */
//typedef struct{
//	ET_REC_CFF_STN		stn;								/**< status  */
//	ET_REC_CFF_EVT		evt;								/**< event  */
//}T_REC_CFF_STS;
//
///**
// * @brief	CFF status management table
// * @note	
// * 
// */
//typedef struct{
//	T_REC_CFF_STS		sts;								/**< management table area  */
//}T_REC_CFF_STSMNG_TBL;
//
///**
// * @brief	CFF paramter table -> RE version request paramter
// * @note	
// * 
// */
//typedef struct{
//	USHORT				tgt;								/**< Target  */
//	USHORT				id;									/**< ID kind  */
//	USHORT				slt;							/**< Slot number  */
//}T_REC_CFF_REVER_PRM;
//
///**
// * @brief	CFF paramter table
// * @note	
// * 
// */
//typedef struct{
//	T_REC_CFF_REVER_PRM	rever[CMD_CPRINO_MAX+CMD_NUM1][CMD_SLTNO_MAX+CMD_NUM1];
//															/**< RE version request paramter  */
//}T_REC_CFF_PRM_TBL;

/**
 * @brief	TRX Set status manament 
 * @note	
 *
 */
typedef struct{
	USHORT				txs[CMD_CPRINO_MAX][CMD_CARNO_MAX];	/**< TRX Use status  */
	USHORT				bnd[CMD_CPRINO_MAX];				/**< System band  */
}T_REC_TXSTSMNG_TBL;

/**
 * @brief   TRX Set status manament
 * @note
 * @date    2015/10/20 TDI)satou 問処番号No.116水平展開
 */
typedef struct{
    USHORT txs[CMD_CPRINO_MAX][CMD_CARNO_MAX_3G];           /**< TRX Use status  */
    USHORT bnd[CMD_CPRINO_MAX];                             /**< System band  */
}T_REC_TXSTSMNG_TBL_3G;

/**
 * @brief	RE auto reset management table -> sts
 * @note	
 * 
 */
typedef struct{
	USHORT				crdtyp[CMD_MAXSLOTNO];				/**< RE card type  */
}T_REC_REAUTRST;

/**
 * @brief	RE auto reset management table
 * @note	
 * 
 */
typedef struct{
	T_REC_REAUTRST		sts[CMD_AMPNO_MAX+CMD_CPRINO_MAX];	/**< management table area  */
}T_REC_REAUTRST_TBL;

/**
 * @brief	Timer management table -> timer management information area
 * @note	
 * 
 */
typedef struct{
	bpf_ru_hrtm_key_inf_t	key_inf;						/**< key information  */
	UINT				tim_id;								/**< timer ID  */
}T_REC_TIMMNG_INF;

/**
 * @brief	Timer management table
 * @note	
 * 
 */
typedef struct{
	USHORT				num;								/**< timer number  */
	USHORT				dmy;								/**< dummy  */
	T_REC_TIMMNG_INF	inf[D_REC_TIM_MAX];					/**< timer management information area  */
}T_REC_TIMMNG_TBL;

/**
 * @brief	AMP/Slot->CPRI No convert table -> Slot information of AMP
 * @note	
 * 
 */
typedef struct{
	USHORT				cpr_no;								/**< CPRI number  */
	USHORT				dmy;								/**< dummy  */
}T_REC_AMPSLT_CPR;

/**
 * @brief	AMP/Slot->CPRI No convert table
 * @note	
 * 
 */
typedef struct{
	T_REC_AMPSLT_CPR	cpr[CMD_AMPNO_MAX+CMD_CPRINO_MAX][CMD_SLTNO_MAX];
															/**< Slot information of AMP  */
}T_REC_AMPSLT_CPR_TBL;

/**
 * @brief	CPRI No -> AMP No convert tabl -> CPRI link information of AMP
 * @note	
 * 
 */
typedef struct{
	USHORT				amp_no;								/**< AMP number  */
	USHORT				dmy;								/**< dummy  */
}T_REC_CPR_AMP;

/**
 * @brief	CPRI No -> AMP No convert tabl
 * @note	
 * 
 */
typedef struct{
	T_REC_CPR_AMP		amp[CMD_CPRINO_MAX];				/**< CPRI link information of AMP  */
}T_REC_CPR_AMP_TBL;

/**
 * @brief	N/E Switch status management
 * @note	
 * 
 */
typedef struct{
	USHORT				end_flg;							/**< N/E Switch Start response flag  */
	USHORT				c03end_flg[CMD_CPRINO_MAX][CMD_CARNO_MAX];
															/**< Auto Signal Stop Flag for C03  */
	USHORT				c04end_flg[CMD_CPRINO_MAX][CMD_CARNO_MAX];
															/**< Auto Signal Stop Flag for C04  */
}T_REC_NESTSMNG_TBL;

/**
 * @brief	Time type -> time value convert table -> timer value area
 * @note	
 * 
 */
typedef struct{
	UINT				tim_val;							/**< timer value  */
}T_REC_TMK_VAL;

/**
 * @brief	Time type -> time value convert table
 * @note	
 * 
 */
typedef struct{
	T_REC_TMK_VAL		val[EC_REC_TMK_MAX];				/**< timer value area  */
}T_REC_TMK_VAL_TBL;

/**
 * @brief	Runhistory log table -> collect flag
 * @note	
 * 
 */
typedef struct{
	UCHAR				use_rests;							/**< RE status in use satte  */
	UCHAR				int_to;								/**< sending interval T.O.  */
	UCHAR				dmy[CMD_NUM2];						/**< dummy  */
}T_REC_RUNHIS_COL_FLG;

/**
 * @brief	Runhistory log table -> run history information -> Log head
 * @note	
 * 
 */
typedef struct{
	UCHAR				year;								/**< year  */
	UCHAR				month;								/**< month  */
	UCHAR				day;								/**< day  */
	UCHAR				hour;								/**< hour  */
	UCHAR				min;								/**< min  */
	UCHAR				sec;								/**< sec  */
	UCHAR				msec1;								/**< msec1  */
	UCHAR				msec2;								/**< msec2  */
	UINT				msgid;								/**< msgid  */
	UINT				fb_no;								/**< fb_no  */
	USHORT				sig_kind;							/**< sig_kind  */
	USHORT				re_kind;							/**< re_kind  */
	UCHAR				cpr_no;								/**< cpr_no  */
	UCHAR				soc_no;								/**< slot number  */
	UCHAR				ret_code;							/**< return code  */
	UCHAR				rhk;								/**< run history kind  */
	USHORT				rhk_ng_fct;							/**< run history NG fact  */
	USHORT				dmy;								/**< Dummy  */
}T_REC_RUNHIS_HEAD;

/**
 * @brief	Runhistory log table -> run history information -> Message body
 * @note	
 * 
 */
typedef struct{
	UCHAR				dat[D_REC_RUNHIS_BSIZE];			/**< data  */
}T_REC_RUNHIS_BODY;

/**
 * @brief	Runhistory log table -> run history information
 * @note	
 * 
 */
typedef struct{
	T_REC_RUNHIS_HEAD	head;								/**< Log head  */
	T_REC_RUNHIS_BODY	body;								/**< Message body  */
}T_REC_RUNHIS;

/**
 * @brief	Runhistory log table
 * @note	
 * 
 */
typedef struct{
	CHAR				tnm[CMD_NUM8];										/**< Table name  */
	T_REC_RUNHIS_COL_FLG	col_flg;										/**< collect flag  */
	UINT				wrt_idx;											/**< write index  */
	T_REC_RUNHIS		inf[CMD_LOG_WRITE_INDEX_MAX][D_REC_RUNHIS_NUM];		/**< run history information  */
}T_REC_RUNHIS_TBL;

/**
 * @brief	Reg history log table -> run history information -> Message head
 * @note	
 * 
 */
typedef struct{
	UCHAR				year;								/**< year  */
	UCHAR				month;								/**< month  */
	UCHAR				day;								/**< day  */
	UCHAR				hour;								/**< hour  */
	UCHAR				min;								/**< min  */
	UCHAR				sec;								/**< sec  */
	UCHAR				msec1;								/**< msec1  */
	UCHAR				msec2;								/**< msec2  */
	UINT				*adr;								/**< address  */
	UINT				val;								/**< register value  */
}T_REC_REGHIS_HEAD;

/**
 * @brief	Reg history log table -> run history information
 * @note	
 * 
 */
typedef struct{
	T_REC_REGHIS_HEAD	head;								/**< Message head  */
}T_REC_REGHIS;

/**
 * @brief	Reg history log table
 * @note	
 * 
 */
typedef struct{
	CHAR				tnm[CMD_NUM8];						/**< Table name  */
	UINT				dmy;								/**< dummy  */
	UINT				wrt_idx;							/**< write index  */
	T_REC_REGHIS		inf[D_REC_REGHIS_NUM];				/**< run history information  */
}T_REC_REGHIS_TBL;

/**
 * @brief	Register address table
 * @note	
 * 
 */
typedef struct{
	UINT				*adr[EC_REC_CRK_MAX][CMD_CPRINO_MAX];
															/**< register address  */
}T_REC_CRA_TBL;

/**
 * @brief	Register address table
 * @note	
 * 
 */
typedef struct{
	UINT				*adr[EC_REC_NRK_MAX];				/**< register address  */
}T_REC_NRA_TBL;

/**
 * @brief	Err fact table
 * @note	
 * 
 */
typedef struct{
	UINT				fct[EC_REC_ERK_MAX][CMD_CPRINO_MAX];/**< ERR cause  */
}T_REC_ERF_TBL;

/**
 * @brief	Time out type notice Table -> Notify type
 * @note	
 * 
 */
typedef struct{
	bpf_ru_hrtm_notify_type_t	ntc_kind;					/**< notify type  */
}T_REC_TMK_NTC;

/**
 * @brief	Time out type notice Table
 * @note	
 * 
 */
typedef struct{
	T_REC_TMK_NTC		ntc[EC_REC_TMK_MAX];				/**< Notify type  */
}T_REC_TMK_NTC_TBL;

/**
 * @brief	RE Function Block trouble status Table
 * @note	
 * 
 */
typedef struct{
	USHORT		fb_typ;										/**< RE Function Block trouble status  */
	USHORT		fb_bit;										/**< RE Function Block Bit  */
}T_REC_RESLTFB_CNV;

/**
 * @brief	RE Function Block trouble status Table
 * @note	
 * 
 */
typedef struct{
	T_REC_RESLTFB_CNV		cnv[CMD_NUM12];					/**< RE Function Block information  */
}T_REC_RESLTFB_TBL;

/**
 * @brief	RE Function Block trouble status Table
 * @note	
 * 
 */
typedef struct{
	USHORT		sts[CMD_NUM16];								/**< RE Function Block trouble status  */
}T_REC_REFBTRB_TBL;

/**
 * @brief	RE Carrie&Branch valid state Table
 * @note	
 * 
 */
typedef struct{
	USHORT		sts[CMD_NUM2][D_REC_BRCNUM_MAX][D_REC_BRCNUM_MAX];
															/**< RE Carrie&Branch valid state  */
}T_REC_CRRBRCSTS_TBL;

/**
 * @brief	ALL Table Log table -> Notify type
 * @note	
 * 
 */
typedef struct{
	CHAR				nic[CMD_NUM8];						/**< notify  */
	CHAR				tbl_name[CMD_NUM32];				/**< Table name  */
	VOID				*tbl_adr;							/**< Table address  */
	UINT				tbl_size;							/**< Table size  */
}T_REC_LOGTBL;

/**
 * @brief	ALL Table Log table
 * @note	
 * 
 */
typedef struct{
	T_REC_LOGTBL		inf[D_REC_LOGTBL_NUM];				/**< Notify type  */
}T_REC_LOGTBL_TBL;

/**
 * @brief	Timer information
 * @note	
 * 
 */
typedef struct{
	USHORT				cls;								/**< notify  */
	USHORT				tmk;								/**< Table name  */
	USHORT				cpr_no;								/**< Dummy  */
	USHORT				soc_no;								/**< Table address  */
}T_REC_TIMELM;

/**
 * @brief	Slot no list
 * @note	
 * 
 */
typedef struct{
	USHORT				num;								/**< number  */
	USHORT				slt_no[CMD_SLTNO_MAX];				/**< slot No  */
	USHORT				dmy;								/**< Dummy  */
}T_REC_SLTLST;

/**
 * @brief	Slot no list
 * @note	
 * 
 */
typedef struct{
	UCHAR				year;								/**< year  */
	UCHAR				month;								/**< month  */
	UCHAR				day;								/**< day  */
	UCHAR				hour;								/**< hour  */
	UCHAR				min;								/**< min  */
	UCHAR				sec;								/**< sec  */
	UCHAR				msec1;								/**< msec1  */
	UCHAR				msec2;								/**< msec2  */
}T_REC_BCDTIM;

/**
 * @brief	Invalid log paramter table
 * @note	
 * 
 */
typedef struct{
	UINT				index;								/**< Index  */
	UCHAR				data[CMD_NUM48];					/**< data  */
}T_REC_IVLPRM_TBL;


/**
 * @brief	REC Receiving buffer
 * @note	
 * 
 */
typedef struct{
//    T_MSGHEAD head;
    union{
        VOID* datp;
        UCHAR data[2048-sizeof(T_MSGHEAD)];
    };
}T_REC_MSGRCV_TBL;


/**
 * @brief	要求送信、応答受信ログ
 * @note	
 * 
 */
typedef struct{
	UINT	Req_Count;
	UINT	Rsp_Count;
	T_REC_BCDTIM	Req_Time;
	T_REC_BCDTIM	Rsp_Time;
}T_REC_REQRSP_TBL;

/**
 * @brief   要求送信、応答受信ログ保存テーブル
 */
typedef struct{
	USHORT	SaveCount;
	USHORT	SaveKind[D_REC_SAVECOUNT_MAX];
	T_REC_REQRSP_TBL	SaveData[D_REC_SAVECOUNT_MAX];
}T_REC_REQRSP_SAVE_TBL;


/** @} */

/**
 * @brief   function block state link manage
 */
typedef struct{
	T_TCM_DLIST_NODE	propNode;
	T_TCM_DLIST_MNG		propList[2];
	T_TCM_DLIST_NODE	refNode;
	T_TCM_DLIST_MNG		refList;
	USHORT				newListIdx;
	USHORT				objIdx;
}T_REC_FBSTSLINKMNG;

/**
 * @brief   RE function block status common/sector
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
}T_REC_REFBSTSCMNSEC;

/**
 * @brief   RE sector function block status
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
}T_REC_RESECFBSTS;

/**
 * @brief   RE card function block status
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
}T_REC_RECARDFBSTS;

/**
 * @brief   RE card function block status information
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
}T_REC_REFBSTSINFO;

/**
 * @brief   RE sector function block status information
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
}T_REC_RESECFBSTSINFO;

/**
 * @brief   RE card function block status information
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
}T_REC_RECARDFBSTSINFO;


/******************************************************************************************************************************/
/** Active Link Group management table
 *  @addtogroup TRIF_REC 
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Active Link Group Object -> Active Link Group Object Link Information -> Active Link Information
 * @note	
 * 
 */
typedef struct{
	USHORT				reObjIdx;							/**< RE Object Index  */
	USHORT				cpriObjIdx;							/**< CPRI Link Object Index  */
}T_REC_ACTINFO;

/**
 * @brief	Active Link Group Object -> Active Link Group Object Link Information
 * @note	オブジェクトリンク情報
 * 
 */
typedef struct{
	T_TCM_DLIST_NODE	freeNode;							/**< free Node  */
	T_TCM_DLIST_MNG		propList[ET_REC_ACTLNKGRP_LINKLIST_NUM];
															/**< property List  */
	T_REC_ACTINFO		actInfo;							/**< Active Link Information  */
}T_REC_ACTLNKGRPOBJLINKINFO;

/**
 * @brief	Active Link Group Object -> RE Function Block status -> RE Function Block Status Information Common -> RE Function Block status Information
 * @note	共通機能部 \n
 * 			Indexは \n
 * 			 \n
 * 			[00]:OA-RA-INF \n
 * 			[01]:EX-OA-RA-INF \n
 * 			[02]:TILT-INF \n
 * 			[03]:PORT-INF \n
 * 			[04]:MT-INF \n
 * 			[05]:CLK-INF \n
 * 			
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
}T_REC_REFBSTSCMN;

/**
 * @brief	Active Link Group Object -> RE Function Block status -> RE Function Block Status Information Common
 * @note	
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	T_REC_REFBSTSCMN	reFbSts[D_REC_CMNFB_NUM];			/**< RE Function Block status Information  */
}T_REC_REFBSTSCMNINFO;

/**
 * @brief	Active Link Group Object -> RE Function Block status -> RE Function Block Status Information Sector -> RE Function Block status Information
 * @note	セクタ機能部 \n
 * 			Indexは \n
 * 			 \n
 * 			[00]:DUP \n
 * 			[01]:LNA \n
 * 			[02]:T-PA \n
 * 			[03]:TRX \n
 * 			[04]:TRX-INF \n
 * 			[05]:3GRF-INF \n
 * 			
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
}T_REC_REFBSTSSEC;

/**
 * @brief	Active Link Group Object -> RE Function Block status -> RE Function Block Status Information Sector
 * @note	
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	T_REC_REFBSTSSEC	reFbSts[D_REC_SECFB_NUM];			/**< RE Function Block status Information  */
}T_REC_REFBSTSSECINFO;

/**
 * @brief	Active Link Group Object -> RE Function Block status
 * @note	
 * 
 */
typedef struct{
	T_REC_REFBSTSCMNINFO	reFbStsCmnInfo;					/**< RE Function Block Status Information Common  */
	T_REC_REFBSTSSECINFO	reFbStsSecInfo;					/**< RE Function Block Status Information Sector  */
}T_REC_REFBSTSMNG;

/**
 * @brief	Active Link Group Object
 * @note	Activeリンクグループオブジェクト \n
 * 			 \n
 * 			理論上の最大値は、接続可能なCPRI LINK最大数
 * 
 */
typedef struct{
	T_REC_ACTLNKGRPOBJLINKINFO	linkInfo;					/**< Active Link Group Object Link Information  */
	USHORT				actLnkGrpNo;						/**< Active Link Group Number  */
	USHORT				actSetSts;							/**< Active Setting Status  */
	T_REC_REFBSTSMNG	reFbStsMng;						/**< RE Function Block status  */
}T_REC_ACTLNKGRPOBJ;

/**
 * @brief	Active Link Group management table
 * @note	This table manage Active Link Group Object.
 * 
 */
typedef struct{
	USHORT				listIdCnt;							/**< list ID Counter  */
	UCHAR				dummy[2];							/**< Dummy  */
	T_TCM_DLIST_MNG		freeList;							/**< frre List  */
	USHORT				actLnkGrp2Obj[D_TCM_ACTLNKGRP_NUM];	/**< Active Link Group to object convert  */
	T_REC_ACTLNKGRPOBJ	actLnkGrpObj[D_REC_ACTLNKGRP_INDEX_NUM];
															/**< Active Link Group Object  */
}T_REC_ACTLNKGRPMNG;

/** @} */

/******************************************************************************************************************************/
/** RE management table
 *  @addtogroup TRIF_REC 
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	RE kind information to object convert
 * @note	RE種別、RE番号から \n
 * 			REオブジェクトの配列Indexへの変換
 * 
 */
typedef struct{
	USHORT				bre[D_TCM_BRE_NUM];					/**< BRE   */
	USHORT				rreLre[D_TCM_RRELRE_NUM];			/**< RRE/LRE  */
	UCHAR				dummy[1];							/**< dummy  */
}T_REC_REKINDINFO2OBJ;

/**
 * @brief	RE Object -> RE Object Link Information
 * @note	オブジェクトリンク情報
 * 
 */
typedef struct{
	T_TCM_DLIST_NODE	freeNode;							/**< free Node  */
	T_TCM_DLIST_NODE	propNode;							/**< property Node  */
	T_TCM_DLIST_MNG		propList[ET_REC_RE_LINKLIST_NUM];	/**< property List  */
	USHORT				slotNo2Obj[CMD_MAXSLOTNO];			/**< Slot Number to Card Object convert table  */
	USHORT				ActLnkGrpObjIdx;					/**< Active Link Group Object Index  */
	UCHAR				dummy[2];							/**< Dummy  */
}T_REC_REOBJLINKINFO;

/**
 * @brief	RE Object -> RE kind information
 * @note	RE情報
 * 
 */
typedef struct{
	USHORT				reKind;								/**< RE Kind  */
	USHORT				reNo;								/**< RE Number  */
}T_REC_REKINDINFO;

/**
 * @brief	RE Object
 * @note	REオブジェクト \n
 * 			 \n
 * 			理論上の最大値は、接続可能なCPRI LINK最大数
 * 
 */
typedef struct{
	T_REC_REOBJLINKINFO	linkInfo;							/**< RE Object Link Information  */
	T_REC_REKINDINFO	reKindInfo;							/**< RE kind information  */
	T_REC_RESTS	rests;										/**< RE status change repot information  */
	T_REC_REPRTSTS	reprtsts;								/**< RE PORT status change report information   */
}T_REC_REOBJ;

/**
 * @brief	RE management table
 * @note	This table manage RE Object.
 * 
 */
typedef struct{
	USHORT				listIdCnt;							/**< list ID Counter  */
	UCHAR				dummy[2];							/**< Dummy  */
	T_TCM_DLIST_MNG		freeList;							/**< free List  */
	T_REC_REKINDINFO2OBJ	reKindInfo2Obj;					/**< RE kind information to object convert  */
	T_REC_REOBJ			reObj[D_REC_RE_INDEX_NUM];			/**< RE Object  */
}T_REC_REMNGTBL;

/** @} */

/******************************************************************************************************************************/
/** CPRI Link management table
 *  @addtogroup TRIF_REC 
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	CPRI Link Object -> CPRI Link Object Link Information
 * @note	オブジェクトリンク情報
 * 
 */
typedef struct{
	T_TCM_DLIST_NODE	freeNode;								/**< free Node  */
	T_TCM_DLIST_NODE	sendNode;								/**< send Node  */
	T_TCM_DLIST_NODE	propNode;								/**< prop Node  */
	T_TCM_DLIST_MNG		propList[ET_REC_CPRILINK_LINKLIST_NUM][ET_REC_CPRILINK_PROPLIST_NUM];	/**< property List  */
	USHORT				newListIdx;
	USHORT				ActLnkGrpObjIdx;						/**< Active Link Group Object Index  */
	USHORT				reObjIdx;								/**< RE Object Index  */
}T_REC_CPRILINKOBJLINKINFO;

/**
 * @brief	CPRI Link Object -> RE Sector Function Block status information -> RE Sector Function Block Status Information Sector -> RE Sector Function Block status -> Carrier&Branch supervision information
 * @note	ブランチ情報 \n
 * 			Indexは \n
 * 			 \n
 * 			(ブランチ番号 - 1)
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				bsvtrm;								/**< Branch basic supervision item  */
	USHORT				esvtrm;								/**< Branch extend supervision item  */
}T_REC_RESECBRCSTS;

/**
 * @brief	CPRI Link Object -> RE Sector Function Block status information -> RE Sector Function Block Status Information Sector -> RE Sector Function Block status
 * @note	セクタ機能部 \n
 * 			Indexは \n
 * 			 \n
 * 			[00]:DUP \n
 * 			[01]:LNA \n
 * 			[02]:T-PA \n
 * 			[03]:TRX \n
 * 			[04]:TRX-INF \n
 * 			[05]:3GRF-INF \n
 * 			
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
	T_REC_RESECBRCSTS	reSecBrcSts[D_REC_BRC_NUM];			/**< Carrier&Branch supervision information  */
}T_REC_RESECFBSTSSEC;

/**
 * @brief	CPRI Link Object -> RE Sector Function Block status information -> RE Sector Function Block Status Information Sector
 * @note	
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	T_REC_RESECFBSTSSEC	reSecFbSts[D_REC_SECFB_NUM];		/**< RE Sector Function Block status  */
}T_REC_RESECFBSTSSECINFO;

/**
 * @brief	CPRI Link Object -> RE Sector Function Block status information
 * @note	
 * 
 */
typedef struct{
	T_REC_RESECFBSTSSECINFO	reSecFbStsSecInfo;				/**< RE Sector Function Block Status Information Sector  */
}T_REC_RESECFBSTSMNG;

/**
 * @brief	CPRI Link Object
 * @note	CPRI LINKオブジェクト \n
 * 			 \n
 * 			理論上の最大値は、接続可能なCPRI LINK最大数
 * 
 */
typedef struct{
	T_REC_CPRILINKOBJLINKINFO	linkInfo;					/**< CPRI Link Object Link Information  */
	USHORT				cpriLinkNo;								/**< CPRI Link Number  */
	USHORT				actSetSts;							/**< Active Set Status  */
	T_REC_RESECFBSTSMNG		reSecFbStsMng;					/**< RE Sector Function Block status information  */
	T_REC_REEQPINF	reeqpinf;								/**< RE equipment composition information  */
	T_REC_RESLTINF	resltinf;								/**< RE slot information  */
	T_REC_RECRRSTS	recrrsts;								/**< RE carrier status   */
	T_REC_TOFVAL	tofval;									/**< Toffset value information  */
}T_REC_CPRILINKOBJ;

/**
 * @brief	CPRI Link management table
 * @note	This table manage CPRI Link Object.
 * 
 */
typedef struct{
	USHORT				listIdCnt;							/**< list ID Counter  */
	UCHAR				dummy1[2];							/**< Dummy  */
	T_TCM_DLIST_MNG		freeList;							/**< free List  */
	T_TCM_DLIST_MNG		sendList;							/**< send List  */
	USHORT				cpriLinkNo2Obj[6];					/**< CPRI Link Number to object convert  */
	UCHAR				dummy2[2];							/**< Dummy  */
	T_REC_CPRILINKOBJ	cpriLinkObj[D_REC_CPRI_INDEX_NUM];	/**< CPRI Link Object  */
}T_REC_CPRILINKMNGTBL;

/** @} */

/******************************************************************************************************************************/
/** Card management table
 *  @addtogroup TRIF_REC 
 *  @{
 */
/******************************************************************************************************************************/
/**
 * @brief	Card Object -> Card Object Link Information
 * @note	
 * 
 */
typedef struct{
	T_TCM_DLIST_NODE	freeNode;							/**< free Node  */
	T_TCM_DLIST_NODE	sendNode;							/**< send Node  */
	T_TCM_DLIST_NODE	propNode[D_REC_CARD_LINKNODE_NUM];	/**< property Node of  All Card  */
	USHORT				ActLnkGrpObjIdx;					/**< Active Link Group Object Index  */
	USHORT				reObjIdx;							/**< RE Object Index  */
	USHORT				cpriObjIdx;							/**< CPRI Link Object Index  */
	UCHAR				dummy[2];							/**< Dummy  */
}T_REC_CARDOBJLINKINFO;

/**
 * @brief	Card Object -> RE card status information -> RE card Function Block Status Information Common -> Function Block informtion
 * @note	共通機能部 \n
 * 			Indexは \n
 * 			 \n
 * 			[00]:OA-RA-INF \n
 * 			[01]:EX-OA-RA-INF \n
 * 			[02]:TILT-INF \n
 * 			[03]:PORT-INF \n
 * 			[04]:MT-INF \n
 * 			[05]:CLK-INF \n
 * 			
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
}T_REC_RECARDFBSTSCMN;

/**
 * @brief	Card Object -> RE card status information -> RE card Function Block Status Information Common
 * @note	
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	T_REC_RECARDFBSTSCMN	reCardFbSts[D_REC_CMNFB_NUM];	/**< Function Block informtion  */
}T_REC_RECARDFBSTSCMNINFO;

/**
 * @brief	Card Object -> RE card status information -> RE card Function Block Status Information Sector -> Function Block informtion -> Carrier supervision information -> Carrier&Branch supervision information
 * @note	ブランチ情報 \n
 * 			Indexは \n
 * 			 \n
 * 			(ブランチ番号 - 1)
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				bsvtrm;								/**< Branch basic supervision item  */
	USHORT				esvtrm;								/**< Branch extend supervision item  */
}T_REC_RECARDBRCSTS;

/**
 * @brief	Card Object -> RE card status information -> RE card Function Block Status Information Sector -> Function Block informtion -> Carrier supervision information
 * @note	キャリア情報 \n
 * 			Indexは \n
 * 			 \n
 * 			(キャリア番号 - 1)
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				sysBand;							/**< Carrier system band  */
	USHORT				bsvtrm;								/**< Carrier function basic supervision item  */
	USHORT				esvtrm;								/**< Carrier function extend supervision item  */
	USHORT				brcinf;								/**< Branch information */
	T_REC_RECARDBRCSTS	reCardBrcSts[D_REC_BRC_NUM];		/**< Carrier&Branch supervision information  */
}T_REC_RECARDCARSTS;

/**
 * @brief	Card Object -> RE card status information -> RE card Function Block Status Information Sector -> Function Block informtion
 * @note	セクタ機能部 \n
 * 			Indexは \n
 * 			 \n
 * 			[00]:DUP \n
 * 			[01]:LNA \n
 * 			[02]:T-PA \n
 * 			[03]:TRX \n
 * 			[04]:TRX-INF \n
 * 			[05]:3GRF-INF \n
 * 			
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	USHORT				type;								/**< Function Block type  */
	USHORT				trbSts;								/**< Function Trouble status  */
	USHORT				bsvtrm;								/**< Function Block basic supervision item  */
	USHORT				esvtrm;								/**< Function Block extend supervision item  */
	T_REC_RECARDCARSTS	reCardCarSts[D_REC_CRR_NUM];		/**< Carrier supervision information  */
}T_REC_RECARDFBSTSSEC;

/**
 * @brief	Card Object -> RE card status information -> RE card Function Block Status Information Sector
 * @note	
 * 
 */
typedef struct{
	T_REC_FBSTSLINKMNG	fbStsLinkMng;						/**< Function Block status Link management  */
	T_REC_RECARDFBSTSSEC	reCardFbSts[D_REC_SECFB_NUM];	/**< Function Block informtion  */
}T_REC_RECARDFBSTSSECINFO;

/**
 * @brief   Card ALM/ERR information
 */
typedef struct{
	USHORT				almInfNum;							/**< Alm information number  */
	USHORT				almInf[16];							/**< ALM information   */
	USHORT				errInfNum;							/**< ERR information number  */
	USHORT				errInf[16];							/**< ERR information  */
}T_REC_RECARDALMERRINFO;


/**
 * @brief	Card Object -> RE card status information
 * @note	
 * 
 */
typedef struct{
	USHORT				sltNo;								/**< Slot number  */
	UCHAR				cardName[8];						/**< Card name  */
	USHORT				y;									/**< Year  */
	USHORT				md;									/**< Month Day  */
	USHORT				h;									/**< Hour  */
	USHORT				ms;									/**< Minutes Second  */
	USHORT				bsvtrm;								/**< Card basic supervision item  */
	USHORT				esvtrm;								/**< Card extend supervision item  */
	USHORT				bctltrm;							/**< Card basic control item  */
	USHORT				ectltrm;							/**< Card extend control item  */
	UCHAR				dummy[2];							/**< Dummy  */
	T_REC_RECARDFBSTSCMNINFO	reCardFbStsCmnInfo;			/**< RE card Function Block Status Information Common  */
	T_REC_RECARDFBSTSSECINFO	reCardFbStsSecInfo;			/**< RE card Function Block Status Information Sector  */
	T_REC_RECARDALMERRINFO		reCardAlmErrInfo;
}T_REC_RECARDSTSMNG;

/**
 * @brief	Card Object -> RE Auto Rest information
 * @note	自律リセット情報
 * 
 */
typedef struct{
	USHORT				sts;								/**< status  */
	UCHAR				dummy[2];							/**< Dummy  */
}T_REC_REAUTORSTINFO;

/**
 * @brief	Card Object
 * @note	Cardオブジェクト
 * 
 */
typedef struct{
	T_REC_CARDOBJLINKINFO	linkInfo;						/**< Card Object Link Information  */
	USHORT				slotNo;								/**< Slot Number  */
	USHORT				crdcap;								/**< RE Card function part information  */
	T_REC_RECARDSTSMNG	reCardStsMng;						/**< RE card status information  */
	T_REC_REAUTORSTINFO	reAutoRstInfo;						/**< RE Auto Rest information  */
}T_REC_CARDOBJ;

/**
 * @brief	Card management table
 * @note	This table manage Card Object.
 * 
 */
typedef struct{
	USHORT				listIdCnt;							/**< list ID Counter  */
	UCHAR				dummy[2];							/**< Dummy  */
	T_TCM_DLIST_MNG		freeList;							/**< free List  */
	T_TCM_DLIST_MNG		sendList;							/**< send List  */
	T_REC_CARDOBJ		cardObj[D_REC_CARD_INDEX_NUM];		/**< Card Object  */
}T_REC_CARDMNGTBL;

/** @} */

/**
 * @brief   object manage table
 */
typedef struct{
	T_REC_ACTLNKGRPMNG		actLnkGrpMngTbl;		/**< Active Link Group management table  */
	T_REC_REMNGTBL			reMngTbl;				/**< RE management table  */
	T_REC_CPRILINKMNGTBL	cpriLinkMngTbl;			/**< CPRI Link management table  */
	T_REC_CARDMNGTBL		cardMngTbl;				/**< Card management table  */
}T_REC_OBJMNG_TBL;

/**
 * @brief   FB state I/F
 */
typedef struct{
	T_REC_ACTLNKGRPOBJ*		actLnkGrpObj;
	T_REC_REFBSTSCMNSEC*	reFbSts;
	
	T_REC_CPRILINKOBJ*		cpriLinkObj;
	T_REC_RESECFBSTS*		reSecFbSts;
	T_REC_RESECBRCSTS*		reSecBrcSts;
	
	T_REC_CARDOBJ*			cardObj;
	T_REC_RECARDFBSTS*		reCardFbSts;
	T_REC_RECARDCARSTS*		reCardCarSts;
	T_REC_RECARDBRCSTS*		reCardBrcSts;
}T_REC_FBSTSIF;


#endif	/* F_TRP_REC_TYP_h */
