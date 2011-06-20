package mapnik;


public class TestBase {
	private static boolean inited=false;
	
	public static void initialize() {
		if (inited) return;
		inited=true;
		Mapnik.initialize();
		DatasourceCache.registerDatasources("/usr/local/lib/mapnik2/input");
	}
}
