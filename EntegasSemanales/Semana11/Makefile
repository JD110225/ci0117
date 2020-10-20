all:filosofos.exe papas.exe
filosofos.exe: filosofos.cc Lock.cc Mutex.cc  Mesa.cc Condition.cc
	g++ filosofos.cc Lock.cc Mutex.cc  Mesa.cc  Condition.cc -o filosofos.exe -fopenmp
papas.exe: papa.cc countingSem.cpp 
	g++ papa.cc countingSem.cpp -fopenmp -o papas.exe
