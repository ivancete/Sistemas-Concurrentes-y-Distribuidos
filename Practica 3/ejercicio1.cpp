#include "mpi.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv []){

	MPI_Init(&argc, &argv);

	int rank, size, mandar_recibir;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status estado;

	if(rank == 0){
		cout << "El proceso 0 recibe una señal" << endl;

		MPI_Recv(&mandar_recibir, 1, MPI_INT, (size-1), 0, MPI_COMM_WORLD, &estado);
		MPI_Send(&mandar_recibir, 1, MPI_INT, (rank + 1), 0, MPI_COMM_WORLD);

		cout << "El proceso 0 manda una señal" << endl;
	}
	else{

		cout << "El proceso " << rank << " manda una señal" << endl;

		MPI_Send(&mandar_recibir, 1, MPI_INT, (size + rank + 1)%size, 0, MPI_COMM_WORLD);
		MPI_Recv(&mandar_recibir, 1, MPI_INT, (size + rank - 1)%size, 0, MPI_COMM_WORLD, &estado);

		cout << "El proceso " << rank << " recibe una señal" << endl;

	}

	MPI_Finalize();
}