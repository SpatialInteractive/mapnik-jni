package mapnik;

import java.util.Arrays;
import java.util.Set;

import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class TestMap {
	public static final String SRS_MERCATOR="+proj=merc +a=6378137 +b=6378137 +lat_ts=0.0 +lon_0=0.0 +x_0=0.0 +y_0=0 +k=1.0 +units=m +nadgrids=@null +no_defs +over";
	@BeforeClass
	public static void initMapnik() {
		System.out.println("Initializing mapnik");
		Mapnik.initialize();
	}
	
	@Test
	public void testAlloc() {
		Map map=new Map();
	}
	
	@Test
	public void testConstructorSettings() {
		Map map;
		
		map=new Map();
		assertEquals(400, map.getWidth());
		assertEquals(400, map.getHeight());
		assertEquals(Mapnik.SRS_DEFAULT, map.getSrs());
		
		map=new Map(500, 500, SRS_MERCATOR);
		assertEquals(500, map.getWidth());
		assertEquals(500, map.getHeight());
		assertEquals(SRS_MERCATOR, map.getSrs());
	}
	
	@Test
	public void testSize() {
		Map map=new Map();
		map.setWidth(500);
		map.setHeight(600);
		assertEquals(500, map.getWidth());
		assertEquals(600, map.getHeight());
		
		map.resize(300, 200);
		assertEquals(300, map.getWidth());
		assertEquals(200, map.getHeight());
	}
	
	@Test
	public void testSrs() {
		Map map=new Map();
		map.setSrs(SRS_MERCATOR);
		assertEquals(SRS_MERCATOR, map.getSrs());
	}
	
	@Test
	public void testBufferSize() {
		Map map=new Map();
		assertEquals(0, map.getBufferSize());
		map.setBufferSize(256);
		assertEquals(256, map.getBufferSize());
	}
	
	@Test
	public void testBasePath() {
		Map map=new Map();
		assertEquals("", map.getBasePath());
		map.setBasePath("/tmp");
		assertEquals("/tmp", map.getBasePath());
	}
	
	@Test
	public void testNullHandling() {
		String mapXml="<Map buffer-size='256'></Map>";
		Map map=new Map();
		map.loadMapString(mapXml, false, null);
		assertEquals("", map.getBasePath());
	}
	
	@Test
	public void testMapLoad() {
		String mapXml="<Map buffer-size='256'></Map>";
		Map map=new Map();
		map.loadMapString(mapXml, false, "");
		
		assertEquals(256, map.getBufferSize());
	}
	
	@Test
	public void testStyle() {
		Map map=new Map();
		assertNull(map.getStyle("notexist"));
		assertEquals(0, map.getStyleNames().size());
		map.removeStyle("notexist");
		
		FeatureTypeStyle style=new FeatureTypeStyle();
		map.addStyle("test", style);
		assertEquals(1, map.getStyleNames().size());
		assertTrue(map.getStyleNames().contains("test"));
		assertNotNull(map.getStyle("test"));
	}
	
	@Test
	public void testCollectAttributes() {
		Map map=new Map();
		map.loadMap("test/testmap.xml", false);
		FeatureTypeStyle style=map.getStyle("landmark-poly-text");
		
		String[] attrs=style.collectAttributes().toArray(new String[0]);
		Arrays.sort(attrs);
		
		System.out.println("Attrs: " + Arrays.toString(attrs));
		assertTrue(Arrays.equals(attrs, new String[] {"landuse", "leisure", "name", "way_area" }));
	}
}
