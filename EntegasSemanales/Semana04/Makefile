all: makeWaterBuzon.exe makeWaterSHM.exe filosofosSemaforo.exe filosofosProfe.exe filosofosBuzon.exe
makeWaterBuzon.exe: MakeWaterBuzon.cpp  BuzonAgua.cpp Semaforo.cpp
	g++ MakeWaterBuzon.cpp  BuzonAgua.cpp Semaforo.cpp -o makeWaterBuzon.exe
makeWaterSHM.exe: MakeWaterSHM.cpp Semaforo.cpp
	g++ MakeWaterSHM.cpp Semaforo.cpp -o MakeWaterSHM.exe
filosofosProfe.exe: Filosofos.cpp Mutex.cpp SemaforoArray.cpp 
	g++ Filosofos.cpp Mutex.cpp SemaforoArray.cpp -o filosofosProfe.exe -lcurses
filosofosSemaforo.exe: filos.cpp Semaforo.cpp
	g++ filos.cpp Semaforo.cpp -o filosofosSemaforo.exe
filosofosBuzon.exe: filosofosBuzon.cpp BuzonAgua.cpp Semaforo.cpp 
	g++ filosofosBuzon.cpp BuzonAgua.cpp Semaforo.cpp -o filosofosBuzon.exe
