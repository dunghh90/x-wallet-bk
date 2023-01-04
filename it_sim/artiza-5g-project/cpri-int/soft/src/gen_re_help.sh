#!/bin/sh

echo "auto-generate from \"../ramdisk/target/opt/sh/setaliases\" to \"re_help and re_ lnk\"..."

setlnk='setlnk'
name='re_help'

rm -rf ../ramdisk/target/opt/lnk/re_*
rm -rf ../ramdisk/target/opt/$name

echo -n "re_help scrpts ... "

cd ../ramdisk/target/opt/sh
echo "#!/bin/sh" > $name
echo "" >> $name
echo "# " >> $name
echo -n "# This script was auto-generated " >> $name
date +"%Y/%m/%d %H:%M:%S" >> $name
echo "# " >> $name
echo "" >> $name
echo ". /opt/sh/setaliases" >> $name
echo "" >> $name
echo "case \$1 in" >> $name
cat ./setaliases | grep "alias re_" | sed -e "s/alias//g" | sed -e "s/\=[^=]*$//g" > ./tmp1
cat ./setaliases | grep "alias re_" | sed -e "s/alias/ shift;/g" | sed -e "s/\=[^=]*$/ \$*;exit 0;;/g" > ./tmp2
paste -d")" ./tmp1 ./tmp2 >> $name
rm -rf ./tmp1 ./tmp2
echo "esac" >> $name
echo "echo \"
----------------------------------------------------------------------
  RE utility commands list (v.1.0.t.t)
 ----------------------------------------------------------------------
  COMMANDS NAME      : Description                      :usage
 ----------------------------------------------------------------------
  re_ver             : Soft,FPGA,and more program ver   :
  re_ps              : Threads list                     :
  re_top             : Threads top                      :
  re_help            : help command                     :
  re_irq             : Interrupts statistics            :
  re_log             : RE logs get                      :
  re_if              : Network interface info           :
  re_ipc_msg         : IPC message queue info           :
  re_ipc_shm         : IPC shared memory info           :
  re_ipc_sem         : IPC semaphore info               :
  re_mem             : Memory info                      :
  re_ftpserver       : FTP servert start                :
  re_reset           : not save log reset               :
  re_self            : Who am I                         :
  re_time            : current time info                :
  re_mon_xx          : Online log monitor commands      :
  re_his_xx          : Offline history log commands     :
  re_stat            : Statistics info commands         :
  RE device control commands                            :
    re_mw            :  memory write (32bit)            :re_mw <write addr> <write val>
    re_md            :  memory dump  (32bit)            :re_md <dump addr> <dump size>
    re_eepw          :  eeprom write                    :re_eepw <write addr> <write val>
    re_eepd          :  eeprom dump                     :re_eepd <dump addr> <dump size>
    re_eepda         :  eeprom dump all                 :re_eepda
    re_flshd         :  qspi flash current dump         :re_flshd <dump ofs> <dump size>
 --------------------------------------------------------------------------------------------------------
\"" >> $name
chmod 777 $name
#cat re_help
echo "finish"

echo -n "re lnk set ..."
cd ../lnk
echo "#!/bin/sh" > $setlnk
cat ../sh/setaliases | grep "alias re_" | sed -e "s/alias/ln -s ..\/sh\/re_help/g" | sed -e "s/\=[^=]*$//g" >> $setlnk
chmod 777 $setlnk
./$setlnk
rm -rf $setlnk
#pwd
#ls -l
echo "finish"
