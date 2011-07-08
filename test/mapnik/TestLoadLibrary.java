package mapnik;

import java.io.File;
import java.util.Arrays;

import org.junit.Test;
import static org.junit.Assert.*;

/**
 * Test library location and loading
 * @author stella
 *
 */
public class TestLoadLibrary {
	@Test
	public void testStaticSettings() {
		System.err.println("Search path=" + Arrays.toString(Mapnik.getLibrarySearchPath().toArray()));
		String expected=new File("build/dist").getAbsolutePath();
		assertEquals("Jar relative search path (will only succeed if testing from jar)", 
				expected, Mapnik.getLibrarySearchPath().get(0));
		
		Mapnik.initialize();
		assertEquals(new File(expected, System.mapLibraryName("mapnik-jni")).toString(), Mapnik.getLoadedLibrary());
	}
	
	@Test
	public void testPluginAndFontPaths() {
		System.err.println("Installed fonts dir=" + Mapnik.getInstalledFontsDir());
		System.err.println("Installed plugin dir=" + Mapnik.getInstalledInputPluginsDir());
		
		assertNotNull(Mapnik.getInstalledFontsDir());
		assertNotNull(Mapnik.getInstalledInputPluginsDir());
		
		assertTrue(new File(Mapnik.getInstalledFontsDir()).isDirectory());
		assertTrue(new File(Mapnik.getInstalledInputPluginsDir()).isDirectory());
	}
}
