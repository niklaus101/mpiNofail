#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
	int proc_rank, proc_count;
	// � ��������� ������ ��� - ***char??
	MPI_Init(&argc, &argv);
	
	// ������ ���������� ��������� 
	MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
	// ����� ����� ��������
	MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
	
	MPI_Finalize();
	
	return 0;
	
}