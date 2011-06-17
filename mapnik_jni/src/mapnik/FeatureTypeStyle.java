package mapnik;

import java.util.Set;

/**
 * Mirrors the feature_type_style in mapnik c++
 *
 * @author stella
 *
 */
public class FeatureTypeStyle {
	private long ptr;
	
	private static native long alloc();
	private static native void dealloc(long ptr);
	
	public FeatureTypeStyle() {
		ptr=alloc();
	}
	@Override
	protected void finalize() throws Throwable {
		dealloc(ptr);
	}
	
	/**
	 * Collect all attribute names referenced in rules
	 * @return Set of attributes
	 */
	public native Set<String> collectAttributes();
}
