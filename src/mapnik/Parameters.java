package mapnik;

import java.util.HashMap;

/**
 * Shadows a mapnik::parameters object
 * @author stella
 *
 */
public class Parameters extends HashMap<String, Object> {
	private void copyToNative(long ptr) {
		for (java.util.Map.Entry<String,Object> entry: entrySet()) {
			String key=entry.getKey();
			Object value=entry.getValue();
			if (value==null) continue;
			
			if (value instanceof Number) {
				Number number=(Number) value;
				if ((value instanceof Double) || (value instanceof Float)) {
					setNativeDouble(ptr, key, number.doubleValue());
				} else {
					setNativeInt(ptr, key, number.intValue());
				}
			} else {
				// Treat as string
				setNativeString(ptr, key, value.toString());
			}
		}
	}
	
	private void setString(String key, String value) {
		put(key, value);
	}
	
	private void setInt(String key, int value) {
		put(key, Integer.valueOf(value));
	}
	
	private void setDouble(String key, double value) {
		put(key, Double.valueOf(value));
	}
	
	private static native void setNativeInt(long ptr, String name, int value);
	private static native void setNativeString(long ptr, String name, String value);
	private static native void setNativeDouble(long ptr, String name, double value);
}
