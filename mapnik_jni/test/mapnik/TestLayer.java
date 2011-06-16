package mapnik;

import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class TestLayer {
	@BeforeClass
	public static void initMapnik() {
		Mapnik.initialize();
	}

	@Test
	public void testCreate() {
		Layer layer=new Layer("test");
		assertEquals("test", layer.getName());
		layer.setName("test1");
		assertEquals("test1", layer.getName());
	}

	@Test
	public void testStyles() {
		Layer layer=new Layer("test");
		String[] styles;
		
		styles=layer.getStyles();
		assertEquals(0, styles.length);
		
		layer.setStyles(new String[] { "one", "two" });
		styles=layer.getStyles();
		assertEquals(2, styles.length);
		assertEquals("one", styles[0]);
		assertEquals("two", styles[1]);
	}


}
