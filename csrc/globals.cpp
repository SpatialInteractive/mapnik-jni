//// -- Globals

static bool initialized=false;
static jclass
	CLASS_NATIVEOBJECT,
	CLASS_MAP,
	CLASS_LAYER,
	CLASS_DATASOURCE,
	CLASS_DATASOURCE_CACHE,
	CLASS_FEATURE_TYPE_STYLE,
	CLASS_PROJECTION,
	CLASS_QUERY,
	CLASS_FEATURESET,
	CLASS_GEOMETRY,
	CLASS_IMAGE;
static jclass
	CLASS_STRING,
	CLASS_DOUBLE,
	CLASS_INTEGER,
	CLASS_HASHSET,
	CLASS_PARAMETERS,
	CLASS_BOX2D,
	CLASS_COLOR,
	CLASS_COORD,
	CLASS_LAYERDESCRIPTOR,
	CLASS_ATTRIBUTEDESCRIPTOR;
static jmethodID
	CTOR_NATIVEOBJECT,
	CTOR_PARAMETERS,
	METHOD_PARAMETERS_SET_STRING,
	METHOD_PARAMETERS_SET_INT,
	METHOD_PARAMETERS_SET_DOUBLE,
	METHOD_PARAMETERS_COPY_TO_NATIVE,
	METHOD_DOUBLE_VALUEOF,
	METHOD_INTEGER_VALUEOF,
	CTOR_HASHSET,
	METHOD_HASHSET_ADD,
	CTOR_LAYERDESCRIPTOR,
	METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR,
	CTOR_ATTRIBUTEDESCRIPTOR;
static jfieldID
	FIELD_PTR,
	FIELD_BOX2D_MINX,
	FIELD_BOX2D_MINY,
	FIELD_BOX2D_MAXX,
	FIELD_BOX2D_MAXY,

	FIELD_COLOR_RED,
	FIELD_COLOR_GREEN,
	FIELD_COLOR_BLUE,
	FIELD_COLOR_ALPHA,

	FIELD_COORD_X,
	FIELD_COORD_Y,

	FIELD_LAYERDESCRIPTOR_NAME,
	FIELD_LAYERDESCRIPTOR_ENCODING,

	FIELD_ATTRIBUTEDESCRIPTOR_NAME,
	FIELD_ATTRIBUTEDESCRIPTOR_TYPE,
	FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY,
	FIELD_ATTRIBUTEDESCRIPTOR_SIZE,
	FIELD_ATTRIBUTEDESCRIPTOR_PRECISION,

	FIELD_FEATURESET_FEATURE_PTR;


void throw_error(JNIEnv* env, const char* msg) {
	jclass clazz=env->FindClass("java/lang/Error");
	env->ThrowNew(clazz, msg);
}

void throw_runtime_exception(JNIEnv* env, const char* msg) {
	jclass clazz=env->FindClass("java/lang/RuntimeException");
	env->ThrowNew(clazz, msg);
}

void throw_java_exception(JNIEnv* env, std::exception& e) {
	jclass clazz=env->FindClass("java/lang/RuntimeException");
	env->ThrowNew(clazz, e.what());
}

#define ASSERT_INITIALIZED if (!initialized) throw_error(env, "Library not initialized")
#define TO_POINTER(ptr) ((void*)ptr)
#define FROM_POINTER(ptr) ((jlong)ptr)

inline static long ASSERT_LONG_POINTER(long ptr) {
	if (!ptr) {
		throw std::runtime_error("Object is no longer valid");
	}
	return ptr;
}

// Exception handling
#define PREAMBLE try {
#define TRAILER(v) } catch (std::exception& e) { throw_java_exception(env, e); return v;}
#define TRAILER_VOID } catch (std::exception& e) { throw_java_exception(env, e); return; }

// Pointer access macros
#define LOAD_OBJECT_POINTER(object) ((void*)(ASSERT_LONG_POINTER(env->GetLongField(object, FIELD_PTR))))
#define LOAD_MAP_POINTER(object) (static_cast<mapnik::Map*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_LAYER_POINTER(object) (static_cast<mapnik::layer*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_DATASOURCE_POINTER(object) (static_cast<mapnik::datasource_ptr*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURE_TYPE_STYLE_POINTER(object) (static_cast<mapnik::feature_type_style*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_PROJECTION_POINTER(object) (static_cast<mapnik::projection*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_QUERY_POINTER(object) (static_cast<mapnik::query*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURESET_POINTER(object) (static_cast<mapnik::featureset_ptr*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_FEATURE_POINTER(object) (static_cast<mapnik::feature_ptr*>(TO_POINTER(env->GetLongField(object, FIELD_FEATURESET_FEATURE_PTR))))
#define LOAD_GEOMETRY_POINTER(object) (static_cast<mapnik::geometry_type*>(LOAD_OBJECT_POINTER(object)))
#define LOAD_IMAGE_POINTER(object) (static_cast<mapnik::image_32*>(LOAD_OBJECT_POINTER(object)))

