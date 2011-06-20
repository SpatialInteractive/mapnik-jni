package mapnik;

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
	 * This class is only ever alloc'd from native code so hide the ctor
	 */
	private FeatureSet() {
	}
	
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr, feature_ptr);
	}
	
	private native static void dealloc(long ptr, long feature_ptr);
	
	// -- FeatureSet methods
	/**
	 * Moves to the next feature in the set.  Returns true if exists.
	 * Initially positioned before the first.
	 */
	public native boolean next();
	
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
	 * The number of geometries
	 * @return number of geometries
	 */
	public native int getGeometryCount();
	
	/**
	 * Get the geometry at index
	 * @param index
	 * @return geometry
	 */
	public native GeometryType getGeometry(int index);
	
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
