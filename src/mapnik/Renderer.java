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
	public static native void renderAgg(MapDefinition map, Image image, double scaleFactor, int offsetX, int offsetY);
	
	/**
	 * Render the given map to the given image with scaleFactor=1.0, offsetX=0, and offsetY=0
	 * @param map
	 * @param image
	 */
	public static void renderAgg(MapDefinition map, Image image) {
		renderAgg(map, image, 1.0, 0, 0);
	}
}
