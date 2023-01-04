all: alllib
clean: allclean

alllib:
	#CMU
	cd ./cmu/;make
	#com
	cd ./com;make
	#dcm
	cd ./dcm;make
	#debug
	cd ./debug;make

allclean:
	#CMU
	cd ./cmu/;make clean
	#com
	cd ./com;make clean
	#dcm
	cd ./dcm;make clean
	#debug
	cd ./debug;make clean

doxygen:
	doxygen ./Doxyfile_cmn

install:
	mv ./cmu/*.a ./lib/
	mv ./com/*.a ./lib/
	mv ./dcm/*.a ./lib/
	mv ./debug/*.a ./lib/

