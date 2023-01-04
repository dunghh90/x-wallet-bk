/*!
 * @skip   $Id$
 * @file   cmd_shmgetmon.c
 * @brief  通信用共有メモリ管理領域表示
 * @date   2014/01/06 ALPHA)宮崎 Create
 *
 * ALL Rights Reserved, Copyright c FUJITSU Limited 2014
 */

/*!
 * @addtogroup RRH_DEBUG
 * @{
 */
#include        "f_dbg_cmd_inc.h"
#include        "bpf_l_ru_ipcm.h"

/**
 * 関数プロトタイプ宣言
 */
extern int  cnvHexStr2Long( unsigned char     *hexStr, unsigned int *outVal );
extern int  BPF_RU_IPCM_PROCSHM_ADDRGET(unsigned int shm_num, void **addr, int *errcd);

/*!
 * @name 通信用共有メモリ管理テーブル
 * @note 通信用共有メモリ管理テーブル構造体
 * @{
 */
#define BPF_RU_IPCM_SHMID_COMU_OCCUR             921            /* 送信用共有メモリ閾値超え(MAXの60%)   */
#define BPF_RU_IPCM_SHMID_COMU_CLEAR             911            /* 送信用共有メモリ閾値超えから復旧             */
#define BPF_RU_IPCM_SHMID_COMU_MAX               1536            /* 送信用共有メモリの1種別あたりのMAX数 */




/*!
 * @brief 関数機能概要:共有メモリの利用状況表示
 * @note  関数処理内容.
 *       -# 共有メモリ管理情報の使用中のものを表示する。
 * @param	argc					[in]int		コマンドアーギュメント数
 * @param	*argv				[in]char		コマンドアーギュメント
 * @param	*outfp				[in]FILE*		出力先ファイルポインタ
 * @return INT
 * @retval RTC_OK               0:正常終了
 * @retval D_SYS_NG             1:異常終了
 * @date 2014/01/06 ALPHA)宮崎 create
 */



int cmd_shm_getmon(int argc, char *argv[], FILE *outfp) {
	int errcd;
	BPF_MNG_COM_MEMORY* t_com_memory; /*      共有メモリ管理テーブル  */
	int bufidx;
	unsigned int count;

	BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_MNG_COM_MEMORY, (void**) &t_com_memory, &errcd);
	
	for( count = 0; count < E_BPF_RU_IPCM_SHMKIND_MAX; count++ )
	{
		fprintf(outfp, "[BUFF KIND = %d]\n", count);
		fprintf(outfp, "  use count:%d\n", t_com_memory[count].useCount);
		for (bufidx = 0; bufidx < BPF_RU_IPCM_SHMID_COMU_MAX; bufidx++) {
			if (t_com_memory[count].com_mem_inf[bufidx].state == 1) {
				fprintf(outfp, "    [%d] %08x,%d\n", bufidx, t_com_memory[count].com_mem_inf[bufidx].keyID, t_com_memory[count].com_mem_inf[bufidx].state);
			}
		}
	}
	return 0;
}


/* @} */
