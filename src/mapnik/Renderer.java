package mapnik;

/**
 * Methods that interface with the rendering subsystem.  We don't mirror these classes directly
 * as they are operation oriented C++ templates.
 * @author stella
 *
 */
public class Renderer {
	/**
	 * Render the given map to the given image
	 * @param map
	 * @param image
	 */
	public static native void renderAgg(Map map, Image image);
}
