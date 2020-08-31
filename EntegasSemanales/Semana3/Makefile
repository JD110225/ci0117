all:sumaUnoB.exe PiPorSeriesB.exe PiPorSeriesSHM.exe sumaUnoSHM.exe
sumaUnoB.exe: sumaUnoBuzon.cpp Semaforo.cpp Buzon.cpp 
	g++ sumaUnoBuzon.cpp Semaforo.cpp Buzon.cpp -o sumaUnoB.exe
PiPorSeriesB.exe: PiPorSeriesBuzon.cpp Semaforo.cpp Buzon.cpp
	g++ PiPorSeriesBuzon.cpp Buzon.cpp Semaforo.cpp -o PiPorSeriesB.exe
PiPorSeriesSHM.exe: PiPorSeriesSHM.cpp Semaforo.cpp 
	g++ PiPorSeriesSHM.cpp Semaforo.cpp -o PiPorSeriesSHM.exe
sumaUnoSHM.exe: sumaUnoSHM.cpp Semaforo.cpp 
	g++ sumaUnoSHM.cpp Semaforo.cpp -o sumaUnoSHM.exe
	