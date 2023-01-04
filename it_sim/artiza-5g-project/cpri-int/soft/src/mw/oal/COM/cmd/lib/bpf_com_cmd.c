/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   bpf_com_plog.c
 *  @brief  BPF Common packet log Library.
 *  @date   2018/09/06  FJT)Taniguchi create
 *
 *  ALL Rights Reserved, Copyright c FUJITSU Limited 2014
 */
/********************************************************************************************************************/
#define UNIT_ID BPF_COM_CMD /* Pre-define */
/********************************************************************************************************************/
/* include file                                                                                                     */
/********************************************************************************************************************/
#include "bpf_i_com_cmd.h"

typedef enum{
	E_BPF_CMD_CUR_CLK_CLASS=1,
	E_BPF_CMD_RESET_REASON,
	E_BPF_CMD_CUR_PORTNO,
	E_BPF_CMD_CUR_START_FACE,
	E_BPF_CMD_SET_VLANTAG,
	E_BPF_CMD_SET_REMOTEMAC,	
	E_BPF_CMD_SET_TXENDP,
	E_BPF_CMD_SET_RXENDP,
	E_BPF_CMD_SET_TXCC,
	E_BPF_CMD_SET_RXCC,
	E_BPF_CMD_SETUP,
	E_BPF_CMD_SET_TXGAIN,
	E_BPF_CMD_SET_PTPINVALID,
	E_BPF_CMD_MAX
}e_bpf_com_cmd_commit;

static	T_RRH_DU_CMD_INFO* bpf_com_cmd_shm_addr = NULL;

/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  mode              [in] cmd_commit
 *  @param  val               [in] commit val
 *  @return none
 *  @date   2018/09/21 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void bpf_com_cmd_commit_to_mem(e_bpf_com_cmd_commit mode , UINT val)
{
	int	endcd;
	int	errcd;
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 3;
	dtime.tv_nsec = 0;
	
	if(bpf_com_cmd_shm_addr == NULL){
		if( (endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DU_CMD_INFO,(VOID **)&bpf_com_cmd_shm_addr,&errcd)) != BPF_RU_IPCM_OK ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget error 0x%x 0x%x 0x%x",E_RRH_SHMID_APL_DU_CMD_INFO,endcd,errcd);
			return;
		}
	}
	endcd = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"semtake error 0x%x 0x%x 0x%x",E_RRH_SEMID_APL_DU_CMD_INFO,endcd,errcd);
		return;
	}
	switch(mode){
		case E_BPF_CMD_CUR_CLK_CLASS:
			bpf_com_cmd_shm_addr->ptp_info.ptp_clock_class = val;
			break;
		case E_BPF_CMD_RESET_REASON:
			bpf_com_cmd_shm_addr->com_info.reset_reason = val;
			break;
		case E_BPF_CMD_CUR_PORTNO:
			bpf_com_cmd_shm_addr->ptp_info.port_number = val;
			break;
		case E_BPF_CMD_CUR_START_FACE:
			bpf_com_cmd_shm_addr->com_info.startup_face = val;
			break;
		case E_BPF_CMD_SETUP:
			bpf_com_cmd_shm_addr->com_info.pid = val;
			break;
		case E_BPF_CMD_SET_TXGAIN:
			bpf_com_cmd_shm_addr->com_info.txgain = val;
			break;
		case E_BPF_CMD_SET_PTPINVALID:
			bpf_com_cmd_shm_addr->com_info.ptp_valid = val;
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"unknown mode 0x%x 0x%x",mode,val);
			break;
	}
	endcd =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"semgive error 0x%x 0x%x 0x%x",E_RRH_SEMID_APL_DU_CMD_INFO,endcd,errcd);
		return;
	}
	return;
}


/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  mode              [in] cmd_commit
 *  @param  val               [in] commit val
 *  @return none
 *  @date   2018/09/21 FJT)Taniguchi create
 */
