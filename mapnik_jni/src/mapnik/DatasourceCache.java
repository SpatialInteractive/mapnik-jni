package mapnik;

public class DatasourceCache {
	private long ptr;
	
	public static native void registerDatasources(String path);
}
