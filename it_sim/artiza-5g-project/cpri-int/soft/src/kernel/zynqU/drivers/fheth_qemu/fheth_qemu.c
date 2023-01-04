/********************************************************************************************************************/
/**
 *  @skip   $Id:$
 *  @file   fheth.c
 *  @brief  25G ethernet driver
 *  @date   2018/03/06 KCN)satou
 *
 *  ALL Rights Reserved, Copyright (C) FUJITSU Limited 2018
 */
/********************************************************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/crc32.h>
#include <linux/ethtool.h>
#include <linux/if_ether.h>
#include <linux/reboot.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <linux/of_address.h>
#include <linux/printk.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include "fheth_hw.h"

#define DRV_NAME        "fheth"
#define DRV_VERSION     "0.1"
#define DRV_AUTHOR      "Fujitsu Ltd."
#define DRV_DESC        "25G Ethernet Driver"

MODULE_AUTHOR(DRV_AUTHOR);
MODULE_DESCRIPTION(DRV_DESC);
MODULE_LICENSE("GPL");

#define E25G_RX_RING_SIZE_M 32
#define E25G_RX_RING_SIZE_S 32
#define E25G_RX_MAX_SIZE_M  2048
#define E25G_RX_MAX_SIZE_S  2048
#define E25G_TX_SLOT_SIZE 32 //暫定値

struct ptp_header { /* PTP パケットフォーマット */
	struct ethhdr		eth;
	char				tsmt; /* transportSpecific | messageType */
	char				ver;  /* reserved		   | versionPTP	 */
	short				messageLength;
	char				domainNumber;
	char				reserved1;
	char				flagField[2];
	long				correction;
	int					reserved2;
	char 				sourcePortIdentity[10];
	short				sequenceId;
	char				control;
	char				logMessageInterval;
}__attribute__((packed));

#define E25G_RTSTAMP 0 /* 受信タイムスタンプ取得 */
#define E25G_TTSTAMP 1 /* 送信タイムスタンプ取得 */

struct e25g_tstamp {
	int kind;                                       /* 種別 0:受信 1:送信 */
	int ptp_tag;                                    /* PTP tag(送信時のみ有効) */
	int ptp_time[4];                                /* PTPタイムスタンプ値 */
};

struct e25g_private {
	int	   portno;                                  /* Etherポート番号 */
	int	   usage;                                   /* M-plane(0),S-plane(1) */
	void __iomem * reg_base;                        /* レジスタベース */
	void __iomem * txbuf_base_m;                    /* 送信バッファベース(M-Plane) */
	void __iomem * txbuf_base_s;                    /* 送信バッファベース(S-Plane) */
	void * rxbuf_base_m;                            /* 受信バッファベース(M-Plane) */
	void * rxbuf_base_s;                            /* 受信バッファベース(S-Plane) */
	struct sk_buff *rx_skb_m[E25G_RX_RING_SIZE_M];  /* M-Plane用受信バッファ */
	struct sk_buff *rx_skb_s[E25G_RX_RING_SIZE_S];  /* S-Plane用受信バッファ */
	int rx_slot_m;                                  /* 受信スロット番号(M-Plane) */
	int rx_slot_s;                                  /* 受信スロット番号(S-Plane) */
	int tx_slot;                                    /* 送信スロット番号(S-Plane) */
	spinlock_t tx_lock_m;                           /* 送信ロック(M-Plane) */
	spinlock_t tx_lock_s;                           /* 送信ロック(S-Plane) */
	bool tx_process_m;                              /* 送信中フラグ(M-Plane) */
	bool tx_process_s;                              /* 送信中フラグ(S-Plane) */
	int ptp_tag[E25G_TX_SLOT_SIZE];                 /* 送信済PTP tag */
	int ptp_time_t[4][E25G_TX_SLOT_SIZE];           /* 送信済PTPタイムスタンプ値 */
	int ptp_time_r[4][E25G_RX_RING_SIZE_S];         /* 受信済PTPタイムスタンプ値 */
};


struct Timestamp {
	short   seconds_msb; /* 16 bits + */
	int     seconds_lsb; /* 32 bits = 48 bits*/
	int     nanoseconds;
};


/* PTP statistic log */
typedef enum {
	E_PTPLOG_SYNC                  = 0x0,
	E_PTPLOG_DELAY_REQ             = 0x1,
	E_PTPLOG_PDELAY_REQ            = 0x2,
	E_PTPLOG_PDELAY_RESP           = 0x3,
	E_PTPLOG_RESERVE1              = 0x4,
	E_PTPLOG_RESERVE2              = 0x5,
	E_PTPLOG_RESERVE3              = 0x6,
	E_PTPLOG_RESERVE4              = 0x7,
	E_PTPLOG_FOLLOW_UP             = 0x8,
	E_PTPLOG_DELAY_RESP            = 0x9,
	E_PTPLOG_PDELAY_RESP_FOLLOW_UP = 0xA,
	E_PTPLOG_ANNOUNCE              = 0xB,
	E_PTPLOG_SIGNALING             = 0xC,
	E_PTPLOG_MANAGEMENT            = 0xD,
	E_PTPLOG_OTHER_PTP             = 0xE,
	E_PTPLOG_SYNCE                 = 0xF,
	E_PTPLOG_OTHER_PRT             = 0x10,
	E_PTPLOG_MAX
}e_ptplog_msgname;

struct ptplog_statistic {
	unsigned long tx_cnt;
	unsigned long rx_cnt;
};

static struct ptplog_statistic ptplog_statistic_cnt[E_PTPLOG_MAX];
static struct proc_dir_entry* ptplog_entry_dir = NULL;

static void ptplogstat_save(int direction , int len, struct ptp_header* ptp_msg )
{
	struct ptplog_statistic *ptplog_p;

	if(len < sizeof(struct ethhdr)){
		return;
	}
	switch(ntohs(ptp_msg->eth.h_proto)){
		case ETH_P_SLOW:	/* syncE packets */
			ptplog_p = &ptplog_statistic_cnt[E_PTPLOG_SYNCE];
			break;
		case ETH_P_1588:	/* ptp packets */
			if( len < ( sizeof(struct ethhdr) + sizeof(ptp_msg->tsmt) )){
				ptplog_p = &ptplog_statistic_cnt[E_PTPLOG_OTHER_PTP];
			}
			else{
				if( ((ptp_msg->tsmt) & 0xF) < E_PTPLOG_OTHER_PTP ){
					ptplog_p = &ptplog_statistic_cnt[((ptp_msg->tsmt) & 0xF)];
				}
				else{
					ptplog_p = &ptplog_statistic_cnt[E_PTPLOG_OTHER_PTP];
				}
			}
			break;
		default:
			ptplog_p = &ptplog_statistic_cnt[E_PTPLOG_OTHER_PRT];
			break;
	}
	if(direction == 0){
		ptplog_p->tx_cnt++;
	}else{
		ptplog_p->rx_cnt++;
	}
	return;
}

