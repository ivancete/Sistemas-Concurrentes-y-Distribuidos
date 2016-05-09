#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

void Cliente( int id);
void Horno ( int id);
void Pizzero( int id );


MPI_Status status;
const int NUMERO = 5;

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=11)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 11" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }

   if(rank == 0)
   	Horno(rank);

   if(rank == 1)
   	Pizzero(rank);

   Cliente(rank); 
   
   MPI_Finalize( );
   return 0;
}
// ---------------------------------------------------------------------
void Cliente( int id )
{
	int npizzas;
	MPI_Status status;


	while(1)
	{
		srand(time(NULL));
		npizzas = 1 +rand() % NUMERO;

		cout <<"Cliente "<<id<< " quiere " << npizzas << " pizzas" << endl << flush;
	   	MPI_Ssend(&npizzas, 1, MPI_INT, 1, 0,MPI_COMM_WORLD);

	   	MPI_Recv(&npizzas,1,MPI_INT, 1,1, MPI_COMM_WORLD, &status);
	   	cout <<"Cliente "<<id<< " coge " << npizzas << " pizzas y las come" << endl << flush;

	   	usleep( 1000U * (100U+(rand()%900U)) );
	}
}
// ---------------------------------------------------------------------
void Pizzero(int id)
{
	int numpedidas ;
	int pizzas_disponibles = 0;
	int pizzas_generadas ;
	int cliente;
	MPI_Status estado;

	while(1)
	{

		MPI_Recv(&numpedidas,1,MPI_INT, MPI_ANY_SOURCE,0, MPI_COMM_WORLD, &estado);
		cout <<"Pizzero necesita " << numpedidas << " pizzas" << endl << flush;

		cliente = estado.MPI_SOURCE;

		if(numpedidas > pizzas_disponibles)
			cout <<"Pizzero pide pizzas al horno " << endl << flush;

		while(numpedidas > pizzas_disponibles)
		{
	   		MPI_Ssend(&pizzas_generadas, 1, MPI_INT, 0, 2,MPI_COMM_WORLD);

	   		MPI_Recv(&numpedidas,1,MPI_INT, 0, 3 , MPI_COMM_WORLD, &estado);

	   		pizzas_disponibles+= pizzas_generadas;
		}

		cout <<"Pizzero "<<id<< " envia las pizzas al cliente " << cliente << endl << flush;
		MPI_Ssend(&numpedidas, 1, MPI_INT, cliente, 1,MPI_COMM_WORLD);
		pizzas_disponibles -= numpedidas;	        
	}
}
//----------------------------------------------------------------------

void Horno(int id)
{
  int basura;
  MPI_Status status;
  int GENERAR = 10;
  
  while(1)
  {
  	MPI_Recv(&basura,1,MPI_INT, 1 , 2, MPI_COMM_WORLD, &status);

  	cout <<"El horno genera 10 pizzas " << endl << flush;

  	MPI_Ssend(&GENERAR, 1, MPI_INT, 1 , 3 ,MPI_COMM_WORLD);
  }
}
// ---------------------------------------------------------------------