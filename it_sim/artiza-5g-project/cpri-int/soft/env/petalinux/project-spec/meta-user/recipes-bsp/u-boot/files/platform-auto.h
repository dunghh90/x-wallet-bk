/*
 * This file is auto-generated by PetaLinux SDK 
 * DO NOT MODIFY this file, the modification will not persist
 */

#ifndef __PLNX_CONFIG_H
#define __PLNX_CONFIG_H

/* The following table includes the supported baudrates */
#define CONFIG_SYS_BAUDRATE_TABLE  {9600, 19200, 38400, 57600, 115200, 230400}

/* use serial multi for all serial devices */
#define CONFIG_SERIAL_MULTI
#define CONFIG_SYS_CONSOLE_IS_IN_ENV 1

/* Board name */

/* processor - psu_cortexa53_0 */
#define CONFIG_CPU_ARMV8
#define CONFIG_CLOCKS
#define CONFIG_CMD_CLK
#define CONFIG_REMAKE_ELF
#define CONFIG_BOARD_EARLY_INIT_R
#define CONFIG_BOARD_LATE_INIT
#define CONFIG_CMD_BOOTI
#define CONFIG_CMD_UNZIP
#define CONFIG_SYS_CACHELINE_SIZE	64

/* main_memory - psu_ddr_0 */

/* uart - psu_uart_1 */
#define CONFIG_ZYNQ_SERIAL
#define PSSERIAL0	"psserial0=setenv stdout ttyPS0;setenv stdin ttyPS0\0"
#define SERIAL_MULTI	"serial=setenv stdout serial;setenv stdin serial\0"
#define CONSOLE_ARG	"console=console=ttyPS0,115200\0"
#define SERIAL_MULTI  "serial=setenv stdout serial;setenv stdin serial\0"
#define CONFIG_BAUDRATE	115200

/* ethernet - psu_ethernet_3 */
#define CONFIG_SYS_FAULT_ECHO_LINK_DOWN
#define CONFIG_PHY_TI
#define CONFIG_PHY_VITESSE
#define CONFIG_PHY_REALTEK
#define PHY_ANEG_TIMEOUT 20000
#define CONFIG_MII
#define CONFIG_PHY_GIGE
#define CONFIG_PHY_MARVELL
#define CONFIG_PHY_NATSEMI
#define CONFIG_NET_MULTI
#define CONFIG_NETCONSOLE	1
#define CONFIG_SERVERIP 192.168.1.100
#define CONFIG_IPADDR 192.168.1.99

/* spi_flash - psu_qspi_0 */
#define XILINX_PS8_QSPI_CLK_FREQ_HZ	200000000
#define CONFIG_SF_DEFAULT_SPEED   (XILINX_PS8_QSPI_CLK_FREQ_HZ / 4)
#define CONFIG_MTD_UBI_WL_THRESHOLD 4096
#define CONFIG_MTD_UBI_BEB_LIMIT 0
#define CONFIG_SPI_GENERIC
#define CONFIG_CMD_UBI
#define CONFIG_RBTREE
#define CONFIG_CMD_UBIFS
#define CONFIG_LZO
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_SPI_FLASH_MTD
#define CONFIG_SF_DUAL_FLASH
#define CONFIG_SYS_NO_FLASH

/* sdio - psu_sd_1 */
#define CONFIG_ZYNQ_SDHCI1
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_SUPPORT_VFAT
#define CONFIG_DOS_PARTITION
#define CONFIG_FAT_WRITE
#define CONFIG_SUPPORT_EMMC_BOOT
#define CONFIG_ZYNQ_SDHCI_MAX_FREQ  200000000

/* rtc - psu_rtc */

/* i2c - psu_i2c_1 */
#define CONFIG_ZYNQ_I2C1
#define CONFIG_SYS_I2C_ZYNQ
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_ZYNQ_SLAVE	0
#define CONFIG_SYS_I2C_ZYNQ_SPEED 100000

/* i2c - psu_i2c_0 */
#define CONFIG_ZYNQ_I2C0

/* usb - psu_usb_0 */
#define CONFIG_USB_CABLE_CHECK
#define CONFIG_CMD_THOR_DOWNLOAD
#define CONFIG_USB_FUNCTION_THOR
#define CONFIG_THOR_RESET_OFF
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS	2
#define CONFIG_SYS_DFU_DATA_BUF_SIZE 0x1800000
#define DFU_DEFAULT_POLL_TIMEOUT 300

