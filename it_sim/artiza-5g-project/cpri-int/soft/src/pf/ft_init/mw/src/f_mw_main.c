/*!
 * @skip  $ld:$
 * @file  main.c
 * @brief 初期化プロセスメイン処理
 * @date  2013/03/16 ALPHA) kondou Create RRH-007-000 初期化
 * @date  2013/11/07 ALPHA) nakamura modify for ZYNQ
 * @data  2014/12/12 ALPHA) okabe Create RRH-007-000 共通ppc対応
 * @date  2015/04/16 ALPHA) tomioka Create RRH-013-000 TDD対応
 * @date  2015/09/08 ALPHA) kamada add＆mod RRH-006-000 対応
 *
 * All Rights Reserved, Copyright (C) FUJITSU LIMITED 2015
 */

/*!
 * @addtogroup RRH_INI
 * @{
 */

#include "f_mw_inc.h"
extern const	T_RRH_SYS_CREATE_SHMMEM				f_comr_shmmemCreMng[E_RRH_SHMID_APL_MAX];

/*!
 * @brief		main
 * @note		関数処理内容.
 *				-# ミドルウェア初期化
 * @param  		argc		[in]	受信パラメータ数
 * @param  		*argv[]		[in]	受信パラメータ
 * @retval 		D_INI_OK		0:正常終了
 * @retval 		D_INI_NG		1:異常終了
 * @return		処理結果
 * @warning		N/A
 * @FeatureID	PF-Init-001-001-001
 * @Bug_No		N/A
 * @CR_No		N/A
 * @TBD_No		N/A
 * @date 2018/04/19 FJT)Taniguchi
 */
INT main(INT argc, CHAR *argv[])
{
	int rtn_bpf;

	if(argc >= 2){
		if((!strncmp(argv[1],"-v",2)) || (!strncmp(argv[1],"-V",2))){
			printf("%s was born on %s %s\n",argv[0],__DATE__,__TIME__);
		}
		return 0;
	}
	BPF_COM_LOG_DMESG("DU Middleware Initialize...\n");
	if((rtn_bpf = BPF_RM_SVRM_INIT(D_RRH_PROCID_MW)) != BPF_RM_SVRM_COMPLETE){
		BPF_COM_LOG_DMESG("SVRM Initialize NG(%d)\n",rtn_bpf);
	}else{
		rtn_bpf = BPF_RM_SVRM_SHM_CREATE(	D_RRH_PROCID_MW,
										E_RRH_SHMID_APL_MAX,
										(VOID *)&f_comr_shmmemCreMng );
		if(BPF_RM_SVRM_COMPLETE != rtn_bpf){
			BPF_COM_LOG_DMESG("Shared Memory Create NG(%d)\n",rtn_bpf);
		}else{
			rtn_bpf = BPF_RM_SVRM_ASSIGN(D_RRH_PROCID_MW);
			if(BPF_RM_SVRM_COMPLETE != rtn_bpf){
				BPF_COM_LOG_DMESG("SVRM assign NG(%d)\n",rtn_bpf);
			}
			
			T_RRH_LOG_LEVEL_MNG*	tLogLevelMng;
			INT errcd = 0;
			
			/************************************************************/
			/* Log Levelを取得											*/
			/************************************************************/
			rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_LOGLEVEL_MNG,
						(VOID **)&tLogLevelMng,
						&errcd );
			if( rtn_bpf == BPF_RU_IPCM_OK ){
				/* ログレベルを設定	*/
				/* Assert -> WARNING/ERROR/CRITICALのみ取得するようにする	*/
				tLogLevelMng->assertLogLevel = D_RRH_LOG_AST_DEFAULT_NOINFO;
				/* Register -> Writeのみ取得するようにする			*/
				tLogLevelMng->registerLogLevel = (D_RRH_LOG_REG_LV_WRITE | D_RRH_LOG_REG_LV_READ_WRITE);
			}
			
			T_RRH_DU_PROCESS_INFO*	tDuProcInfo;
			UINT					count;
			
			rtn_bpf = BPF_RU_IPCM_PROCSHM_ADDRGET(
						E_RRH_SHMID_APL_DU_PROCESS_INFO,
						(VOID **)&tDuProcInfo,
						&errcd );
			if( rtn_bpf == BPF_RU_IPCM_OK ){
				for(count=0;count < D_RRH_PROCID_MAX ; count++){
					tDuProcInfo->pState[count] = D_RRH_ON;
				}
			}
		}
	}
	return(rtn_bpf);
}
/* @} */
