#include <sys/syscall.h>
#include <sys/types.h>
#include "bpf_i_com_log.h"
#include "bpf_i_rm_svrm.h" 

/*
 * SV情報（子側）変化ログ保存
 * 共有メモリへSV情報（子側）変化を格納する
 */
void BPF_COM_LOG_SVC_STORE(  unsigned int chgType, UINT *svinf ) {
	
}
/*
 * SV情報（子側）変化ログ提供
 * 全件転送
 */
void BPF_COM_LOG_SVC_LOAD( T_RRH_SVSTATE_RECORD *loadbuf ) {
	

}
