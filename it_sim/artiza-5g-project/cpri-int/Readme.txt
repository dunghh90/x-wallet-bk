
Before start building the source code of middle device, please make sure following points:

1. Please delete the older folder (cpri-int/soft/src), then copy the new folder (cpri-int/soft/src) that we send you	
	DO NOT replace (or overwrite) because below reasons:
		We changed the folder tree in soft/src directory, in particular:
		 - Remove following processes:
		   + ft_cpr, ft_eth, ft_fmg, ft_frw, ft_log, ft_rsv from src/pf
			   because they exists in src/pf/pf_fhm

		 - Change locations of following files:
		   + src/cmn/com/src/makefile/Makefile is moved to src/cmn/com/src/Makefile
		   + src/pf/ft_com/com/makefile/Makefile is moved to src/pf/ft_com/com/Makefile 
		   + src/pf/pf_fhm/ft_com/com/makefile/Makefile is moved to src/pf/pf_fhm/ft_com/com/Makefile 

2. chmod +x ./tools/socat/make.sh
3. chmod +x ./tools/linuxptp/make.sh
4. chmod +x  ./gen_re_help.sh
5. Copy https://csvn.cloud9-solutions.com/svn/s107.02/deliverables/trunk/03.implementation/Artiza_MiddleDevice_Base_Source/cpri-int/soft/ramdisk/target/lib/libmruby.a 
		to /home/user/develop/cpri-int/soft/ramdisk/target/lib/ 
		
	And final, we expecte that source code will be build without error and output will available at /home/user/develop/cpri-int/soft/ramdisk/target/opt/proc/


LIMITATIONS:
The source code currently has following limitations which will be fixed during the MK phase:
1. Many warnings (unused parameters, type format truncation, type casting, uninitialized variables, implicit function declaration, etc.) during compilation
2. Many macros (related to thread id and queue id) do not match the document "s107.02\to_VN\design\CPRI中間装置ソフトウェア構成検討資料.xlsx"