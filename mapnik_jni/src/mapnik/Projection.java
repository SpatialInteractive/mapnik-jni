package mapnik;

public class Projection {
	public static final String LATLNG_PARAMS="+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	
	/**
	 * Spherical web mercator
	 */
	public static final String SRS900913_PARAMS="+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs +over";
	
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
	
	public void forward(Box2d box) {
		Coord c=new Coord(box.minx, box.miny);
		forward(c);
		box.minx=c.x;
		box.miny=c.y;
		
		c.x=box.maxx;
		c.y=box.maxy;
		forward(c);
		box.maxx=c.x;
		box.maxy=c.y;
	}
	
	public void inverse(Box2d box) {
		Coord c=new Coord(box.minx, box.miny);
		inverse(c);
		box.minx=c.x;
		box.miny=c.y;
		
		c.x=box.maxx;
		c.y=box.maxy;
		inverse(c);
		box.maxx=c.x;
		box.maxy=c.y;
	}

}
