#include "rrhlib_local.h"

E_RRHAPI_RCODE rrhApi_Rfp_Mqr_PaControl(INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT onoff)
{
	printf("PA control ,branch = %d, onoff = %d\n", branch, onoff);
	return E_API_RCD_OK;
}

E_RRHAPI_RCODE rrhApi_Rfp_Mqr_CarSet(INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT carrier, USHORT bandwidth, USHORT frequency, USHORT snd_br)
{
	printf("Carrior set ,branch = %d, bandwidth = %d, frequency =%d, snd_br =%d\n", branch, bandwidth, frequency, snd_br);
	return E_API_RCD_OK;
}

E_RRHAPI_RCODE rrhApi_Tilt_Mqr_SendData(	INT	qid, INT wtime,VOID *data_p,
								UINT system,UINT devno,UCHAR *data,UINT datasize)
{
	return E_API_RCD_OK;
}
E_RRHAPI_RCODE rrhApi_Tilt_Mqr_ReceiveData(	INT		qid, INT	wtime,void *data_p,
									UINT system,UINT	devno)
{
	return E_API_RCD_OK;
}

E_RRHAPI_RCODE rrhApi_Tilt_Mnt_DeleteData( INT		qid, INT		wtime,VOID *data_p,
								UINT system,UINT		devno )

{
	return E_API_RCD_OK;
}

E_RRHAPI_RCODE rrhApi_Tilt_Mpr_SubscribeState( INT		qid, INT		wtime,VOID *data_p)

{
	return E_API_RCD_OK;
}

