
import monitor.*;
import java.util.Random ;

class aux
{
  static Random genAlea = new Random() ;

  static void beber(int sMax)
  { 
    try
    { 
      	Thread.sleep( 1000 + genAlea.nextInt( sMax) ) ;
    } 
    catch( InterruptedException e )
    { 
      System.err.println("sleep interumpido en 'aux.beber()'");
    }
  }
}

class Machine extends AbstractMonitor
{
	private int typesCans [];
	private Condition [] puede_beber;
	private int tipo_bebida;
	private int numero_usuarios_totales;
	private int tiposDeBebida;
	private Random numAle;
	private boolean encontrado;
	private int parados;

	public Machine(int tipos, int nU){

		numAle = new Random();

		typesCans = new int [tipos];

		for(int i = 0; i < tipos; i++){
			typesCans[i] = (numAle.nextInt(10 - 4 + 1) + 4);
		}

		puede_beber = new Condition[tipos];

		for(int i = 0; i < tipos; i++)
			puede_beber[i] = makeCondition();

		tipo_bebida = -1;

		numero_usuarios_totales = nU;

		tiposDeBebida = tipos;

		parados = 0;
	}

	public int numTipos(){

		enter();

		leave();

		return tiposDeBebida;
	}

	public void pedirLata(int bebida, int usu){

		enter();

		System.out.println("El usuario "+usu+" selecciona la bebida "+bebida);

		encontrado = false;

		for(int i = 0; i < typesCans.length && !encontrado; i++){

			parados += puede_beber[i].count();
			if(parados > (numero_usuarios_totales / 3))
				encontrado = true;
		}

		if(encontrado){

			System.out.println("El usuario "+usu+" se dio cuenta que habia demasiados usuarios esperando un tipo de lata");

			for(int i = 0; i < typesCans.length; i++){
				typesCans[i] += (numAle.nextInt(10 - 4 + 1) + 4) + puede_beber[i].count();

				while(!puede_beber[i].isEmpty())
					puede_beber[i].signal();
			}
		}

		parados = 0;

		if(typesCans[bebida] == 0)
			puede_beber[bebida].await();

		typesCans[bebida]--;

		System.out.println("El usuario "+usu+" pasará a beberse la lata");

		leave();
	}
}

class Usuario implements Runnable
{
	public Thread thr;
	private int user;
	private Machine mach;
	private int drink;
	private Random bebidaAle = new Random() ;


	public Usuario(int numero_usuario, Machine m){

		thr = new Thread(this, "Usuario "+numero_usuario);
		user = numero_usuario;
		mach = m;
	}

	public void run(){

		while(true){
			int valor = mach.numTipos();
			drink = bebidaAle.nextInt(valor);
			mach.pedirLata(drink,user);
			aux.beber(2000);
		}
	}
}

public class Maquina{

	public static void main(String [] args)
	{

		Random aleatorio = new Random();

		int num_tipos = aleatorio.nextInt(20 - 10 + 1) + 10;

		int num_user = 7 * num_tipos;

		Machine maq = new Machine(num_tipos,num_user);

		Usuario usuarios[] = new Usuario [num_user];

		for(int i = 0; i < num_user; i++)
			usuarios[i] = new Usuario (i,maq);

		for(int i = 0; i < num_user; i++)
			usuarios[i].thr.start();
	}
}