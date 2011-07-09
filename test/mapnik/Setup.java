package mapnik;


public class Setup {
	private static boolean inited=false;
	
	public static void initialize() {
		if (inited) return;
		inited=true;
		Mapnik.initialize();
	}
	
	public static void tearDown() {
		System.err.println("Mapnik native allocation report:");
		System.err.println("--------------------------------");
		System.err.println(Mapnik.reportNativeAllocations());
	}
}
