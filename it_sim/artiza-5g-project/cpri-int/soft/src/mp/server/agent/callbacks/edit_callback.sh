#!/bin/sh

# *** 説明 ***
#
# make_callback.sh で生成したソースコードのうち、
# 以下のファイルは原則的に変更せずにそのまま使用する。
#
#  y_*.[ch]
#  u_*.h
#
# しかし一部のファイルについては変更が必要となるので
# 本スクリプトによって変更する。
#
# *** 使い方 ***
#
#  edit_callback.sh 自動生成したソースコードのあるディレクトリ名
#
# *** 注意 ***
#
# 変更が成功した場合、変更前のファイルは元のディレクトリに *.bak として保存されます。
# 


#------ 共通部 ------

replace () {
    target=$1
    mv $target ${target}.bak
    mv ${target}.new $target
    diff ${target}.bak ${target}
}

warn () {
    target=$1
    result=$2
    echo "Failed($result): ${target}" 1>&2
}

#------ 個別部 ------

# u_o-ran-module-cap.h
edit_o_ran_module_cap () {
    target=${TOPDIR}/o-ran_all/src/u_o-ran-module-cap.h
    echo ""
    echo $target
    
    sed '/^#define u_o_ran_module_cap_F_LAA/{
        s#^#//#
    }' $target > ${target}.new
    
    result=$?
    if [ $result -eq 0 ]
    then
        replace $target
    else
        warn $target $result
    fi
}


# u_o-ran-interfaces.h
edit_o_ran_interfaces () {
    target=${TOPDIR}/o-ran_all/src/u_o-ran-interfaces.h
    echo ""
    echo $target
    
    sed '/^#include "u_/{
        s#^#//#
    }' $target > ${target}.new
    
    result=$?
    if [ $result -eq 0 ]
    then
        replace $target
    else
        warn $target $result
    fi
}


# y_o-ran-software-management.c
edit_o_ran_software_management() {
    target=${TOPDIR}/o-ran_all/src/y_o-ran-software-management.c
    echo ""
    echo $target

    sed '
    /static status_t o_ran_software_management_software_inventory_software_slot_files_get/, /^}/{
        /\/\* ancestor key o-ran-software-management:name \*\//, /^$/{
            s/getcb_find_key_str/getcb_find_key_num/
            s/y_o_ran_software_management_N_name);/y_o_ran_software_management_N_name, 1);/
        }
        /\/\* local key o-ran-software-management:name \*\//, /^$/{
            s/getcb_find_key_str/getcb_find_key_num/
            s/y_o_ran_software_management_N_name);/y_o_ran_software_management_N_name, 2);/
        }
    }' $target > ${target}.new

    result=$?
    if [ $result -eq 0 ]
    then
        replace $target
    else
        warn $target $result
    fi
}
#------ main ------

TOPDIR=$1

edit_o_ran_module_cap
edit_o_ran_interfaces
edit_o_ran_software_management
