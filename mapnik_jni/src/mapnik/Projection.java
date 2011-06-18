package mapnik;

public class Projection {
	public static final String LATLNG_PARAMS="+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	private long ptr;
	
	private static native long alloc(String params);
	private static native void dealloc(long ptr);
	
	public Projection(String params) {
		if (params==null) {
			throw new IllegalArgumentException("Projection params cannot be null");
		}
		ptr=alloc(params);
	}
	public Projection() {
		ptr=alloc(LATLNG_PARAMS);
	}
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	@Override
	public boolean equals(Object other) {
		if (other==null) return false;
		if (!(other instanceof Projection)) return false;
		return ((Projection)other).getParams().equals(getParams());
	}
	
	public native String getParams();
	public native String getExpanded();
	
	public native void forward(Coord coord);
	public native void inverse(Coord coord);
}
