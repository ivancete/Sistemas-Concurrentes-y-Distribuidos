/*Realizado por: Iván Rodríguez Millán.
2º Grado en ingenieria Informatica. Asignatura SCD.
*/

import monitor.* ;
import java.util.Random ;

class aux
{
  static Random genAlea = new Random() ;

  static void dormir_max( int milisecsMax)
  { 
    try
    { 
      	Thread.sleep( 1000 + genAlea.nextInt( milisecsMax) ) ;
    } 
    catch( InterruptedException e )
    { 
      System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}

class Barberia extends AbstractMonitor
{
	private Condition pelando = makeCondition();
	private Condition clientes = makeCondition();
	private Condition fin_pelar = makeCondition();
	private int clent;

	public Barberia()
	{
		clent = -1;
	}

	public void cortarPelo(int num_c)
	{
		enter();
		System.out.println("El cliente"+num_c+"espera a pelarse");
		if(pelando.isEmpty())clientes.await();
		else pelando.signal();

		clent = num_c;

		System.out.println("El barbero esta pelando al cliente "+num_c);
		fin_pelar.await();

		leave();
	}

	public void siguienteCliente()
	{
		enter();
		System.out.println("El barbero espera al siguiente cliente");
		if(clientes.isEmpty())pelando.await();

		else
			clientes.signal();

		leave();

	}

	public void finCliente()
	{
		enter();

		fin_pelar.signal();
		System.out.println("El barbero termina de pelar al cliente "+clent);
		leave();

	}
}

class Cliente implements Runnable
{
	public Thread thr;
	private Barberia b;
	private int num_cliente;

	public Cliente(Barberia b1,int numero)
	{
		b = b1;
		thr = new Thread(this);
		num_cliente = numero;
	}

	public void run()
	{
		while(true)
		{
			b.cortarPelo(num_cliente);
			aux.dormir_max(2000);
		}
	}


}

class Barbero implements Runnable
{
	public Thread thr;
	private Barberia b;

	public Barbero(Barberia b2)
	{
		b = b2;
		thr = new Thread(this);
	}

	public void run(){

		while(true){
			b.siguienteCliente();
			aux.dormir_max(2500);
			b.finCliente();
		}
	}

}

class MainBarberia
{
	public static void main(String[] args)
	{

		/*if(args.length != 1)
		{
			System.err.println("Uso: nclientes");
      		return ;
		}*/

		Barberia pelo = new Barberia();

		Cliente [] clientes = new Cliente [5];

		Barbero barbe = new Barbero (pelo);

		for(int i = 0; i < 5; i++)
			clientes[i] = new Cliente(pelo,i);

		for(int i = 0; i < 5; i++)
			clientes[i].thr.start();

		barbe.thr.start();
	}
}