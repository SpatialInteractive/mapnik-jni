package mapnik;

public class FreetypeEngine {
	public static native boolean registerFont(String filename);
	public static native boolean registerFonts(String dir, boolean recurse);
	public static boolean registerFonts(String dir) {
		return registerFonts(dir, false);
	}
	public static native boolean isFontFile(String filename);
}
