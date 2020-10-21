all:filosofos.exe papas.exe semTest.exe
filosofos.exe: filosofos.cc Lock.cc Mutex.cc  Mesa.cc Condition.cc
	g++ filosofos.cc Lock.cc Mutex.cc  Mesa.cc  Condition.cc -o filosofos.exe -fopenmp
papas.exe: papa.cc countingSem.cpp 
	g++ papa.cc countingSem.cpp -fopenmp -o papas.exe
semTest.exe:omp_sem_test.cc countingSem.cpp
	g++ omp_sem_test.cc countingSem.cpp -fopenmp -o semTest.exe