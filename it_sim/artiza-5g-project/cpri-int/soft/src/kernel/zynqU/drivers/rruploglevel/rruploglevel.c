/*
 *  ALL Rights Reserved, Copyright (c) FUJITSU Limited 2014-2015.
 *   Toru Taniguchi <toru_taniguchi@jp.fujitsu.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/list.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/version.h>
#include <linux/ip.h>
#include <asm/smp.h>
#include <asm/uaccess.h>

#define PROCNAME_DIR  "rru"
#define MAXBUF	10

typedef enum {
	E_PLOG_NAME_MAX = 32
}e_plog_name;

typedef struct
{
	int          level;
	unsigned int counter;
}t_ploglevel_info;

static t_ploglevel_info    toplevel;
static struct proc_dir_entry* plog_entry_dir = NULL;

static char*    name[E_PLOG_NAME_MAX];
static int      namenum = 0;
module_param_array(name, charp, &namenum , S_IRUGO);
MODULE_PARM_DESC(name, "rru plog level name");

static t_ploglevel_info    level[E_PLOG_NAME_MAX];

#define PLOG_INFO_LEVEL(info)		(((t_ploglevel_info*)info)->level)
#define PLOG_INFO_COUNTER(info)		(((t_ploglevel_info*)info)->counter)

/*!
 * @brief		rru plog level write module
 * @note		rru plog level write module
 * @param		N/A
 * @return		N/A
 * @date		2014/05/21 FJT)Taniguchi Create
 * @date		2015/02/21 FJT)Taniguchi modify for kernel version up
 * @warning		this module is called when echo x > procfile.
 */
static ssize_t rruploglevel_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *ppos)
{
	char wbuf[MAXBUF];
	unsigned int lvl = 0;
	char *data;

	if(( count > MAXBUF )||(count <= 0)){
		printk(KERN_ERR "level write error1 \n");
		return -ENOSPC;
	}
	memset(wbuf, 0, sizeof(wbuf));
	if( copy_from_user( wbuf, user_buf, count ) ){
		printk(KERN_ERR "level write error2 \n");
		return -EFAULT;
	}
	if( wbuf[0] == '0'){
		lvl = 0;
	}
	else
	{
		if(( lvl = simple_strtoul(wbuf,NULL,0) ) == 0){
			printk(KERN_ERR "level write error3 \n");
			return -EFAULT;
		}
	}
	data = PDE_DATA(file_inode(filp));
	memcpy(&(PLOG_INFO_LEVEL(data)),&lvl,sizeof(lvl));
	return count;
}

/*!
 * @brief		rru plog level read module
 * @note		rru plog level read module
 * @param		N/A
 * @return		N/A
 * @date		2014/05/21 FJT)Taniguchi Create
 * @date		2015/02/21 FJT)Taniguchi modify for kernel version up
 * @warning		this module is called when cat procfile.
 */
static ssize_t rruploglevel_read(struct file *filp, char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned long outbyte = 0;
	char wbuf[32];
	char *data;

	/* また読み出しきたら、0で返す */
	if( *ppos > 0 ){
		return 0;
	}
	data = PDE_DATA(file_inode(filp));
	PLOG_INFO_COUNTER(data)++;
	/* topが設定されている場合、topを常に表示する */
	if(toplevel.level != 0){
		data = (char*)&toplevel;
	}
	outbyte = sprintf( wbuf, "%d\n", PLOG_INFO_LEVEL(data) );
	return simple_read_from_buffer(user_buf, count, ppos, wbuf, outbyte);
}

/*!
 * @brief		rru plog stat write module
 * @note		rru plog stat write module
 * @param		N/A
 * @return		N/A
 * @date		2015/04/23 FJT)Taniguchi Create
 * @warning		this module is called when echo x > procfile.
 */
static ssize_t rruplogstat_write(struct file *filp, const char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned int ncount;

	for(ncount=0 ; ncount < namenum ; ncount++){
		level[ncount].level = 0;
		level[ncount].counter = 0;
	}
	return count;
}