/********************************************************************************************************************/
static void bpf_com_cmd_commit_to_mem_buffer(e_bpf_com_cmd_commit mode ,  UINT index1 , UINT index2 ,UINT size , VOID* buffer)
{
	int	endcd;
	int	errcd;
	struct bpf_timespec		dtime;	/* sem waittime */	
	dtime.tv_sec = 3;
	dtime.tv_nsec = 0;
	
	if(bpf_com_cmd_shm_addr == NULL){
		if( (endcd = BPF_RU_IPCM_PROCSHM_ADDRGET(E_RRH_SHMID_APL_DU_CMD_INFO,(VOID **)&bpf_com_cmd_shm_addr,&errcd)) != BPF_RU_IPCM_OK ){
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"shmget error 0x%x 0x%x 0x%x",E_RRH_SHMID_APL_DU_CMD_INFO,endcd,errcd);
			return;
		}
	}
	endcd = BPF_RU_IPCM_PROCSEM_TAKE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, BPF_RU_IPCM_WAIT, &dtime, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"semtake error 0x%x 0x%x 0x%x",E_RRH_SEMID_APL_DU_CMD_INFO,endcd,errcd);
		return;
	}
	switch(mode){
		case E_BPF_CMD_SET_VLANTAG:
			bpf_com_cmd_shm_addr->fh_info[index1].vid_dl = ((T_RRH_DU_CMD_INFO_FH*)buffer)->vid_dl;
			bpf_com_cmd_shm_addr->fh_info[index1].vid_ul = ((T_RRH_DU_CMD_INFO_FH*)buffer)->vid_ul;
			bpf_com_cmd_shm_addr->fh_info[index1].pcp_ul = ((T_RRH_DU_CMD_INFO_FH*)buffer)->pcp_ul;
			break;
		case E_BPF_CMD_SET_REMOTEMAC:
			bpf_com_cmd_shm_addr->fh_info[index1].remote_mac_addr_h = ((T_RRH_DU_CMD_INFO_FH*)buffer)->remote_mac_addr_h;
			bpf_com_cmd_shm_addr->fh_info[index1].remote_mac_addr_l = ((T_RRH_DU_CMD_INFO_FH*)buffer)->remote_mac_addr_l;
			break;
		case E_BPF_CMD_SET_TXENDP:
			memcpy(&(bpf_com_cmd_shm_addr->txcc_info[index1].endpoint[index2]),buffer,size);
			break;
		case E_BPF_CMD_SET_RXENDP:
			memcpy(&(bpf_com_cmd_shm_addr->rxcc_info[index1].endpoint[index2]),buffer,size);
			break;
		case E_BPF_CMD_SET_TXCC:
			memcpy(&(bpf_com_cmd_shm_addr->txcc_info[index1].array[index2]),buffer,size);
			break;
		case E_BPF_CMD_SET_RXCC:
			memcpy(&(bpf_com_cmd_shm_addr->rxcc_info[index1].array[index2]),buffer,size);
			break;
		default:
			bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"unknown mode 0x%x 0x%x",mode,size);
			break;
	}
	endcd =  BPF_RU_IPCM_PROCSEM_GIVE(E_RRH_SEMID_APL_DU_CMD_INFO,BPF_RU_IPCM_LOCK_RW, &errcd);
	if(BPF_RU_IPCM_OK != endcd){
		bpf_com_dbglog_fmt(BPF_COM_LOG_ERR_L3,"semgive error 0x%x 0x%x 0x%x",E_RRH_SEMID_APL_DU_CMD_INFO,endcd,errcd);
		return;
	}
	return;
}


/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  current_clock_class      [in] current clock class
 *  @return none
 *  @date   2018/09/06 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_CUR_CLK_CLASS(UCHAR current_clock_class)
{
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_CUR_CLK_CLASS,(UINT)current_clock_class);
	return;
}
/* @} */

/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  reset_reason      [in] last reset reason
 *  @return none
 *  @date   2018/09/21 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_RESET_REASON(UINT reset_reason)
{
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_RESET_REASON,reset_reason);
	return;
}
/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  port_number  [in] current port number
 *  @return none
 *  @date   2018/09/25 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_CUR_PORTNO(UINT port_number)
{
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_CUR_PORTNO,port_number);
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  face   [in] current face
 *  @return none
 *  @date   2018/09/25 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_CUR_START_FACE(UINT startup_face)
{
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_CUR_START_FACE,startup_face);
	return;
}
/* @} */


/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  port                [in] portno
 *  @param  vid_dl              [in] vid_dl
 *  @param  vid_ul              [in] vid_ul
 *  @param  pcp_ul              [in] pcp_ul
 *  @return none
 *  @date   2018/09/25 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_VLANTAG(UINT port ,UINT vid_dl , UINT vid_ul , UINT pcp_ul )
{
	T_RRH_DU_CMD_INFO_FH buffer;
	
	memset(&buffer,0,sizeof(buffer));
/*	buffer.link_onoff = D_RRH_ON;	現状使用していないため */
	buffer.vid_dl = vid_dl;
	buffer.vid_ul = vid_ul;
	buffer.pcp_ul = pcp_ul;
	bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_VLANTAG,port & 1,0,sizeof(buffer),(void*)&buffer);
	return;
}
/* @} */



/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  port                [in] portno
 *  @param  remote_mac_addr_h   [in] remote_mac_addr_h
 *  @param  remote_mac_addr_l   [in] remote_mac_addr_l
 *  @return none
 *  @date   2018/09/25 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_REMOTEMAC(UINT port ,UINT remote_mac_addr_h , UINT remote_mac_addr_l )
{
	T_RRH_DU_CMD_INFO_FH buffer;
	
	memset(&buffer,0,sizeof(buffer));
/*	buffer.link_onoff = D_RRH_ON;	現状使用していないため */
	buffer.remote_mac_addr_h = remote_mac_addr_h;
	buffer.remote_mac_addr_l = remote_mac_addr_l;
	bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_REMOTEMAC,port & 1,0,sizeof(buffer),(void*)&buffer);
	return;
}
/* @} */


