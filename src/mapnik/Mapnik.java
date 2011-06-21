package mapnik;

public class Mapnik {
	public static final String SRS_DEFAULT="+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs";
	
	private static boolean initialized;
	
	public synchronized static void initialize() {
		if (initialized) return;
		try {
			System.loadLibrary("mapnik-jni");
		} finally {
			nativeInit();
			initialized=true;
		}
	}
	
	private static native void nativeInit();
}
