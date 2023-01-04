/****************************************************************************************************/
/*!
 * @skip  $ld:$
 * @file  f_main_userIpQspiSet.c
 * @brief ユーザーIP QSPI初期化設定
 * @date  2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */
/****************************************************************************************************/

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */

#include "f_main_inc.h"

/****************************************************************************************************/
/*!
 * @brief 関数機能概要:ユーザーIP QSPI初期化設定
 * @note  関数処理内容.
 *      -# ユーザーIP QSPI初期化設定を行う
 * @return UINT
 * @retval D_SYS_OK		0:正常終了
 * @retval D_SYS_NG		1:異常終了
 * @date   2015/06/08 ALPHA) tomioka Create RRH-007-000 TDD-Zynq対応
 * @date   2015/08/03 TDIPS) sasaki Update I/F変更対応(RE DLファイル格納用FLASH対応)
 * @date   2015/08/06 TDIPS) sasaki Update レビュー指摘事項No.15～17対応
 */
/****************************************************************************************************/
UINT f_main_userIpQspiSet(UINT qspi_offset_base, USHORT ssn)
{
	UINT	waitCnt = 0;
	UINT	regw_data = 0;
	UINT	regr_data = 0;
	UINT	ss;
	UINT	offset_reset = (qspi_offset_base|0x00000040);
	UINT	offset_cnt = (qspi_offset_base|0x00000060);
	UINT	offset_stat = (qspi_offset_base|0x00000064);
	UINT	offset_tx = (qspi_offset_base|0x00000068);
	UINT	offset_rx = (qspi_offset_base|0x0000006C);
	UINT	offset_cs = (qspi_offset_base|0x00000070);
	/* FLASH EOL対応 add start */
	UINT	*manufactureId_w;
	UINT	*manufactureId;
	UINT	fsCnt;
	UCHAR	name[2][4] = {"PL", "RE"};
	UINT	kind;
	
	/*	PL FLASH用のbaseアドレスの場合	*/
	if((qspi_offset_base == 0x40020000) && (ssn <= 1))
	{
		manufactureId = &f_comw_plManufactureId;
		kind = 0;
	}
	/*	RE FLASH用のbaseアドレスの場合	*/
	else if((qspi_offset_base == 0x40030000) && (ssn <= 4))
	{
		manufactureId = &f_comw_reManufactureId[0];
		kind = 1;
	}
	else
	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: base offset/ssnum Failed.", qspi_offset_base, ssn );
		return D_SYS_NG;
	}
	/* FLASH EOL対応 add end */

	/*------------------------------------------------------------------------------*/
	/* A. FIFOリセット	                                                            */
	/*------------------------------------------------------------------------------*/
	/* 1 SPIソフトリセット						*/
	regw_data = 0x0000000A;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_reset, &regw_data );

	/* 2 Wait 1us以上							*/
	f_com_taskDelay(1,0,0,0);

	/* 3 SPIシステムイネーブルをディセーブル	*/
	regw_data = 0x00000184;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );
	
	/* 4 CS選択なし								*/
	regw_data = 0xFFFFFFFF;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

	/* 5 FIFOリセット							*/
	regw_data = 0x000001E4;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );

	/* 6 SPIシステムイネーブルをイネーブル		*/
	regw_data = 0x00000186;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );

	/* FLASH EOL対応 add start */
	/*------------------------------------------------------------------------------*/
	/* A-2. Flash ID読出(READID[Manufacture ID READ])                               */
	/*------------------------------------------------------------------------------*/
	/* PL側およびRE側のManufacture IDを読出し */
	for( ss = 0; ss < ssn; ss++ )
	{
		/* 1 SPIシステムイネーブルをイネーブル						*/
		/* 既にイネーブルであるため、ここではイネーブル設定しない	*/
		
		/* 2 READID コマンド						*/
		regw_data = 0x0000009F;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data );

		/* 3 CS選択										*/
		regw_data = 0xFFFFFFFF ^ (0x1 << ss);
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

		/* 4 Master transaction inhibit=Enable			*/
		regw_data = 0x00000086;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );

		/* 5 Rx_Empty = "0"であることを確認(ポーリング 10ms間隔で100回)			*/
		for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
		{
			BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
			if( ( regr_data & 0x0001) == 0 ) {
				break;
			}
			
			/* 10ms Wait*/
			f_com_taskDelay(10, 0, 0, 0);
		}
		
		if( D_MAIN_QSPI_PORING_CNT == waitCnt )
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty5 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
			return D_SYS_NG;
		}

		/* 6 手順1に対するダミーリード							*/
		BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

		/* 7 FLASHリード(1byte)に対するダミーWrite		*/
		/*   (手順9に対となるダミーWrite)				*/
		regw_data = 0x00000000;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data );

		/* 8 Rx_Empty = "0"であることを確認(ポーリング 10ms間隔で100回)			*/
		for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
		{
			BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
			if( ( regr_data & 0x0001) == 0 ) {
				break;
			}
			
			/* 10ms Wait*/
			f_com_taskDelay(10, 0, 0, 0);
		}
		
		if( D_MAIN_QSPI_PORING_CNT == waitCnt )
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty8 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
			return D_SYS_NG;
		}

		/* 9 FLASHからのリード値取得(1byte)[Manufacture ID]				*/
		BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

		manufactureId_w = manufactureId + ss;
		if( ( regr_data & 0x000000FF) == QSPI_MACRONIX_ID ) {
			*manufactureId_w = QSPI_MACRONIX_ID;  /* 0xC2 */
		}
		else
		{
			*manufactureId_w = QSPI_MICRON_ID;    /* 0x20 */
		}

		/* 10 CS選択解除									*/
		regw_data = 0xFFFFFFFF;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

		/* 11 Master transaction inhibit=Desable			*/
		regw_data = 0x00000186;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );
		
		/* 12 SPIシステムイネーブルをディセーブル					*/
		/* 次のコマンドがあるので、ここではディセーブル設定しない	*/
		
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s#%d Manufacturer ID set(0x%x = 0x%x).", name[kind], ss, manufactureId_w, *manufactureId_w );

