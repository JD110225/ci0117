all:filosofos.exe papa.exe semTest.exe
filosofos.exe: filosofos.cc LockOMP.cpp   Mesa.cc Condition.cc
	g++ filosofos.cc LockOMP.cpp  Mesa.cc  Condition.cc -o filosofos.exe -fopenmp
papa.exe: papa.cc countingSem.cpp 
	g++ papa.cc countingSem.cpp -fopenmp -o papa.exe
semTest.exe:omp_sem_test.cc countingSem.cpp
	g++ omp_sem_test.cc countingSem.cpp -fopenmp -o semTest.exe