package mapnik;

/**
 * Wraps a mapnik::datasource object.  Datasources should be created
 * by passing parameters to DatasourceCache.
 * 
 * @author stella
 *
 */
public class Datasource extends NativeObject {
	// Native pointer: boost::shared_ptr<mapnik::datasource>
	
	@Override
	native void dealloc(long ptr);
	
	private Datasource() {
	}
	
	public native Parameters getParameters();
	public native int getType();
	public native void bind();
	public native Box2d getEnvelope();
	
	public native FeatureSet features(Query q);
	public native FeatureSet featuresAtPoint(Coord pt);
	
	public native LayerDescriptor getDescriptor();
}
