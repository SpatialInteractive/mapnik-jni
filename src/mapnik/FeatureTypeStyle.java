package mapnik;

import java.util.Set;

/**
 * Mirrors the feature_type_style in mapnik c++
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
