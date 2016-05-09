

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

	int ingrediente_generado;
	private Condition [] puede_obtener;
  	private Condition puede_poner = makeCondition();
  	private int numero_fumadores;

  	public Estanco(int fum)
  	{

  		numero_fumadores = fum;

  		puede_obtener = new Condition [numero_fumadores];
  		ingrediente_generado = -1;

  		for(int i = 0; i < numero_fumadores; i++)
  			puede_obtener[i] = makeCondition();
  	}

	public void obtenerIngrediente(int miIngrediente, int num_fumador)
	{
		enter();
		if(ingrediente_generado != miIngrediente)
			puede_obtener[miIngrediente].await();

		System.out.println((new Date()).toString() + "Fumando, el fumador "+ num_fumador);

		ingrediente_generado = -1;
		puede_poner.signal();

		leave();
	}

	public void ponerIngrediente(int ingrediente,int est)
	{
		enter();
		System.out.println((new Date()).toString() + "El estanquero "+est+ " esta colocando el ingrediente "+ingrediente);

		ingrediente_generado = ingrediente;

		puede_obtener[ingrediente].signal();
		leave();
	}

	public void esperarRecogidaDeIngrediente(int est)
	{
		enter();
		System.out.println((new Date()).toString() + "Esperando el estanquero " +est+" a que cojan el ingrediente");
		if(ingrediente_generado != -1)
			puede_poner.await();
		
		leave();
	}
}

class Fumador implements Runnable
{
	private int miIngrediente;
	public Thread thr;
	private Estanco estanco;
	private int numero_de_fumador;


	public Fumador(int p_miIngrediente,int num, Estanco otro)
	{
		miIngrediente = p_miIngrediente;
		numero_de_fumador = num;
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
				estanco.obtenerIngrediente(miIngrediente, numero_de_fumador);
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

class FumadorEmpedernido implements Runnable
{
	public Thread thr;
	private Estanco estanconum0;
	private Estanco estanconum1;
	private int ingre;
	private int numero_fumador;

	public FumadorEmpedernido(int numero_hebra, Estanco primero, Estanco segundo)
	{
		numero_fumador = numero_hebra;
		thr = new Thread(this, "Fumador "+numero_hebra);
		estanconum0 = primero;
		estanconum1 = segundo;
	}

	public void run()
	{
		try
    	{ 
			while(true)
			{
				ingre = (int) (Math.random () * 3.0); //Entre 0,1 ó 2.
				estanconum0.obtenerIngrediente(ingre, numero_fumador);
				estanconum1.obtenerIngrediente(ingre, numero_fumador);
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
	private int num_est;

	public Estanquero (int num_estanquero, Estanco uno)
	{
		thr = new Thread(this, "Estanquero "+num_estanquero);
		estanco = uno;
		num_est = num_estanquero;
	}

	public void run()
	{
		try
    	{
			int ingrediente;

			while(true)
			{
				ingrediente = (int) (Math.random () * 3.0); //Entre 0,1 ó 2.
				estanco.ponerIngrediente(ingrediente, num_est);
				estanco.esperarRecogidaDeIngrediente(num_est);
			} 
		}
	    catch( Exception e ) 
	    { 
	      System.err.println("Excepcion en main: " + e);
	    }
	}
}

public class FumadoresExamen
{
	public static void main(String[] args)
	{

		Random numAle = new Random() ;

		int k = numAle.nextInt(3)+1;

		int nF = 6 * k;

		System.out.println((new Date()).toString() + "El numero de fumadores es: "+nF);

		Estanco estanco0 = new Estanco(nF / 2);

		Estanco estanco1 = new Estanco(nF / 2);

		Estanquero estanquero0 = new Estanquero(0, estanco0);

		Estanquero estanquero1 = new Estanquero(1, estanco1);



		Fumador fumadores[] = new Fumador[nF];

		for(int i = 0; i < fumadores.length; i++) {

			if(((i/3)%2) == 0)
	    		fumadores[i] = new Fumador(i%3,i, estanco0);
	    	else
	    		fumadores[i] = new Fumador(i%3,i, estanco1);
	    }

	    for(int i = 0; i < fumadores.length; i++) 
     		fumadores[i].thr.start();



	    FumadorEmpedernido fumadoresEmpedernidos[] = new FumadorEmpedernido[nF];

	    for(int j = 0; j < fumadoresEmpedernidos.length; j++)
	    	fumadoresEmpedernidos[j] = new FumadorEmpedernido(j,estanco0,estanco1);	    

     	for(int j = 0; j < fumadoresEmpedernidos.length; j++) 
     		fumadoresEmpedernidos[j].thr.start();

     	estanquero0.thr.start();
     	estanquero1.thr.start();
    }
}
