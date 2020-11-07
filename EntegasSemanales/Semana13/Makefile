all:papa.out matrices.out
matrices.out: matrices.c
	mpicc matrices.c -o matrices.out
papa.out: papa.c
	mpicc papa.c -o papa.out
#  mpirun -n 4 --oversubscribe ./vec.out