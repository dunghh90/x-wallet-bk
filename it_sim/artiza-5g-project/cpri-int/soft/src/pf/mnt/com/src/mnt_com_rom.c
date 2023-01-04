/*!
 * @skip  $ld:$
 * @file  mnt_com_rom.c
 * @brief ROM table(com)
 * @date  2013/11/20 FFCS)zhaodx Create for zynq\n
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
 */
 
/*!
 * @addtogroup RRH_MNT
 * @{
 */
#include "m_cm_header.h" 
#include "mnt_com_inc.h"


#ifndef OPT_RRH_ZYNQ_REC
UINT	const	 mnt_comr_cprimsg_subTbl[MBD_CPRIMSGNUM]=                   /*!< */
{
/* 		MTアドレス設定要求,					MT管理タスク				*/
		CMD_CPRID_MTADDSETREQ,
/* 		FLD-MT接続応答,						MT管理タスク				*/
		CMD_CPRID_FLDMTCNCRES,
/* 		FLD-MT接続解放応答,					MT管理タスク				*/
		CMD_CPRID_FLMTCNCRELRES,
/* 		FLD-MT接続強制解放要求,				MT管理タスク				*/
		CMD_CPRID_FLMTCMPRELREQ,
/* 		FLD-MTデータコマンド転送送信要求,	MT管理タスク				*/
		CMD_CPRID_FLDCOMSNDREQ,
/* 		FLD-MTデータコマンド転送受信応答,	MT管理タスク				*/
		CMD_CPRID_FLDCOMRCVRES,
/* 		FLD-MTデータファイル転送送信要求,	MT管理タスク				*/
		CMD_CPRID_FLDFILSNDREQ,
/* 		FLD-MTデータファイル転送受信応答,	MT管理タスク				*/
		CMD_CPRID_FLDFILRCVRES,
/* 		SV-MT接続応答,						MT管理タスク				*/
		CMD_CPRID_SVMTCNTRES,
/* 		SV-MT接続解放応答,					MT管理タスク				*/
		CMD_CPRID_SVMTCNTRELRES,
/* 		SV-MT接続強制解放要求,				MT管理タスク				*/
		CMD_CPRID_SVMTFORRELREQ,
/* 		SV-MTデータ送信要求,				MT管理タスク				*/
		CMD_CPRID_SVMTDATSNDREQ,
/* 		SV-MTデータ受信応答,				MT管理タスク				*/
		CMD_CPRID_SVMTDATRCVRES,
/* 		メーカMT接続要求,					メーカMT管理タスク			*/
		CMD_CPRID_MKMTCNTREQ,
/* 		メーカMT接続応答,					メーカMT管理タスク			*/
		CMD_CPRID_MKMTCNTRES,
/* 		メーカMTデータ送信,					メーカMT管理タスク			*/
		CMD_CPRID_MKMTDATSND,
/* 		メーカMT接続解放要求,				メーカMT管理タスク			*/
		CMD_CPRID_MKMTCNTRELREQ,
/* 		メーカMT接続解放応答,				メーカMT管理タスク			*/
		CMD_CPRID_MKMTCNTRELRES,
/* 		メーカMT接続強制解放要求,			メーカMT管理タスク			*/
		CMD_CPRID_MKMTFORRELREQ,
/* 		メーカMT接続強制解放応答,			メーカMT管理タスク			*/
		CMD_CPRID_MKMTFORRELRES,
/* 		メーカMT接続強制解放応答,			メーカMT管理タスク			*/
		CMD_CPRID_FTPPORTSETREQ
};
#else
UINT	const	 mnt_comr_cprimsg_subTbl[MBD_CPRIMSGNUM]=                   /*!< */
{
	CMD_CPRID_HCRES,			/**< ヘルスチェック応答	*/
	CMD_CPRID_RETIMSETREQ,		/**< RE時刻設定要求	*/
	CMD_CPRID_LINKSTSRES,		/**< RE共通部リンク確立応答	*/
	CMD_CPRID_FIRMFILERES,		/**< ファームウェアファイル報告応答	*/
	CMD_CPRID_FILEINFRES,		/**< ファイル情報報告応答	*/
	CMD_CPRID_FILEDATSND,		/**< ファイルデータ送信	*/
	CMD_CPRID_FILSNDFINRES,		/**< ファイルデータ送信完了報告応答	*/
	CMD_CPRID_UFIRMFILERES,		/**< 運用中ファームウェアファイル報告応答	*/
	CMD_CPRID_UFILEINFRES,		/**< 運用中ファイル情報報告応答	*/
	CMD_CPRID_UFILEDATSND,		/**< 運用中ファイルデータ送信	*/
	CMD_CPRID_UFILSNDFINRES,	/**< 運用中ファイルデータ送信完了報告応答	*/
	CMD_CPRID_TRXSETRES,		/**< TRX設定応答	*/
	CMD_CPRID_TRXRELRES,		/**< TRX解放応答	*/
	CMD_CPRID_TOFFSETINFRES,	/**< Toffset値報告応答	*/
	CMD_CPRID_CARONOFFRES,		/**< 無変調キャリアON/OFF指定応答	*/
	CMD_CPRID_EQPSTRRES,		/**< RE装置構成情報報告応答	*/
	CMD_CPRID_SLOTINFRES,		/**< REスロット情報報告応答	*/
	CMD_CPRID_RESTSRES,			/**< RE状態報告応答	*/
	CMD_CPRID_CARDINFRES,		/**< REカード情報報告応答	*/
	CMD_CPRID_REVERRES,			/**< REバージョン報告応答	*/
	CMD_CPRID_RECARDCNTRES,		/**< REカード制御応答	*/
	CMD_CPRID_CARDSTSRES,		/**< REカード状態報告応答	*/
	CMD_CPRID_CARLSTSRES,		/**< REキャリア状態報告応答	*/
	CMD_CPRID_REPORTCNTRES,		/**< RE PORT制御応答	*/
	CMD_CPRID_REPORTSTSRES,		/**< RE PORT状態報告応答	*/
	CMD_CPRID_RECRDDIARES,		/**< REカード診断報告	*/
	CMD_CPRID_REOBSLOGRES,		/**< RE障害ログ取得応答	*/
	CMD_CPRID_REOBLOGSTPRES,	/**< RE障害ログ取得中止応答	*/
	CMD_CPRID_EXTDATSNDRES,		/**< 外部装置データ送信応答	*/
	CMD_CPRID_EXTDATRCVREQ,		/**< 外部装置データ受信要求	*/
	CMD_CPRID_MTADDSETRES,		/**< MTアドレス設定応答	*/
	CMD_CPRID_FLDMTCNCREQ,		/**< FLD-MT接続要求	*/
	CMD_CPRID_FLDCOMSNDRES,		/**< FLD-MTデータコマンド転送送信応答	*/
	CMD_CPRID_FLDCOMRCVREQ,		/**< FLD-MTデータコマンド転送受信要求	*/
	CMD_CPRID_FLDFILSNDRES,		/**< FLD-MTデータファイル転送送信応答	*/
	CMD_CPRID_FLDFILRCVREQ,		/**< FLD-MTデータファイル転送受信要求	*/
	CMD_CPRID_FLMTCNCRELREQ,	/**< FLD-MT接続解放要求	*/
	CMD_CPRID_FLMTCMPRELRES,	/**< FLD-MT接続強制解放応答	*/
	CMD_CPRID_SVMTCNTRES,		/**< SV-MT接続応答	*/
	CMD_CPRID_SVMTDATSNDRES,	/**< SV-MTデータ送信応答	*/
	CMD_CPRID_SVMTDATRCVREQ,	/**< SV-MTデータ受信要求	*/
	CMD_CPRID_SVMTCNTRELREQ,	/**< SV-MT接続解放要求	*/
	CMD_CPRID_SVMTFORRELRES,	/**< SV-MT接続強制解放応答	*/
	CMD_CPRID_FTPPORTSETRES		/**< FTP データ送信用PORT 番号設定応答	*/
};
#endif


/* @} */

