package mapnik;

import java.util.Set;

/**
 * Wraps the mapnik::feature_type_style value type.
 * 
 * @author stella
 *
 */
public class FeatureTypeStyle extends NativeObject {
	private static native long alloc();
	native void dealloc(long ptr);
	
	public FeatureTypeStyle() {
		ptr=alloc();
	}
	
	/**
	 * Collect all attribute names referenced in rules
	 * @return Set of attributes
	 */
	public native Set<String> collectAttributes();
}