static void init_class(JNIEnv* env, const char* name, jclass& classref) {
	classref=(jclass)env->NewGlobalRef(env->FindClass(name));
	if (!classref) {
		std::string message("Unable to initialize class: ");
		throw std::runtime_error(message + name);
	}
}

static jfieldID lookup_field(JNIEnv* env, jclass c, const char* name, const char* sig)
{
	jfieldID ret=env->GetFieldID(c, name, sig);
	if (!ret) {
		std::string message("Unable to find field: ");
		throw std::runtime_error(message + name + ", Signature: " + sig);
	}
	return ret;
}

static jmethodID lookup_method(JNIEnv* env, jclass c, const char* name, const char* sig)
{
	jmethodID ret=env->GetMethodID(c, name, sig);
	if (!ret) {
		std::string message("Unable to find method: ");
		throw std::runtime_error(message + name + ", Signature: " + sig);
	}
	return ret;
}

static jmethodID lookup_static_method(JNIEnv* env, jclass c, const char* name, const char* sig)
{
	jmethodID ret=env->GetStaticMethodID(c, name, sig);
	if (!ret) {
		std::string message("Unable to find method: ");
		throw std::runtime_error(message + name + ", Signature: " + sig);
	}
	return ret;
}

static bool init_ids(JNIEnv* env) {
	// Load NativeObject classes
	init_class(env, "mapnik/NativeObject", CLASS_NATIVEOBJECT);
	init_class(env, "mapnik/MapDefinition", CLASS_MAP);
	init_class(env, "mapnik/Datasource", CLASS_DATASOURCE);
	init_class(env, "mapnik/DatasourceCache", CLASS_DATASOURCE_CACHE);
	init_class(env, "mapnik/Layer", CLASS_LAYER);
	init_class(env, "mapnik/FeatureTypeStyle", CLASS_FEATURE_TYPE_STYLE);
	init_class(env, "mapnik/Projection", CLASS_PROJECTION);
	init_class(env, "mapnik/Query", CLASS_QUERY);
	init_class(env, "mapnik/FeatureSet", CLASS_FEATURESET);
	init_class(env, "mapnik/Geometry", CLASS_GEOMETRY);
	init_class(env, "mapnik/Image", CLASS_IMAGE);

	// Ptr
	CTOR_NATIVEOBJECT=lookup_method(env, CLASS_NATIVEOBJECT, "<init>", "()V");
	FIELD_PTR=lookup_field(env, CLASS_NATIVEOBJECT, "ptr", "J");

	// FeatureSet
	FIELD_FEATURESET_FEATURE_PTR=lookup_field(env, CLASS_FEATURESET, "feature_ptr", "J");

	// String
	init_class(env, "java/lang/String", CLASS_STRING);

	// Integer
	init_class(env, "java/lang/Integer", CLASS_INTEGER);
	METHOD_INTEGER_VALUEOF=lookup_static_method(env, CLASS_INTEGER, "valueOf", "(I)Ljava/lang/Integer;");

	// Double
	init_class(env, "java/lang/Double", CLASS_DOUBLE);
	METHOD_DOUBLE_VALUEOF=lookup_static_method(env, CLASS_DOUBLE, "valueOf", "(D)Ljava/lang/Double;");

	// Parameters
	init_class(env, "mapnik/Parameters", CLASS_PARAMETERS);
	CTOR_PARAMETERS=lookup_method(env, CLASS_PARAMETERS, "<init>", "()V");
	METHOD_PARAMETERS_SET_STRING=lookup_method(env, CLASS_PARAMETERS, "setString", "(Ljava/lang/String;Ljava/lang/String;)V");
	METHOD_PARAMETERS_SET_INT=lookup_method(env, CLASS_PARAMETERS, "setInt", "(Ljava/lang/String;I)V");
	METHOD_PARAMETERS_SET_DOUBLE=lookup_method(env, CLASS_PARAMETERS, "setDouble", "(Ljava/lang/String;D)V");
	METHOD_PARAMETERS_COPY_TO_NATIVE=lookup_method(env, CLASS_PARAMETERS, "copyToNative", "(J)V");

	// HashSet
	init_class(env, "java/util/HashSet", CLASS_HASHSET);
	CTOR_HASHSET=lookup_method(env, CLASS_HASHSET, "<init>", "()V");
	METHOD_HASHSET_ADD=lookup_method(env, CLASS_HASHSET, "add", "(Ljava/lang/Object;)Z");

	// Box2d
	init_class(env, "mapnik/Box2d", CLASS_BOX2D);
	FIELD_BOX2D_MINX=lookup_field(env, CLASS_BOX2D, "minx", "D");
	FIELD_BOX2D_MINY=lookup_field(env, CLASS_BOX2D, "miny", "D");
	FIELD_BOX2D_MAXX=lookup_field(env, CLASS_BOX2D, "maxx", "D");
	FIELD_BOX2D_MAXY=lookup_field(env, CLASS_BOX2D, "maxy", "D");

	// Color
	init_class(env, "mapnik/Color", CLASS_COLOR);
	FIELD_COLOR_RED=lookup_field(env, CLASS_COLOR, "red", "I");
	FIELD_COLOR_GREEN=lookup_field(env, CLASS_COLOR, "green", "I");
	FIELD_COLOR_BLUE=lookup_field(env, CLASS_COLOR, "blue", "I");
	FIELD_COLOR_ALPHA=lookup_field(env, CLASS_COLOR, "alpha", "I");

	// Coord
	init_class(env, "mapnik/Coord", CLASS_COORD);
	FIELD_COORD_X=lookup_field(env, CLASS_COORD, "x", "D");
	FIELD_COORD_Y=lookup_field(env, CLASS_COORD, "y", "D");

	// LayerDescriptor
	init_class(env, "mapnik/LayerDescriptor", CLASS_LAYERDESCRIPTOR);
	CTOR_LAYERDESCRIPTOR=lookup_method(env, CLASS_LAYERDESCRIPTOR, "<init>", "()V");
	FIELD_LAYERDESCRIPTOR_NAME=lookup_field(env, CLASS_LAYERDESCRIPTOR, "name", "Ljava/lang/String;");
	FIELD_LAYERDESCRIPTOR_ENCODING=lookup_field(env, CLASS_LAYERDESCRIPTOR, "encoding", "Ljava/lang/String;");
	METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR=lookup_method(env, CLASS_LAYERDESCRIPTOR, "addDescriptor", "(Lmapnik/AttributeDescriptor;)V");

	// AttributeDescriptor
	init_class(env, "mapnik/AttributeDescriptor", CLASS_ATTRIBUTEDESCRIPTOR);
	CTOR_ATTRIBUTEDESCRIPTOR=lookup_method(env, CLASS_ATTRIBUTEDESCRIPTOR, "<init>", "()V");
	FIELD_ATTRIBUTEDESCRIPTOR_NAME=lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "name", "Ljava/lang/String;");
	FIELD_ATTRIBUTEDESCRIPTOR_TYPE=lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "type", "I");
	FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY=lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "primaryKey", "Z");
	FIELD_ATTRIBUTEDESCRIPTOR_SIZE=lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "size", "I");
	FIELD_ATTRIBUTEDESCRIPTOR_PRECISION=lookup_field(env, CLASS_ATTRIBUTEDESCRIPTOR, "precision", "I");

	return true;
}

