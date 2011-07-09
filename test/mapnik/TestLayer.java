package mapnik;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class TestLayer {
	@BeforeClass
	public static void initMapnik() {
		Setup.initialize();
	}
	@AfterClass
	public static void tearDownMapnik() {
		Setup.tearDown();
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

	@Test
	public void testMapLayers() {
		MapDefinition map=new MapDefinition();
		assertEquals(0, map.getLayerCount());
		
		Layer layer;
		
		layer=new Layer("test1");
		map.addLayer(layer);
		assertEquals(1, map.getLayerCount());
		assertEquals("test1", map.getLayer(0).getName());
		
		// layer is a shallow copy - verify
		layer.setName("blah");
		assertEquals("test1", map.getLayer(0).getName());
		
		// Update it
		map.setLayer(0, layer);
		assertEquals("blah", map.getLayer(0).getName());
		
		map.removeLayer(0);
		assertEquals(0, map.getLayerCount());
	}
	
	@Test
	public void testDatasource() {
		Layer layer=new Layer("test");
		assertNull(layer.getDatasource());
		
		Parameters params=new Parameters();
		params.put("type", "postgis");
		params.put("table", "test");
		
		Datasource ds=DatasourceCache.create(params, false);
		layer.setDatasource(ds);
		assertNotNull(layer.getDatasource());
		
		params=ds.getParameters();
		assertEquals("postgis", params.get("type"));
		assertEquals("test", params.get("table"));
	}
}
