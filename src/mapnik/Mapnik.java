package mapnik;

import java.io.File;
import java.net.URL;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.regex.Pattern;


public class Mapnik {
	private static boolean initialized;
	private static boolean registered;
	private static String loadedLibrary;
	private static List<String> librarySearchNames=new ArrayList<String>();
	private static List<String> librarySearchPath=new ArrayList<String>();
	private static boolean initializationFailure;
	
	static {
		librarySearchNames.add("mapnik-jni");
		
		// Discover where we are
		URL url=Mapnik.class.getResource("/mapnik/Mapnik.class");
		if (url!=null) {
			//System.err.println("Jar url=" + url);
			// URL should be of the form jar:file:...!/mapnik/Mapnik.class
			String urlString=url.toString();
			String prefix="jar:file:";
			String suffix="!/mapnik/Mapnik.class";
			if (urlString.startsWith(prefix) && urlString.endsWith(suffix)) {
				String jarFile=urlString.substring(prefix.length(), urlString.length() - suffix.length());
				File jarPath=new File(jarFile).getParentFile();
				librarySearchPath.add(jarPath.getAbsolutePath());
			}
		}
		
		// Add java.library.path names
		String sysLibraryPath=System.getProperty("java.library.path");
		if (sysLibraryPath!=null) {
			String[] paths=sysLibraryPath.split(Pattern.quote(File.pathSeparator));
			for (String path: paths) librarySearchPath.add(path);
		}
	}
	
	private static Map<Class<? extends NativeObject>, AtomicInteger> nativeAllocCounts;
	static {
		nativeAllocCounts=new HashMap<Class<? extends NativeObject>, AtomicInteger>();
		nativeAllocCounts.put(Datasource.class, new AtomicInteger());
		nativeAllocCounts.put(FeatureSet.class, new AtomicInteger());
		nativeAllocCounts.put(FeatureTypeStyle.class, new AtomicInteger());
		nativeAllocCounts.put(Geometry.class, new AtomicInteger());
		nativeAllocCounts.put(Image.class, new AtomicInteger());
		nativeAllocCounts.put(Layer.class, new AtomicInteger());
		nativeAllocCounts.put(MapDefinition.class, new AtomicInteger());
		nativeAllocCounts.put(Projection.class, new AtomicInteger());
		nativeAllocCounts.put(Query.class, new AtomicInteger());
	}
	
	/**
	 * Return a Map of Object type to allocation count of current active native allocations.
	 * Native allocations are cleared either through an explicit call to dispose() or
	 * finalization.
	 * 
	 * @return Map of object type to count for all counts greater than zero
	 */
	public static Map<String,Integer> getNativeAllocations() {
		Map<String, Integer> ret=new HashMap<String, Integer>();
		for (Map.Entry<Class<? extends NativeObject>, AtomicInteger> entry: nativeAllocCounts.entrySet()) {
			int count=entry.getValue().get();
			if (count==0) continue;
			
			String name=entry.getKey().getName();
			int dotPos=name.lastIndexOf('.');
			if (dotPos>=0) name=name.substring(dotPos+1);
			
			ret.put(name, count);
		}
		return ret;
	}
	
	/**
	 * Generate a brief textual report of outstanding native allocations
	 */
	public static CharSequence reportNativeAllocations() {
		Map<String,Integer> counts=getNativeAllocations();
		StringBuilder buffer=new StringBuilder();
		buffer.append("MapnikAllocations(");
		boolean first=true;
		for (Map.Entry<String, Integer> entry: counts.entrySet()) {
			if (first) first=false;
			else buffer.append(", ");
			buffer.append(entry.getKey()).append("=").append(entry.getValue());
		}
		buffer.append(")");
		return buffer;
	}
	
	static void incrementAlloc(Class<? extends NativeObject> clazz, int delta) {
		AtomicInteger i=nativeAllocCounts.get(clazz);
		if (i==null) {
			throw new IllegalStateException("Not allocation counter defined for " + clazz.getName());
		}
		i.addAndGet(delta);
	}
	
	private static boolean tryLoadLibrary(String path) {
		if (loadedLibrary!=null) return true;
		
		File file=new File(path);
		if (file.isFile()) {
			// Load it without error checking.  We want the first that exists.
			System.load(file.getAbsolutePath());
			loadedLibrary=path;
			return true;
		} else {
			return false;
		}
	}
	
	private static void loadLibrary() {
		if (loadedLibrary!=null) return;
		
		for (String path: librarySearchPath) {
			for (String name: librarySearchNames) {
				String mappedName=System.mapLibraryName(name);
				String libPath=path + File.separator + mappedName;
				if (tryLoadLibrary(libPath)) return;
			}
		}
		
		// Report
		String message="Unable to find mapnik-jni library.  Search names=" +
			Arrays.toString(librarySearchNames.toArray()) +
			", Search path=" + Arrays.toString(librarySearchPath.toArray());
		throw new LinkageError(message);
	}
	
	/**
	 * If initialized, this is the full path of the loaded library
	 * @return path of library
	 */
	public static String getLoadedLibrary() {
		return loadedLibrary;
	}
	
	/**
	 * @return modifiable list of library search names to be probed on initialize
	 */
	public static List<String> getLibrarySearchNames() {
		return librarySearchNames;
	}

	/**
	 * The library search path.  This defaults to the directory containing
	 * this jar file (if found) followed by all entries on java.library.path.
	 * The system default path is implied.
	 * 
	 * @return modifiable list of library search paths to be probed on initialize
	 */
	public static List<String> getLibrarySearchPath() {
		return librarySearchPath;
	}
	
	/**
	 * Initialize the mapnik library.  Relies on the following to have been set prior to
	 * call:
	 * <ul>
	 * <li>getLibrarySearchNames()
	 * <li>getLibrarySearchPath()
	 * </ul>
	 * In most situations, the defaults should suffice.
	 * @param register If true, then also register input plugins and fonts
	 */
	public synchronized static void initialize(boolean register) {
		if (!initialized) {
			if (initializationFailure) {
				throw new IllegalStateException("Previous call to Mapnik.initialize() failed");
			}
			loadLibrary();
	
			try {
				nativeInit();
				initialized=true;
			} finally {
				if (!initialized) initializationFailure=true;
			}
		}
		
		if (register && !registered) {
			registered=true;
			String path;
			
			path=getInstalledFontsDir();
			if (path!=null) {
				FreetypeEngine.registerFonts(path);
			}
			
			path=getInstalledInputPluginsDir();
			if (path!=null) {
				DatasourceCache.registerDatasources(path);
			}
		}
	}
	
	/**
	 * Equivilent to initialize(true).  Fully links and initializes the mapnik library.
	 */
	public static void initialize() {
		initialize(true);
	}
	
	private static native void nativeInit();
	
	/**
	 * @return the Mapnik fonts dir as reported by mapnik at compile time or null
	 */
	public static native String getInstalledFontsDir();
	
	/**
	 * @return the Mapnik input plugins dir as reported by mapnik at compile time or null
	 */
	public static native String getInstalledInputPluginsDir();
}
