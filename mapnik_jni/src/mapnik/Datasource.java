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
	public native int getType();
	public native void bind();
	public native Box2d getEnvelope();
	public native FeatureSet features(Query q);
	public native FeatureSet featuresAtPoint(Coord pt);
	public native LayerDescriptor getDescriptor();
}
