package mapnik;

import java.util.ArrayList;
import java.util.List;

/**
 * Read-only access to a mapnik::layer_descriptor on a Datasource.
 * As a value type, this class has Java fields that mirror the native fields.
 * @author stella
 *
 */
public class LayerDescriptor {
	private String name;
	private String encoding;
	private ArrayList<AttributeDescriptor> descriptors=new ArrayList<AttributeDescriptor>();
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getEncoding() {
		return encoding;
	}
	public void setEncoding(String encoding) {
		this.encoding = encoding;
	}
	public List<AttributeDescriptor> getDescriptors() {
		return descriptors;
	}
	public void addDescriptor(AttributeDescriptor desc) {
		descriptors.add(desc);
	}
}
