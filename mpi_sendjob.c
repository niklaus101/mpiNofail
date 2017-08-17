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

int failCond(int proc_num)
{
	if (proc_num == 2)
		return 1;
	return 0;
}

int main(int argc, char *argv[])
{
	double t;

	int proc_rank, proc_count;

	int *job_ids, *job_res;
	char *count_res;

	int i;
        int job_num, loc_res; 
	
	int *flag;
	char all_job_done;
	
	MPI_Request *request, *request1;
    
	MPI_Status status;
  
	MPI_Init(&argc, &argv);
	
	// узнаем количество процессов 
	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
	// узнаём номер процесса
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	
	request = (MPI_Request *)malloc(proc_count * sizeof(MPI_Request));
	request1 = (MPI_Request *)malloc(proc_count * sizeof(MPI_Request));
	flag = (int *)calloc(proc_count, sizeof(int));
	
	job_ids = (int *)malloc(proc_count*sizeof(int));
	for(i=0; i<proc_count; i++)
		job_ids[i] = i;
	job_res = (int *)malloc(proc_count*sizeof(int));
	count_res = (char *)malloc(proc_count*sizeof(char));

	if (proc_rank == 0) 
	{
		//ping
		for(i=1; i<proc_count; i++)
			MPI_Irecv(&job_res[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request[i]);
		for(i=1; i<proc_count; i++)
			MPI_Isend(&job_ids[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request1[i]);
	
		
		//sleep(0.1); - too small
		//sleep(0.5); - too small
		sleep(1);
		
		for(i=1; i<proc_count; i++)
			MPI_Test(&request[i], &flag[i], &status);

		for(i=1; i<proc_count; i++)
			if(flag[i])
			{
				printf("Worker %i is alive\n", i);
				count_res[i] = 1;
			}
			else
				printf("Worker %i has stopped\n", i);
	
		printf("Job results\n");
		for(i=1; i<proc_count; i++)
			printf("%i ", job_res[i]);
		printf("\n");
		
		all_job_done = 1;
		for(i = 1; i<proc_count; i++)
			if(count_res[i] == 0)
			{ 
				all_job_done = 0;
				break; 
			}

		if(all_job_done)
			printf("All jobs done");
		else
			printf("Not all jobs done");
	} 
	else if(!failCond(proc_rank))
	{

		t = MPI_Wtime();
	
		MPI_Recv(&job_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
				 MPI_STATUS_IGNORE);
		printf("Proc %i: leader is alive\n", proc_rank);
		
		loc_res = doJob(job_num);
		
		MPI_Send(&loc_res, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	
		t = MPI_Wtime()-t;	
		printf("Proc %i: elapsed time is %f\n", proc_rank, t);
	}
	
	MPI_Finalize();
	
	return 0;
	
}


