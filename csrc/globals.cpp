//// -- Globals
struct classinfo_t {
	jclass java_class;
	jfieldID ptr_field;
	jfieldID parentref_field;
};

static bool initialized=false;
static classinfo_t
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
#define LOAD_MAP_POINTER(mapobj) (static_cast<mapnik::Map*>((void*)(env->GetLongField(mapobj, CLASS_MAP.ptr_field))))
#define LOAD_LAYER_POINTER(layerobj) (static_cast<mapnik::layer*>((void*)(env->GetLongField(layerobj, CLASS_LAYER.ptr_field))))
#define LOAD_DATASOURCE_POINTER(dsobj) (static_cast<mapnik::datasource_ptr*>((void*)(env->GetLongField(dsobj, CLASS_DATASOURCE.ptr_field))))
#define LOAD_FEATURE_TYPE_STYLE_POINTER(styleobj) (static_cast<mapnik::feature_type_style*>((void*)(env->GetLongField(styleobj, CLASS_FEATURE_TYPE_STYLE.ptr_field))))
#define LOAD_PROJECTION_POINTER(projobj) (static_cast<mapnik::projection*>((void*)(env->GetLongField(projobj, CLASS_PROJECTION.ptr_field))))
#define LOAD_QUERY_POINTER(qobj) (static_cast<mapnik::query*>((void*)(env->GetLongField(qobj, CLASS_QUERY.ptr_field))))
#define LOAD_FEATURESET_POINTER(fsobj) (static_cast<mapnik::featureset_ptr*>((void*)(env->GetLongField(fsobj, CLASS_FEATURESET.ptr_field))))
#define LOAD_FEATURE_POINTER(fsobj) (static_cast<mapnik::feature_ptr*>((void*)(env->GetLongField(fsobj, FIELD_FEATURESET_FEATURE_PTR))))
#define LOAD_GEOMETRY_POINTER(gobj) (static_cast<mapnik::geometry_type*>((void*)(ASSERT_LONG_POINTER(env->GetLongField(gobj, CLASS_GEOMETRY.ptr_field)))))
#define LOAD_IMAGE_POINTER(iobj) (static_cast<mapnik::image_32*>((void*)(ASSERT_LONG_POINTER(env->GetLongField(iobj, CLASS_IMAGE.ptr_field)))))

static bool init_class(JNIEnv* env, const char* name, classinfo_t& ci, bool has_parentref) {
	//printf("Initing %s\n", name);

	ci.ptr_field=0;
	ci.parentref_field=0;
	ci.java_class=(jclass)env->NewGlobalRef(env->FindClass(name));
	if (!ci.java_class) return false;

	ci.ptr_field=env->GetFieldID(ci.java_class, "ptr", "J");
	if (!ci.ptr_field) return false;

	if (has_parentref) {
		ci.parentref_field=env->GetFieldID(ci.java_class, "parentref", "Ljava/lang/Object;");
		if (!ci.parentref_field) {
			return false;
		}
	}

	//printf("Inited %s\n", name);
	return true;
}

