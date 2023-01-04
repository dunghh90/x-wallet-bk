#include "planedump.h"

static int func_ptif_comdata(FILE* fp , char* data_p ,int datalen, unsigned int kind )
{
	unsigned int count;
	unsigned char* wdata_p = (unsigned char* )data_p;

	if(datalen > 0)
	{
		fprintf(fp,"           <0x0000>;");
		for(count=0; count < datalen  ;count++ , wdata_p++)
		{
			fprintf(fp,"%02x ", *wdata_p );
			if(((count+1) % 32) == 0)
			{
				fprintf(fp,"\n           <0x%04x>;",count+1);
			}
		}
	}
	return 0;
}

pcap_ptif_name rrupf_ptif_name_trc [] = 
{
	{0,0,""       ,func_ptif_comdata },
	{0,0,"Unknown",NULL}
};


pcap_proc_name proc_name [] = 
{
	{"OTH"     , rrupf_ptif_name_trc  },
	{"PTPv2"   , rrupf_ptif_name_trc  },
	{"SyncE"   , rrupf_ptif_name_trc  },
	{"M-Plane" , rrupf_ptif_name_trc  },
	{"UC-Plane", rrupf_ptif_name_trc  },
	{"ARP"     , rrupf_ptif_name_trc  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  },
	{"UnKnown" , NULL  }
};

static void func_ptif_header(FILE* fp , char* data_p ,int datalen,unsigned int index)
{
	if((index & 0x10000) != 0){		/* VLAN */
		fprintf(fp,"%02x:%02x:%02x:%02x:%02x:%02x > %02x:%02x:%02x:%02x:%02x:%02x pt:0x%04x Vtag:0x%02x Vpt:0x%04x(%s);len(%u)\n",
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_source[0]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_source[1]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_source[2]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_source[3]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_source[4]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_source[5]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_dest[0]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_dest[1]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_dest[2]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_dest[3]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_dest[4]),
			(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_dest[5]),
			ntohs(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_vlan_proto),
			ntohs(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_vlan_TCI),
			ntohs(((pcap_rrupf_vlanethheadr* )data_p)->h_vlaneth.h_vlan_encapsulated_proto),
			proc_name[index & 0xF].name,datalen
		);
	}
	else{	/* Not Vlan */
		fprintf(fp,"%02x:%02x:%02x:%02x:%02x:%02x > %02x:%02x:%02x:%02x:%02x:%02x pt:0x%04x(%s);len(%u)\n",
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_shost[0]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_shost[1]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_shost[2]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_shost[3]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_shost[4]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_shost[5]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_dhost[0]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_dhost[1]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_dhost[2]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_dhost[3]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_dhost[4]),
			(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_dhost[5]),
			ntohs(((pcap_rrupf_ethheader* )data_p)->h_eth.ether_type),
			proc_name[index & 0xF].name,datalen
		);
	}
	return;
}

unsigned int func_com_msg_index(unsigned int etype)
{
	unsigned int index;
	switch(etype & 0xFFFF){
		case ETH_P_IP:		/* Mplane IP packet */
			index = 3;
			break;
		case ETH_P_ARP:		/* ARP packet */
			index = 5;
			break;
		case ETH_P_1588:	/* PTPv2 */
			index = 1;
			break;
		case ETH_P_SLOW:	/* SyncE */
			index = 2;
			break;
		case 0xaefe:		/* U-plane */
			index = 4;
			break;
		default:
			index = 0;
			break;
	}
	if((etype & 0x10000) != 0){
		index |= 0x10000;
	}
	return index;
}

/* ユーザデータ表示関数 */
int func_userdata_dsp(
	FILE* fp,
	pcap_rrupf_userheader* user_p,
	int len,
	unsigned int index
)
{
	unsigned int count;
	int rtn = D_RRUPKTDUMP_TYPE_UNKNOWN;
	
	for(count=0 ; ; count++)
	{
		if(proc_name[index].ptif[count].func == NULL)
		{
			break;
		}
		func_ptif_header(fp,(char*)user_p,len,index);
		rtn = proc_name[index].ptif[count].func(fp,(char*)user_p,len,0);
		break;
	}
	return(rtn);
}

/* IPヘッダ表示関数 */
unsigned int func_head_dsp(
	FILE* fp ,
	pcap_data_timeoffs_t* time_p,
	char* data
)
{

	unsigned int etype;
	struct tm lctimep;
	time_t local_sec;

	if(time_p != NULL)
	{
		memset(&lctimep,0,sizeof(lctimep));
		local_sec = (time_t)time_p->sec;
		localtime_r((const time_t *)&local_sec,&lctimep);
		fprintf(fp,"%04d/%02d/%02d %02d:%02d:%02d.%06d;",
			lctimep.tm_year + 1900,
			lctimep.tm_mon+1,
			lctimep.tm_mday,
			lctimep.tm_hour,
			lctimep.tm_min,
			lctimep.tm_sec,
			time_p->usec
		);
	}
	etype = (unsigned int)(ntohs(((pcap_rrupf_ethheader*)data)->h_eth.ether_type));
	if(etype == ETH_P_8021Q){
		etype = (unsigned int)(ntohs(((pcap_rrupf_vlanethheadr*)data)->h_vlaneth.h_vlan_encapsulated_proto));
		etype |= 0x10000;
	}
	return func_com_msg_index(etype);
}

void func_help_fil_list( void )
{
	printf("Not supported \n");
	return;
}

static int plane_sd = -1;

void func_mirror_send( int len , char* data )
{
	struct ifreq			ifrq;
	struct sockaddr_ll		sd_bif;
	int ret;

	if(plane_sd < 0){
		if((plane_sd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0){
			perror("socket");
			return;
		}
		memset(&ifrq, 0, sizeof(ifrq));
		snprintf(ifrq.ifr_name,sizeof(ifrq.ifr_name),"eth0");
		if((ret = ioctl(plane_sd, SIOCGIFINDEX, &ifrq)) < 0){
			perror("ioctl");
			close(plane_sd);
			plane_sd = -1;
			return;
		}
		memset(&sd_bif, 0, sizeof(sd_bif));
		sd_bif.sll_family = AF_PACKET;
		sd_bif.sll_ifindex = ifrq.ifr_ifindex;
		sd_bif.sll_protocol = htons(ETH_P_ALL);
		sd_bif.sll_halen = ETHER_ADDR_LEN;
		if ((ret = bind(plane_sd, (const struct sockaddr *)&sd_bif, sizeof(sd_bif))) < 0) {
			perror("bind");
			close(plane_sd);
			plane_sd = -1;
			return;
		}
	}
	if(( ret = send(plane_sd, data, (size_t)len, 0)) < 0){
		perror("send");
		close(plane_sd);
		plane_sd = -1;
	}
	return;
}


