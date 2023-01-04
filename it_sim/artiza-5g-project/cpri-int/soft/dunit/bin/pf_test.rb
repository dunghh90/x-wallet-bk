#
# rrh_pf プロセス用テストシナリオ
#
Core.require("./verbose")
mode = Core.cmode_get()


puts "PF Main Threads functions test Start"

Core.i2c0write("00","40","13")
Core.i2c0write("00","41","11")

PF.module("f_main_iniTbl")
rtn = PF.cunit("test_suites_array_ft_main",mode)
if ( rtn == false ) then
 puts "ERROR"
end

puts "PF Main Threads functions test END"


