//import SomaLong;
class TesteInvoke {
	private static SomaLong somaLong;
	public static void main (String[ ] s)	{
		long j = 1000L;
		
		somaLong = new SomaLong();
		j = somaLong.soma(j, 500L);

		System.out.println(j);
	}
}