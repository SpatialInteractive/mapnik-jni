package mapnik;

import java.util.Set;

import org.junit.BeforeClass;
import org.junit.Test;

public class TestDatasource {
	@BeforeClass
	public static void initMapnik() {
		TestBase.initialize();
	}

	@Test
	public void testBindAndTableDescriptors() {
		Parameters params=new Parameters();
		params.put("type", "postgis");
		params.put("host", "localhost");
		params.put("port", 5432);
		params.put("user", "stella");
		params.put("dbname", "stella");
		params.put("table", "planet_osm_line");
		
		Datasource ds=DatasourceCache.create(params, true);
		System.out.println("Bound datasource");
		
		LayerDescriptor ld=ds.getDescriptor();
		System.out.println("planet_osm_line descriptor:");
		System.out.println(ld.toString());
	}
	
	@Test
	public void testBindAndSubqueryDescriptors() {
		Parameters params=new Parameters();
		params.put("type", "postgis");
		params.put("host", "localhost");
		params.put("port", 5432);
		params.put("user", "stella");
		params.put("dbname", "stella");
		params.put("table", "(select way,landuse from planet_osm_polygon where landuse in ('military') order by z_order,way_area desc) as military");
		
		Datasource ds=DatasourceCache.create(params, true);
		System.out.println("Bound datasource");
		LayerDescriptor ld=ds.getDescriptor();
		System.out.println("subquery  descriptor:");
		System.out.println(ld.toString());
	}
	
	@Test
	public void testFeatures() {
		Parameters params=new Parameters();
		params.put("type", "postgis");
		params.put("host", "localhost");
		params.put("port", 5432);
		params.put("user", "stella");
		params.put("dbname", "stella");
		params.put("table", "planet_osm_line");
		params.put("row_limit", 10);
		//params.put("cursor_size", "100");
		
		Datasource ds=DatasourceCache.create(params, true);
		LayerDescriptor ld=ds.getDescriptor();
		
		Box2d bbox=new Box2d(-180, -85, 180, 85);
		Projection prj=new Projection(Projection.SRS900913_PARAMS);
		prj.forward(bbox);
		
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