/* PS */
#define COUNTER_FREQUENCY 4000000
#define CONFIG_FPGA_ZYNQMPPL

/* intc - psu_acpu_gic */
#define ACPU_GIC_BASEADDR	0xF9000000
#define CONFIG_GICV2	1
#define GICD_BASE	(ACPU_GIC_BASEADDR + 0x10000)
#define GICC_BASE (ACPU_GIC_BASEADDR + 0x20000)

/* FPGA */
#define CONFIG_FPGA
#define CONFIG_FPGA_XILINX

/* Memory testing handling */
#define CONFIG_SYS_MEMTEST_START	0x0
#define CONFIG_SYS_MEMTEST_END	(0x0 + 0x1000)
#define CONFIG_SYS_LOAD_ADDR	(0x0 + 0x100000) /* default load address */
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR - GENERATED_GBL_DATA_SIZE)
#define CONFIG_NR_DRAM_BANKS	2

/* Size of malloc() pool */
#define SIZE	0x2000000
#define CONFIG_SYS_MALLOC_LEN	SIZE

/* BOOTP options */
#define CONFIG_BOOTP_SERVERIP
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_MAY_FAIL

/*Command line configuration.*/
#define CONFIG_CMDLINE_EDITING
#define CONFIG_CMD_SAVES

/* Miscellaneous configurable options */
#define CONFIG_SYS_CBSIZE	2048/* Console I/O Buffer Size      */
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)

/* Boot Argument Buffer Size */
#define CONFIG_SYS_MAXARGS	32	/* max number of command args */
#define CONFIG_SYS_LONGHELP

/* architecture dependent code */
#define CONFIG_SYS_USR_EXCEP	/* user exception */
#define CONFIG_SYS_HZ	1000

/* Use the HUSH parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2 "> "

/* Don't define BOOTARGS, we get it from the DTB chosen fragment */
#undef CONFIG_BOOTARGS

#define CONFIG_ENV_OVERWRITE	/* Allow to overwrite the u-boot environment variables */

#define CONFIG_LMB

/* Initial memory map for Linux */
#define CONFIG_SYS_BOOTMAPSZ 0x8000000

/* Environment settings*/
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SPI_MAX_HZ	30000000
#define CONFIG_ENV_OFFSET	0x100000
#define CONFIG_ENV_SIZE	0x40000
#define CONFIG_ENV_SECT_SIZE	0x20000

/* SPI BUS */
#define CONFIG_ENV_SPI_BUS 2

/* PREBOOT */
#define CONFIG_PREBOOT	"echo U-BOOT for 5gdu-lls-2;setenv preboot"

