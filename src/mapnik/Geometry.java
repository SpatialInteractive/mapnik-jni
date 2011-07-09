package mapnik;

/**
 * A mapnik::geometry_type instance.  These instances are always owned
 * by the corresponding feature.
 * 
 * @author stella
 *
 */
public class Geometry extends NativeObject {
	// mapnik::geometry_type*
	
	/**
	 * Only ever alloc'd from native code
	 */
	private Geometry() {
	}

	@Override
	void dealloc(long ptr) {
	}
	
	public static final int
		TYPE_POINT=1,
		TYPE_LINESTRING=2,
		TYPE_POLYGON=3,
		TYPE_MULTIPOINT=4,
		TYPE_MULTILINESTRING=5,
		TYPE_MULTIPOLYGON=6;
	
	public native int getType();
	public native int getNumPoints();
	public native int getVertex(int pos, Coord c);
	public native Box2d getEnvelope();
	
	public String toString() {
		StringBuilder sb=new StringBuilder();
		sb.append("Geometry(").append(getType()).append(",[");
		Coord c=new Coord();
		for (int i=0; i<getNumPoints(); i++) {
			int code=getVertex(i, c);
			if (i>0) sb.append(",");
			sb.append(c.x);
			sb.append(" ");
			sb.append(c.y);
		}
		
		sb.append("])");
		return sb.toString();
	}
}
