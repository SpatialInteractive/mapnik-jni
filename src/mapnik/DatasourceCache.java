package mapnik;

/**
 * Manage the registry of datasources.  To create a Datasource, use the create(...)
 * method.
 * 
 * @author stella
 *
 */
public class DatasourceCache {
	public static native String[] pluginNames();
	public static native String pluginDirectories();
	public static native void registerDatasources(String path);
	public static native Datasource create(Parameters params, boolean bind);
	public static Datasource create(Parameters params) {
		return create(params, true);
	}
}
