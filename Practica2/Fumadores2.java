/*Realizado por: Iván Rodríguez Millán.
2º Grado en ingenieria Informatica. Asignatura SCD.
*/

import monitor.* ;
import java.util.Random ;
import java.util.Date;

// ****************************************************************************

class aux
{
  static Random genAlea = new Random() ;

  static void dormir_max( int milisecsMax, int fumador )
  { 
    try
    { 
    	//System.out.println("Fumando, el fumador "+fumador);
      	Thread.sleep( 1000 + genAlea.nextInt( milisecsMax) ) ;
    } 
    catch( InterruptedException e )
    { 
      System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}

class Estanco extends AbstractMonitor
{
	final int FUMADORES = 3;
	final int ESTANQUEROS = 2;

	int ingrediente_generado;
	int estanquero_ponedor;
	private Condition [] puede_obtener = new Condition [FUMADORES];
  	private Condition [] puede_poner = new Condition [ESTANQUEROS];

  	public Estanco()
  	{
  		ingrediente_generado = -1;
  		estanquero_ponedor = -1;

  		for(int i = 0; i < FUMADORES; i++)
  			puede_obtener[i] = makeCondition();

  		for(int i = 0; i < ESTANQUEROS; i++)
  			puede_poner[i] = makeCondition();
  	}

	public void obtenerIngrediente(int miIngrediente)
	{
		enter();
		if(ingrediente_generado != miIngrediente)
			puede_obtener[miIngrediente].await();

		System.out.println((new Date()).toString() + "Fumando, el fumador "+miIngrediente);

		ingrediente_generado = -1;
		puede_poner[estanquero_ponedor].signal();

		leave();
	}

	public void ponerIngrediente(int ingrediente, int num_estanquero)
	{
		enter();
		if(estanquero_ponedor != -1)
			puede_poner[num_estanquero].await();

		estanquero_ponedor = num_estanquero;

		System.out.println((new Date()).toString() + "El estanquero "+num_estanquero" esta colocando el ingrediente "+ ingrediente);

		ingrediente_generado = ingrediente;

		puede_obtener[ingrediente].signal();

		leave();
	}

	public void esperarRecogidaDeIngrediente(int num_est)
	{
		enter();

		System.out.println((new Date()).toString() + "Esperando el estanquero"+ num_est "a que cojan el ingrediente");

		if(ingrediente_generado != -1)
			puede_poner[num_est].await();

		puede_poner[(num_est + 1) % ESTANQUEROS].signal();
		
		leave();
	}
}

class Fumador implements Runnable
{
	int miIngrediente;
	public Thread thr;
	private Estanco estanco;

	public Fumador(int p_miIngrediente, Estanco otro)
	{
		miIngrediente = p_miIngrediente;
		thr = new Thread(this, "Fumador "+p_miIngrediente);
		estanco = otro;
	}

	public void run()
	{
		try
    	{ 
			while(true)
			{
				//System.out.println("Esperando para fumar, el fumador "+miIngrediente);
				estanco.obtenerIngrediente(miIngrediente);
				aux.dormir_max(200, miIngrediente);
				//System.out.println("Terminó de fumar el fumador "+miIngrediente);
			}
		}
	    catch( Exception e ) 
	    { 
	      System.err.println("Excepcion en main: " + e);
	    }
		}
}

class Estanquero implements Runnable
{
	public Thread thr;
	private Estanco estanco;
	int num;

	public Estanquero (int num_estanquero, Estanco uno)
	{
		thr = new Thread(this, "Estanquero "+num_estanquero);
		estanco = uno;
		num = num_estanquero;
	}

	public void run()
	{
		try
    	{
			int ingrediente;

			while(true)
			{
				ingrediente = (int) (Math.random () * 3.0); //Entre 0,1 ó 2.
				estanco.ponerIngrediente(ingrediente, num);
				estanco.esperarRecogidaDeIngrediente(num);
			} 
		}
	    catch( Exception e ) 
	    { 
	      System.err.println("Excepcion en main: " + e);
	    }
	}
}

public class Fumadores
{
	public static void main(String[] args)
	{
		final int FUMADORES = 3;

		Estanco abrir = new Estanco();

		Estanquero estanquero1 = new Estanquero(0, abrir);

		Estanquero estanquero2 = new Estanquero(1, abrir);

		Fumador fumadores[] = new Fumador[FUMADORES];

		for(int i = 0; i < fumadores.length; i++) 
	    	fumadores[i] = new Fumador(i, abrir);

	    for(int i = 0; i < fumadores.length; i++) 
     		fumadores[i].thr.start();

     	estanquero1.thr.start();
     	estanquero2.thr.start();
    }
}