public class Exemplolookupswitch {
	public static int chooseFar(int i) {
		switch (i) {
			case -100: return -1;
			case 0:    return  0;
			case 100:  return  1;
			default:   return -1;
		}
	}

	public static void main(String args[]) {
	int escolha = 100;
	System.out.println(chooseFar(escolha));
	}
}