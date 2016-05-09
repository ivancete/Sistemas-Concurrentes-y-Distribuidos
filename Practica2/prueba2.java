import java.lang.Runnable;
import java.lang.Thread;

class Hebra implements Runnable
{
	Thread thr;
	private String cadena;

	public void run()
	{
		System.out.println("Hola soy la hebra" + "\t" + cadena);
	}

	Hebra(int num)
	{
		cadena = Integer.toString(num);
		thr = new Thread(this);
	}
}

public class prueba2
{
	public static void main(String [] args)
	{

		int num_hebras = 50;

		//Declaracion de un array estatico
		Hebra [] h = new Hebra[num_hebras];

		//Inicializamos cada hebra
		for(int i = 0; i < num_hebras; i++)
			h[i] = new Hebra(i);

		//Mandamos las hebras
		for(int i = 0; i < num_hebras; i++)
			h[i].thr.start();

		//Detenemos las hebras
		try
		{	for(int i = 0; i < num_hebras; i++)
				h[i].thr.join();
		}catch(InterruptedException ex)
		{
		}
	}
}