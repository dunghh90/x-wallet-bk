/*********************************************************************************/
/**
 *  @skip $Id:$
 *  @file f_fmng_rom.c
 *  @brief  fmg management ROM table define
 *  @date 2013/11/14 FFCS)linb create
 *  @date 2015/08/14 TDIPS)sasaki update FHM対応
 *  @date 2015/08/19 TDIPS)sasaki RE-FLASH WRITE WAIT中のT.O.時コール関数誤りを修正(f_fmng_re_flashAccessTO -> f_fmng_re_flashWriteTO)
 *  @date 2015/08/31 TDIPS)sasaki CT検出問題(FWファイル情報保存中にf_fmng_apiGetFirmVersion()が呼ばれない)の対処
 * 
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008-2015
 */
/*********************************************************************************/

/** @addtogroup RRH_PF_FMNG
* @{ */

#include "f_fmng_inc.h"

/** main matrix table */
void    (* const f_fmngr_matrix[D_FMNG_STATMAX][D_FMNG_EVTMAX]) (VOID *buff_adr) = 
{
	/* task init */
	{   
		f_fmng_nop,							/* 0: API Firm Erase Request	*/
		f_fmng_nop,							/* 1: Flash Erase Response		*/
		f_fmng_nop,							/* 2: API Firm Update Request	*/
		f_fmng_nop,							/* 3: Flash Write Response		*/
		f_fmng_nop,							/* 4: API Get Firm Version 		*/
		f_fmng_nop,							/* 5: Time Out					*/
		f_fmng_nop,							/* 6: API Cancel Firm Update 	*/
		f_fmng_nop,							/* 7: Tar Thread Response		*/
		f_fmng_nop,							/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_nop,							/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_nop,							/* 10: RE-FW Info Save Response	*/
		f_fmng_nop							/* 11: RE-FLASH R/W T.O.		*/
	},                                        
	/* running  */
	{                                         
		f_fmng_apiLoadEraseReq,				/* 0: API Firm Erase Request	*/
		f_fmng_nop,							/* 1: Flash Erase Response		*/
		f_fmng_apiLoadUpdReq,				/* 2: API Firm Updata Request	*/
		f_fmng_nop,							/* 3: Flash Write Response		*/
		f_fmng_apiGetFirmVersion,			/* 4: API Get Firm Version 		*/
		f_fmng_nop,							/* 5: Time Out					*/
		f_fmng_nop,							/* 6: API Cancel Firm Update 	*/
		f_fmng_nop,							/* 7: Tar Thread Response		*/
		f_fmng_flashUpdReFwInfo,			/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_flashDelReFwInfo,			/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_nop,							/* 10: RE-FW Info Save Response	*/
		f_fmng_nop							/* 11: RE-FLASH R/W T.O.		*/
	},                                        
	/* Flash R/W  */
	{                                         
		f_fmng_rejApiLoadEraseReq,			/* 0: API Firm Erase Request	*/
		f_fmng_flashEraseRsp,				/* 1: Flash Erase Response		*/
		f_fmng_apiLoadUpdReq,				/* 2: API Firm Updata Request	*/
		f_fmng_flashWrtRsp,					/* 3: Flash Write Response		*/
		f_fmng_apiGetFirmVersion,			/* 4: API Get Firm Version 		*/
		f_fmng_flashAccessTimout,			/* 5: Time Out					*/
		f_fmng_apiCancelUpdNtc,				/* 6: API Cancel Firm Update 	*/
		f_fmng_nop,							/* 7: Tar Thread Response		*/
		f_fmng_nop,							/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_nop,							/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_nop,							/* 10: RE-FW Info Save Response	*/
		f_fmng_nop							/* 11: RE-FLASH R/W T.O.		*/
	},  
	/* Flash R/W  WAIT */
	{										  
		f_fmng_nop,							/* 0: API Firm Erase Request	*/
		f_fmng_flashEraseRsp,				/* 1: Flash Erase Response		*/
		f_fmng_rejApiLoadUpdReq,			/* 2: API Firm Updata Request	*/
		f_fmng_nop,							/* 3: Flash Write Response		*/
		f_fmng_apiGetFirmVersion,			/* 4: API Get Firm Version 		*/
		f_fmng_flashAccessTimout,			/* 5: Time Out					*/
		f_fmng_apiCancelUpdNtc,				/* 6: API Cancel Firm Update 	*/
		f_fmng_nop,							/* 7: Tar Thread Response		*/
		f_fmng_nop,							/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_nop,							/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_nop,							/* 10: RE-FW Info Save Response	*/
		f_fmng_nop							/* 11: RE-FLASH R/W T.O.		*/
	},
	/* Flash R/W  CANCEL */
	{
		f_fmng_nop, 						/* 0: API Firm Erase Request	*/
		f_fmng_flashEraseRsp,				/* 1: Flash Erase Response		*/
		f_fmng_apiLoadUpdReq,				/* 2: API Firm Updata Request	*/
		f_fmng_apiLoadEraseReq, 			/* 3: Flash Write Response		*/
		f_fmng_apiGetFirmVersion,			/* 4: API Get Firm Version		*/
		f_fmng_flashAccessTimout,			/* 5: Time Out					*/
		f_fmng_nop,							/* 6: API Cancel Firm Update 	*/
		f_fmng_nop,			 				/* 7: Tar Thread Response		*/
		f_fmng_nop,							/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_nop,							/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_nop,							/* 10: RE-FW Info Save Response	*/
		f_fmng_nop							/* 11: RE-FLASH R/W T.O.		*/
	},
	/* Decompress File */
	{
		f_fmng_nop, 						/* 0: API Firm Erase Request	*/
		f_fmng_flashEraseRsp,				/* 1: Flash Erase Response		*/
		f_fmng_nop,							/* 2: API Firm Updata Request	*/
		f_fmng_nop, 						/* 3: Flash Write Response		*/
		f_fmng_apiGetFirmVersion,			/* 4: API Get Firm Version		*/
		f_fmng_nop,							/* 5: Time Out					*/
		f_fmng_nop,							/* 6: API Cancel Firm Update	*/
		f_fmng_tarProcessRsp,				/* 7: Tar Thread Response		*/
		f_fmng_nop,							/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_nop,							/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_nop,							/* 10: RE-FW Info Save Response	*/
		f_fmng_nop							/* 11: RE-FLASH R/W T.O.		*/
	},
	/* FW Info Saving		*/
	{
		f_fmng_nop, 						/* 0: API Firm Erase Request	*/
		f_fmng_nop,							/* 1: Flash Erase Response		*/
		f_fmng_nop,							/* 2: API Firm Updata Request	*/
		f_fmng_nop, 						/* 3: Flash Write Response		*/
		f_fmng_apiGetFirmVersion,			/* 4: API Get Firm Version		*/
		f_fmng_nop,							/* 5: Time Out					*/
		f_fmng_nop,							/* 6: API Cancel Firm Update	*/
		f_fmng_nop,							/* 7: Tar Thread Response		*/
		f_fmng_nop,							/* 8: RE-FW Info Update Start	*//*	未使用	*/
		f_fmng_nop,							/* 9: RE-FW Info Delete Start	*//*	未使用	*/
		f_fmng_flashSaveReFwInfoCmp,		/* 10: RE-FW Info Save Response	*/
		f_fmng_flashSaveReFwInfoCmp			/* 11: RE-FLASH R/W T.O.		*/
	}
};         