/*!
 * @brief		rru plog stat read module
 * @note		rru plog stat read module
 * @param		N/A
 * @return		N/A
 * @date		2015/04/23 FJT)Taniguchi Create
 * @warning		this module is called when cat procfile.
 */
static ssize_t rruplogstat_read(struct file *filp, char __user *user_buf, size_t count, loff_t *ppos)
{
	unsigned long outbyte = 0;
	char wbuf[2048];
	unsigned int ncount;

	if(*ppos > 0){
		return 0;
	}
	memset(wbuf,0,sizeof(wbuf));
	outbyte = sprintf( wbuf, "Current Log level and Statistic information \n");
	for(ncount = 0; ncount < namenum ; ncount++){
		outbyte += sprintf( &wbuf[outbyte], "%16s cur lvl(%d) %dcnt\n", name[ncount],level[ncount].level,level[ncount].counter);
		if(outbyte >= sizeof(wbuf)){
			break;
		}
	}
	*ppos = 0;
	return simple_read_from_buffer(user_buf, count, ppos, wbuf, outbyte);
}

/* file_operations struct */
static const struct file_operations fops_rruploglevel = {
        .read = rruploglevel_read,
        .write = rruploglevel_write
};

/* file_operations struct for stat */
static const struct file_operations fops_rruplogstat = {
        .read = rruplogstat_read,
        .write = rruplogstat_write
};

/*!
 * @brief		rru plog level init module
 * @note		rru plog level init module
 * @param		N/A
 * @return		N/A
 * @date		2014/05/21 FJT)Taniguchi Create
 * @date		2015/02/21 FJT)Taniguchi modify for kernel version up 
 * @warning		this module is called when insmod.
 */
static int __init rruploglevel_init( void )
{
	struct proc_dir_entry* entry;	
	unsigned int count;

	plog_entry_dir = proc_mkdir(PROCNAME_DIR,NULL);
	memset(&toplevel,0,sizeof(toplevel));
	memset(level,0,sizeof(level));
	entry = proc_create_data( "top" , 0666, plog_entry_dir ,&fops_rruploglevel, &toplevel);
	if( !entry ){
		remove_proc_entry( PROCNAME_DIR, NULL );
		printk(KERN_ERR "top log level create error \n");
		return -EBUSY;
	}
	entry = proc_create_data( "statistic" , 0666, plog_entry_dir ,&fops_rruplogstat, level);
	if( !entry ){
		printk(KERN_ERR "statistic create error \n");
		remove_proc_entry( "top" , plog_entry_dir );
		remove_proc_entry( PROCNAME_DIR, NULL );
		return -EBUSY;
	}
	for(count = 0 ; (count < namenum) && (count < (sizeof(name)/sizeof(name[0]))) ; count++)
	{
		entry = proc_create_data( name[count] , 0666, plog_entry_dir ,&fops_rruploglevel, &level[count]);
		if( !entry ){
			printk(KERN_ERR "%s log level create error \n",name[count]);
			for(count=0 ; count < namenum ; count++)
			{
				remove_proc_entry( name[count] , plog_entry_dir );
			}
			remove_proc_entry( "top" , plog_entry_dir );
			remove_proc_entry( PROCNAME_DIR, NULL );
			return -EBUSY;
		}
	}
	return 0;
}

/*!
 * @brief		rru plog level exit module
 * @note		rru plog level exit module
 * @param		N/A
 * @return		N/A
 * @date		2014/05/21 FJT)Taniguchi Create
 * @warning		this module is called when rmmod.
 */
static void __exit rruploglevel_exit( void )
{
	unsigned int count;
	
	remove_proc_entry( "top"  , plog_entry_dir );
	remove_proc_entry( "statistic"  , plog_entry_dir );
	for(count=0 ; count < namenum ; count++)
	{
		remove_proc_entry( name[count] , plog_entry_dir );
	}
	remove_proc_entry( PROCNAME_DIR, NULL );
	return;
}

module_init( rruploglevel_init );
module_exit( rruploglevel_exit );

MODULE_DESCRIPTION("rru plog level module");
MODULE_AUTHOR("Toru.Taniguchi");
MODULE_LICENSE("GPL");
MODULE_VERSION("2.0.2");