static ssize_t ptplogstat_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *ppos)
{
	memset(ptplog_statistic_cnt,0,sizeof(ptplog_statistic_cnt));
	return count;
}

static ssize_t ptplogstat_read(struct file *filp, char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned long outbyte = 0;
	char wbuf[2048];
	e_ptplog_msgname msg_kind;
	char*	msg_name;
	unsigned long msg_txcounter;
	unsigned long msg_rxcounter;

	if(*ppos > 0){
		return 0;
	}
	memset(wbuf,0,sizeof(wbuf));
	for(msg_kind = E_PTPLOG_SYNC ; msg_kind < E_PTPLOG_MAX ; msg_kind++){
		msg_txcounter = ptplog_statistic_cnt[msg_kind].tx_cnt;
		msg_rxcounter = ptplog_statistic_cnt[msg_kind].rx_cnt;
		switch(msg_kind){
			case E_PTPLOG_SYNC:
				msg_name = "SYNC";
				break;
			case E_PTPLOG_DELAY_REQ:
				msg_name = "DELAY_REQ";
				break;
			case E_PTPLOG_PDELAY_REQ:
				msg_name = "PDELAY_REQ";
				break;
			case E_PTPLOG_PDELAY_RESP:
				msg_name = "PDELAY_RESP";
				break;
			case E_PTPLOG_FOLLOW_UP:
				msg_name = "FOLLOW_UP";
				break;
			case E_PTPLOG_DELAY_RESP:
				msg_name = "DELAY_RESP";
				break;
			case E_PTPLOG_PDELAY_RESP_FOLLOW_UP:
				msg_name = "PDELAY_RESP_FOLLOW_UP";
				break;
			case E_PTPLOG_ANNOUNCE:
				msg_name = "ANNOUNCE";
				break;
			case E_PTPLOG_SIGNALING:
				msg_name = "SIGNALING";
				break;
			case E_PTPLOG_MANAGEMENT:
				msg_name = "MANAGEMENT";
				break;
			case E_PTPLOG_OTHER_PTP:
				msg_name = "UnknownPTPpkts";
				break;
			case E_PTPLOG_SYNCE:
				msg_name = "SyncE";
				break;
			default:
				msg_name = "UnknownPkts";
				break;
		}
		if((msg_txcounter > 0) || (msg_rxcounter > 0)){
			outbyte += sprintf( &wbuf[outbyte], "%22s(0x%x) : %lu txPkts %lu rxPkts\n", msg_name ,msg_kind, msg_txcounter , msg_rxcounter);
			if(outbyte >= sizeof(wbuf)){
				break;
			}
		}
	}
	*ppos = 0;
	return simple_read_from_buffer(user_buf, count, ppos, wbuf, outbyte);
}


static int e25g_rx_clean_m(struct net_device *ndev);
static int e25g_tx_clean_m(struct net_device *ndev);
static int e25g_rx_clean_s(struct net_device *ndev);
static int e25g_tx_clean_s(struct net_device *ndev);
static int e25g_start_xmit_m(struct sk_buff *skb, struct net_device *ndev);
static int e25g_start_xmit_s(struct sk_buff *skb, struct net_device *ndev);
static int e25g_add_dev(struct platform_device *pdev);
static int e25g_ptp_set(struct net_device *ndev, struct ifreq *ifr);
static int e25g_ptp_get(struct net_device *ndev, struct ifreq *ifr);
static int e25g_probe_count=0;

static struct net_device *qemu_ndev_m[2];
static struct net_device *qemu_ndev_s[2];

static inline u32 e25g_read_reg(void __iomem * base , u32 offset)
{
//	return readl(base + offset);
	return 0;
}



static inline void e25g_write_reg(void __iomem * base , u32 offset , u32 val)
{
//	writel(val, base + offset);
}

static irqreturn_t e25g_intr_mplane(int __always_unused irq, void *data)
{
	struct net_device *ndev = data;
	struct e25g_private *e25g = netdev_priv(ndev);
	int reg;
	int rc;
	
	/* 割り込みステータス読み出し */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQMP);

	if((reg & E25G_IRQMP_RXDONE) !=0 ){
//		e25g_write_reg(e25g->reg_base, E25G_IRQMP, E25G_IRQMP_RXDONE);
		/* 受信処理 */
		rc = e25g_rx_clean_m( ndev );
	}
	if((reg & E25G_IRQMP_TXDONE) !=0 ){
		/* 送信完了処理 */
		rc = e25g_tx_clean_m( ndev );
	}
	return IRQ_HANDLED;

}

static irqreturn_t e25g_intr_splane(int __always_unused irq, void *data)
{
	struct net_device *ndev = data;
	struct e25g_private *e25g = netdev_priv(ndev);
	int reg;
	int rc;

	/* 割り込みステータス読み出し */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQSP);

	if((reg & E25G_IRQSP_RXDONE) !=0 ){
		e25g_write_reg(e25g->reg_base, E25G_IRQSP, E25G_IRQSP_RXDONE);
		/* 受信処理 */
		rc = e25g_rx_clean_s( ndev );
	}
	if((reg & (E25G_IRQSP_TXDONE | E25G_IRQSP_PTUPD)) !=0 ){
		/* 送信完了処理 */
		rc = e25g_tx_clean_s( ndev );
	}
	return IRQ_HANDLED;

}


