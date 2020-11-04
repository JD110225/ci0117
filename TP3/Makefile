
all: clusters.exe
clusters.exe: Punto.cc VectorPuntos.cc medios.cc
	g++ -g -lm medios.cc VectorPuntos.cc Punto.cc -fopenmp -o clusters.exe 
