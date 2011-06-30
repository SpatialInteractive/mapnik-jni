package mapnik;

/**
 * Java access to a mapnik image_32 class
 * @author stella
 *
 */
public class Image {
	/**
	 * mapnik::image_32*
	 */
	private long ptr;
	
	private static native long alloc(int width, int height);
	private static native long alloc(Image other);
	private static native void dealloc(long ptr);
	
	public Image(int width, int height) {
		ptr=alloc(width, height);
	}
	public Image(Image other) {
		ptr=alloc(other);
	}
	@Override
	protected void finalize() throws Throwable {
		dispose();
	}
	
	/**
	 * Dispose of native resources associated with this instance
	 */
	public void dispose() {
		if (ptr!=0) dealloc(ptr);
		ptr=0;
	}
	
	public native int getWidth();
	public native int getHeight();
	
	public native void saveToFile(String fileName, String type);
	public native byte[] saveToMemory(String type);
}
