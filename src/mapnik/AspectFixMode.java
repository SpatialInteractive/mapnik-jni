package mapnik;

/**
 * Mirror of the aspect_fix_mode C++ enum
 * @author stella
 *
 */
public enum AspectFixMode {
	GROW_BBOX(0),
	GROW_CANVAS(1),
	SHRINK_BBOX(2),
	SHRINK_CANVAS(3),
	ADJUST_BBOX_WIDTH(4),
	ADJUST_BBOX_HEIGHT(5),
	ADJUST_CANVAS_WIDTH(6),
	ADJUST_CANVAS_HEIGHT(7);
	
	AspectFixMode(int nativeCode) {
		this.nativeCode=nativeCode;
	}
	public final int nativeCode;
	
	public static AspectFixMode fromNativeCode(int nativeCode) {
		for (AspectFixMode m: values()) {
			if (m.nativeCode==nativeCode) return m;
		}
		throw new IllegalArgumentException("Cannot convert code " + nativeCode + " to AspectFixMode");
	}
}
