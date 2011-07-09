package mapnik;

import java.util.Set;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

public class TestDatasource {
	public static final String SHAPEFILE_NAME="data/TM_WORLD_BORDERS_SIMPL_0.3_MOD";
	@BeforeClass
	public static void initMapnik() {
		Setup.initialize();
	}
	@AfterClass
	public static void tearDownMapnik() {
		Setup.tearDown();
	}

	@Test
	public void testBindAndTableDescriptors() {
		Parameters params=new Parameters();
		params.put("type", "shape");
		params.put("file", SHAPEFILE_NAME);
		
		Datasource ds=DatasourceCache.create(params, true);
		System.out.println("Bound datasource");
		
		LayerDescriptor ld=ds.getDescriptor();
		System.out.println("planet_osm_line descriptor:");
		System.out.println(ld.toString());
	}
	
	@Test
	public void testFeatures() {
		Parameters params=new Parameters();
		params.put("type", "shape");
		params.put("file", SHAPEFILE_NAME);
		
		Datasource ds=DatasourceCache.create(params, true);
		LayerDescriptor ld=ds.getDescriptor();
		
		Box2d bbox=new Box2d(-180, -85, 180, 85);
		
		Query query=new Query(bbox);
		for (AttributeDescriptor attr: ld.getDescriptors()) {
			query.addPropertyName(attr.getName());
		}
		
		FeatureSet fs=ds.features(query);
		System.out.println("FeatureSet queried");

		while (fs.next()) {
			StringBuilder sb=new StringBuilder();
			sb.append("Feature:");
			Set<String> names=fs.getPropertyNames();
			for (String name: names) {
				Object value=fs.getProperty(name);
				if (value==null) continue;
				sb.append(" ").append(name).append("=").append(value);
			}
			for (int i=0; i<fs.getGeometryCount(); i++) {
				sb.append("\n\t");
				System.out.println("Getting geometry");
				Geometry g=fs.getGeometry(i);
				System.out.println("Got geometry");
				if (g!=null) {
					System.out.println("Appending geometry");
					sb.append(g.toString());
					System.out.println("Done with geometry");
				}
			}
			
			System.out.println(sb.toString());
		}
	}
}
