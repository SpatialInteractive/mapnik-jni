package mapnik;

import java.util.Set;

/**
 * Wraps a mapnik::featureset_ptr and the current mapnik::feature into a
 * java.sql.ResultSet type interface.
 * <p>
 * Note that geometries referenced off of the current feature will become
 * invalid once the FeatureSet moves to the next feature or the FeatureSet
 * is disposed.
 * 
 * @author stella
 *
 */
public class FeatureSet extends NativeObject {
	// Pointer to mapnik::featureset_ptr

	/**
	 * Pointer to mapnik::feature_ptr
	 */
	private long feature_ptr;
	
	/**
	 * Java references to geometries for the current feature
	 */
	private Geometry[] feature_geometries;
	
	void dealloc(long ptr) {
		disposeGeometries();
		long localFeaturePtr=feature_ptr;
		feature_ptr=0;
		dealloc(ptr, localFeaturePtr);
	}
	
	private native void dealloc(long ptr, long feature_ptr);
	private native boolean _next();
	private native Geometry[] _loadGeometries();
	
	private void disposeGeometries() {
		if (feature_geometries!=null) {
			for (int i=0; i<feature_geometries.length; i++) {
				feature_geometries[i].dispose();
			}
			feature_geometries=null;
		}
	}
	
	// -- FeatureSet methods
	/**
	 * Moves to the next feature in the set.  Returns true if exists.
	 * Initially positioned before the first.
	 */
	public boolean next() {
		// First release the geometries
		disposeGeometries();
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
