package mapnik;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

public class TestProjection {
	@BeforeClass
	public static void initMapnik() {
		Mapnik.initialize();
	}
	@AfterClass
	public static void tearDownMapnik() {
		Setup.tearDown();
	}


	@Test
	public void testForward() {
		Projection prj=new Projection(Projection.LATLNG_PARAMS);
		Coord c=new Coord(10,10);
		prj.forward(c);
		assertEquals(10, (int)c.x);
		assertEquals(10, (int)c.y);
	}
	@Test
	public void testInverse() {
		Projection prj=new Projection(Projection.LATLNG_PARAMS);
		Coord c=new Coord(10,10);
		prj.inverse(c);
		assertEquals(10, (int)c.x);
		assertEquals(10, (int)c.y);
	}
	@Test
	public void testParams() {
		Projection prj=new Projection(Projection.LATLNG_PARAMS);
		assertEquals(Projection.LATLNG_PARAMS, prj.getParams());
	}
}
