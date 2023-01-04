/****************************************************************************/
/*!
 * @skip   $ld:$
 * @file   f_trp_rec_Fcom_t_tbldump_dsp.c
 * @brief  ログ表示
 * @date   2010/07/23 FJT)Tokunaga create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2010
 */
/****************************************************************************/
#include "f_trp_rec.h"

/*! @addtogroup TRIF_REC
 * @{ */

/****************************************************************************/
/*!
 * @brief  ログ表示
 * @note   ログを表示する
 * @param  stream [in]  ストリーム
 * @return None
 * @date   2010/07/23 FJT)Tokunaga create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_tbldump_dsp(                                 /* なし:R */
    FILE* stream                                            /* ストリーム:I */
) {
    CHAR	tim_req[CMD_NUM32],tim_rsp[CMD_NUM32];
    USHORT	i,j,k;

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

	fprintf( stream, "## f_trp_rec_Wcom_ReqRspLog_tbl ##\n");
	for(i=0;i<D_REC_LOGKIND_MAX;i++){
		for(j=0;j<CMD_CPRINO_MAX;j++){

			if(i == D_REC_LOGKIND_CARRIER){
				fprintf( stream, " === Carrie Status Information ===\n");
			}
			else if(i == D_REC_LOGKIND_CARD){
				fprintf( stream, " === Card Status Information ===\n");
			}
			else{
				fprintf( stream, " === Unkonwn Log ===\n");
				continue;
			}

			fprintf( stream, "  [CPRI Link %d]\n",j);
			fprintf( stream, "   Request Send...           Response Receive...\n");
			fprintf( stream, "   %010d                %010d\n",
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Count,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Count);

			sprintf(tim_req, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.year,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.month,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.day,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.hour,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.min,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.sec,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.msec1,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Req_Time.msec2
			);
			sprintf(tim_rsp, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.year,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.month,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.day,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.hour,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.min,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.sec,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.msec1,
				f_trp_rec_Wcom_ReqRspLog_tbl[i][j].Rsp_Time.msec2
			);
			fprintf( stream, "   [%s]  [%s]\n",tim_req,tim_rsp);

		}
	}

	fprintf( stream, "## f_trp_rec_Wcom_ReqRspLogSave_tbl ##\n");
	for(i=0;i<D_REC_LOGKIND_MAX;i++){
		for(j=0;j<CMD_CPRINO_MAX;j++){

			if(i == D_REC_LOGKIND_CARRIER){
				fprintf( stream, " === Carrie Status Information ===\n");
			}
			else if(i == D_REC_LOGKIND_CARD){
				fprintf( stream, " === Card Status Information ===\n");
			}
			else{
				fprintf( stream, " === Unkonwn Log ===\n");
				continue;
			}

			for(k=0;k<D_REC_SAVECOUNT_MAX;k++){

				fprintf( stream, "  [CPRI Link %d]\n",j);

				if(f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveKind[k] == D_REC_SAVEKIND_TIMEOUT){
					fprintf( stream, "   Time Out Happen...\n");
				}
				else if(f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveKind[k] == D_REC_SAVEKIND_TIMESTOPNG){
					fprintf( stream, "   Timer ID not exist...\n");
				}
				else{
					fprintf( stream, "   Unknown reason\n");
					continue;
				}

				fprintf( stream, "   Request Send...           Response Receive...\n");
				fprintf( stream, "   %010d                %010d\n",
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Count,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Count);

				sprintf(tim_req, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.year,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.month,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.day,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.hour,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.min,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.sec,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.msec1,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Req_Time.msec2
				);
				sprintf(tim_rsp, "%02x/%02x/%02x %02x:%02x:%02x.%02x%02x",
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.year,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.month,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.day,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.hour,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.min,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.sec,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.msec1,
					f_trp_rec_Wcom_ReqRspLogSave_tbl[i][j].SaveData[k].Rsp_Time.msec2
				);
				fprintf( stream, "   [%s]  [%s]\n",tim_req,tim_rsp);

			}

		}
	}

	return;

}

/****************************************************************************/
/*!
 * @brief  ログ表示
 * @note   ログを表示する
 * @param  stream [in]  ストリーム
 * @return None
 * @date   2016/08/24 FJT)Koshida create.
 */
/****************************************************************************/
VOID f_trp_rec_Fcom_t_tbldump_dsp2(                                 /* なし:R */
    FILE* stream )                                            /* ストリーム:I */
{
	UINT i;

    /* ストリームがNULLの場合 */
    if (stream == NULL) {
        /* 標準出力に変更 */
        stream = stdout;
    }

	fprintf( stream, "## f_trp_com_timerMngTbl ##\n");
	fprintf( stream, "total Use=%03d, Max=%03d\n",
				f_trp_com_timerMngTbl.useNum, f_trp_com_timerMngTbl.useNumMax);
	for(i=0; i<256; i++)
	{
		fprintf( stream, "[%03d] Use=%03d, Max=%03d\n",
				i, f_trp_com_timerMngTbl.info[i].num, f_trp_com_timerMngTbl.info[i].numMax);
	}
	return;

}

/* @} */  /* group TRIF_REC */

/****************************************************************************/
/*
 */
/****************************************************************************/
