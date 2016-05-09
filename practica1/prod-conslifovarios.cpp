#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h> 

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned 
  num_items  = 40 ,
  tam_vector = 10 ;              

// ---------------------------------------------------------------------

int vector [tam_vector];
int pos_libre = 0;

sem_t 
	puede_escribir,
  turno_1,
  turno_2,
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
void * productor_1( void * )
{   
  for( unsigned i = 0 ; i < num_items/2 ; i++ )
  {     

    sem_wait(&turno_1); 

    int ele = producir_dato() ;      

    sem_wait(&puede_escribir);

    sem_wait(&mutex);

    vector[pos_libre] = ele;

    pos_libre++;

    sem_post(&mutex);

    sem_post(&puede_leer);

    sem_post(&turno_2);

  }
  return NULL ;
}

// ---------------------------------------------------------------------


void * productor_2( void * )
{   
  for( unsigned i = 0 ; i < num_items/2 ; i++ )
  {

    sem_wait(&turno_2);

    int ele = producir_dato() ;    

    sem_wait(&puede_escribir); 

    sem_wait(&mutex);

    vector[pos_libre] = ele;

    pos_libre++;

    sem_post(&mutex);

    sem_post(&puede_leer);

    sem_post(&turno_1);

  }
  return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * )
{   
  for( unsigned i = 0 ; i < num_items ; i++ ){

  	sem_wait(&puede_leer);	

    int dato ;    

    sem_wait(&mutex);

    dato = vector[pos_libre - 1];

    pos_libre--;

    sem_post(&mutex);

    consumir_dato( dato ) ;   

    sem_post(&puede_escribir);

  }
  return NULL ;
}
//----------------------------------------------------------------------

int main()
{

	pthread_t escritora_1, escritora_2 , lectora;

	sem_init(&puede_escribir, 0 , tam_vector - 1);
	sem_init(&puede_leer, 0, 0);
	sem_init(&mutex, 0, 1);

  sem_init(&turno_1, 0, 1 );
  sem_init(&turno_2, 0, 0 );


	pthread_create( &escritora_1, NULL, productor_1, NULL);
  pthread_create( &escritora_2, NULL, productor_2, NULL);
	pthread_create( &lectora, NULL, consumidor, NULL);

	pthread_join(escritora_1, NULL);
  pthread_join(escritora_2, NULL);
	pthread_join(lectora, NULL);

	sem_destroy(&puede_escribir);
	sem_destroy(&puede_leer);
  sem_destroy(&mutex);

   return 0 ; 
}
