/*!
 * @skip  $ld:$
 * @file  f_ini_typ.h
 * @brief 初期化タスク 構造体宣言
 * @date  2013/11/25 ALPHA) 中村 Create
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2013
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
	

#endif
/* @} */