/** main matrix table for RE-FLASH */
void    (* const f_fmngr_re_matrix[D_FMNG_RE_STATMAX][D_FMNG_RE_EVTMAX]) (VOID *buff_adr) = 
{
	/* idle					*/
	{                                         
		f_fmng_re_LoadFwDataReq,			/* 0: API Firm Read Request		*/
		f_fmng_re_SaveFwDataReq,			/* 1: API Firm Save Request		*/
		f_fmng_re_rejCancelFwAccessReq,		/* 2: API Firm Cancel			*/
		f_fmng_re_nop,						/* 3: Flash Read Response		*/
		f_fmng_re_nop,						/* 4: Flash Write Response		*/
		f_fmng_re_nop,						/* 5: Flash Erase Response		*/
		f_fmng_re_nop,						/* 6: FW Info Save Response		*/
		f_fmng_re_nop						/* 7: Time out					*/
	},
	/* Flash R/W			*/
	{                                         
		f_fmng_re_rejLoadFwDataReq,			/* 0: API Firm Read Request		*/
		f_fmng_re_nop,						/* 1: API Firm Save Request		*/
		f_fmng_re_CancelFwAccessReq,		/* 2: API Firm Cancel			*/
		f_fmng_re_flashReadRsp,				/* 3: Flash Read Response		*/
		f_fmng_re_flashWriteRsp,			/* 4: Flash Write Response		*/
		f_fmng_re_flashEraseRsp,			/* 5: Flash Erase Response		*/
		f_fmng_re_nop,						/* 6: FW Info Save Response		*/
		f_fmng_re_flashAccessTO				/* 7: Time out					*/
	},
	/* Flash WRITE WAIT		*/
	{										  
		f_fmng_re_rejLoadFwDataReq,			/* 0: API Firm Read Request		*/
		f_fmng_re_nop,						/* 1: API Firm Save Request		*/
		f_fmng_re_DiscardSavedWriteReq,		/* 2: API Firm Cancel			*/
		f_fmng_re_nop,						/* 3: Flash Read Response		*/
		f_fmng_re_nop,						/* 4: Flash Write Response		*/
		f_fmng_re_ExecSavedWriteReq,		/* 5: Flash Erase Response		*/
		f_fmng_re_nop,						/* 6: FW Info Save Response		*/
		f_fmng_re_flashWriteTO				/* 7: Time out					*/
	},
	/* Flash R/W CANCEL		*/
	{
		f_fmng_re_rejLoadFwDataReq,			/* 0: API Firm Read Request		*/
		f_fmng_re_nop,						/* 1: API Firm Save Request		*/
		f_fmng_re_nop,						/* 2: API Firm Cancel			*/
		f_fmng_re_flashReadCancelRsp,		/* 3: Flash Read Response		*/
		f_fmng_re_ExecSavedEraseReq,		/* 4: Flash Write Response		*/
		f_fmng_re_nop,						/* 5: Flash Erase Response		*/
		f_fmng_re_ExecSavedEraseReq,		/* 6: FW Info Save Response		*/
		f_fmng_re_flashAccessTO				/* 7: Time out					*/
	},
	/* FW Info Saving		*/
	{
		f_fmng_re_rejLoadFwDataReq,			/* 0: API Firm Read Request		*/
		f_fmng_re_nop,						/* 1: API Firm Save Request		*/
		f_fmng_re_CancelSaveFwInfo,			/* 2: API Firm Cancel			*/
		f_fmng_re_nop,						/* 3: Flash Read Response		*/
		f_fmng_re_nop,						/* 4: Flash Write Response		*/
		f_fmng_re_nop,						/* 5: Flash Erase Response		*/
		f_fmng_re_flashSaveReFwInfoRsp,		/* 6: FW Info Save Response		*/
		f_fmng_re_SaveFwInfoTO				/* 7: Time out					*/
	}
};


/*!
 * @name 3G and S3G maker name for 3001 type table.
 * @{
 */
const	USHORT	f_fmngr_makername_settbl[] =
{
	D_FMNG_MAKER1,				/* the maker name 'F' */
	D_FMNG_MAKER2,				/* the maker name 'R' */
	/** DCM Zynq: add "K" **/
	D_FMNG_MAKER3,				/* the maker name 'K' */
	/**TDD: add "V"**/
	D_FMNG_MAKER4,				/* the maker name 'V' */
	/**FHM: add "X"**/
	D_FMNG_MAKER5				/* the maker name 'X' */
};

/*! @} */
/*! @} */