/* Extra U-Boot Env settings */
#define CONFIG_EXTRA_ENV_SETTINGS \
	SERIAL_MULTI \ 
	CONSOLE_ARG \ 
	DFU_ALT_INFO \ 
	PSSERIAL0 \ 
	"nc=setenv stdout nc;setenv stdin nc;\0" \ 
	"ethact=ethernet@ff0e0000\0" \
	"ethaddr=00:0a:35:00:22:01\0" \
	"sdbootdev=0\0" \ 
	"bootenv=uEnv.txt\0" \ 
	"importbootenv=echo \"Importing environment from SD ...\"; " \ 
		"env import -t ${loadbootenv_addr} $filesize\0" \ 
	"loadbootenv=load mmc $sdbootdev:$partid ${loadbootenv_addr} ${bootenv}\0" \ 
	"sd_uEnvtxt_existence_test=test -e mmc $sdbootdev:$partid /uEnv.txt\0" \ 
	"uenvboot=" \ 
		"if run sd_uEnvtxt_existence_test; then " \ 
			"run loadbootenv; " \ 
			"echo Loaded environment from ${bootenv}; " \ 
			"run importbootenv; " \ 
			"fi; " \ 
		"if test -n $uenvcmd; then " \ 
			"echo Running uenvcmd ...; " \ 
			"run uenvcmd; " \ 
		"fi\0" \ 
	"autoload=no\0" \ 
	"clobstart=0x10000000\0" \ 
	"netstart=0x10000000\0" \ 
	"dtbnetstart=0x11800000\0" \ 
	"loadaddr=0x10000000\0" \ 
	"boot_img=BOOT.BIN\0" \ 
	"load_boot=tftpboot ${clobstart} ${boot_img}\0" \ 
	"update_boot=setenv img boot; setenv psize ${bootsize}; setenv installcmd \"install_boot\"; run load_boot ${installcmd}; setenv img; setenv psize; setenv installcmd\0" \ 
	"install_boot=mmcinfo && fatwrite mmc 0 ${clobstart} ${boot_img} ${filesize}\0" \ 
	"bootenvsize=0x40000\0" \ 
	"bootenvstart=0x100000\0" \ 
	"eraseenv=sf probe 0 && sf erase ${bootenvstart} ${bootenvsize}\0" \ 
	"jffs2_img=rootfs.jffs2\0" \ 
	"load_jffs2=tftpboot ${clobstart} ${jffs2_img}\0" \ 
	"update_jffs2=setenv img jffs2; setenv psize ${jffs2size}; setenv installcmd \"install_jffs2\"; run load_jffs2 test_img; setenv img; setenv psize; setenv installcmd\0" \ 
	"sd_update_jffs2=echo Updating jffs2 from SD; mmcinfo && fatload mmc 0:1 ${clobstart} ${jffs2_img} && run install_jffs2\0" \ 
	"install_jffs2=sf probe 0 && sf erase ${jffs2start} ${jffs2size} && " \ 
		"sf write ${clobstart} ${jffs2start} ${filesize}\0" \ 
	"kernel_img=image.ub\0" \ 
	"load_kernel=tftpboot ${clobstart} ${kernel_img}\0" \ 
	"update_kernel=setenv img kernel; setenv psize ${kernelsize}; setenv installcmd \"install_kernel\"; run load_kernel ${installcmd}; setenv img; setenv psize; setenv installcmd\0" \ 
	"install_kernel=mmcinfo && fatwrite mmc 0 ${clobstart} ${kernel_img} ${filesize}\0" \ 
	"eep_read=rrh -pserr 50\0" \
	" cp_kernel2ram=" \
		"if ${eep_read} ; then " \
			"sf probe 2:0 && sf read ${netstart} 04c00000 01400000;" \
			" else " \
			"sf probe 2:0 && sf read ${netstart} 0cc00000 01400000;" \
		"fi\0" \
	"dtb_img=system.dtb\0" \ 
	"load_dtb=tftpboot ${clobstart} ${dtb_img}\0" \ 
	"update_dtb=setenv img dtb; setenv psize ${dtbsize}; setenv installcmd \"install_dtb\"; run load_dtb test_img; setenv img; setenv psize; setenv installcmd\0" \ 
	"sd_update_dtb=echo Updating dtb from SD; mmcinfo && fatload mmc 0:1 ${clobstart} ${dtb_img} && run install_dtb\0" \ 
	"loadbootenv_addr=0x00100000\0" \ 
	"fault=echo ${img} image size is greater than allocated place - partition ${img} is NOT UPDATED\0" \ 
	"test_crc=if imi ${clobstart}; then run test_img; else echo ${img} Bad CRC - ${img} is NOT UPDATED; fi\0" \ 
	"test_img=setenv var \"if test ${filesize} -gt ${psize}\\; then run fault\\; else run ${installcmd}\\; fi\"; run var; setenv var\0" \ 
	"netboot=tftpboot ${netstart} ${kernel_img} && bootm\0" \ 
	"preboot=" \
		"if ${eep_read} ; then " \
			"sf probe 2:0 && sf read 2000000 00400000 2400000 && rrh -cw 28 ff && fpga loadb 0 2000000 2400000;" \ 
			" else " \
			"sf probe 2:0 && sf read 2000000 08400000 2400000 && rrh -cw 28 ff && fpga loadb 0 2000000 2400000;" \ 
		"fi\0" \
	"default_bootcmd=run cp_kernel2ram && bootm ${netstart}\0" \ 
""

/* BOOTCOMMAND */
#define CONFIG_BOOTCOMMAND	"run default_bootcmd"

#endif /* __PLNX_CONFIG_H */
