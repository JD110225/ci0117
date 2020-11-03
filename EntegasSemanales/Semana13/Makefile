all:hello.out avg.out
hello.out: hello.c
	mpicc hello.c -o hello.out
avg.out: avg.c
	mpicc avg.c -o avg.out

#  mpirun -n 4 --oversubscribe ./vec.out