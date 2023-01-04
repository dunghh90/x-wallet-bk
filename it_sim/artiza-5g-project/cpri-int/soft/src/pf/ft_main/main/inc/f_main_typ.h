/*!
 * @skip  $ld:$
 * @file  f_ini_typ.h
 * @brief 初期化タスク 構造体宣言
 * @date  2013/11/25 ALPHA) 中村 Create
 * @date  2015/10/28 ALPHA) 宮崎 update ハソ1.13対応
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_PF_MAIN
 * @{
 */
#ifndef F_MAIN_TYP_H
#define F_MAIN_TYP_H

/**
 * @brief	ThreadID変換情報テーブル
 * @note	ThreadID変換情報テーブルの定義
 * 
 */
typedef struct {
	UINT		threadID;						/* Thread ID							*/
	UINT		threadQID;						/* Thread QID							*/
	CHAR		threadName[32]; 				/* Thread Name							*/
	UINT		thdCreMngIdx; 					/* f_mainr_thdCreMng用インデックス		*/
} T_MAIN_THEAD_CHG_TBL;

/**
 * @brief	レジスタアクセス用テーブル
 * @note	レジスタアクセス用テーブルの定義
 * 
 */
typedef struct{
	UINT	addr;
	UINT	reg_data;
}T_MAIN_REG_TABLE;
	
/* ハソ1.13対応 ここから */
/**
 * @brief	SFP Type情報
 * @note	SFP Type情報の定義
 * 
 */
typedef struct{
	CHAR	vendorSpecName[13];					/* SFP の 図番							*/
	CHAR	cardNameChar[1];					/* カード名に付与する文字列				*/
	UCHAR	dummy[2];							/* SFP の 図番							*/
	UINT	trxInfType;							/* TRXINFタイプ							*/
}T_MAIN_SFP_TYPE_TBL;
/* ハソ1.13対応 ここまで */
#endif
/* @} */
