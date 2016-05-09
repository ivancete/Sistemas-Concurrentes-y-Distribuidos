import java.lang.Runnable;
import java.lang.Thread;
import monitor.* ;

class MiVariable extends AbstractMonitor
{
	private int valor;

	MiVariable(int valor)
	{
		this.valor = valor;
	}

	public int getValor()
	{
		enter();
		int val = valor;
		leave();
		return valor;
	}

	public void incrementar()
	{
		enter();
		valor++;
		leave();
	}
}

class Hebra implements Runnable
{
	private MiVariable v;
	Thread thr;

	Hebra(MiVariable v)
	{
		this.v = v;
		thr = new Thread(this);
	}

	public void run()
	{
		for(int i = 0; i < 20; i++)
			v.incrementar();
	}
}

public class prueba3
{
	public static void main(String [] args)
	{
		Hebra h1, h2;
		MiVariable v = new MiVariable(0);
		h1 = new Hebra(v);
		h2 = new Hebra(v);
		h1.thr.start();
		h2.thr.start();

		try{
			h1.thr.join();
			h2.thr.join();
		}catch(InterruptedException ex)
		{}

		System.out.println(v.getValor());
	}
}