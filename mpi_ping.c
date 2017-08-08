#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int proc_rank, proc_count;
	int number;
	
	// в прототипе второй арг - ***char??
	MPI_Init(&argc, &argv);
	
	// узнаем количество процессов 
	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
	// узнаём номер процесса
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

	
	if (proc_rank == 0) 
	{
		number = -1;
		//ping
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
		printf("Worker is alive\n");
		// int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
        //      int tag, MPI_Comm comm, MPI_Request *request)
		// MPI_Test - check, if Irecv has complete
        /* int MPI_Test(
  MPI_Request *request,
  int *flag,
  MPI_Status *status*/
       //sleep() в #include <unistd.h>
);
	} 
	else if (proc_rank == 1) 
	{
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
		printf("Leader is alive\n");
		//printf("Process 1 received number %d from process 0\n",
		//	   number);
		MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	
	return 0;
	
}


