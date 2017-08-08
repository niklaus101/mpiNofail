#include <unistd.h> //sleep()
#include <stdio.h>
#include <stdlib.h> //malloc

#include "mpi.h"


int main(int argc, char *argv[])
{
	int proc_rank, proc_count;
	int number;
	int i;

	int *flag;
	MPI_Request *request;
    
	MPI_Status status;
  
	MPI_Init(&argc, &argv);
	
	// узнаем количество процессов 
	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
	// узнаём номер процесса
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	
	request = (MPI_Request *)malloc(proc_count * sizeof(MPI_Request));
	flag = (int *)calloc(proc_count, sizeof(int));

	if (proc_rank == 0) 
	{
		number = -1;
		//ping
		for(i=1; i<proc_count; i++)
			MPI_Irecv(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]);
		for(i=1; i<proc_count; i++)
			MPI_Send(&number, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		
		//sleep(0.1); - too small
		sleep(1);
		
		for(i=1; i<proc_count; i++)
			MPI_Test(&request[i], &flag[i], &status);

		for(i=1; i<proc_count; i++)
			if(flag[i])
				printf("Worker %i is alive\n", i);
			else
				printf("Worker %i has stopped\n", i);
		/*MPI_Recv(&number, 1, MPI_INT, 1, 0, MPI_COMM	_WORLD,
				 MPI_STATUS_IGNORE);*/
		// int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source,
        //      int tag, MPI_Comm comm, MPI_Request *request)
		// MPI_Test - check, if Irecv has complete
        /* int MPI_Test(MPI_Request *request, int *flag,
						MPI_Status *status);*/
	} 
	else
	{
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
		printf("Proc %i: leader is alive\n", proc_rank);
		//printf("Process 1 received number %d from process 0\n",
		//	   number);
		if(proc_rank!=2) 
			MPI_Send(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	
	return 0;
	
}