/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @date   2018/12/17 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_TXENDP(UINT carno ,UINT antno , UINT onoff, UINT Acell_id, UINT CC_id,UINT Port_id, UINT Acell_id_ex, UINT CC_id_ex,UINT Port_id_ex)
{
	T_RRH_DU_CMD_INFO_ENDP buffer;

	memset(&buffer,0,sizeof(buffer));
	buffer.onoff = onoff;
	buffer.Acell_id = Acell_id;
	buffer.CC_id = CC_id;
	buffer.Port_id = Port_id;
	buffer.Acell_id_ex = Acell_id_ex;
	buffer.CC_id_ex = CC_id_ex;
	buffer.Port_id_ex = Port_id_ex;
	bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_TXENDP,carno,antno,sizeof(buffer),(void*)&buffer);
	return;
}
/* @} */


/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @date   2018/12/17 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_RXENDP(UINT carno ,UINT antno , UINT onoff, UINT Acell_id, UINT CC_id,UINT Port_id, UINT Acell_id_ex, UINT CC_id_ex,UINT Port_id_ex)
{
	T_RRH_DU_CMD_INFO_ENDP buffer;

	memset(&buffer,0,sizeof(buffer));
	buffer.onoff = onoff;
	buffer.Acell_id = Acell_id;
	buffer.CC_id = CC_id;
	buffer.Port_id = Port_id;
	buffer.Acell_id_ex = Acell_id_ex;
	buffer.CC_id_ex = CC_id_ex;
	buffer.Port_id_ex = Port_id_ex;
	bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_RXENDP,carno,antno,sizeof(buffer),(void*)&buffer);
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @date   2018/12/17 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_CLR_ENDP(UINT	val)
{
	T_RRH_DU_CMD_INFO_ENDP buffer;
	UINT	carno;
	UINT	antno;

	memset(&buffer,0,sizeof(buffer));
	buffer.Acell_id_ex = val;
	for(carno=0;carno < 8 ;carno++){
		for(antno=0;antno < 8 ;antno++){
			bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_TXENDP,carno,antno,sizeof(buffer),(void*)&buffer);
			bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_RXENDP,carno,antno,sizeof(buffer),(void*)&buffer);
		}
	}
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @date   2018/12/17 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_TXCC(UINT carno ,UINT antno ,UINT nr_freq, UINT nr_bandwidth, UINT nr_scs )
{
	T_RRH_DU_CMD_INFO_ARRAYCAR buffer;

	memset(&buffer,0,sizeof(buffer));
	buffer.onoff = D_RRH_ON;
	buffer.nr_freq = nr_freq;
	buffer.nr_bandwidth = nr_bandwidth;
	buffer.nr_scs = nr_scs;

	bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_TXCC,carno,antno,sizeof(buffer),(void*)&buffer);
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @date   2018/12/17 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_RXCC(UINT carno ,UINT antno ,UINT nr_freq, UINT nr_bandwidth, UINT nr_scs,UINT ul_fft_smpl_ofs,UINT ta_offset )
{
	T_RRH_DU_CMD_INFO_ARRAYCAR buffer;

	memset(&buffer,0,sizeof(buffer));
	buffer.onoff = D_RRH_ON;
	buffer.nr_freq = nr_freq;
	buffer.nr_bandwidth = nr_bandwidth;
	buffer.nr_scs = nr_scs;
	buffer.ul_fft_smpl_ofs = ul_fft_smpl_ofs;
	buffer.ta_offset = ta_offset;

	bpf_com_cmd_commit_to_mem_buffer(E_BPF_CMD_SET_RXCC,carno,antno,sizeof(buffer),(void*)&buffer);
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @date   2019/04/09 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SETUP( VOID )
{
	int	selfpid = getpid();
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_SETUP,selfpid);
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  txgain   [in] txgain
 *  @return none
 *  @date   2019/05/14 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_TXGAIN(UINT txgain)
{
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_SET_TXGAIN,txgain);
	return;
}
/* @} */

/* @} */
/********************************************************************************************************************/
/**
 *  @brief  
 *  @note   Common function command library.
 *  @param  N/A
 *  @return none
 *  @date   2020/02/27 FJT)Taniguchi create
 */
/********************************************************************************************************************/
VOID BPF_COM_CMD_SET_PTPINVALID(VOID)
{
	bpf_com_cmd_commit_to_mem(E_BPF_CMD_SET_PTPINVALID,D_RRH_OFF);
	return;
}
/* @} */

/* @} */