static int e25g_rx_clean_m(struct net_device *ndev)
{
	int i;
	int count;
	int reg;
	int flag;
	int rc = 0;
	int len;
	int bank;
	void __iomem * buf;      /* 受信バッファポインタ */
	void* tmp1;
	void* tmp2; /* バッファポインタワーク */
	struct e25g_private *e25g = netdev_priv(ndev);
	
	/* 受信IRQマスク設定 */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQMPM);
	reg |= E25G_IRQMPM_RXDONE;
	e25g_write_reg(e25g->reg_base, E25G_IRQMPM, reg);

	/* 受信ステータス読み出し */
	reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSM);
	
	if( reg == 0 ){
		/* 受信データなし */
		/* 受信IRQマスク解除 */
		reg = e25g_read_reg(e25g->reg_base, E25G_IRQMPM);
		reg &= ~E25G_IRQMPM_RXDONE;
		e25g_write_reg(e25g->reg_base, E25G_IRQMPM, reg);
		return 0;
	}
	/* 受信バンクおよび受信データ長取得 */
	reg = e25g_read_reg(e25g->reg_base, E25G_BANKNOM);
	len = (reg & BANKNOM_LENGTH) >> BANKNOM_LENGTH_SHIFT;
	bank = reg & BANKNOM_BANKNO;

	/* 受信バッファアドレス取得 */
	buf = (e25g->rxbuf_base_m + (0x800 * bank));
	
	/* DDR受信フラグ確認 */
	i = 0;
	flag = *((int *)(e25g->rxbuf_base_m +(E25G_MPLANE_RX_FLAG + (0x10 * bank))));

	while((flag && 0x80000000) == 0 ){
		udelay(1);
		i ++;
		if(i > 100){
			/* 受信ステータスのクリア */
			reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSM);
			reg &= ~(0x00000001 << bank);
			e25g_write_reg(e25g->reg_base, E25G_BANKSTATUSM, reg);
			
			/* 受信IRQマスク解除 */
			reg = e25g_read_reg(e25g->reg_base, E25G_IRQMPM);
			reg &= ~E25G_IRQMPM_RXDONE;
			e25g_write_reg(e25g->reg_base, E25G_IRQMPM, reg);
			
			return 0;
		}
		flag = *((int *)(e25g->rxbuf_base_m +(E25G_MPLANE_RX_FLAG + (0x10 * bank))));
	}
	/* 受信バッファ読み出し */
	/* 受信データ長からカウンタを算出 */
	count = (len) / 4;
	if((len) % 4){
		count++;
	}
	
	skb_reserve(e25g->rx_skb_m[bank], NET_IP_ALIGN);
	tmp1 = buf;
	tmp2 = skb_put(e25g->rx_skb_m[bank], len);
	
	for (i = 0; i < count; i++){
		/* 受信データをskbuffに読み出し */
		reg = e25g_read_reg(tmp1, 0);
		memcpy((void*)tmp2 , (const void*)&reg , 4);
		tmp1+=4;
		tmp2+=4;
	}
	
	/* 受信フラグのクリア */
	*((int *)(e25g->rxbuf_base_m +(E25G_MPLANE_RX_FLAG + (0x10 * bank)))) = 0;
	
	e25g->rx_skb_m[bank]->protocol =
	    eth_type_trans(e25g->rx_skb_m[bank], ndev);
	rc = netif_rx(e25g->rx_skb_m[bank]);
	
	ndev->stats.rx_packets ++;
	ndev->stats.rx_bytes += len;

	/* 受信ステータスのクリア */
	reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSM);
	reg |= (0x00000001 << bank);
	e25g_write_reg(e25g->reg_base, E25G_BANKSTATUSM, reg);
	
	/* skb補充 */
	e25g->rx_skb_m[bank] = NULL;
	e25g->rx_skb_m[bank] = dev_alloc_skb(E25G_RX_MAX_SIZE_M);
	
	/* 割込み要因のクリア */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQMP);
	reg |= E25G_IRQMP_RXDONE;
	e25g_write_reg(e25g->reg_base, E25G_IRQMP, reg);

	/* 受信IRQマスク解除 */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQMPM);
	reg &= ~E25G_IRQMPM_RXDONE;
	e25g_write_reg(e25g->reg_base, E25G_IRQMPM, reg);
	
	return 0;
}


static void e25g_rx_clean_m_qemu(struct sk_buff *skb, struct net_device *ndev)
{
	int bank;
	struct e25g_private *e25g = netdev_priv(ndev);
	int rc = 0;

	bank = 0;
	e25g->rx_skb_m[bank]->len = skb->len;
	memcpy(e25g->rx_skb_m[bank]->data,skb->data,e25g->rx_skb_m[bank]->len);
	e25g->rx_skb_m[bank]->protocol = eth_type_trans(e25g->rx_skb_m[bank], ndev);
	rc = netif_rx(e25g->rx_skb_m[bank]);
	ndev->stats.rx_packets ++;
	ndev->stats.rx_bytes += skb->len;
	e25g->rx_skb_m[bank] = NULL;
	e25g->rx_skb_m[bank] = dev_alloc_skb(E25G_RX_MAX_SIZE_M);
	
	return;
}

