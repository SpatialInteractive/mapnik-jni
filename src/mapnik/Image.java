package mapnik;

/**
 * Java access to a mapnik image_32 class
 * @author stella
 *
 */
public class Image extends NativeObject {
	// mapnik::image_32*
	
	private static native long alloc(int width, int height);
	private static native long alloc(Image other);
	native void dealloc(long ptr);
	
	public Image(int width, int height) {
		ptr=alloc(width, height);
	}
	public Image(Image other) {
		ptr=alloc(other);
	}
	
	public native int getWidth();
	public native int getHeight();
	
	public native void saveToFile(String fileName, String type);
	public native byte[] saveToMemory(String type);
}
