package mapnik;


public class Setup {
	private static boolean inited=false;
	
	public static void initialize() {
		if (inited) return;
		inited=true;
		Mapnik.initialize();
	}
}
