#include <unistd.h> //sleep()
#include <stdio.h>
#include <stdlib.h> //malloc

#include "mpi.h"

int doJob(int job_num)
{
	int i, t;
	/*for(i=0; i<100000; i++)
		t=10;*/
	return 2*job_num+1;
}

int main(int argc, char *argv[])
{
	int proc_rank, proc_count;
	int *job_ids, *job_res;
	int i;
    int job_num, loc_res; 
	
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
	
	job_ids = (int *)malloc(proc_count*sizeof(int));
	for(i=0; i<proc_count; i++)
		job_ids[i] = i;
	job_res = (int *)malloc(proc_count*sizeof(int));
	
	if (proc_rank == 0) 
	{
		//ping
		for(i=1; i<proc_count; i++)
			MPI_Irecv(&job_res[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]);
		for(i=1; i<proc_count; i++)
			MPI_Send(&job_ids[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		
		//sleep(0.1); - too small
		sleep(1);
		
		for(i=1; i<proc_count; i++)
			MPI_Test(&request[i], &flag[i], &status);

		for(i=1; i<proc_count; i++)
			if(flag[i])
				printf("Worker %i is alive\n", i);
			else
				printf("Worker %i has stopped\n", i);
	
		printf("Job results\n");
		for(i=1; i<proc_count; i++)
			printf("%i ", job_res[i]);
		printf("\n");
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
		MPI_Recv(&job_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
		printf("Proc %i: leader is alive\n", proc_rank);
		//printf("Process 1 received number %d from process 0\n",
		//	   number);
		
		loc_res = doJob(job_num);
		
		//if(proc_rank!=2) 
			MPI_Send(&loc_res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	
	MPI_Finalize();
	
	return 0;
	
}


