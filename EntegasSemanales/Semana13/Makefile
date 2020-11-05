all:hello.out avg.out papa
hello.out: hello.c
	mpicc hello.c -o hello.out
avg.out: avg.c
	mpicc avg.c -o avg.out
papa: papa.c
	mpicc papa.c -o papa
#  mpirun -n 4 --oversubscribe ./vec.out