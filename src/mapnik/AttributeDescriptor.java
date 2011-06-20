package mapnik;

public class AttributeDescriptor {
	public static final int TYPE_INTEGER=1;
	public static final int TYPE_FLOAT=2;
	public static final int TYPE_DOUBLE=3;
	public static final int TYPE_STRING=4;
	public static final int TYPE_GEOMETRY=5;
	public static final int TYPE_OBJECT=6;
	
	private String name;
	private int type;
	private boolean primaryKey;
	private int size;
	private int precision;
	
	public String getName() {
		return name;
	}
	public int getType() {
		return type;
	}
	public boolean isPrimaryKey() {
		return primaryKey;
	}
	public int getSize() {
		return size;
	}
	public int getPrecision() {
		return precision;
	}
	
	@Override
	public String toString() {
		StringBuilder sb=new StringBuilder();
		sb.append("Attribute(name=").append(name);
		sb.append(",type=").append(type);
		sb.append(",primaryKey=").append(primaryKey);
		sb.append(",size=").append(size);
		sb.append(",precision=").append(precision);
		sb.append(")");
		return sb.toString();
	}
}
