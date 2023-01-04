/*!
 * @skip  $ld:$
 * @file  BPF_COM_LOG_SFP.c (base:f_com_RegLog.c)
 * @brief Register log function.
 * @date  2015/10/16 alfa)Miyazaki Create
 * ALL Rights Reserved, Copyright (C) FUJITSU Limited 2015-
 */

/*!
 * @addtogroup RRH_COM
 * @{
 */

#include <sys/syscall.h>
#include <sys/types.h>
#include "bpf_i_com_log.h"
#include "bpf_i_rm_svrm.h"

/*!
 *  @brief  SFP情報共有メモリ格納
 *  @note   This function is processed as follows.\n
 *          - store sfp log
 *  @param  ps             [in]   Primary(0)/Secondary(1)
 *  @param  sfpinfo        [in]   R/W data
 *  @return int 
 *  @retval int            BPF_HM_DEVC_NG:Fail/BPF_HM_DEVC_COMPLATE:Success
 *  @date  2015/10/16 Alpha)Miyazaki Create
 */

/* データ追加時比較対象 */
/*   以下の項目が前回登録時と一致する場合は登録しない	*/
#define	D_LOG_SFP_INFO_VENDOR_NAME_OFFSET		20	/*	ベンダー名		*/
#define	D_LOG_SFP_INFO_VENDOR_NAME_LENGTH		16	
#define	D_LOG_SFP_INFO_VENDOR_PART_NO_OFFSET	40	/* ベンダー部品番号	*/
#define	D_LOG_SFP_INFO_VENDOR_PART_NO_LENGTH	16
#define	D_LOG_SFP_INFO_VENDOR_REV_OFFSET		56	/* ベンダーrev		*/
#define	D_LOG_SFP_INFO_VENDOR_REV_LENGTH		4
	
/* local memory to shared */

/* ps is primary = 1/slave=0 */

int BPF_COM_LOG_SFP_INFO_STORE( int ps, T_RRH_SFPINFO *param_sfpinfo )	{

	T_RRH_SFPINFO_HIS		*sh_sfpinfo_adr			;	/* 共有メモリアドレス		*/
	int						errcd			=	0	;
	int						endcd			=	0	;


	/* 共有メモリ取得：管理情報履歴  */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
				( ps == 0 ) ? 	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS:	E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,
				(VOID **)&sh_sfpinfo_adr, &errcd);
	if( endcd != BPF_RU_IPCM_OK )	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,	"[BPF_COM_LOG_SFP_DATA_STORE]: Attach fault(Shared Memory) %d", errcd);
		return BPF_HM_DEVC_NG;
	}

	/****************************************************************/
	/* 格納判定(一番最新のデータと比較し同じ情報であれば格納しない）*/
	/*   比較対象は以下の３項目　　　　　　　　　　　　　　　　　　 */
	/*   	・VENDOR_NAME											*/
	/*		・VENDOR_PART_NO										*/
	/*		・VENDOR_REV											*/
	/****************************************************************/
	if( ( memcmp( &sh_sfpinfo_adr->sfpinfrec[0].sfpinfo.sfpinf[D_LOG_SFP_INFO_VENDOR_NAME_OFFSET],
				&param_sfpinfo->sfpinf[D_LOG_SFP_INFO_VENDOR_NAME_OFFSET],
				D_LOG_SFP_INFO_VENDOR_NAME_LENGTH                   ) == 0 ) &&
		( memcmp( &sh_sfpinfo_adr->sfpinfrec[0].sfpinfo.sfpinf[D_LOG_SFP_INFO_VENDOR_PART_NO_OFFSET],
				&param_sfpinfo->sfpinf[D_LOG_SFP_INFO_VENDOR_PART_NO_OFFSET], 
				D_LOG_SFP_INFO_VENDOR_PART_NO_LENGTH				) == 0 ) &&
		( memcmp( &sh_sfpinfo_adr->sfpinfrec[0].sfpinfo.sfpinf[D_LOG_SFP_INFO_VENDOR_REV_OFFSET], 
				&param_sfpinfo->sfpinf[D_LOG_SFP_INFO_VENDOR_REV_OFFSET], 
				D_LOG_SFP_INFO_VENDOR_REV_LENGTH					) == 0 )
		){	/* 同じデータ？ */
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_INFO, "[BPF_COM_LOG_SFP_DATA_STORE]: same info. not store. ");
		return BPF_HM_DEVC_NG;
	}
		
	/****************************************************/
	/*ログ格納方針
	 *　配列先頭に最新のものを置く。そのため、新規データを配列[0]へ
	 *　格納すると共に、indexに従った場所へも格納する
	 ****************************************************/
	/* タイムスタンプを設定 */
	BPF_HM_DEVC_GETTIME( &sh_sfpinfo_adr->sfpinfrec[0].sysTime );

	/* 今回登録するデータを共有メモリの先頭へ格納 */
	memcpy( (char *)&sh_sfpinfo_adr->sfpinfrec[0].sfpinfo, param_sfpinfo, sizeof(T_RRH_SFPINFO));
	
	
	if( sh_sfpinfo_adr->logIndex  >= D_RRH_SFPLOG_MAX - 1 )	{	/* SFPインデックスが異常もしくは回った */
	    /* 0で初期化 (+1した先へ書き込まれるため実質は[1]からとなる */
		sh_sfpinfo_adr->logIndex  = 0 ;	
	}
	/* 最新データをindexの箇所へ格納 格納先は1足した場所へ書く*/
	memcpy( (char *)&sh_sfpinfo_adr->sfpinfrec[sh_sfpinfo_adr->logIndex + 1], &sh_sfpinfo_adr->sfpinfrec[0], sizeof(T_RRH_SFPINFO_RECORD));

	/* インデックス更新 */
	sh_sfpinfo_adr->logIndex++;
	
	return BPF_HM_DEVC_COMPLETE;

}


