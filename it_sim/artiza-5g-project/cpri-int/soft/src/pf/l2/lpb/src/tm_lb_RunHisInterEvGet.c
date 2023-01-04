/*!
 * @skip    $Id$
 * @file    tm_lb_RunHisInterEvGet.c
 * @brief   Get internal event Nunber(LAPB走行履歴)
 * @date    2008.07.22 FFCS)Shihzh create.
 * 
 * ALL Rights Reserved, Copyright (c) FUJITSU Limited 2008
*/


/********************************************************************************************************/

/* @addtogroup RRH_L2_LPB
 * @{
 */
#include "m_cm_header.h"
#include "tm_lb_header.h"





/*!
 *  @brief   Get internal event Nunber(LAPB走行履歴)
 *  @note    Get internal event Nunber for LAPB run history
 *  @param   per_adr		[in]  length
 *  @param   own_adr		[in]  own_adr
 *  @param   n1				[in]  n1
 *  @param   length			[in]  length
 *  @param   ctl			[in]  ctl
 *  @param   addr			[in]  addr
 *  @return  inter_event	
 *  @date    2008.07.22 FFCS)Shihzh create.
 */
UCHAR	tm_lb_RunHisInterEvGet(UINT length, UCHAR addr, UCHAR ctl,
											UCHAR own_adr,UCHAR per_adr,UINT n1)
{
												/*---------------------------*/
												/* 本モジュール専用 ローカル 変数*/
												/*---------------------------*/
	union CTL_FLD1	ctl_work;                               /*!< Control Field work   */
	UCHAR			cmd_rsp_flg;                            /*!< Length Work      */
	UCHAR			rxctl_sv;                               /*!< */
	UCHAR			inter_event;                            /*!< */

	inter_event  = 0xFF;
	ctl_work.ctl1_all =  ctl;
													/*-----------------------*/
													/*  Total Length 1以下 ? */
													/*-----------------------*/
	if(length <= CMD_NUM1)
	{
													/*-----------------------*/
													/* YES (INVALID Frame)   */
													/*   (Short Frame)       */
													/*-----------------------*/
		inter_event = LBD_OTHERS;
													/*-----------------------*/
													/* lblap_chkctl処理終了  */
													/*-----------------------*/
		return(inter_event);
	}
	
	if(addr == own_adr)
	{
													/*-----------------------*/
													/* YES                   */
													/*      Response Frame   */
													/*-----------------------*/
		cmd_rsp_flg = CMD_NUM0;
	}
	else
	{
		if(addr == per_adr)
		{
													/*-----------------------*/
 													/* アドレス = 相手局アドレス ? */
													/*-----------------------*/
													/*-----------------------*/
													/* YES                   */
													/*      Command  Frame   */
													/*-----------------------*/
			cmd_rsp_flg = CMD_NUM1;
		}
		else
		{
													/*-----------------------*/
													/* NO (INVALID Frame)    */
													/*   無効アドレス        */
													/*-----------------------*/
			inter_event = LBD_OTHERS;
			return(inter_event);
		}
	}
 
	if(ctl_work.ctli_8.i8zero == CMD_NUM0) /* pgr0872 */
	{
		rxctl_sv = LBD_L2_RXI;
	}
	else
	{
		if(ctl_work.ctls_8.s82s != CMD_NUM1) /* pgr0872 */
		{
			rxctl_sv = LBD_L2_RXS;
		}
		else
		{
			rxctl_sv = LBD_L2_RXU;
		}
	}

	switch(rxctl_sv)
	{
											/*-------------------------------*/
											/*         I  Frame        */
											/*-------------------------------*/

		case LBD_L2_RXI:
													/*-----------------------*/
													/* YES (i8zero 0)        */
													/*コマンド･レスポンス フラグ 1 ?  */
													/*-----------------------*/
			if(cmd_rsp_flg == CMD_NUM1)
			{
													/*-----------------------*/
													/* YES                   */
													/* (Command Frame)       */
													/*   Frame 種別設定      */
													/*-----------------------*/
				inter_event = LBD_ICTL;
														/*-------------------*/
														/*  Length Decriment */
														/* (アドレス制御部削*/
														/*-------------------*/
				length -= CMD_NUM2;

													/*-----------------------*/
													/* 最大I Frame 長越え ?  */
													/*-----------------------*/
				if((length + CMD_NUM4) > n1)
				{
													/*-----------------------*/
													/* YES (INVALID Frame)   */
													/*   (Long Frame)        */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_ERROR;
				}
			}
			else
			{
													/*-----------------------*/
													/*   Frame 種別設定      */
													/*-----------------------*/
				inter_event = LBD_ERROR;
			}
			break;
											/*-------------------------------*/
											/*         S  Frame        */
											/*-------------------------------*/

		case LBD_L2_RXS:
													/*-----------------------*/
													/* Total Length 2 以外 ? */
													/*-----------------------*/
			if(length != CMD_NUM2)
			{
													/*-----------------------*/
													/* YES (ERROR Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
				inter_event = LBD_ERROR;
				break;

			}
												/*---------------------------*/
												/*   S  Frame 種別判別処理   */
												/* 1:RR Frame                */
												/* 2:RNR Frame               */
												/* 3:REJ Frame               */
												/* OTHERS                    */
												/*---------------------------*/
			switch(ctl_work.ctls_8.s83s) /* pgr0872 */
			{
				case  0:							/*-----------------------*/
													/*     RR Frame (B'00)   */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_RRCTL;
					break;

				case  1:							/*-----------------------*/
													/*    RNR Frame (B'01)   */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_RNRCTL;
					break;

				case  2:							/*-----------------------*/
													/*    REJ Frame (B'10)   */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_REJCTL;
					break;

				default:							/*-----------------------*/
													/*         OTHERS        */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_ERROR;
					break;
			}
			break;
											/*-------------------------------*/
											/*         U  Frame        */
											/*-------------------------------*/

		default:
													/*-----------------------*/
													/*       Work 設定       */
													/*-----------------------*/
			ctl_work.ctlus_8.us8pf  = CMD_NUM0;
	
												/*---------------------------*/
												/*    U Frame 種別判別処理   */
												/* 1:SABM Frame              */
												/* 2:SABME Frame             */
												/* 3:DISC Frame              */
												/* 4:DM Frame                */
												/* 5:UA Frame                */
												/* 6:FRMR Frame              */
												/* 7:XID Frame               */
												/* OTHERS                    */
												/*---------------------------*/
			switch(ctl_work.ctl1_all)
			{
				case  0x2F :						/*-----------------------*/
													/*        SABM Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_SBMCTL;
													/*-----------------------*/
													/* コマンド･レスポンス フラグ 1 ? */
													/*-----------------------*/
					if(cmd_rsp_flg == CMD_NUM1)
					{
													/*-----------------------*/
													/* YES (Command Frame)   */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							inter_event = LBD_ERROR;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Response Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						inter_event = LBD_ERROR;
					}
					break;

				case 0x43 :							/*-----------------------*/
													/*       DISC  Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_DISCCTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 1 ? */
													/*-----------------------*/
					if(cmd_rsp_flg == CMD_NUM1)
					{
													/*-----------------------*/
													/* YES (Command Frame)   */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							inter_event = LBD_ERROR;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Response Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						inter_event = LBD_ERROR;
					}
					break;


				case 0x0F :							/*-----------------------*/
													/*        DM   Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_DMCTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 0 ? */
													/*-----------------------*/
					if(cmd_rsp_flg == CMD_NUM0)
					{
													/*-----------------------*/
													/* YES (Response Frame)  */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							inter_event = LBD_ERROR;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Command  Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						inter_event = LBD_ERROR;
					}
					break;

				case 0x63 :							/*-----------------------*/
													/*        UA   Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_UACTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 0 ? */
													/*-----------------------*/
					if(cmd_rsp_flg == CMD_NUM0)
					{
													/*-----------------------*/
													/* YES (Response Frame)  */
													/* Total Length 2 以外 ? */
													/*-----------------------*/
						if(length != CMD_NUM2)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							inter_event = LBD_ERROR;
						}
					}
					else
					{
													/*-----------------------*/
													/* N0  (Command  Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						inter_event = LBD_ERROR;
					}
					break;

				case 0x87 :							/*-----------------------*/
													/*       FRMR  Frame     */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_FRMRCTL;

													/*-----------------------*/
													/* コマンド･レスポンス フラグ 0 ? */
													/*-----------------------*/
					if(cmd_rsp_flg == CMD_NUM0)
					{
													/*-----------------------*/
													/* NO  (モジュロ  8)     */
													/* Total Length 5 以外 ? */
													/*-----------------------*/
						if(length != CMD_NUM5)
						{
													/*-----------------------*/
													/* YES (Error Frame)     */
													/*   Frame 種別設定      */
													/*-----------------------*/
							inter_event = LBD_ERROR;
						}
					}
					else
					{
													/*-----------------------*/
													/* NO  (Command  Frame)  */
													/*   Frame 種別設定      */
													/*-----------------------*/
						inter_event = LBD_ERROR;
					}
					break;

				default:							/*-----------------------*/
													/*         OTHERS        */
													/*   Frame 種別設定      */
													/*-----------------------*/
					inter_event = LBD_ERROR;
					break;
			}
			break;
	}
	
	return(inter_event);

}

/* @} */


