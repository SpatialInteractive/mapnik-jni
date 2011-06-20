package mapnik;

public class Query {
	private long ptr;
	
	private static native long alloc(Box2d bbox, double resx, double resy, double scaleDenominator);
	private static native void dealloc(long ptr);
	
	public Query(Box2d bbox, double resx, double resy, double scaleDenominator) {
		if (bbox==null) throw new IllegalArgumentException("Query bbox cannot be null");
		ptr=alloc(bbox, resx, resy, scaleDenominator);
	}
	public Query(Box2d bbox) {
		this(bbox, 1.0, 1.0, 1.0);
	}
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	public native void addPropertyName(String name);
	
	// TODO: Add getters
}