class refjavastring {
	JNIEnv* env;
	jstring js;
	jboolean isCopy;
public:
	const char* stringz;
	bool isnull;

	refjavastring(JNIEnv* env, jstring js) {
		this->env=env;
		this->js=js;
		if (js) {
			stringz=env->GetStringUTFChars(js, &isCopy);
			isnull=false;
		} else {
			stringz="";
			isnull=true;
			isCopy=0;
		}
	}
	~refjavastring() {
		if (isCopy) {
			env->ReleaseStringUTFChars(js, stringz);
		}
	}
};

static jobject box2dFromNative(JNIEnv *env, mapnik::box2d<double> const& box) {
	jobject ret=env->AllocObject(CLASS_BOX2D);
	env->SetDoubleField(ret, FIELD_BOX2D_MINX, box.minx());
	env->SetDoubleField(ret, FIELD_BOX2D_MINY, box.miny());
	env->SetDoubleField(ret, FIELD_BOX2D_MAXX, box.maxx());
	env->SetDoubleField(ret, FIELD_BOX2D_MAXY, box.maxy());
	return ret;
}

static mapnik::box2d<double> box2dToNative(JNIEnv *env, jobject box) {
	mapnik::box2d<double> ret(
			env->GetDoubleField(box, FIELD_BOX2D_MINX),
			env->GetDoubleField(box, FIELD_BOX2D_MINY),
			env->GetDoubleField(box, FIELD_BOX2D_MAXX),
			env->GetDoubleField(box, FIELD_BOX2D_MAXY)
			);
	return ret;
}

static mapnik::color colorToNative(JNIEnv *env, jobject c) {
	return mapnik::color(
			env->GetIntField(c, FIELD_COLOR_RED),
			env->GetIntField(c, FIELD_COLOR_GREEN),
			env->GetIntField(c, FIELD_COLOR_BLUE),
			env->GetIntField(c, FIELD_COLOR_ALPHA)
			);
}

static jobject colorFromNative(JNIEnv *env, mapnik::color const& c) {
	jobject ret=env->AllocObject(CLASS_COLOR);
	env->SetIntField(ret, FIELD_COLOR_RED, c.red());
	env->SetIntField(ret, FIELD_COLOR_GREEN, c.green());
	env->SetIntField(ret, FIELD_COLOR_BLUE, c.blue());
	env->SetIntField(ret, FIELD_COLOR_ALPHA, c.alpha());
	return ret;
}