static bool init_ids(JNIEnv* env) {
	// Load classes
	if (!(
		init_class(env, "mapnik/Map", CLASS_MAP, false) &&
		init_class(env, "mapnik/Datasource", CLASS_DATASOURCE, false) &&
		init_class(env, "mapnik/DatasourceCache", CLASS_DATASOURCE_CACHE, false) &&
		init_class(env, "mapnik/Layer", CLASS_LAYER, false) &&
		init_class(env, "mapnik/FeatureTypeStyle", CLASS_FEATURE_TYPE_STYLE, false) &&
		init_class(env, "mapnik/Projection", CLASS_PROJECTION, false) &&
		init_class(env, "mapnik/Query", CLASS_QUERY, false) &&
		init_class(env, "mapnik/FeatureSet", CLASS_FEATURESET, false) &&
		init_class(env, "mapnik/Geometry", CLASS_GEOMETRY, false) &&
		init_class(env, "mapnik/Image", CLASS_IMAGE, false)
		)) {
		throw_error(env, "Error initializing native references");
		return false;
	}

	// FeatureSet
	FIELD_FEATURESET_FEATURE_PTR=env->GetFieldID(CLASS_FEATURESET.java_class, "feature_ptr", "J");

	// String
	CLASS_STRING=(jclass)env->NewGlobalRef(env->FindClass("java/lang/String"));

	// Integer
	CLASS_INTEGER=(jclass)env->NewGlobalRef(env->FindClass("java/lang/Integer"));
	METHOD_INTEGER_VALUEOF=env->GetStaticMethodID(CLASS_INTEGER, "valueOf", "(I)Ljava/lang/Integer;");

	// Double
	CLASS_DOUBLE=(jclass)env->NewGlobalRef(env->FindClass("java/lang/Double"));
	METHOD_DOUBLE_VALUEOF=env->GetStaticMethodID(CLASS_DOUBLE, "valueOf", "(D)Ljava/lang/Double;");

	// Parameters
	CLASS_PARAMETERS=(jclass)env->NewGlobalRef(env->FindClass("mapnik/Parameters"));
	CTOR_PARAMETERS=env->GetMethodID(CLASS_PARAMETERS, "<init>", "()V");
	METHOD_PARAMETERS_SET_STRING=env->GetMethodID(CLASS_PARAMETERS, "setString", "(Ljava/lang/String;Ljava/lang/String;)V");
	METHOD_PARAMETERS_SET_INT=env->GetMethodID(CLASS_PARAMETERS, "setInt", "(Ljava/lang/String;I)V");
	METHOD_PARAMETERS_SET_DOUBLE=env->GetMethodID(CLASS_PARAMETERS, "setDouble", "(Ljava/lang/String;D)V");
	METHOD_PARAMETERS_COPY_TO_NATIVE=env->GetMethodID(CLASS_PARAMETERS, "copyToNative", "(J)V");

	// HashSet
	CLASS_HASHSET=(jclass)env->NewGlobalRef(env->FindClass("java/util/HashSet"));
	CTOR_HASHSET=env->GetMethodID(CLASS_HASHSET, "<init>", "()V");
	METHOD_HASHSET_ADD=env->GetMethodID(CLASS_HASHSET, "add", "(Ljava/lang/Object;)Z");

	// Box2d
	CLASS_BOX2D=(jclass)env->NewGlobalRef(env->FindClass("mapnik/Box2d"));
	FIELD_BOX2D_MINX=env->GetFieldID(CLASS_BOX2D, "minx", "D");
	FIELD_BOX2D_MINY=env->GetFieldID(CLASS_BOX2D, "miny", "D");
	FIELD_BOX2D_MAXX=env->GetFieldID(CLASS_BOX2D, "maxx", "D");
	FIELD_BOX2D_MAXY=env->GetFieldID(CLASS_BOX2D, "maxy", "D");

	// Color
	CLASS_COLOR=(jclass)env->NewGlobalRef(env->FindClass("mapnik/Color"));
	FIELD_COLOR_RED=env->GetFieldID(CLASS_COLOR, "red", "I");
	FIELD_COLOR_GREEN=env->GetFieldID(CLASS_COLOR, "green", "I");
	FIELD_COLOR_BLUE=env->GetFieldID(CLASS_COLOR, "blue", "I");
	FIELD_COLOR_ALPHA=env->GetFieldID(CLASS_COLOR, "alpha", "I");

	// Coord
	CLASS_COORD=(jclass)env->NewGlobalRef(env->FindClass("mapnik/Coord"));
	FIELD_COORD_X=env->GetFieldID(CLASS_COORD, "x", "D");
	FIELD_COORD_Y=env->GetFieldID(CLASS_COORD, "y", "D");

	// LayerDescriptor
	CLASS_LAYERDESCRIPTOR=(jclass)env->NewGlobalRef(env->FindClass("mapnik/LayerDescriptor"));
	CTOR_LAYERDESCRIPTOR=env->GetMethodID(CLASS_LAYERDESCRIPTOR, "<init>", "()V");
	FIELD_LAYERDESCRIPTOR_NAME=env->GetFieldID(CLASS_LAYERDESCRIPTOR, "name", "Ljava/lang/String;");
	FIELD_LAYERDESCRIPTOR_ENCODING=env->GetFieldID(CLASS_LAYERDESCRIPTOR, "encoding", "Ljava/lang/String;");
	METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR=env->GetMethodID(CLASS_LAYERDESCRIPTOR, "addDescriptor", "(Lmapnik/AttributeDescriptor;)V");

	// AttributeDescriptor
	CLASS_ATTRIBUTEDESCRIPTOR=(jclass)env->NewGlobalRef(env->FindClass("mapnik/AttributeDescriptor"));
	CTOR_ATTRIBUTEDESCRIPTOR=env->GetMethodID(CLASS_ATTRIBUTEDESCRIPTOR, "<init>", "()V");
	FIELD_ATTRIBUTEDESCRIPTOR_NAME=env->GetFieldID(CLASS_ATTRIBUTEDESCRIPTOR, "name", "Ljava/lang/String;");
	FIELD_ATTRIBUTEDESCRIPTOR_TYPE=env->GetFieldID(CLASS_ATTRIBUTEDESCRIPTOR, "type", "I");
	FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY=env->GetFieldID(CLASS_ATTRIBUTEDESCRIPTOR, "primaryKey", "Z");
	FIELD_ATTRIBUTEDESCRIPTOR_SIZE=env->GetFieldID(CLASS_ATTRIBUTEDESCRIPTOR, "size", "I");
	FIELD_ATTRIBUTEDESCRIPTOR_PRECISION=env->GetFieldID(CLASS_ATTRIBUTEDESCRIPTOR, "precision", "I");

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
