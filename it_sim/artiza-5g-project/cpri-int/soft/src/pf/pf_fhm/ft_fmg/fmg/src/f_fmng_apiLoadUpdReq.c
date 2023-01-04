/*!
 * @skip  $ld:$
 * @file  f_fmng_apiLoadUpdReq.c
 * @brief Reception of API Load Update request
 * @date  2013/11/15 FFCS)linb create
 * @date  2015/08/10 TDIPS)sasaki update
 * @date  2015/08/14 TDIPS)sasaki FHM対応
 * @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 * @date  2015/08/25 TDIPS)sasaki PG-Relief指摘対応
 * @date  2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 * @date  2015/09/01 TDIPS)sasaki CT検出問題(RUNNING状態でFHMファイル処理中になる)の対処
 * @date  2015/10/21 TDIPS)sasaki Update
 * 
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013-2015
 */

 /*!
 * @addtogroup RRH_PF_FMNG
 * @{
 */

#include "f_fmng_inc.h"
#include <sys/syscall.h>

/*!
 *  @brief  Send API Load Update Response (result = NG) 
 *  @note   This function is processed as follows.\n
 *          - Send API Load Update Response (result = NG) .\n
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/08/10 TDIPS)sasaki update
 *  @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 *  @date  2015/08/20 TDIPS)sasaki REファイル処理中の場合の復帰処理を削除
 *  @warning	N/A
 *  @FeatureID	RRH-007-000
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
 */
VOID f_fmng_rejApiLoadUpdReq(void *pRcvMsg)
{
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_rejApiLoadUpdReq] ENTER" );				

	/* Send API Load Update Response (result = NG)  */
	f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR,"flash is access");

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_rejApiLoadUpdReq] RETURN" );				
	return;
}

