// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

#include <iostream>
#include <pthread.h>
#include "stdlib.h"
#include "fun_tiempo.h"
#include "math.h"

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)
const long double pi = M_PI;
unsigned long m;  // número de muestras (un millón)
unsigned n;                      // número de hebras

double resultado_parcial[5000] ; // tabla de sumas parciales (una por hebra)

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( )
{  
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void) 
{  
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ;
   // calcular suma parcial en "sumap"
   for(unsigned  long i = ih; i < m; i+=n) 
      sumap = sumap + f ((i+0.5) / m);

   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   return NULL;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente()
{  
   // crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
   pthread_t id_hebra[n] ; // vector de identificadores de hebra
   
   for (unsigned long i = 0; i < n; i++) {
      void * arg_ptr = (void *) i; // convertir entero a puntero
      pthread_create (&(id_hebra[i]), NULL, funcion_hebra, arg_ptr);
   }
   
   // esperar (join) a que termine cada hebra, sumar su resultado
   double suma_integral = 0;
   for(unsigned i = 0 ; i < n ; i++ )
      pthread_join( id_hebra[i], NULL);

   for (unsigned i = 0; i < n ; i++)
      suma_integral += resultado_parcial[i];
   // devolver resultado completo
   return suma_integral/m;
}

// ---------------------------------------------------------------------

int main(int argc, char *argv[])
{

	if(argc != 3) {
		cout << "Error en la introducción de parámetros" << endl;
		return -1;
	}

   unsigned n_hebras = atoi(argv[1]);
   unsigned n_muestras = atoi(argv[2]);

	n = n_hebras;
	m = n_muestras;

   cout << "Ejemplo 4 (cálculo de PI)" << endl ;
   double pi_sec = 0.0, pi_conc = 0.0 ;
   
   cout << "Valor 'exacto' de PI: " << pi << endl;
   
   //struct timespec inicio = ahora();
   pi_sec  = calcular_integral_secuencial() ;
   //struct timespec fin = ahora();
   cout << "valor de PI (calculado secuencialmente)  == " << pi_sec  << endl;
   //cout << "Tiempo secuencial: " << duracion(&inicio, &fin) << endl;

   //inicio = ahora();
   pi_conc = calcular_integral_concurrente() ;
   //fin = ahora();
   cout << "valor de PI (calculado concurrentemente) == " << pi_conc << endl ; 
   //cout << "Tiempo concurrente: " << duracion(&inicio, &fin) << endl;

   return 0 ;
}
// ---------------------------------------------------------------------



// ----------------------------------------------------------------------------
