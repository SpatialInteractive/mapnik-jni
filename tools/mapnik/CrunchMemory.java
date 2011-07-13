package mapnik;

import java.io.File;
import java.io.IOException;
import java.util.Random;

/**
 * Workbench program that I use for trying to force certain memory conditions
 *  
 * @author stella
 *
 */
public class CrunchMemory {
	private static Random random=new Random();
	
	public static int runRender(MapDefinition m, Box2d extent, int width, int height) {
		m.resize(width, height);
		m.zoomToBox(extent);
		m.setBufferSize(128);
		
		Image image=new Image(width, height);
		Renderer.renderAgg(m, image);
		
		byte[] contents=image.saveToMemory("png");
		image.dispose();
		return contents.length;
	}

	private static void runProjection(String srs, Box2d extent) {
		Projection prj=new Projection(srs);
		Box2d other=new Box2d(extent);
		prj.inverse(other);
		prj.dispose();
	}
	
	public static MapDefinition loadMap(File file) throws IOException {
		MapDefinition m=new MapDefinition();
		m.loadMap(file.getCanonicalPath(), false);
		return m;
	}
	
	public static void randomizeExtent(Box2d extent) {
		int magnitude=1000000;
		int offset=random.nextInt(magnitude) - magnitude /2;
		extent.minx+=offset;
		extent.miny+=offset;
		extent.maxx+=offset;
		extent.maxy+=offset;
	}
	
	public static void main(String[] args) throws IOException {
		Mapnik.initialize();
		File file=new File("data/memorytest.mapnik.xml");
		MapDefinition m=loadMap(file);
		Box2d extent=new Box2d(-13614352.0,6044028.7,-13613129.0,6045251.7);
		
		int count=0;
		while (true) {
			m=loadMap(file);
			Box2d currentExtent=new Box2d(extent);
			randomizeExtent(currentExtent);
			
			runProjection(m.getSrs(), extent);
			runRender(m, extent, 256, 256);
			count++;
			if ((count%1000)==0) {
				System.out.println("Completed " + count + " iterations");
			}
			m.dispose();
		}
	}
}
