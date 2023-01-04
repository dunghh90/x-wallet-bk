/*!
 * @skip   $Id$
 * @file   m_lg_send_logdata.c
 * @brief  the processing for trouble log data sending.
 * @date   2008/07/29 FFCS)Wuh Create for eNB-009-004.
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2008
 */

/*!
 * @addtogroup TRA_LOG
 * @{ 
 */

#include "m_cm_header.h"
#include "m_lg_header.h"

/*!
 *  @brief  the processing function for trouble log data sending.
 *  @note   This function is processed as follows.
 *          1) send RE trouble log data to REC 
 *  @param  a_almlog_adr [in]  the address of alarm log
 *  @param  a_sendsize [in]  the size of the data to be sent
 *  @param  a_rec_typ  [in]  the type of RE system 3G or S3G
 *  @return VOID
 *  @retval -
 *  @date   2008/07/29 FFCS)Wuh Create.
 *  @date   2013/11/22 ALPHA)yokoyama modify for ZYNQ
 *  @date   2015/08/02 FPT)Tung modify for FHM.
 *  @date   2015/11/10 FPT)Yen IT2問処168の対応.
 */

VOID m_lg_send_logdata( UINT a_almlog_adr, UINT a_sendsize, USHORT a_rec_typ )
{
                                            
    CMT_TSKIF_CPRISND_FILEDATSND *a_rsp_p = NULL ;    /* タスク間フレームポインタ   */
    UINT                         a_bufget_rslt  = CMD_RES_OK ;  /* バッファ獲得結果 */
    UINT                         a_ressnd_rslt  = CMD_RES_OK ;  /* 応答送信結果     */
    UINT                         bufget_size;                   /*  バッファ取得サイズ  */
    USHORT                       a_signal_kind  = CMD_CPRID_FILEDATSND ;  /* file data send kind  */
    FILE                         *fp;
    size_t                       fw_cnt;
	USHORT						 lwReadSize = a_sendsize;
    
    cm_Assert(D_RRH_LOG_AST_LV_ENTER, 0, "[m_lg_send_logdata] enter") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s start\n", __LINE__, __FUNCTION__);
#endif
    
    /* バッファ取得サイズ算出(バッファサイズを可変長に対応) */
    bufget_size = sizeof(CMT_TSKIF_HEAD)+sizeof(CMT_CPRISIGSND_INF)
                +sizeof(CMT_CPRIF_FILEDATSND)-CMD_FILEDAT_MAX+a_sendsize;

    /*======================================================================*/
    /*                                                                        */
    /*    ■ファイルデータ送信形式                                            */
    /*                                                                        */
    /*        ファイルデータ送信    (   2Byte)    0x2300                            */
    /*        分割番号上位        (   2Byte)    分割番号(1-n)                    */
    /*        分割番号下位        (   2Byte)    分割番号(1-n)                    */
    /*        データサイズ        (   2Byte)    分割データサイズ                */
    /*        ファイルデータ        ( 10KByte)    分割データ                        */
    /*                                                                        */
    /*======================================================================*/
    
    /* 送信用バッファ獲得    */
    a_bufget_rslt = cm_L2BfGet( CMD_BUFCA_LAPSND, 
                                bufget_size,
                                CMD_NUM1,
                                (VOID **)&a_rsp_p ) ;
    
    /* バッファ獲得失敗 */
    if(a_bufget_rslt != CMD_RES_OK)
    {
        /* アサートログ書込み                                    */
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] m_lg_send_logdata");
        cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_logdata] return") ;
        return ;
    }
    
    /* タスク間ヘッダ部編集    */
    /* 送信元に返すのでテーブルに保存していたQueueに返す    */
    a_rsp_p->head.uiDstPQueueID    = lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_SEND].dstPQID;
    a_rsp_p->head.uiDstTQueueID    = lgw_rspmng_tbl[a_rec_typ][LGD_L3_FILEDATA_SEND].dstTQID;
    a_rsp_p->head.uiSrcPQueueID    = D_RRH_PROCQUE_L3;
    a_rsp_p->head.uiSrcTQueueID    = D_RRH_THDQID_L3_LOG;
    
    /* CPRI通信用データ編集    */
    /* 信号種別設定            */
    /*----------------the CPRI signal response for S3G-------------------*/
    a_signal_kind |= CMD_SYS_S3G ;
    a_rsp_p->cpridat_filedatsnd.signal_kind = a_signal_kind;    /*-----for S3G----*/

    /*-------------------------------------------------------------------------*/
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s Message: <DstPQueueID>0x%x <DstTQueueID>0x%x <SignalKind>0x%x\n", __LINE__, __FUNCTION__, a_rsp_p->head.uiDstPQueueID, a_rsp_p->head.uiDstTQueueID, a_signal_kind);
#endif
    
    /* (バウンダリ考慮されたヘッダの為、sgmtno1は未使用) */
    a_rsp_p->cpridat_filedatsnd.sgmtno1     = CMD_NUM0 ;
    
    /* 分割番号設定            */
    a_rsp_p->cpridat_filedatsnd.sgmtno2     = 
                        (USHORT)(lgw_almlogmng_tbl[a_rec_typ].snd_cmplt_num + CMD_NUM1) ;
    
    /* データサイズ設定        */
    /* memcpyで行うと逆になってしまうのでずらして設定する    */
    a_rsp_p->cpridat_filedatsnd.datsize1 = (USHORT)((a_sendsize & 0xFFFF0000) >> 16);
    a_rsp_p->cpridat_filedatsnd.datsize2 = (USHORT)( a_sendsize & 0x0000FFFF);
    
    /* ファイルデータ設定                                            */
    /* StartAddressが実サイズより小さい場合のみ処理を行う            */
    /* 実サイズより大きい場合は何もしない(Data部は0保障されている)    */
    if( a_almlog_adr < lgw_almlogmng_tbl[a_rec_typ].realsize )
    {
        /* 出力されたファイルを開く    */
        fp = fopen( lgw_almlogmng_tbl[a_rec_typ].logfile_name_path, "rb+" );  /* pgr0520 */
        if ( fp == NULL )
        {
            cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] m_lg_send_logdata");
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s fopen Null\n", __LINE__, __FUNCTION__);
#endif
        }
        else
        {
            /* StartAddressまで移動する    */
            fseek( fp, a_almlog_adr, SEEK_SET ); /* pgr0520 fopenでファイルポインタを取得しているため問題無し */
        	if( lwReadSize > (lgw_almlogmng_tbl[a_rec_typ].realsize - a_almlog_adr))
        	{
        		lwReadSize = lgw_almlogmng_tbl[a_rec_typ].realsize - a_almlog_adr;
        	}
            /* 10kbyteを読み出す        */
            fw_cnt = fread( a_rsp_p->cpridat_filedatsnd.fildat, lwReadSize, 1, fp );
            if( fw_cnt < 1 )
            {
                cm_Assert( D_RRH_LOG_AST_LV_WARNING, a_rec_typ, "fread error." );
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s fread error (file path:%s, lwReadSize:%d) \n", __LINE__, __FUNCTION__,lgw_almlogmng_tbl[a_rec_typ].logfile_name_path,lwReadSize);
#endif
            }
        }
        /* ファイルを閉じる            */
        fclose( fp );  /* pgr0520 */
    }
    
    /* Message送信する                                    */
    /* 送信先はCMUなのでヘッダに設定した送信先と異なることに注意    */
    a_ressnd_rslt = m_cm_L3MsgSend_REC( a_rsp_p ) ;
    
    if( a_ressnd_rslt != 0 )
    {
        cm_Assert(D_RRH_LOG_AST_LV_ERROR, CMD_NUM0, "[ALMLOG] m_lg_send_logdata");
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s m_cm_L3MsgSend_REC error\n", __LINE__, __FUNCTION__);
#endif
    }
    
    cm_Assert(D_RRH_LOG_AST_LV_RETURN, 0, "[m_lg_send_logdata] return") ;
#ifdef FHM_TROUBLELOG_DEBUG_FOR_IT1
    printf( "[%d]%s end\n", __LINE__, __FUNCTION__);
#endif
    
    return ;
}

/* @} */