static int e25g_rx_clean_s(struct net_device *ndev)
{
	int i;
	int count;
	int reg;
	int rc = 0;
	int len;
	int flag;
	int bank;
	void __iomem * buf;      /* 受信バッファポインタ */
	void* tmp1;
	void* tmp2; /* バッファポインタワーク */
	struct e25g_private *e25g = netdev_priv(ndev);

	/* 受信IRQマスク設定 */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQSPM);
	reg |= E25G_IRQSPM_RXDONE;
	e25g_write_reg(e25g->reg_base, E25G_IRQSPM, reg);

	/* 受信ステータス読み出し */
	reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSS);
	if( reg == 0 ){
		/* 受信データなし */
		/* 受信IRQマスク解除 */
		reg = e25g_read_reg(e25g->reg_base, E25G_IRQSPM);
		reg &= ~E25G_IRQSPM_RXDONE;
		e25g_write_reg(e25g->reg_base, E25G_IRQSPM, reg);
		return 0;
	}
	/* 受信バンクおよび受信データ長取得 */
	reg = e25g_read_reg(e25g->reg_base, E25G_BANKNOS);
	len = (reg & E25G_BANKNOS_LENGTH) >> E25G_BANKNOS_LENGTH_SHIFT;
	bank = reg & E25G_BANKNOS_BANKNO;
	
	/* 受信バッファアドレス取得 */
	buf = (e25g->rxbuf_base_s + (0x800 * bank));
	
	/* DDR受信フラグ確認 */
	i = 0;
	flag = *((int*)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_FLAG + (0x10 * bank) )));
	while((flag && 0x80000000) == 0 ){
		udelay(1);
		i ++;
		if(i > 100){
			/* 受信ステータスのクリア */
			reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSS);
			reg &= ~(0x00000001 << bank);
			e25g_write_reg(e25g->reg_base, E25G_BANKSTATUSS, reg);
			return 0;
			/* 受信IRQマスク解除 */
			reg = e25g_read_reg(e25g->reg_base, E25G_IRQSPM);
			reg &= ~E25G_IRQSPM_RXDONE;
			e25g_write_reg(e25g->reg_base, E25G_IRQSPM, reg);
		}
		flag = *((int*)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_FLAG + (0x10 * bank) )));
	}
	/* 受信バッファ読み出し */
	/* 受信データ長からカウンタを算出 */
	count = (len) / 4;
	if((len) % 4){
		count++;
	}
	
	skb_reserve(e25g->rx_skb_s[bank], NET_IP_ALIGN);
	tmp1 = buf;
	tmp2 = skb_put(e25g->rx_skb_s[bank], len);
	
	for (i = 0; i < count; i++){
		/* 受信データをskbuffに読み出し */
		reg = e25g_read_reg(tmp1, 0);
		memcpy((void*)tmp2 , (const void*)&reg , 4);
		tmp1 +=4;
		tmp2 +=4;
	}
	
	/* 受信フラグのクリア */
	*((int*)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_FLAG + (0x10 * bank)))) = 0;
	
	e25g->rx_skb_s[bank]->protocol =
	    eth_type_trans(e25g->rx_skb_s[bank], ndev);
	rc = netif_rx(e25g->rx_skb_s[bank]);

	/* 受信ステータスのクリア */
	reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSS);
	reg &= ~(0x00000001 << bank);
	e25g_write_reg(e25g->reg_base, E25G_BANKSTATUSS, reg);
	
	/* 受信タイムスタンプの読み出し */
	e25g->ptp_time_r[0][bank] = *((int *)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_SEC1 + (0x10 * bank))));
	e25g->ptp_time_r[1][bank] = *((int *)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_SEC2 + (0x10 * bank))));
	e25g->ptp_time_r[2][bank] = *((int *)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_NSEC + (0x10 * bank))));
	
	ndev->stats.rx_packets ++;
	ndev->stats.rx_bytes += len;

	/* skb補充 */
	e25g->rx_skb_s[bank] = NULL;
	e25g->rx_skb_s[bank] = dev_alloc_skb(E25G_RX_MAX_SIZE_S);
	
	/* 割込み要因のクリア */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQSP);
	reg |= E25G_IRQSP_RXDONE;
	e25g_write_reg(e25g->reg_base, E25G_IRQSP, reg);

	/* 受信IRQマスク解除 */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQSPM);
	reg &= ~E25G_IRQSPM_RXDONE;
	e25g_write_reg(e25g->reg_base, E25G_IRQSPM, reg);
	return 0;
}

static int qemu_ptp_time_r_sec1 = 0;
static int qemu_ptp_time_r_sec2 = 0;
static int qemu_ptp_time_r_nsec3 = 0;

static void e25g_rx_clean_s_qemu(struct sk_buff *skb, struct net_device *ndev)
{
	int bank;
	struct e25g_private *e25g = netdev_priv(ndev);
	int rc = 0;

	bank = 0;
	e25g->rx_skb_s[bank]->len = skb->len;
	memcpy(e25g->rx_skb_s[bank]->data,skb->data,e25g->rx_skb_s[bank]->len);
	e25g->rx_skb_s[bank]->protocol = eth_type_trans(e25g->rx_skb_s[bank], ndev);
	ptplogstat_save(1,skb->len,(struct ptp_header* )skb->data);

	rc = netif_rx(e25g->rx_skb_s[bank]);
	if(qemu_ptp_time_r_sec2 == 0xffffffff){
		qemu_ptp_time_r_sec1++;
	}
	e25g->ptp_time_r[0][bank] = qemu_ptp_time_r_sec1;
	e25g->ptp_time_r[1][bank] = qemu_ptp_time_r_sec2++;
	e25g->ptp_time_r[2][bank] = qemu_ptp_time_r_nsec3++;
	ndev->stats.rx_packets ++;
	ndev->stats.rx_bytes += skb->len;
	e25g->rx_skb_s[bank] = NULL;
	e25g->rx_skb_s[bank] = dev_alloc_skb(E25G_RX_MAX_SIZE_S);
	
	return;
}


static int e25g_start_xmit(struct sk_buff *skb, struct net_device *ndev)
{
	int rc;
	struct ethhdr * eth;
	struct e25g_private *e25g = netdev_priv(ndev);

	eth = eth_hdr(skb);

	if(e25g->usage == 0){
		/* M-Plane */
		rc = e25g_start_xmit_m(skb , ndev); /* M-Plane send */
	}else{
		/* S-Plane */
		rc = e25g_start_xmit_s(skb , ndev); /* S-Plane send */
	}	
	return rc;
}

static int e25g_start_xmit_m(struct sk_buff *skb, struct net_device *ndev)
{
	int i;
	struct e25g_private *e25g = netdev_priv(ndev);
	int count = 0;
	int len;
	int* tmp;
	char* tmp2;
	unsigned long flags;
	
	/* ロック設定 */
	spin_lock_irqsave(&e25g->tx_lock_m, flags);

	if ((e25g->tx_process_m) == 1) {
		/* 送信実行中の場合はBUSYエラーを返送 */
		spin_unlock_irqrestore(&e25g->tx_lock_m, flags);
		return NETDEV_TX_BUSY;
	}
	
	/* 送信バッファの書き込み */
	len = skb->len;
	if(skb->len < 60){
		/* フレーム長が60byte未満の場合はパディングを挿入する */
		count = 60 - (skb->len);
		tmp2 = (skb->data) + len;
		for (i = 0; i < count; i++){
			*(tmp2 + i) = 0;
		}
		len += count;
	}
	
	/* 送信データ長からカウンタを算出 */
	count = len / 4;
	if(len % 4){
		count++;
	}
	tmp = (int *)(skb->data);
	
	for (i = 0; i < count; i++){
		/* 送信バッファにデータを書込 */
		e25g_write_reg(e25g->txbuf_base_m ,  i*4, *tmp);
		tmp ++;
	}
	/* 送信バッファ長書込 */
	e25g_write_reg(e25g->reg_base, E25G_TXLENGTHM, len);
	
/*	e25g->tx_process_m = 1; *//* 送信実行中フラグを設定 */
	
	/* 送信開始 */
	e25g_write_reg(e25g->reg_base, E25G_TXTRIGM, 0x00000001);

	ndev->stats.tx_packets ++;
	ndev->stats.tx_bytes += (skb->len);

	e25g_rx_clean_m_qemu(skb,qemu_ndev_m[e25g->portno]);

	dev_kfree_skb_any(skb);

	/* ロック解除 */
	spin_unlock_irqrestore(&e25g->tx_lock_m, flags);
	

	return NETDEV_TX_OK;
}

