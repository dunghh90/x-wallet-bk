#
# rrh_INIT �v���Z�X�p�e�X�g�V�i���I
#

Core.require("./verbose")

# �֐��P�̃e�X�g�J�n

INIT.module("ct_init_get_tbl_size")

puts "TEST01_f_ini_iniTbl"
if !INIT.module("f_ini_iniTbl") then
 puts "Error"
 exit(0)
end
puts "=>OK"


mode = Core.cmode_get()


puts "INIT Main Threads functions test Start"

#PF.module("f_main_iniTbl")
rtn = INIT.cunit("test_suites_array_ft_init",mode)
if ( rtn == false ) then
 puts "ERROR"
end

puts "INIT Main Threads functions test END"