/* 2020/12/28 M&C) Merge 4G FHM src (start modify) */
		/* FLASH-ID設定(Manufacturer IDを元にFlash種別を決定) */
		// (VOID)BPF_HM_DEVC_PL_FLASH_ID_SET(kind, ss, *manufactureId_w);
		(VOID)BPF_HM_DEVC_PL_FLASH_ID_SET(*manufactureId_w);
/* 2020/12/28 M&C) Merge 4G FHM src (end modify) */
	}
	/*------------------------------------------------------------------------------*/
	/* A-3. Flash ID読出(READID[Manufacture ID READ])                               */
	/*------------------------------------------------------------------------------*/
	/* PS側のManufacture IDを読出し */
	if(f_comw_psManufactureId == 0)
	{
		BPF_HM_DEVC_QSPI_FLASH_READ_FLASH_KIND( &f_comw_psManufactureId );
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "PS Manufacturer ID get(0x%x = 0x%x).", &f_comw_psManufactureId, f_comw_psManufactureId );
	}
	/* FLASH EOL対応 add end */
	
	/*------------------------------------------------------------------------------*/
	/* B. Dummy cycle設定                                                           */
	/*------------------------------------------------------------------------------*/
	for( ss = 0; ss < ssn; ss++ )
	{
		/* 1 WRITE ENABLE コマンド						*/
		regw_data = 0x00000006;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data );

		/* 2 CS選択										*/
		regw_data = 0xFFFFFFFF ^ (0x1 << ss);
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

		/* 3 Master transaction inhibit=Enable			*/
		regw_data = 0x00000086;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );

		/* 4 Rx_Empty = "0"であることを確認(ポーリング 10ms間隔で100回)			*/
		for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
		{
			BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
			if( ( regr_data & 0x0001) == 0 ) {
				break;
			}
			
			/* 10ms Wait*/
			f_com_taskDelay(10, 0, 0, 0);
		}
		
		if( D_MAIN_QSPI_PORING_CNT == waitCnt )
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty4 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
			return D_SYS_NG;
		}

		/* 5 手順1に対するダミーリード							*/
		BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

		/* 6 CS選択解除									*/
		regw_data = 0xFFFFFFFF;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

		/* 7 Master transaction inhibit=Desable			*/
		regw_data = 0x00000186;
		BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );
		
		/* FLASH EOL対応 add start */
		manufactureId_w = manufactureId + ss;
		/*	FLASH Manufacture IDがMacronixの場合(FLASH EOL対応)	*/
		if(*manufactureId_w == QSPI_MACRONIX_ID)
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s#%d Dummy cycle(MACRONIX)[0x%x=0x%x].", name[kind], ss, manufactureId_w, *manufactureId_w );
			
			/* 8 0x400X0068 に 0x0001 Write	write status/configuration register コマンド*/
			regw_data = 0x00000001;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data);

			/* 9 0x400X0068 に 0x0040 Write	Status Register設定値*/
			regw_data = 0x00000040;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data);

			/* 10 0x400X0068 に 0x0047 Configuration Register設定値 [7:6]=Dummy cycle設定(01=6clock cycles)*/
			regw_data = 0x00000047;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data);

			/* 11 CS選択 0x40020070 に 0xFFFE or 0xFFFD or 0xFFFB or 0xFFF7 Write										*/
			regw_data = 0xFFFFFFFF ^ (0x1 << ss);
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data);

			/* 12 0x400X0060 に 0x0086 Write	Master transaction inhibit=Enable		*/
			regw_data = 0x00000086;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data);

			/* 13 0x400X0064 を Read	Rx_Empty = "0"であることを確認(ポーリング)		*/
			for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
			{
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
				if( ( regr_data & 0x0001) == 0 ) {
					break;
				}
				/* 10ms Wait*/
				f_com_taskDelay(10, 0, 0, 0);
			}
			
			if( D_MAIN_QSPI_PORING_CNT == waitCnt )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty13 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
				return D_SYS_NG;
			}
			
			/* 14 0x400X006C を Read	手順8に対するダミーリード						*/
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

			/* 15 0x400X0064 を Read	Rx_Empty = "0"であることを確認(ポーリング)		*/
			for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
			{
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
				if( ( regr_data & 0x0001) == 0 ) {
					break;
				}
				/* 10ms Wait*/
				f_com_taskDelay(10, 0, 0, 0);
			}
			
			if( D_MAIN_QSPI_PORING_CNT == waitCnt )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty15 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
				return D_SYS_NG;
			}
			
			/* 16 0x400X006C を Read	手順9に対するダミーリード						*/
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

			/* 17 0x400X0064 を Read	Rx_Empty = "0"であることを確認(ポーリング)		*/
			for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
			{
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
				if( ( regr_data & 0x0001) == 0 ) {
					break;
				}
				/* 10ms Wait*/
				f_com_taskDelay(10, 0, 0, 0);
			}
			
			if( D_MAIN_QSPI_PORING_CNT == waitCnt )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty17 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
				return D_SYS_NG;
			}
			
			/* 18 0x400X006C を Read	手順9に対するダミーリード						*/
			BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

			/* 19 0x400X0070 に 0xFFFF Write	CS選択解除								*/
			regw_data = 0xFFFFFFFF;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data);

			/* 20 0x400X0060 に 0x0186 Write	Master transaction inhibit=Desable		*/
			regw_data = 0x00000186;
			BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data);

			/* 31 Flag Status Register値確認 LOOP 										*/
			for( fsCnt=0; fsCnt<D_MAIN_QSPI_FLAGSTATUS_CNT; fsCnt++ )
			{
				/* 21 0x400X0068 に 0x0005 Write	Status register read コマンド		*/
				regw_data = 0x00000005;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data);

				/* 22 0x400X0068 に 0x0000 Write										*/
				regw_data = 0x00000000;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data);

				/* 23 0x40020070 に 0xFFFE Write	CS選択 X=0xE(SS0) or 0xD(SS1) or 0xB(SS2) or 0x7(SS3)	*/
				regw_data = 0xFFFFFFFF ^ (0x1 << ss);
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data);

				/* 24 0x400X0060 に 0x0086 Write	Master transaction inhibit=Enable	*/
				regw_data = 0x00000086;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data);

				/* 25 0x40020064 を Read	Rx_Empty = "0"であることを確認(ポーリング)	*/
				for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
				{
					BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
					if( ( regr_data & 0x0001) == 0 ) {
						break;
					}
					/* 10ms Wait*/
					f_com_taskDelay(10, 0, 0, 0);
				}
				
				if( D_MAIN_QSPI_PORING_CNT == waitCnt )
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty25 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
					return D_SYS_NG;
				}
				
				/* 26 0x400X006C を Read	手順21に対するリード						*/
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

				/* 27 0x400X0064 を Read	Rx_Empty = "0"であることを確認(ポーリング)	*/
				for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
				{
					BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
					if( ( regr_data & 0x0001) == 0 ) {
						break;
					}
					/* 10ms Wait*/
					f_com_taskDelay(10, 0, 0, 0);
				}
				
				if( D_MAIN_QSPI_PORING_CNT == waitCnt )
				{
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty27 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
					return D_SYS_NG;
				}
				
				/* 28 0x400X006C を Read	手順22に対するリード						*/
				BPF_HM_DEVC_REG_READ(D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

				/* 29 0x400X0070 に 0xFFFF Write	CS選択解除							*/
				regw_data = 0xFFFFFFFF;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data);

				/* 30 0x400X0060 に 0x0186 Write	Master transaction inhibit=Desable	*/
				regw_data = 0x00000186;
				BPF_HM_DEVC_REG_WRITE(D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data);

				/* 31 Flag Status Register値確認 手順28のリード値						*/
				/* Status Register(Bit0:write in progress) =							*/
				/* "0"(not in progress)であることを確認 								*/
				/* "1"(in progress)の場合は、手順21から繰り返す 						*/
				if((regr_data & 0x0001) == 0) {
					BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s#%d Dummy cycle end: FlagStatusReg not inprgress(cnt=%d, reg=0x%x).", name[kind], ss, waitCnt, regr_data );
					break;
				}
				/* 10ms Wait */
				f_com_taskDelay(10, 0, 0, 0);

			}
			if( D_MAIN_QSPI_FLAGSTATUS_CNT == fsCnt )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: FlagStatusReg inprgress(cnt=%d).", qspi_offset_base, ss, waitCnt );
				return D_SYS_NG;
			}
		}
		/*	FLASH Manufacture IDがMicronの場合(今まで通り)	*/
		else
		{
			BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "%s#%d Dummy cycle(MICRON)[0x%x=0x%x].", name[kind], ss, manufactureId_w, *manufactureId_w );
		/* FLASH EOL対応 add end */
			
			/* 8 WRITE VOLATILE CONFIGURATION REGISTER コマンド*/
			regw_data = 0x00000081;
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data );

			/* 9 Dummy cycle数を6回に設定					*/
			regw_data = 0x0000006B;
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_tx, &regw_data );

			/* 10 CS選択									*/
			regw_data = 0xFFFFFFFF ^ (0x1 << ss);
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

			/* 11 Master transaction inhibit=Enable		*/
			regw_data = 0x00000086;
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );

			/* 12 Rx_Empty = "0"であることを確認(ポーリング 10ms間隔で100回)		*/
			for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
			{
				BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
				if( ( regr_data & 0x0001) == 0 ) {
					break;
				}
				/* 10ms Wait*/
				f_com_taskDelay(10, 0, 0, 0);
			}
			
			if( D_MAIN_QSPI_PORING_CNT == waitCnt )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty12 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
				return D_SYS_NG;
			}
			
			/* 13 手順8に対するダミーリード						*/
			BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

			/* 14 Rx_Empty = "0"であることを確認(ポーリング 10ms間隔で100回)		*/
			for( waitCnt=0; waitCnt<D_MAIN_QSPI_PORING_CNT; waitCnt++ )
			{
				BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_stat, &regr_data );
				if( ( regr_data & 0x0001) == 0 ) {
					break;
				}
				/* 10ms Wait*/
				f_com_taskDelay(10, 0, 0, 0);
			}
			
			if( D_MAIN_QSPI_PORING_CNT == waitCnt )
			{
				BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR, "[f_main_userIpQspiSet(0x%x,%d)]: Rx Empty14 Failed(cnt=%d).", qspi_offset_base, ss, waitCnt );
				return D_SYS_NG;
			}
			
			/* 15 手順9に対するダミーリード						*/
			BPF_HM_DEVC_REG_READ( D_RRH_LOG_REG_LV_READ, offset_rx, &regr_data );

			/* 16 CS選択解除								*/
			regw_data = 0xFFFFFFFF;
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cs, &regw_data );

			/* 17 Master transaction inhibit=Desable		*/
			regw_data = 0x00000186;
			BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );
		
	/* FLASH EOL対応 chg start */
		}
	}
	/* 32(Macronix) or 18(MIcron) SPIシステムイネーブルをディセーブル	*/
	regw_data = 0x00000184;
	BPF_HM_DEVC_REG_WRITE( D_RRH_LOG_REG_LV_WRITE, offset_cnt, &regw_data );
	/* FLASH EOL対応 chg end */

	return D_SYS_OK;
}
/* @} */
