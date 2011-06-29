package mapnik;

import java.util.List;
import java.util.Set;

/**
 * Wraps a mapnik::featureset_ptr and the current mapnik::feature into a
 * java.sql.ResultSet type interface.
 * @author stella
 *
 */
public class FeatureSet {
	/**
	 * Pointer to mapnik::featureset_ptr
	 */
	private long ptr;
	
	/**
	 * Pointer to mapnik::feature_ptr
	 */
	private long feature_ptr;
	
	/**
	 * Java references to geometries for the current feature
	 */
	private Geometry[] feature_geometries;
	
	/**
	 * This class is only ever alloc'd from native code so hide the ctor
	 */
	private FeatureSet() {
	}
	
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr, feature_ptr);
	}
	
	private native static void dealloc(long ptr, long feature_ptr);
	private native boolean _next();
	private native Geometry[] _loadGeometries();
	
	// -- FeatureSet methods
	/**
	 * Moves to the next feature in the set.  Returns true if exists.
	 * Initially positioned before the first.
	 */
	public boolean next() {
		// First release the geometries
		if (feature_geometries!=null) {
			for (int i=0; i<feature_geometries.length; i++) {
				feature_geometries[i].invalidate();
			}
			feature_geometries=null;
		}
		return _next();
	}

	/**
	 * The number of geometries
	 * @return number of geometries
	 */
	public int getGeometryCount() {
		if (feature_geometries==null) {
			feature_geometries=_loadGeometries();
		}
		return feature_geometries.length;
	}
	
	/**
	 * Get the geometry at index
	 * @param index
	 * @return geometry
	 */
	public Geometry getGeometry(int index) {
		if (feature_geometries==null) {
			feature_geometries=_loadGeometries();
		}
		return feature_geometries[index];
	}
	

	/**
	 * The id of the current feature
	 * @return id
	 */
	public native int getId();
	
	/**
	 * @return envelope of the current feature
	 */
	public native Box2d getEnvelope();
	
	/**
	 * Names of all properties
	 * @return names
	 */
	public native Set<String> getPropertyNames();
	
	/**
	 * Get a property by name
	 * @param name
	 * @return property or null
	 */
	public native Object getProperty(String name);
}
