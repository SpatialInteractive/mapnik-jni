package mapnik;

public class Datasource {
	/**
	 * Native pointer: boost::shared_ptr<mapnik::datasource>
	 */
	private long ptr;
	
	private static native void dealloc(long ptr);
	
	private Datasource() {
	}
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	public native Parameters getParameters();
}