E_RRHAPI_RCODE rrhApi_Tilt_Mnt_UnsubscribeState( INT		qid, INT		wtime,VOID *data_p)
{
	return E_API_RCD_OK;
}

 E_RRHAPI_RCODE rrhApi_Dbg_Mqr_StartCpriSimu(
						INT		qid, INT		wtime,VOID *data_p,
						UINT    linkno,
						UINT	mode) { 	return E_API_RCD_OK; }

 E_RRHAPI_RCODE rrhApi_Dbg_Mqr_StopCpriSimu(
						INT		qid, INT		wtime,VOID *data_p,
						UINT    linkno) { 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Dbg_Mnt_StartupCpriMonitor(
						INT		qid, INT		wtime,VOID *data_p,
						UINT	linkno) { 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Dbg_Mnt_StopCpriMonitor(
						INT		qid, INT		wtime,VOID *data_p,
						UINT    linkno) { 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Dbg_Mqr_IfConfig(
						INT		qid, INT		wtime,VOID *data_p,
						UINT	ipAddr,
						UINT	ipMask) { 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Dbg_Mnt_SendSimuCpriMsg(
								INT		qid, INT		wtime,VOID *data_p,
								VOID*	msg_p) { 	return E_API_RCD_OK; }

 E_RRHAPI_RCODE rrhApi_Diag_Mqr_DiaReq(INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }

 E_RRHAPI_RCODE rrhApi_L2_Mqr_SetupLink(
						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_L2_Mqr_ReleaseLink(
						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_L2_Mnt_DeactLapbLink(
						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_L2_Mpr_SubscribeCpriMsg(
						INT		qid, INT		wtime,VOID	*data_p,
						UINT*   msgs,UINT    nMsg ){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_L2_Mnt_UnSubscribeCpriMsg(
						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_L2_Mpr_SubscribeLinkState(
						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
/* 2020/12/28 M&C) Merge 4G FHM src (start modify - add param 'linkno') */
// E_RRHAPI_RCODE rrhApi_L2_Mnt_UnsubscribeLinkState(
//						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_L2_Mnt_UnsubscribeLinkState(	USHORT linkno,
						INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
/* 2020/12/28 M&C) Merge 4G FHM src (end modify - add param 'linkno') */
 E_RRHAPI_RCODE rrhApi_L2_Mnt_CpriMsgStatistic(
								INT		qid, INT		wtime,VOID	*data_p){ 	return E_API_RCD_OK; }

 E_RRHAPI_RCODE rrhApi_Cpri_Mnt_StartupLink(
						INT		qid, INT		wtime,VOID *data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Cpri_Mnt_EnableLink(
						INT		qid, 	INT		wtime,VOID *data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Cpri_Mnt_DisableLink(
						INT		qid,	INT		wtime,VOID *data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Cpri_Mpr_SubscribeLinkState(
						INT		qid,	INT		wtime,VOID *data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Cpri_Mnt_UnsubscribeLinkState(
						INT		qid, 	INT		wtime,VOID *data_p){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Cpri_Mnt_UpdateSystemPara(
						INT		qid, 	INT		wtime,VOID *data_p,
						UINT	value){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Ext_Mnt_PortControl(
								INT			qid,
								INT			wtime,
								VOID		*data_p,
								UINT		control,
								UINT		infbit ){ 	return E_API_RCD_OK; }

E_RRHAPI_RCODE rrhApi_Rfp_Mnt_UpdateSystemParam( INT qid, INT wtime, VOID *data_p, USHORT bandwidth ,USHORT sec_chip0, USHORT sec_chip1,USHORT sig_kind, USHORT conn_kind ,USHORT car_work_3G ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_CarRel( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT carrier ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_NoModCar( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT carrier, USHORT onoff, USHORT power ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_WaveControl( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT onoff ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_IQControl( INT qid, INT wtime, VOID *data_p, USHORT cpriPort, USHORT onoff ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_AllPaCarOff( INT qid, INT wtime, VOID *data_p ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_PowerAdjust( INT qid, INT wtime, VOID *data_p, USHORT branch, USHORT command ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mnt_PowerSave( INT qid, INT wtime, VOID *data_p ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Rfp_Mqr_GetCarFreq( INT qid, INT wtime, VOID *data_p, USHORT getKind, USHORT branch, USHORT carrier ){ 	return E_API_RCD_OK; }

 E_RRHAPI_RCODE rrhApi_Svp_Mnt_CardControl(
						INT		qid, 	INT		wtime,VOID *data_p,
						UINT	cntkind,
						UINT	cntcode){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Svp_Mnt_SetDateTime(
						INT		qid, 	INT		wtime,VOID *data_p,
						T_RRH_SYSTEM_TIME		datetime ){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Svp_Mnt_ResetCard(
						INT		qid, 	INT		wtime,VOID *data_p,
						UINT	rstkind,
						UINT	rstFact){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Svp_Mnt_ErrSet(
						INT		qid, 	INT		wtime,VOID *data_p,
						UINT	act_flag,		UINT	writeDat){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Svp_Mpr_SubscribeCardState(
						INT		qid, 	INT		wtime,VOID *data_p ){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Svp_Mnt_UnsubscribeCardState(
						INT		qid, 	INT		wtime,VOID *data_p ){ 	return E_API_RCD_OK; }
 E_RRHAPI_RCODE rrhApi_Svp_Mnt_LedSet(
								INT		qid, 	INT		wtime,VOID *data_p ,
								UINT	led1Sta,
								UINT	led2Sta){ 	return E_API_RCD_OK; }

E_RRHAPI_RCODE rrhApi_File_Mpr_SubGetFirmVersion(	INT		qid, INT wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_File_Mnr_EraseFirm(
								INT		qid, INT wtime,VOID	*data_p,
								UINT	side ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_File_Mnr_UpdateFirm(	INT	qid, INT wtime,VOID *data_p,
								UINT dlwType, UINT checksum, UINT time ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_File_Mnt_CancelUpdFirm(	INT		qid, INT wtime,VOID	*data_p){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_File_Fsm_loadMemory(INT *handle){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_File_Fsm_SaveData(INT fd,VOID *data, UINT size){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_File_Fsm_unLoadMemory(INT fd, INT size, USHORT recvCheckSum){ 	return E_API_RCD_OK; }

E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetDevKind(
							VOID	*data_p,INT size )
{
	UINT	kind;
	UINT	data1;
	UINT	data2;

	BPF_HM_DEVC_EEPROM_READ(0x70,&data1);
	BPF_HM_DEVC_EEPROM_READ(0x71,&data2);
	/*	RRE/SRE�ȊO�̏ꍇ	*/
	if(data1 > 1)
	{
		/*	RRE�Ƃ��ē���	*/
		data1 = 0;
	}
	/*	1.5G/1.7G/700M�ȊO�̏ꍇ	*/
	if((data2 > 3)||(data2 < 1))
	{
		/*	1.5G�Ƃ��ē���	*/
		data2 = 1;
	}
	kind = (data2-1) + data1*3;
	rrhlib_dmsg("devicekind = %d\n", kind);
	memcpy(data_p,&kind,size);
	return E_API_RCD_OK;
}

E_RRHAPI_RCODE rrhApi_Inv_Fsm_GetTraInvInfo(
							VOID	*data_p,INT size ){ 	return E_API_RCD_OK; }

E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogGet( INT qid, INT wtime, VOID *data_p, UCHAR *filename, UINT size ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Log_Mnr_TroubleLogDelete( INT qid, INT wtime, VOID *data_p, UCHAR *filename ){ 	return E_API_RCD_OK; }
E_RRHAPI_RCODE rrhApi_Log_Mnt_TroubleLogGetStop( INT qid, INT wtime, VOID *data_p ){ 	return E_API_RCD_OK; }

void f_com_pa_off(void) { return; }

