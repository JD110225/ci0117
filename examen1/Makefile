# Makefile para primer examen parcial
# CI-0117 Programacion concurrente
# Grupo 2
#

all: piratas.out estudiantes.out

%.o: %.cc
	g++ -g -c $<

%.o: %.c
	gcc -g -c $<

piratas.out: piratas.o collar.o
	g++ -g piratas.o collar.o -o piratas.out -lpthread
estudiantes.out: estudiantes.cc Condition.cc Lock.cc Monitor.cpp Semaforo.cc
	g++ estudiantes.cc Condition.cc Lock.cc Monitor.cpp Semaforo.cc -o estudiantes.out
#estudiantes.out: estudiantes.o
	#gcc -g estudiantes.o -o estudiantes.out

clean:
	rm -f *.o *.out