/*!
 *  @brief  SFP情報取得
 *  @note   This function is processed as follows.\n
 *          - load sfp log
 *  @param  ps             [in]   Primary(0)/Secondary(1)
 *  @param  sfpinfo        [in]   R/W data
 *  @return none 
 *  @retval none
 *  @date  2015/10/16 Alpha)Miyazaki Create
 */
int BPF_COM_LOG_SFP_INFO_LOAD( int ps, T_RRH_SFPINFO_HIS *sfpinfo ) {

	T_RRH_SFPINFO_HIS		*sh_sfpinfo_adr			;	/* 共有メモリアドレス		*/
	int						errcd			=	0	;
	int						endcd			=	0	;
	
	/* 共有メモリ取得：管理情報履歴  */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
				( ps == 0 ) ? 	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS:	E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,
				(VOID **)&sh_sfpinfo_adr, &errcd);
	if( endcd != BPF_RU_IPCM_OK )	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,	"[BPF_COM_LOG_SFP_DATA_STORE]: Attach fault(Shared Memory) %d", errcd);
		return BPF_HM_DEVC_NG;
	}
	/* ＳＦＰ情報をコピー */
	memcpy( sfpinfo, sh_sfpinfo_adr, sizeof(T_RRH_SFPINFO_HIS));

	return BPF_HM_DEVC_COMPLETE;
}
/*!
 *  @brief  SFP情報共有メモリクリア
 *  @note   This function is processed as follows.\n
 *          - clear sfp  log
 *  @param  ps             [in]   Primary(0)/Secondary(1)
 *  @return none 
 *  @retval none
 *  @date  2015/10/16 Alpha)Miyazaki Create
 */
void BPF_COM_LOG_SFP_INFO_CLR(int ps) {

	T_RRH_SFPINFO_HIS		*sh_sfpinfo_adr			;	/* 共有メモリアドレス		*/
	int						errcd			=	0	;
	int						endcd			=	0	;
	
	/* 共有メモリ取得：管理情報履歴  */
	endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(
				( ps == 0 ) ? 	E_RRH_SHMID_APL_SFP0_MNG_INFO_HIS:
								E_RRH_SHMID_APL_SFP1_MNG_INFO_HIS,
				(VOID **)&sh_sfpinfo_adr, &errcd);
	if( endcd != BPF_RU_IPCM_OK )	{
		BPF_COM_LOG_ASSERT(D_RRH_LOG_AST_LV_ERROR,	"[BPF_COM_LOG_SFP_DATA_STORE]: Attach fault(Shared Memory) %d", errcd);
		return ;
	}
	/* ＳＦＰ情報をクリア */
	memset( sh_sfpinfo_adr, 0, sizeof(T_RRH_SFPINFO_HIS));

	return;
}

/* @} */

