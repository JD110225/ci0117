all: pruebaPuntos.out

%.o: %.cc
	g++ -g -c $<

%.o: %.c
	gcc -g -c $<
#pruebaPuntos.out: Punto.cc VectorPuntos.cc medios.cc
	#g++ -g -lm  VectorPuntos.cc Punto.cc -o pruebaPuntos.out
pruebaPuntos.out: Punto.o VectorPuntos.o medios.o
	g++ -g -lm medios.o VectorPuntos.o Punto.o -o pruebaPuntos.out
#pruebaPuntos.out: Punto.o VectorPuntos.o pruebaPuntos.o
	#g++ -g -lm pruebaPuntos.o VectorPuntos.o Punto.o -o pruebaPuntos.out

clean:
	rm -f *.o *.out

