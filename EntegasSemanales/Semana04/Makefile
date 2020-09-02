all: makeWaterBuzon.exe makeWaterSHM.exe filosofosPropio.exe filosofosProfe.exe
makeWaterBuzon.exe: MakeWaterBuzon.cpp  BuzonAgua.cpp Semaforo.cpp
	g++ MakeWaterBuzon.cpp  BuzonAgua.cpp Semaforo.cpp -o makeWaterBuzon.exe
makeWaterSHM.exe: MakeWaterSHM.cpp Semaforo.cpp
	g++ MakeWaterSHM.cpp Semaforo.cpp -o MakeWaterSHM.exe
filosofosProfe.exe: Filosofos.cpp Mutex.cpp SemaforoArray.cpp 
	g++ Filosofos.cpp Mutex.cpp SemaforoArray.cpp -o filosofosProfe.exe -lcurses
filosofosPropio.exe: filos.cpp Semaforo.cpp
	g++ filos.cpp Semaforo.cpp -o filosofosPropio.exe