static int e25g_start_xmit_s(struct sk_buff *skb, struct net_device *ndev)
{
	int i;
	struct e25g_private *e25g = netdev_priv(ndev);
	struct ptp_header *ptph = (struct ptp_header *)skb->data;
	int count = 0;
	int len;
	int* tmp;
	char* tmp2;
	unsigned long flags;
	int tag = 0;

	/* ロック設定 */
	spin_lock_irqsave(&e25g->tx_lock_s, flags);
	
	if ((e25g->tx_process_s) == 1) {
		/* 送信実行中の場合はBUSYエラーを返送 */
		spin_unlock_irqrestore(&e25g->tx_lock_s, flags);
		return NETDEV_TX_BUSY;
	}
	
	/* 送信バッファの書き込み */
	len = skb->len;
	if(skb->len < 60){
		/* フレーム長が60byte未満の場合はパディングを挿入する */
		count = 60 - (skb->len);
		tmp2 = (skb->data) + len;
		for (i = 0; i < count; i++){
			*(tmp2 + i) = 0;
		}
		len += count;
	}
	
	/* 送信データ長からカウンタを算出 */
	count = len / 4;
	if(len % 4){
		count++;
	}
	tmp = (int *)(skb->data);

	for (i = 0; i < count; i++){
		/* 送信バッファにデータを書込 */
		e25g_write_reg(e25g->txbuf_base_s ,  i*4, *tmp);
		tmp ++;
	}
	/* 送信バッファ長書込 */
	e25g_write_reg(e25g->reg_base, E25G_TXLENGTHS, len);

	/* 送信タグ設定 */
	tag = (int)(ntohs(ptph->sequenceId));
	e25g_write_reg(e25g->reg_base, E25G_TXPTPTAGS, tag );
	
	if(ptph->tsmt == 0x1){
		struct Timestamp* ptimestamp;
		ptimestamp = (struct Timestamp*)(((void*)(ptph))+sizeof(*ptph));
		e25g_write_reg(e25g->reg_base, E25G_TXTSTAMP1, ntohs(ptimestamp->seconds_msb) );
		e25g_write_reg(e25g->reg_base, E25G_TXTSTAMP2, ntohl(ptimestamp->seconds_lsb) );
		e25g_write_reg(e25g->reg_base, E25G_TXTSTAMP3, ntohl(ptimestamp->nanoseconds) );
	
		e25g->ptp_tag[(e25g->tx_slot)] = tag;
		e25g->ptp_time_t[0][(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXTSTAMP1);
		e25g->ptp_time_t[1][(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXTSTAMP2);
		e25g->ptp_time_t[2][(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXTSTAMP3);
		e25g->tx_slot++;
		if(e25g->tx_slot >= E25G_TX_SLOT_SIZE){
			e25g->tx_slot = 0;
		}
	}

/*	e25g->tx_process_s = 1; *//* 送信実行中フラグを設定 */
	
	/* 送信開始 */
	e25g_write_reg(e25g->reg_base, E25G_TXTRIGS, 0x00000001);

	ndev->stats.tx_packets ++;
	ndev->stats.tx_bytes += (skb->len);

	ptplogstat_save(0,skb->len,(struct ptp_header* )skb->data);

	e25g_rx_clean_s_qemu(skb,qemu_ndev_s[e25g->portno]);
	
	dev_kfree_skb_any(skb);

	/* ロック解除 */
	spin_unlock_irqrestore(&e25g->tx_lock_s, flags);

	return NETDEV_TX_OK;
}

static int e25g_tx_clean_m(struct net_device *ndev)
{
	struct e25g_private *e25g = netdev_priv(ndev);
	unsigned long flags;
	
	/* ロック設定 */
	spin_lock_irqsave(&e25g->tx_lock_m, flags);

	/* 割り込みステータス読み出し */
	e25g_write_reg(e25g->reg_base, E25G_IRQMP, E25G_IRQMP_TXDONE);

	e25g->tx_process_m = 0; /* 送信実行中フラグを解除 */

	/* ロック解除 */
	spin_unlock_irqrestore(&e25g->tx_lock_m, flags);
	
	return 0;

}

static int e25g_tx_clean_s(struct net_device *ndev)
{
	int i;
	struct e25g_private *e25g = netdev_priv(ndev);
	int reg;
	unsigned long flags;
	
	/* ロック設定 */
	spin_lock_irqsave(&e25g->tx_lock_s, flags);

	/* 割り込みステータス読み出し */
	reg = e25g_read_reg(e25g->reg_base, E25G_IRQSP);
	e25g_write_reg(e25g->reg_base, E25G_IRQSP, E25G_IRQSP_TXDONE | E25G_IRQSP_PTUPD);

	/* 送信タグアップデート確認 */
	i = 0;
	while( (reg && E25G_IRQSP_PTUPD) == 0 ){
		udelay(1);
		i ++;
		if(i > 100){
			/* 送信タグアップデートタイムアウト(TBD) */
			/* ロック解除 */
			e25g->tx_process_s = 0; /* 送信実行中フラグを解除 */
			spin_unlock_irqrestore(&e25g->tx_lock_s, flags);
			return 0;
		}
		reg = e25g_read_reg(e25g->reg_base, E25G_IRQSP);
	}
	/* 送信タグとタイムスタンプの読み出し */
	e25g->ptp_tag[(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXPTPTAG);
	e25g->ptp_time_t[0][(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXTSTAMP1);
	e25g->ptp_time_t[1][(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXTSTAMP2);
	e25g->ptp_time_t[2][(e25g->tx_slot)] = e25g_read_reg(e25g->reg_base, E25G_TXTSTAMP3);

	/* 送信スロット番号更新 */

	e25g->tx_process_s = 0; /* 送信実行中フラグを解除 */

	/* ロック解除 */
	spin_unlock_irqrestore(&e25g->tx_lock_s, flags);
	
	return 0;

}
static int e25g_ioctl(struct net_device *ndev, struct ifreq *ifr, int cmd)
{
	struct e25g_private *e25g = netdev_priv(ndev);

	if(e25g->usage == E25G_USAGE_PLANE_S){
		switch (cmd) {
			case SIOCSHWTSTAMP:
				return e25g_ptp_set(ndev, ifr);
			case SIOCGHWTSTAMP:
				return e25g_ptp_get(ndev, ifr);
			default:
//				printk(KERN_ERR "%s: cmd(%d) error  \n", ndev->name,cmd);
				return -EOPNOTSUPP;
		}
	}else{
//		printk(KERN_ERR "%s: usage(%d) error  \n", ndev->name,e25g->usage);
		return -EOPNOTSUPP;
	}
}

static int e25g_ptp_set(struct net_device *ndev, struct ifreq *ifr)
{
	/* TBD */
	return 0;
}
static int e25g_ptp_get(struct net_device *ndev, struct ifreq *ifr)
{
	struct e25g_private *e25g = netdev_priv(ndev);
	struct e25g_tstamp tstamp;
	int i;

	if (copy_from_user(&tstamp, ifr->ifr_data, sizeof(tstamp))){
		printk(KERN_ERR "%s: copy_from_user error  \n", ndev->name);
		return -EFAULT;
	}
	/* 処理種別で分岐 */
	if(tstamp.kind == E25G_RTSTAMP){
		/* 受信タイムスタンプ取得 */
		tstamp.ptp_time[0] = e25g->ptp_time_r[0][0];
		tstamp.ptp_time[1] = e25g->ptp_time_r[1][0];
		tstamp.ptp_time[2] = e25g->ptp_time_r[2][0];
	}else if(tstamp.kind == E25G_TTSTAMP){
		/* 送信タイムスタンプ取得 */
		for(i = 0 ; i < E25G_TX_SLOT_SIZE ; i++){
			if(tstamp.ptp_tag == e25g->ptp_tag[i]){
				break;
			}
		}
		
		if(i >= E25G_TX_SLOT_SIZE){
			printk(KERN_ERR "%s: PTP_TAG Error(0x%x(%d)) \n", ndev->name,tstamp.ptp_tag,tstamp.ptp_tag);
			return -ENOENT;
		}
		
		tstamp.ptp_time[0] = e25g->ptp_time_t[0][i];
		tstamp.ptp_time[1] = e25g->ptp_time_t[1][i];
		tstamp.ptp_time[2] = e25g->ptp_time_t[2][i];
		
	}else{
		/* パラメータエラー */
		printk(KERN_ERR "%s: Param Error(%d) \n", ndev->name,tstamp.kind);
		return -EINVAL;
	}
	
	if (copy_to_user(ifr->ifr_data, &tstamp, sizeof(tstamp))){
		printk(KERN_ERR "%s: copy_to_user error  \n", ndev->name);
		return -EFAULT;
	}
	return 0;
}

static int e25g_open(struct net_device *ndev)
{
	int i, rc = 0;
	int reg;
	struct e25g_private *e25g = netdev_priv(ndev);

	printk(KERN_INFO "%s: open %s\n", DRV_NAME, ndev->name);

	e25g->tx_slot = 0;

	switch(e25g->usage){
		case E25G_USAGE_PLANE_M:
			/* Initialize rx ring */
			for (i = 0; i < E25G_RX_RING_SIZE_M; i++){
				e25g->rx_skb_m[i] = NULL;
				e25g->rx_skb_m[i] = dev_alloc_skb(E25G_RX_MAX_SIZE_M);
			}
			e25g->rx_slot_m = 0;

			/* M-Plane初期化 */
			/* 受信ステータス読み出し */
			reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSM);
			if( reg != 0 ){
				for (i = 0; i < E25G_RX_RING_SIZE_M; i++){
				/* レジスタの空読み */
				reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSM);
				/* 受信フラグのクリア */
				*((int *)(e25g->rxbuf_base_m +(E25G_MPLANE_RX_FLAG + (0x10 * i)))) = 0;
				/* 受信ステータスクリア */
				reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSM);
				reg &= ~(0x00000001 << i);
				e25g_write_reg(e25g->reg_base, E25G_BANKSTATUSM, reg);
				}
			}
			e25g_write_reg(e25g->reg_base, E25G_IRQMPM, 0x0);
			/* MAC受信Enable */
			reg = e25g_read_reg(e25g->reg_base, E25G_RXCTL);
			if((reg & E25G_RXCTL_RXEN) == 0){
				reg |= E25G_RXCTL_RXEN;
				e25g_write_reg(e25g->reg_base, E25G_RXCTL, reg);
			}
			break;
		case E25G_USAGE_PLANE_S:	
			/* Initialize rx ring */
			for (i = 0; i < E25G_RX_RING_SIZE_S; i++){
				e25g->rx_skb_s[i] = NULL;
				e25g->rx_skb_s[i] = dev_alloc_skb(E25G_RX_RING_SIZE_S);
			}

			e25g->rx_slot_s = 0;

			for (i = 0; i < E25G_RX_RING_SIZE_S; i++){
				e25g->ptp_time_r[0][i] = 0;
				e25g->ptp_time_r[1][i] = 0;
				e25g->ptp_time_r[2][i] = 0;
			}

			/* Initialize tx ring */
			for (i = 0; i < E25G_TX_SLOT_SIZE; i++){
				e25g->ptp_tag[i] = 0;
				e25g->ptp_time_t[0][i] = 0;
				e25g->ptp_time_t[1][i] = 0;
				e25g->ptp_time_t[2][i] = 0;
			}
					/* S-Plane初期化 */
			/* 受信ステータス読み出し */
			reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSS);
			if( reg != 0 ){
				for (i = 0; i < E25G_RX_RING_SIZE_S; i++){
					/* レジスタの空読み */
					reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSS);
					/* 受信フラグのクリア */
					*((int *)(e25g->rxbuf_base_s +(E25G_SPLANE_RX_FLAG + (0x10 * i)))) = 0;
					/* 受信ステータスクリア */
					reg = e25g_read_reg(e25g->reg_base, E25G_BANKSTATUSS);
					reg &= ~(0x00000001 << i);
					e25g_write_reg(e25g->reg_base, E25G_BANKSTATUSS, reg);
				}
			}
			e25g_write_reg(e25g->reg_base, E25G_IRQSPM, 0x0);
			/* MAC受信Enable */
			reg = e25g_read_reg(e25g->reg_base, E25G_RXCTL);
			if((reg & E25G_RXCTL_RXEN) == 0){
				reg |= E25G_RXCTL_RXEN;
				e25g_write_reg(e25g->reg_base, E25G_RXCTL, reg);
			}
			break;
		default:
			break;
	}

	netif_carrier_on(ndev);
	netif_start_queue(ndev);

	return rc;
}

static int e25g_close(struct net_device *ndev)
{
	struct e25g_private *e25g = netdev_priv(ndev);
	int i;
	int reg;

	printk(KERN_INFO "%s: close %s\n", DRV_NAME, ndev->name);

	netif_stop_queue(ndev);
	netif_carrier_off(ndev);

	/* 割り込みマスク設定 */
	if(e25g->usage == E25G_USAGE_PLANE_M){
		for (i = 0; i < E25G_RX_RING_SIZE_M; i++)
			kfree_skb(e25g->rx_skb_m[i]);
		/* MAC受信Disable */
		reg = e25g_read_reg(e25g->reg_base, E25G_RXCTL);
		if((reg & E25G_RXCTL_RXEN) != 0){
			reg &= ~E25G_RXCTL_RXEN;
			e25g_write_reg(e25g->reg_base, E25G_RXCTL, reg);
		}
		e25g_write_reg(e25g->reg_base, E25G_IRQMPM, 
		E25G_IRQMPM_TXDONE | E25G_IRQMPM_RXDONE) ;
	}else{
		for (i = 0; i < E25G_RX_RING_SIZE_S; i++)
			kfree_skb(e25g->rx_skb_s[i]);

		/* MAC受信Disable */
		reg = e25g_read_reg(e25g->reg_base, E25G_RXCTL);
		if((reg & E25G_RXCTL_RXEN) != 0){
			reg &= ~E25G_RXCTL_RXEN;
			e25g_write_reg(e25g->reg_base, E25G_RXCTL, reg);
		}
		e25g_write_reg(e25g->reg_base, E25G_IRQSPM, 
			E25G_IRQSP_PTUPD | E25G_IRQSP_TXDONE | E25G_IRQSP_RXDONE);
	}
	return 0;
}

static int e25g_set_mac_addr(struct net_device *ndev, void *p)
{
	struct sockaddr *addr = p;
	u32 high,low;
	struct e25g_private *e25g = netdev_priv(ndev);

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;
	
	memcpy(ndev->dev_addr, addr->sa_data, ndev->addr_len);
	low = ((u32)addr->sa_data[5] | ((u32)addr->sa_data[4] << 8) |
			((u32)addr->sa_data[3] << 16) | ((u32)addr->sa_data[2] << 24));
	high = ((u32)addr->sa_data[1] | ((u32)addr->sa_data[0] << 8));
	
	/* MACアドレス設定 */
	e25g_write_reg(e25g->reg_base, E25G_FILTDA11, high);
	e25g_write_reg(e25g->reg_base, E25G_FILTDA12, low);
	
	return 0;
}

static const struct net_device_ops e25g_netdev_ops = {
	.ndo_open				= e25g_open,
	.ndo_stop				= e25g_close,
	.ndo_start_xmit			= e25g_start_xmit,
	.ndo_do_ioctl			= e25g_ioctl,
	.ndo_set_mac_address	= e25g_set_mac_addr,
};

static const struct ethtool_ops e25g_netdev_ethtool_ops = {
	.get_ts_info		= ethtool_op_get_ts_info,
};



static int e25g_add_dev(struct platform_device *pdev)
{
	int rc ;
	int reg;
	struct net_device *ndev;
	struct e25g_private *e25g;
	void* intr_func;
	struct resource res_mem;
	struct resource *res = &res_mem;
	struct device_node *np = pdev->dev.of_node;
	int count=0;

	/* 初期設定 */
	/* ETH1 初期設定 */
	/* Allocate our net_device structure */
	ndev = alloc_etherdev(sizeof(struct e25g_private));
	if (ndev == NULL) {
		printk(KERN_ERR "%s: alloc_etherdev error \n", DRV_NAME);
		rc = -ENOMEM;
		goto out;
	}
	
	/* Set the default MAC address */
	ndev->dev_addr[0] = 0x00;
	ndev->dev_addr[1] = 0xe0;
	ndev->dev_addr[2] = 0x00;
	ndev->dev_addr[3] = 0x00;
	ndev->dev_addr[4] = 0x00;

	ndev->netdev_ops = &e25g_netdev_ops;
	ndev->ethtool_ops = &e25g_netdev_ethtool_ops;
	
	ndev->mtu = 1500;
	ndev->features = NETIF_F_LLTX; /* TBD */
	/* Privateデータ取得 */
	e25g = netdev_priv(ndev);
	e25g->portno = 0;
	if(e25g_probe_count == 0){
		sprintf(ndev->name, "fheth%d", e25g->portno );
		e25g->usage = E25G_USAGE_PLANE_M;
		intr_func = e25g_intr_mplane;
		qemu_ndev_m[0] = ndev;
	}else{
		sprintf(ndev->name, "fhethS%d", e25g->portno );
		e25g->usage = E25G_USAGE_PLANE_S;
		intr_func = e25g_intr_splane;
		qemu_ndev_s[0] = ndev;

	}
	ndev->dev_addr[5] = e25g->portno + 1;

	printk(KERN_ERR "%s: Set the default Private DATA \n", ndev->name);
	rc = 0;
	for( count=0 ; count < 3 ; count++ ){
		memset(&res_mem,0,sizeof(res_mem));
		if(of_address_to_resource(np, count, res)) {
			printk(KERN_ERR "%s: Can't get registers address\n", np->full_name);
			rc = -ENODEV;
		}
		else{
#if 0
			switch(count){
				case 0:
					if((e25g->reg_base = phys_to_virt(res->start)) == NULL){
						printk(KERN_ERR "%s:%d ERROR: Base memory mapping failed I/O addr 0x%llx 0x%llu\n", __func__ , count,res->start, resource_size(res));
						rc = -ENOMEM;
					}
					
					if(e25g_probe_count <= 1){
						/* SFP TXEN */
						reg = e25g_read_reg(e25g->reg_base, E25G_SFPCNT);
						reg |= E25G_SFPCNT_TXEN;
						e25g_write_reg(e25g->reg_base, E25G_SFPCNT, reg);

						/* 自立リセットマスク */
						reg = e25g_read_reg(e25g->reg_base, E25G_RSTMASK);
						reg |= E25G_RSTMASK_MASK;
						e25g_write_reg(e25g->reg_base, E25G_RSTMASK, reg);

						/* Ether PHYリセット解除 */
						reg = e25g_read_reg(e25g->reg_base, E25G_MACSYSRST);
						reg &= ~E25G_MACSYSRST_RST;
						e25g_write_reg(e25g->reg_base, E25G_MACSYSRST, reg);
						
						msleep(10);
						
						/* MACアドレス設定 */
						/* (TBD)暫定で固定値を設定する */
						reg = (0x000000e0);
						e25g_write_reg(e25g->reg_base, E25G_FILTDA11, reg);
						reg = (0x00000001) + e25g_probe_count;
						e25g_write_reg(e25g->reg_base, E25G_FILTDA12, reg);
					}
					break;
				case 1:
					if(e25g_probe_count <= 1){
						if((e25g->txbuf_base_m = phys_to_virt(res->start)) == NULL){
							printk(KERN_ERR "%s:%d TX ERROR: memory mapping failed I/O addr 0x%llx 0x%llu\n", __func__ , count,res->start, resource_size(res));
							rc = -ENOMEM;
						}
					}else{
						if((e25g->txbuf_base_s = phys_to_virt(res->start)) == NULL){
							printk(KERN_ERR "%s:%d TX ERROR: memory mapping failed I/O addr 0x%llx 0x%llu\n", __func__ , count,res->start, resource_size(res));
							rc = -ENOMEM;
						}
					}
					break;
				case 2:
					if((e25g_probe_count <= 1)){
						if((e25g->rxbuf_base_m = phys_to_virt(res->start)) == NULL){
							printk(KERN_ERR "%s:%d RX ERROR: memory mapping failed I/O addr 0x%llx 0x%llu\n", __func__ , count,res->start, resource_size(res));
							rc = -ENOMEM;
						}
					}else{
						if((e25g->rxbuf_base_s = phys_to_virt(res->start)) == NULL){
							printk(KERN_ERR "%s:%d RX ERROR: memory mapping failed I/O addr 0x%llx 0x%llu\n", __func__ , count,res->start, resource_size(res));
							rc = -ENOMEM;
						}
					}
					break;
				default:
					break;
			}
#endif
		}
		if(rc != 0){
			goto out;
		}
	}

	rc = register_netdev(ndev);
	if (rc != 0) {
		printk(KERN_ERR "%s: register_netdev error %d\n", ndev->name ,rc);
		goto out;
	}
	platform_set_drvdata(pdev, ndev);
	ndev->irq = platform_get_irq(pdev, 0);
	if(ndev->irq <= 0){
		printk(KERN_ERR "%s: platform_get_irq error %d\n", ndev->name ,ndev->irq);
		rc = -EIO;
		goto out;
	}
	rc = devm_request_irq(&pdev->dev, ndev->irq, intr_func,IRQF_SHARED, ndev->name, ndev);
	if(rc < 0){
		printk(KERN_ERR "%s: devm_request_irq error %d\n", ndev->name ,rc);
		rc = -EIO;
		goto out;
	}

	e25g_probe_count++;
	return 0;
out:
	return rc;
}




static const struct file_operations fops_ptplogstat = {
        .read = ptplogstat_read,
        .write = ptplogstat_write
};


static int __init e25g_init(struct platform_device *pdev)
{
	int rc = 0;

	struct proc_dir_entry* entry;

	if(e25g_probe_count == 0){
		memset(ptplog_statistic_cnt,0,sizeof(ptplog_statistic_cnt));
		ptplog_entry_dir = proc_mkdir("ptp",NULL);
		entry = proc_create_data( "statistic" , 0666, ptplog_entry_dir ,&fops_ptplogstat, ptplog_statistic_cnt);
		if( !entry ){
			remove_proc_entry( "ptp", NULL );
			printk(KERN_ERR "ptplog create error \n");
			return -EBUSY;
		}
	}

//	rc = register_reboot_notifier(&e25g_notifier);
//	if (rc) {
//		pr_err("%s: failed to register reboot notifier (err=%d)\n",
//		       DRV_NAME, rc);
//		return rc;
//	}

//	rc = class_interface_register(&e25g_mport_interface);
//	if (rc) {
//		pr_err("%s: class_interface_register error: %d\n",
//		       DRV_NAME, rc);
//		return rc;
//	}

//	return subsys_interface_register(&e25g_interface);
	rc = e25g_add_dev(pdev);
	return rc;
}

static void __exit e25g_exit(void)
{
	if(ptplog_entry_dir != NULL){
		remove_proc_entry( "statistic"  , ptplog_entry_dir );
		remove_proc_entry( "ptp", NULL );
	}
	//unregister_reboot_notifier(&e25g_notifier);
	//subsys_interface_unregister(&e25g_interface);
	//class_interface_unregister(&e25g_mport_interface);
}

static const struct of_device_id fheth_dt_ids[] = {
	{ .compatible = "fujitsu.fheth.qemu" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, fheth_dt_ids);

static int fheth_probe(struct platform_device *pdev)
{
	return e25g_init(pdev);
}

static int fheth_remove(struct platform_device *pdev)
{
	e25g_exit();
	return 0;
}

static struct platform_driver fheth_driver = {
	.probe		= fheth_probe,
	.remove		= fheth_remove,
	.driver		= {
		.name		= "fheth",
		.of_match_table	= of_match_ptr(fheth_dt_ids),
	},
};

module_platform_driver(fheth_driver);
