#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned 
  num_items  = 40 ,
  tam_vector = 10 ;              

// ---------------------------------------------------------------------

int vector [tam_vector];
int pos_libre = 0;
int pos_ocupada = 0;

sem_t 
	puede_escribir,
	puede_leer,
	mutex;

unsigned producir_dato()
{
  static int contador = 0 ;
  cout << "producido: " << contador << endl << flush ;
  return contador++ ;
}
// ---------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void Finalizado(){

  cout << "Fin" << endl;
}
// ---------------------------------------------------------------------

void * productor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i++ )
  { 
  	sem_wait(&puede_escribir);

  	//sem_wait(&mutex);

    int dato = producir_dato() ;

    vector[pos_libre] = dato;

    pos_libre = (pos_libre + 1) % tam_vector;

    //sem_post(&mutex);

    sem_post(&puede_leer);      

  }
  return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i++ ){

  	sem_wait(&puede_leer);

  	//sem_wait(&mutex);

    int dato ;

    dato = vector[pos_ocupada];

    pos_ocupada = (pos_ocupada + 1) % tam_vector;

    consumir_dato( dato ) ;

    //sem_post(&mutex);

    sem_post(&puede_escribir);

  }
  return NULL ;
}

//----------------------------------------------------------------------

int main()
{

	pthread_t escritora , lectora;

	sem_init(&puede_escribir, 0 , tam_vector - 1);
	sem_init(&puede_leer, 0, 0);
	//sem_init(&mutex, 0, 1);

	pthread_create( &escritora, NULL, productor, NULL);
	pthread_create( &lectora, NULL, consumidor, NULL);

	pthread_join(escritora, NULL);
	pthread_join(lectora, NULL);

	sem_destroy(&puede_escribir);
	sem_destroy(&puede_leer);

   return 0 ; 
}
