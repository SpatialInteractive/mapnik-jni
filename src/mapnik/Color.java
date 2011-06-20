package mapnik;

/**
 * A mapnik color.  This is a pure java class which is converted to a mapnik::color
 * on the native side.
 * @author stella
 *
 */
public class Color {
	private int red;
	private int green;
	private int blue;
	private int alpha;
	
	public Color(int red, int green, int blue, int alpha) {
		this.red=red;
		this.green=green;
		this.blue=blue;
		this.alpha=alpha;
	}
	public Color() {
		this(0xff, 0xff, 0xff, 0xff);
	}
	public Color(int red, int green, int blue) {
		this(red, green, blue, 0xff);
	}
	public int getRed() {
		return red;
	}
	public void setRed(int red) {
		this.red = red;
	}
	public int getGreen() {
		return green;
	}
	public void setGreen(int green) {
		this.green = green;
	}
	public int getBlue() {
		return blue;
	}
	public void setBlue(int blue) {
		this.blue = blue;
	}
	public int getAlpha() {
		return alpha;
	}
	public void setAlpha(int alpha) {
		this.alpha = alpha;
	}
	
	
}
