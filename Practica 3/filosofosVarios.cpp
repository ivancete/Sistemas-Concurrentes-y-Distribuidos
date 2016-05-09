#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);
void Camarero( int id );


MPI_Status status;
int basura;

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=15)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 15" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }

   if(rank == 11 || rank == 12 || rank == 13 || rank == 14)
   	CamareroSirviente(rank);
   
   if(rank == 10)
   	CamareroJefe(rank);

   if ((rank%2) == 0)  
      Filosofo(rank,size - 1); // Los pares son Filosofos 
   else 
      Tenedor(rank,size - 1);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}

// ---------------------------------------------------------------------

void CamareroSirviente(int id)
{

}

// ---------------------------------------------------------------------

void Filosofo( int id, int nprocesos )
{
   int izq = ((id+nprocesos)+1) % nprocesos;
   int der = ((id+nprocesos)-1) % nprocesos;   
   
   while(1)
   {

   		if(id != 0){

   			cout <<"Filosofo "<<id<< " quiere sentarse " << endl << flush;
	    	MPI_Ssend(&basura, 1, MPI_INT, 10, 50,MPI_COMM_WORLD);

	        // Solicita tenedor izquierdo

	        cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
	        MPI_Ssend(&basura, 1, MPI_INT, (nprocesos + id + 1)%nprocesos, (nprocesos + id + 1)%nprocesos, MPI_COMM_WORLD);
	        MPI_Recv(&basura,1,MPI_INT, (nprocesos + id + 1)%nprocesos,(nprocesos + id + 1)%nprocesos, MPI_COMM_WORLD, &status);
	        cout << "Filosofo "<<id<< " coge tenedor " << izq << endl << flush;

	        // Solicita tenedor derecho
	        cout <<"Filosofo "<<id<< " solicita tenedor der ..." << der << endl << flush;
	        MPI_Ssend(&basura, 1, MPI_INT, (nprocesos + id - 1)%nprocesos, (nprocesos + id - 1)%nprocesos, MPI_COMM_WORLD);
	        MPI_Recv(&basura,1,MPI_INT, (nprocesos + id - 1)%nprocesos,(nprocesos + id - 1)%nprocesos, MPI_COMM_WORLD, &status);
	        cout <<"Filosofo "<<id<< " coge tenedor " << der << endl << flush;
	    }
	    else
	    {

	    	cout <<"Filosofo "<<id<< " quiere sentarse " << endl << flush;
	    	MPI_Ssend(&basura, 1, MPI_INT, 10, 50,MPI_COMM_WORLD);


	    	// Solicita tenedor derecho
	        cout <<"Filosofo "<<id<< " solicita tenedor der ..." << der << endl << flush;
	        MPI_Ssend(&basura, 1, MPI_INT, (nprocesos + id - 1)%nprocesos, (nprocesos + id - 1)%nprocesos, MPI_COMM_WORLD);
	        MPI_Recv(&basura,1,MPI_INT, (nprocesos + id - 1)%nprocesos,(nprocesos + id - 1)%nprocesos, MPI_COMM_WORLD, &status);
	        cout <<"Filosofo "<<id<< " coge tenedor " << der << endl << flush;

	        // Solicita tenedor izquierdo

	        cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
	        MPI_Ssend(&basura, 1, MPI_INT, (nprocesos + id + 1)%nprocesos, (nprocesos + id + 1)%nprocesos, MPI_COMM_WORLD);
	        MPI_Recv(&basura,1,MPI_INT, (nprocesos + id + 1)%nprocesos,(nprocesos + id + 1)%nprocesos, MPI_COMM_WORLD, &status);
	        cout << "Filosofo "<<id<< " coge tenedor " << izq << endl << flush;	    	
	    }
	  
	    cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
	    sleep(3);  //comiendo
	      
	    // Suelta el tenedor izquierdo
	    cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
	    MPI_Ssend(&basura, 1, MPI_INT, (nprocesos + id + 1)%nprocesos, ((nprocesos + id + 1)%nprocesos) + 10, MPI_COMM_WORLD);
	      
	    // Suelta el tenedor derecho
	    cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
	    MPI_Ssend(&basura, 1, MPI_INT, (nprocesos + id - 1)%nprocesos, ((nprocesos + id - 1)%nprocesos) + 10, MPI_COMM_WORLD);

	    cout <<"Filosofo "<<id<< " quiere levantarse " << endl << flush;
	    MPI_Ssend(&basura, 1, MPI_INT, 10, 51,MPI_COMM_WORLD);
	      
	    // Piensa (espera bloqueada aleatorio del proceso)
	    cout << "Filosofo " << id << " PENSANDO" << endl << flush;
	      
	    // espera bloqueado durante un intervalo de tiempo aleatorio 
	    // (entre una décima de segundo y un segundo)
	    usleep( 1000U * (100U+(rand()%900U)) );
    }
}
// ---------------------------------------------------------------------
void CamareroJefe(int id)
{
	MPI_Status status;
	int contiene;
	int id_camarero;
	int id_filo;
	bool estado [4];
	bool encontrado = false;
	int cont = 0;
	int rama;

	while(1)
	{
		if(cont == 0)
		{
			rama = 0;
		}
		else if(cont == 4)
		{
			rama = 1;
		}
		else
		{
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG,MPI_COMM_WORLD, &status);			
		}
	}
}
//----------------------------------------------------------------------

void Tenedor(int id, int nprocesos)
{
  int buf; 
  MPI_Status status; 
  int Filo;
  
  while(1)
  {
    // Espera un peticion desde cualquier filosofo vecino ...
    MPI_Recv(&basura,1,MPI_INT, MPI_ANY_SOURCE,id, MPI_COMM_WORLD, &status);
    
    // Recibe la peticion del filosofo ...
    Filo = status.MPI_SOURCE;

    MPI_Ssend(&basura, 1, MPI_INT, Filo, id, MPI_COMM_WORLD);
    
    cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
    
    // Espera a que el filosofo suelte el tenedor...
    MPI_Recv(&basura,1,MPI_INT, MPI_ANY_SOURCE,id + 10, MPI_COMM_WORLD, &status);
    cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush; 
  }
}
// ---------------------------------------------------------------------