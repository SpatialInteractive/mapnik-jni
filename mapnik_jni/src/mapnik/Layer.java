package mapnik;


public class Layer {
	private long ptr;
	
	private static native long alloc(String name, String srs);
	private static native void dealloc(long ptr);

	public Layer(String name, String srs) {
		ptr=alloc(name, srs);
	}
	public Layer(String name) {
		this(name, Mapnik.SRS_DEFAULT);
	}
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	public native String getName();
	public native void setName(String name);
	public native String getTitle();
	public native void setTitle(String title);
	public native String getAbstract(); 
	public native void setAbstract(String abs);
	public native String getSrs();
	public native void setSrs(String srs);
	
	public native String[] getStyles();
	public native void setStyles(String[] styles);
	
	public native double getMinZoom();
	public native void setMinZoom(double z);
	
	public native double getMaxZoom();
	public native void setMaxZoom(double z);
	
	public native boolean isActive();
	public native void setActive(boolean active);
	
	public native boolean isQueryable();
	public native void setQueryable(boolean b);
	
	public native boolean isVisible(double scale);
	
	public native boolean isClearLabelCache();
	public native void setClearLabelCache(boolean b);
	
	public native boolean isCacheFeatures();
	public native void setCacheFeatures(boolean b);
	
	// -- datasource
	// -- envelope
}