/*!
 *  @brief  Reception of API Load Update request
 *  @note   This function is processed as follows.\n
 *          - if Erase flag (firm or fpag) send erase request to frw, and return;
 *          - otherwise,  send write request to frw
 *
 *  @param  *pRcvMsg     [in] Receive message
 *  @retval none
 *  @date  2013/11/15 FFCS)linb create\n
 *  @date  2015/06/15 ALP)murakami TDD-ZYNQ対応
 *  @date  2015/07/14 ALP)murakami TDD-ZYNQ SRE対応
 *  @date  2015/08/14 TDIPS)sasaki FHM対応
 *  @date  2015/08/19 TDIPS)sasaki FLASHアクセスNG時は応答結果(50:ハードウェア異常)を返す様に修正(MKレビュー指摘事項No.62対応)
 *  @date  2015/08/25 TDIPS)sasaki PG-Relief指摘対応
 *  @date  2015/08/28 TDIPS)sasaki MKレビュー指摘No.117対処
 *  @date  2015/09/01 TDIPS)sasaki CT検出問題(RUNNING状態でFHMファイル処理中になる)の対処
 *  @date  2015/10/21 TDIPS)sasaki Update M-DCM-CBBUPF-03847対処
 *  @warning	N/A
 *  @FeatureID	RRH-011-011
 *  @Bug_No	N/A
 *  @CR_No	N/A
 *  @TBD_No	N/A
*/
VOID f_fmng_apiLoadUpdReq(void *pRcvMsg)
{
	T_SYS_RCVDATA_MKHEAD_S3G *rcvdata_p = NULL;
	T_SYS_DLINFO_S3G		  *pMkDlInfo = NULL;
	T_SYS_FILEHEAD_S3G		  *pFileHead = NULL;	
	UINT					  dlNum = D_SYS_NUM0;
	UINT					  zip_type = D_SYS_NUM0;
	UINT					  zipfile_size = D_SYS_NUM0;
	UINT					  needDlNum = D_SYS_NUM0;
	UINT					  needDlNum_slave = D_SYS_NUM0;
	UINT					  index = D_SYS_NUM0;
	CHAR					  fileName[D_SYS_NUM30];
	CHAR					  unzipCmd[D_SYS_NUM128];
	INT						  retUnZip = D_SYS_NUM0;
	INT						  ret = D_SYS_NUM0;
	INT						  fileDsc = D_SYS_NUM0;
	INT						  errcd = D_SYS_NUM0;
	UINT					  actWrtSize = D_SYS_NUM0;
	UINT					  zipfile_offset = D_SYS_NUM0;
	USHORT					  a_mk_jdg = D_SYS_NUM0;				
	USHORT					  a_mk_jdg2 = D_SYS_NUM0;			
	UINT					  fileType = D_SYS_NUM0;
	UINT					  a_bootface = D_SYS_NUM0;			
	UINT					  a_bootface_cpri = D_SYS_NUM0;
	UINT					  flashWrtSize = D_SYS_NUM0;
	UINT					  cpriWrtAddr = D_SYS_NUM0;
	INT 					  block = D_SYS_NUM0;
	T_RRH_SIDE_USE 				ltside_fhm;
	T_RRH_FW_INFO				a_fwinfo;
	T_SYS_LMCWRT_REQ			flashWrtReq;
	T_SYS_LMCWRT_REQ			flashWrtReq_slave;
	T_API_FILE_SAVE_FW_REQ*		ptThrdif = NULL;
	struct sched_param	s_param;
	UINT					  dl_valid_f;
	UINT					  threadId	= D_SYS_NUM0;
	
	
	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ENTER, "[f_fmng_apiLoadUpdReq] ENTER" );				

	if(f_fmngw_fw_type == D_API_FILE_FW_TYPE_RE)
	{
		/************************************************************************/
		/* REファイルのR/W中(REC主導のREファイルダウンロード中)に				*/
		/* DLMスレッドがFWファイル更新要求(FHMファイル)を送信することはない		*/
		/************************************************************************/
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "Processing RE File RETURN");
		return;
	}

	memset(unzipCmd, 0, sizeof(unzipCmd));
	memset(&flashWrtReq, 0, sizeof(flashWrtReq));
	memset(&flashWrtReq_slave, 0, sizeof(flashWrtReq_slave));

	/* Get Common thread header from Receivd message */
	ptThrdif = (T_API_FILE_SAVE_FW_REQ *)pRcvMsg;
	
	/* Save Response QId  */
	f_fmngw_respQid = ptThrdif->header.uiSrcPQueueID;
	f_fmngw_dlwType = ptThrdif->data.dlwType;
	
	/* get pointers from data */
	rcvdata_p = (T_SYS_RCVDATA_MKHEAD_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32);
	pMkDlInfo = (T_SYS_DLINFO_S3G*)(&rcvdata_p->mk_reserve.dl_info[0]);
	pFileHead = (T_SYS_FILEHEAD_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(rcvdata_p->mk_reserve.filehead_offset));

	dlNum = swap_32(rcvdata_p->mk_reserve.dl_num);
	zip_type = swap_32(rcvdata_p->mk_reserve.zip_type);
	
	/* get values from file head */
	zipfile_size = swap_32(pFileHead->zipfile_size);
	zipfile_offset = swap_32(pFileHead->zipfile_offset);

	/****************************************************************/
	/*	実際に転送されたデータのヘッダ情報からメーカ名の確認実施	*/
	/****************************************************************/
	a_mk_jdg = swap_16(rcvdata_p->mk_jdg)>> D_SYS_NUM8;
	a_mk_jdg2 = swap_16(rcvdata_p->mk_jdg2)>> D_SYS_NUM8;

	//メーカ識別子チェック(FHM:mk1='X', mk2='X')
	if(a_mk_jdg != f_fmngr_makername_settbl[D_FMNG_MJ5] || a_mk_jdg2 != f_fmngr_makername_settbl[D_FMNG_MJ5])
	/* end for 13B */
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "maker name NG(mk_jdg=%d, mk_jdg2=%d)",
							a_mk_jdg, a_mk_jdg2 );
		
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"maker name is NG");
		return ;
	}

	/************************************************************************/
	/*	実際に転送されたデータのヘッダ情報からファイル種別名称の確認実施 */
	/************************************************************************/
	if( (rcvdata_p->file_kind[D_SYS_NUM0] != 'R') ||
		(rcvdata_p->file_kind[D_SYS_NUM1] != 'E') ||
		(rcvdata_p->file_kind[D_SYS_NUM2] != D_SYS_NUM0) )
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "file kind NG 0x%x,0x%x,0x%x",
							rcvdata_p->file_kind[D_SYS_NUM0],
							rcvdata_p->file_kind[D_SYS_NUM1],
							rcvdata_p->file_kind[D_SYS_NUM2]
		);
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"filekind data is NG");
		return ;
	}

	/* check download file number */
	/* start for 13B */
	/*	case of download valid flag(not swap) is disable	*/
	/*	download valid flagをREヘッダに設定する前のREファイルの場合	*/
	if((pFileHead->dl_valid_f == 0xFFFFFFFF) || (pFileHead->dl_valid_f == 0))
	{
		/*	Set the dev_info in the top of download infomation 	*/
		dl_valid_f = swap_32(pMkDlInfo->dev_info);
	}
	/*	case of download valid flag(not swap) is enable	*/
	else
	{
		/*	Set the download valid flag in the files infomation 	*/
		dl_valid_f = swap_32(pFileHead->dl_valid_f);
	}
	
	if( D_SYS_OK != f_fmng_checkDLInfo(dl_valid_f,0,&fileType) )
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "download invalid = %08x", dl_valid_f);
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"download invalid");
		return;
	}
	
	/* end for 13B */
	
	if( dlNum > D_SYS_NUM15)
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "download number invalid %d",dlNum);
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"file structure has error");
		return;
	}

	/* get download information and check */
	/* FIRM PL-FPGA */
	a_bootface = f_fmngw_usebootface & D_SYS_BIT1;
	f_fmngw_hard_firm_decode_size 	= 0;
	f_fmngw_hard_pl_decode_size 	= 0;
	
	if(a_bootface == D_RRH_BOOT_ACT0)
	{	
		/* set the standby face */
		a_bootface = D_RRH_BOOT_ACT1;
	}
	else
	{
		a_bootface = D_RRH_BOOT_ACT0;	
	}
	
	/* get download information and check */
	/* CPRI-FPGA */
	a_bootface_cpri = f_fmngw_usebootface_cprifpga & D_SYS_BIT1;
	f_fmngw_hard_cpri_decode_size		= 0;
	
	if(a_bootface_cpri == D_RRH_BOOT_ACT0)
	{	
		/* set the standby face */
		a_bootface_cpri = D_RRH_BOOT_ACT1;
		cpriWrtAddr = D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE1;
	}
	else
	{
		a_bootface_cpri = D_RRH_BOOT_ACT0;	
		cpriWrtAddr = D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE0;
	}
	
	for(index = D_SYS_NUM0; index < dlNum; index++, pMkDlInfo++)
	{
        /* check whether need to download */
        /* OK: need to download; NG: do not need to download */
        if( D_SYS_OK == f_fmng_checkDLInfo(swap_32(pMkDlInfo->dev_info),index,&fileType) )
        {
            if(swap_32(pMkDlInfo->wrt_add) == D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE0 || swap_32(pMkDlInfo->wrt_add) == D_RRH_QSPI_FLASH_OFFSET_FPGA_SLAVE_FACE1)
            {
				/* CPRI-FPGA size set */
				f_fmngw_hard_cpri_decode_size  = swap_32(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_size);

				/*** save cpri-fpga information ***/
				switch( f_fmngw_thrdState )
				{
					case D_FMNG_THRDSTA_RUN:			/* thread state is running */
						flashWrtReq_slave.writeFlashRom[needDlNum_slave].face = a_bootface_cpri;
						flashWrtReq_slave.writeFlashRom[needDlNum_slave].size = f_fmngw_hard_cpri_decode_size;
						flashWrtReq_slave.writeFlashRom[needDlNum_slave].offset = cpriWrtAddr;
						strcpy((CHAR*)flashWrtReq_slave.writeFlashRom[needDlNum_slave].fileName, (CHAR*)(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_name));

						f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].face = a_bootface_cpri;
						f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].size = f_fmngw_hard_cpri_decode_size;
						f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].offset = cpriWrtAddr;
						strcpy((CHAR*)f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].fileName, (CHAR*)(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_name));
						break;
					case D_FMNG_THRDSTA_FLSHRW:			/* thread state is flash RW */
					case D_FMNG_THRDSTA_FLSHRWCANCEL:	/* thread state is flash RW cancel */
						f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].face = a_bootface_cpri;
						f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].size = f_fmngw_hard_cpri_decode_size;
						f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].offset = cpriWrtAddr;
						strcpy((CHAR*)f_fmngw_flashWaitWrtReq_slave.writeFlashRom[needDlNum_slave].fileName, (CHAR*)(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_name));
						break;
					default:
						cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "Thread State NG(%d)",f_fmngw_thrdState);
						/* Send API Load Update Response (result = NG)	*/
						f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR,"Thread State NG");
						return;
				}
                needDlNum_slave++;
            }
            else
            {
                flashWrtSize = swap_32(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_size);

                if(swap_32(pMkDlInfo->wrt_add) == D_RRH_QSPI_FLASH_OFFSET_OS)
                {
                    /* FIRM size set */
                    f_fmngw_hard_firm_decode_size  = flashWrtSize;
                }
                else if(swap_32(pMkDlInfo->wrt_add) == D_RRH_QSPI_FLASH_OFFSET_PL)
                {
                    /* PL-FPGA size set */
                    f_fmngw_hard_pl_decode_size  = flashWrtSize;
                }

                /*** save firm or pl-fpga information ***/
				switch( f_fmngw_thrdState )
				{
					case D_FMNG_THRDSTA_RUN:			/* thread state is running */
						flashWrtReq.writeFlashRom[needDlNum].face = a_bootface;
						flashWrtReq.writeFlashRom[needDlNum].size = flashWrtSize;
						flashWrtReq.writeFlashRom[needDlNum].offset = swap_32(pMkDlInfo->wrt_add);
						strcpy((CHAR*)flashWrtReq.writeFlashRom[needDlNum].fileName, (CHAR*)(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_name));

						f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].face = a_bootface;
						f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].size = flashWrtSize;
						f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].offset = swap_32(pMkDlInfo->wrt_add);
						strcpy((CHAR*)f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].fileName, (CHAR*)(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_name));
						break;
					case D_FMNG_THRDSTA_FLSHRW:			/* thread state is flash RW */
					case D_FMNG_THRDSTA_FLSHRWCANCEL:	/* thread state is flash RW cancel */
						f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].face = a_bootface;
						f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].size = flashWrtSize;
						f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].offset = swap_32(pMkDlInfo->wrt_add);
						strcpy((CHAR*)f_fmngw_flashWaitWrtReq.writeFlashRom[needDlNum].fileName, (CHAR*)(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->unzipfile_name));
						break;
					default:
						cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_WARNING, "Thread State NG(%d)",f_fmngw_thrdState);
						/* Send API Load Update Response (result = NG)	*/
						f_fmng_com_sndLoadUpdRsp(D_FMNG_HW_ERR,"Thread State NG");
						return;
				}

				/* T.B.D */
				f_fmngw_file_ver[needDlNum] = swap_32(((T_SYS_FILEINFO_S3G*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+swap_32(pMkDlInfo->file_offset)))->file_ver);
				needDlNum++;
            }
        }
	}

	/************************************************************************/
	/* change the bootface to current used side 							*/
	/* In order to prevent the CARD can't start up if download faild		*/
	/************************************************************************/
	f_fmng_cm_ChgBootFace( D_SYS_BOOTFACE_USE );
	/* for cprifpga */
	f_fmng_cm_ChgBootFace_cprifpga( D_SYS_BOOTFACE_USE );
	/* set the startup version the same as use version	*/
	f_fmngw_stupver_tbl = f_fmngw_usever_tbl;

	/*********************************************************************************/
	/* FHM保持FWファイル情報(運用面)を FHM保持FWファイル情報(待機面)共有メモリに展開 */
	/*********************************************************************************/
	f_cmn_com_side_use_get(&ltside_fhm);
	f_cmn_com_fw_info_fhm_get(ltside_fhm.use_side, &a_fwinfo);
	f_cmn_com_fw_info_fhm_set((ltside_fhm.use_side ^ 0x1), &a_fwinfo);

	/* send SubGetVerRsp to update values */
	f_fmng_com_sndSubGetVerRsp();
	
	/* get the pointer of TraInvTbl */	
	a_bootface = f_fmngw_usebootface & D_SYS_BIT1;
	/* 起動面が0面のとき */
	if(a_bootface == D_RRH_BOOT_ACT0)
	{
		/* set the standby face */
		a_bootface = D_RRH_BOOT_ACT1;
	}
	else
	{
		a_bootface = D_RRH_BOOT_ACT0;
	}
	
	/**********************************************************/
	/* Create ZYNQ.tar.gz in temp path and Write data into it */	
	/**********************************************************/
	/* set the file name */
	strcpy(fileName, D_SYS_TARGZ_FILENAME);

	/* create the file */
	ret = BPF_RU_FACC_FOPEN( (bpf_ru_facc_dpathname *)D_SYS_UNZIP_DIRNAME,
                                (bpf_ru_facc_fname *)fileName,
                                 FACC_WRITE, 
                                 BPF_RU_FACC_CREATE, 
                                 &fileDsc, 
                                 &errcd );
	if( ret != BPF_RU_FACC_COMPLETE )
	{
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_RU_FACC_FOPEN NG(%d)",errcd);		
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"file open NG");
		return;
	}

	/* write the file */
	block = zipfile_size/BPF_RU_FACC_WDATA_MAX;
	for(index=0;index<block;index++)
	{
		ret = BPF_RU_FACC_FWRITE( fileDsc, 
								 BPF_RU_FACC_WDATA_MAX, 
								 (CHAR*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+zipfile_offset+BPF_RU_FACC_WDATA_MAX*index),
								 &actWrtSize,	  
								 &errcd );		

		if( (ret != BPF_RU_FACC_COMPLETE ) || (actWrtSize != BPF_RU_FACC_WDATA_MAX))
		{
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_RU_FACC_FWRITE NG(%d,%d,0x%x,0x%x)",ret,errcd,actWrtSize,index);		
			(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );
			/* Send API Load Update Response (result = NG)	*/
			f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"flash write NG");
			
			return;
		}
	}

	if(zipfile_size%BPF_RU_FACC_WDATA_MAX)
	{
		ret = BPF_RU_FACC_FWRITE( fileDsc, 
								 zipfile_size%BPF_RU_FACC_WDATA_MAX, 
								 (CHAR*)(f_cmw_rcvfirm_tbl+D_SYS_NUM32+zipfile_offset+BPF_RU_FACC_WDATA_MAX*index),
								 &actWrtSize,	  
								 &errcd );
		
		if( (ret != BPF_RU_FACC_COMPLETE ) || (actWrtSize != (zipfile_size%BPF_RU_FACC_WDATA_MAX)))
		{
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "BPF_RU_FACC_FWRITE NG(%d,%d,0x%x,0x%x)",ret,errcd,zipfile_size,actWrtSize);
			(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );
			/* Send API Load Update Response (result = NG)	*/
			f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"flash write NG");
			
			return;
		}
	}

	/* close the file */
	(VOID)BPF_RU_FACC_FCLOSE( fileDsc, &errcd );

	/**************************/
	/* Decompress ZYNQ.tar.gz */	
	/**************************/
	/* check the compressed type */
	/* if it is gzip, decompress */
	if(zip_type == D_SYS_GZIP)
	{
		if(f_fmngw_dlwType != D_FMNG_DL_USE)
		{
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "Uncompress start sts:0x%08x,size:0x%08x",f_fmngw_thrdState,zipfile_size);
			/* 種別を変更する */
			/* OTHERにするときはプライオリティは0にする	*/
			memset(&s_param, 0, sizeof(s_param));
			ret = sched_setscheduler(0, SCHED_OTHER, &s_param);
			sprintf(unzipCmd, "tar -zxf %s%s -C %s ",D_SYS_UNZIP_DIRNAME,D_SYS_TARGZ_FILENAME,D_SYS_UNZIP_DIRNAME);
			/* tar -zxvf ZYNQ.tar.gz -C \/tmp\/ */
			/* decompress ZYNQ.tar.gz to \/tmp\/ */
			retUnZip = system(unzipCmd);
			/* 種別を元に戻す */
			memset(&s_param, 0, sizeof(s_param));
			/* プライオリティ設定	*/
			s_param.sched_priority = D_RRH_PRIORITY_FMNG;
			ret |= sched_setscheduler(0, SCHED_FIFO, &s_param);
			sprintf(unzipCmd, "rm -fr %s%s",D_SYS_UNZIP_DIRNAME,D_SYS_TARGZ_FILENAME);
			/* decompress ZYNQ.tar.gz to \/tmp\/ */
			ret |= system(unzipCmd);
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "Uncompress function end ret:%d",ret);
			if(retUnZip != CMD_OK)
			{
				cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_INFO, "Uncompress NG ret:%d",retUnZip);
				/* Send API Load Update Response (result = NG)	*/
				f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"Uncompress NG");
				return;
			}
		}
		else
		{
			cmn_com_assert_th_get(f_fmngw_assert_p,  D_RRH_LOG_AST_LV_INFO, "Online Uncompress start face:%d,sts:0x%08x",a_bootface,f_fmngw_thrdState );
			/* create tar thread to decompress firm file temporarily */
			ret = BPF_RU_ITCM_THREAD_CREATE(
							(unsigned long int *)&threadId,							/* Thread id						*/
							BPF_RU_ITCM_SCHED_OTHER,								/* Scheduling policy				*/
							D_SYS_NUM0,												/* Thread priority					*/
							D_RRH_STACKSIZE_DEFAULT,								/* Thread stack size				*/
							(INT*)(FUNCPTR)f_fmng_tarThread,						/* IAスレッドメイン関数ポインタ		*/
							D_RRH_NULL,												/* オプション						*/
							&errcd);												/* Error code						*/

			/*	ERRORの場合	*/
			if(ret != BPF_RU_ITCM_OK)
			{
				cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_ERROR, "pf_tar CREATE NG(%d)",errcd);
				/* Send API Load Update Response (result = NG)	*/
				f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"Uncompress start NG");
				return;
			}
			/* save the writing request */
			f_fmngw_flashWaitWrtReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_REQ;
			f_fmngw_flashWaitWrtReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
			f_fmngw_flashWaitWrtReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQ;
			f_fmngw_flashWaitWrtReq.thdIf.uiLength = sizeof(f_fmngw_flashWaitWrtReq);
			f_fmngw_flashWaitWrtReq.writeFileNum = needDlNum;
			
			/* save the writing request for slave */
			f_fmngw_flashWaitWrtReq_slave.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_REQ;
			f_fmngw_flashWaitWrtReq_slave.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
			f_fmngw_flashWaitWrtReq_slave.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQS;
			f_fmngw_flashWaitWrtReq_slave.thdIf.uiLength = sizeof(f_fmngw_flashWaitWrtReq_slave);
			f_fmngw_flashWaitWrtReq_slave.writeFileNum = needDlNum_slave;

			/* set processing FW type */
			f_fmngw_fw_type = ptThrdif->data.fw_type;

			/* record current thread state */
			f_fmngw_preThrdState = f_fmngw_thrdState;
			
			/* Change thread state into Decompressing */
			f_fmngw_thrdState = D_FMNG_THRDSTA_TARPROCESS;
			
			cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_apiLoadUpdReq] RETURN to TARPROCESS" );				
			return;
		}

	}
	else
	{
		/* Send API Load Update Response (result = NG)	*/
		f_fmng_com_sndLoadUpdRsp(D_FMNG_REFW_NG,"compressed type is NG");
		return ;
	}
	
	/*******************************/
	/* if thread state is flash R/W or Cancel  */
	/* save the writing request                   */
	/*******************************/
	if((f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRW) || 
		(f_fmngw_thrdState == D_FMNG_THRDSTA_FLSHRWCANCEL))
	{
		f_fmngw_flashWaitWrtReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_REQ;
		f_fmngw_flashWaitWrtReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
		f_fmngw_flashWaitWrtReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQ;
		f_fmngw_flashWaitWrtReq.thdIf.uiLength = sizeof(f_fmngw_flashWaitWrtReq);
		f_fmngw_flashWaitWrtReq.writeFileNum = needDlNum;

		f_fmngw_flashWaitWrtReq_slave.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_REQ;
		f_fmngw_flashWaitWrtReq_slave.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
		f_fmngw_flashWaitWrtReq_slave.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQS;
		f_fmngw_flashWaitWrtReq_slave.thdIf.uiLength = sizeof(f_fmngw_flashWaitWrtReq_slave);
		f_fmngw_flashWaitWrtReq_slave.writeFileNum = needDlNum_slave;

		/* set processing FW type */
		f_fmngw_fw_type = ptThrdif->data.fw_type;

		/* Change thread state into flash R/W */
		f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRWWAIT;
		
		cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_apiLoadUpdReq] RETURN to FLSHRWWAIT" );					
		return;
	}

	/* set erasing flag of EEPROM to OFF */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FIRM0, D_SYS_OFF);		
	/* set erasing flag of shared memory to OFF */
	f_cmw_tra_inv_tbl->cmx_erase_firm_flg = D_SYS_OFF;

   	/* set erasing flag of EEPROM to OFF */
   	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_RFRX0, D_SYS_OFF);			
   	/* set erasing flag of shared memory to OFF */
   	f_cmw_tra_inv_tbl->cmx_erase_rx_flg = D_SYS_OFF;
	
	/* set erasing flag of EEPROM to OFF */
	f_fmng_cm_WriteEeprom(D_RRH_EEP_DL_ERASE_FLG_FPGA0, D_SYS_OFF);			
	/* set erasing flag of shared memory to OFF */
	f_cmw_tra_inv_tbl->cmx_erase_fpga_flg = D_SYS_OFF;
	
	/****************************/
	/* Send file info to pf_frw */	
	/****************************/
	flashWrtReq.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_REQ;
	flashWrtReq.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashWrtReq.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQ;
	flashWrtReq.thdIf.uiLength = sizeof(flashWrtReq);
	flashWrtReq.writeFileNum = needDlNum;
	
	f_fmngw_flashRsp[D_SYS_NUM0] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQ, 
					(CHAR*)&flashWrtReq,
					sizeof(flashWrtReq), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_TIMEOUT_NTC );
	
	/*******************************/
	/* Send file info to pf_frwsla */	
	/*******************************/
	flashWrtReq_slave.thdIf.uiEventNo = D_SYS_MSGID_FLASH_WRITE_REQ;
	flashWrtReq_slave.thdIf.uiSrcTQueueID = D_SYS_THDQID_PF_FRMG;
	flashWrtReq_slave.thdIf.uiDstTQueueID = D_SYS_THDQID_PF_FRQS;
	flashWrtReq_slave.thdIf.uiLength = sizeof(flashWrtReq_slave);
	flashWrtReq_slave.writeFileNum = needDlNum_slave;

	f_fmngw_flashRsp[D_SYS_NUM1] = D_SYS_OFF;
	f_com_msgQSendFHM( D_SYS_THDQID_PF_FRQS, 
					(CHAR*)&flashWrtReq_slave,
					sizeof(flashWrtReq_slave), 
					D_SYS_THDQID_PF_FRMG, 
					f_fmngw_thrdState, 
					D_SYS_MSGID_FLASH_TIMEOUT_NTC );
	
	/***************************/
	/* Start flash Write timer */
	/***************************/
	f_fmng_com_startTimer(
			E_RRH_TIMID_TDLWRTSV,		/* Timer ID 			*/
			D_FMNG_TIMEVAL_FLSWRT, 	/* Timer Value		*/
			D_SYS_MSGID_FLASH_TIMEOUT_NTC
			);		
	
	/* set received api kind */
	f_fmngw_apikind = D_FMNG_API_WRT;

	/* set processing FW type */
	f_fmngw_fw_type = ptThrdif->data.fw_type;

	/* Change thread state into flash R/W */
	f_fmngw_thrdState = D_FMNG_THRDSTA_FLSHRW;

	cmn_com_assert_th_get(f_fmngw_assert_p, D_RRH_LOG_AST_LV_RETURN, "[f_fmng_apiLoadUpdReq] RETURN to FLSHRW" );				

	return ;
}
/* @} */

