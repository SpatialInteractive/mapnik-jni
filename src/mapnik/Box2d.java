package mapnik;

import java.text.DecimalFormat;

/**
 * Mirros the mapnik::box2d&lt;double&gt; class.
 * <p>
 * This is a pure java class that can be converted to a native box2d when needed.
 * 
 * @author stella
 *
 */
public class Box2d {
	public double minx;
	public double miny;
	public double maxx;
	public double maxy;
	
	public Box2d() {
	}
	
	public Box2d(double minx, double miny, double maxx, double maxy) {
		this.minx=minx;
		this.miny=miny;
		this.maxx=maxx;
		this.maxy=maxy;
	}
	
	public Box2d(Box2d other) {
		this.minx=other.minx;
		this.miny=other.miny;
		this.maxx=other.maxx;
		this.maxy=other.maxy;
	}
	
	@Override
	public String toString() {
		DecimalFormat fmt=new DecimalFormat("0.0");
		return "Box2d(" + fmt.format(minx) + "," + fmt.format(miny) + "," + fmt.format(maxx) + "," + fmt.format(maxy) + ")";
	}
}
