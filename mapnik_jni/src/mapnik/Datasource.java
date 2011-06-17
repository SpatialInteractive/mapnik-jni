package mapnik;

public class Datasource {
	private long ptr;
	
	public static native void dealloc(long ptr);
	
	private Datasource() {
	}
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	public native Parameters getParameters();
}
