package mapnik;


public class TestBase {
	private static boolean inited=false;
	
	public static void initialize() {
		if (inited) return;
		inited=true;
		Mapnik.initialize();
		
		System.out.println("Plugin directories=" + DatasourceCache.pluginDirectories());
		try {
			DatasourceCache.registerDatasources("/usr/local/lib/mapnik2/input");
		} catch (Throwable t) { }
		for (String s: DatasourceCache.pluginNames()) {
			System.out.println("Plugin=" + s);
		}
	}
}
