#include <jni.h>
#include <mapnik/map.hpp>
#include <mapnik/load_map.hpp>
#include <mapnik/layer.hpp>

#include "mapnikjni.h"

//// -- Mapnik class and globals
struct classinfo_t {
	jclass java_class;
	jfieldID ptr_field;
	jfieldID parentref_field;
};

static bool initialized=false;
static classinfo_t
	CLASS_MAP,
	CLASS_LAYER,
	CLASS_DATASOURCE_CACHE;
static jclass
	CLASS_STRING;

void throw_error(JNIEnv* env, const char* msg) {
	jclass clazz=env->FindClass("java/lang/Error");
	env->ThrowNew(clazz, msg);
}

#define ASSERT_INITIALIZED if (!initialized) throw_error(env, "Library not initialized")
#define TO_POINTER(ptr) ((void*)ptr)
#define FROM_POINTER(ptr) ((jlong)ptr)

// Pointer access macros
#define LOAD_MAP_POINTER(mapobj) (static_cast<mapnik::Map*>((void*)(env->GetLongField(mapobj, CLASS_MAP.ptr_field))))
#define LOAD_LAYER_POINTER(layerobj) (static_cast<mapnik::layer*>((void*)(env->GetLongField(layerobj, CLASS_LAYER.ptr_field))))

bool init_class(JNIEnv* env, const char* name, classinfo_t& ci, bool has_parentref) {
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

/*
 * Class:     mapnik_Mapnik
 * Method:    nativeInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Mapnik_nativeInit
  (JNIEnv *env, jclass c)
{
	if (initialized) return;

	// Load classes
	if (!(
		init_class(env, "mapnik/Map", CLASS_MAP, false) &&
		init_class(env, "mapnik/DatasourceCache", CLASS_DATASOURCE_CACHE, false) &&
		init_class(env, "mapnik/Layer", CLASS_LAYER, false)
		)) {
		throw_error(env, "Error initializing native references");
		return;
	}

	CLASS_STRING=(jclass)env->NewGlobalRef(env->FindClass("java/lang/String"));

	initialized=true;
}

//// --- Map class members
/*
 * Class:     mapnik_Map
 * Method:    alloc
 * Signature: (IILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Map_alloc__IILjava_lang_String_2
  (JNIEnv *env, jclass c, jint width, jint height, jstring srsj)
{
	ASSERT_INITIALIZED;
	refjavastring srs(env, srsj);

	return FROM_POINTER(new mapnik::Map(width, height, srs.stringz));
}

/*
 * Class:     mapnik_Map
 * Method:    alloc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Map_alloc__
  (JNIEnv *env, jclass c)
{
	ASSERT_INITIALIZED;
	return FROM_POINTER(new mapnik::Map());
}

/*
 * Class:     mapnik_Map
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_dealloc
  (JNIEnv *env, jclass c, jlong ptr)
{
	if (ptr) {
		delete static_cast<mapnik::Map*>(TO_POINTER(ptr));
	}
}

/*
 * Class:     mapnik_Map
 * Method:    loadMap
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_loadMap
  (JNIEnv *, jobject, jstring, jboolean);

/*
 * Class:     mapnik_Map
 * Method:    loadMapString
 * Signature: (Ljava/lang/String;ZLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_loadMapString
  (JNIEnv *env, jobject mapobject, jstring strj, jboolean strict, jstring basepathj)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring str(env, strj);
	refjavastring basepath(env, basepathj);
	mapnik::load_map_string(*map, str.stringz, (bool)strict, basepath.stringz);
}

/*
 * Class:     mapnik_Map
 * Method:    getLayerCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getLayerCount
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->layer_count();
}

/*
 * Class:     mapnik_Map
 * Method:    getLayer
 * Signature: (I)Lmapnik/Layer;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getLayer
  (JNIEnv *env, jobject mapobject, jint index)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	std::vector<mapnik::layer> &layers(map->layers());
	if (index<0 || index>layers.size()) {
		return 0;
	}

	mapnik::layer* layer_copy=new mapnik::layer(layers[index]);
	jobject layer=env->AllocObject(CLASS_LAYER.java_class);
	env->SetLongField(layer, CLASS_LAYER.ptr_field, FROM_POINTER(layer_copy));
	return layer;
}

/*
 * Class:     mapnik_Map
 * Method:    setLayer
 * Signature: (ILmapnik/Layer;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setLayer
  (JNIEnv *env, jobject mapobject, jint index, jobject layerobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	std::vector<mapnik::layer> &layers(map->layers());
	if (index<0 || index>layers.size()) {
		return;
	}

	if (!layerobject) return;
	mapnik::layer* layer=
			static_cast<mapnik::layer*>(TO_POINTER(env->GetLongField(layerobject, CLASS_LAYER.ptr_field)));
	layers[index]=*layer;
}


/*
 * Class:     mapnik_Map
 * Method:    removeLayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_removeLayer
  (JNIEnv *env, jobject mapobject, jint index)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->removeLayer(index);
}

/*
 * Class:     mapnik_Map
 * Method:    addLayer
 * Signature: (Lmapnik/Layer;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_addLayer
  (JNIEnv *env, jobject mapobject, jobject layerobject)
{
	if (!layerobject) return;

	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	mapnik::layer* layer=
			static_cast<mapnik::layer*>(TO_POINTER(env->GetLongField(layerobject, CLASS_LAYER.ptr_field)));

	map->addLayer(*layer);
}

/*
 * Class:     mapnik_Map
 * Method:    removeAllLayers
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_removeAllLayers
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	std::vector<mapnik::layer> &layers(map->layers());
	layers.clear();
}

/*
 * Class:     mapnik_Map
 * Method:    getWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getWidth
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->width();
}

/*
 * Class:     mapnik_Map
 * Method:    getHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getHeight
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->height();
}

/*
 * Class:     mapnik_Map
 * Method:    setWidth
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setWidth
  (JNIEnv *env, jobject mapobject, jint width)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_width(width);
}

/*
 * Class:     mapnik_Map
 * Method:    setHeight
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setHeight
  (JNIEnv *env, jobject mapobject, jint height)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_height(height);
}

/*
 * Class:     mapnik_Map
 * Method:    resize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_resize
  (JNIEnv *env, jobject mapobject, jint width, jint height)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->resize(width, height);
}

/*
 * Class:     mapnik_Map
 * Method:    getSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Map_getSrs
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return env->NewStringUTF(map->srs().c_str());
}

/*
 * Class:     mapnik_Map
 * Method:    setSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Map_setSrs
  (JNIEnv *env, jobject mapobject, jstring srsj)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring srs(env, srsj);

	map->set_srs(srs.stringz);
}

/*
 * Class:     mapnik_Map
 * Method:    setBufferSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setBufferSize
  (JNIEnv *env, jobject mapobject, jint s)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_buffer_size(s);
}

/*
 * Class:     mapnik_Map
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getBufferSize
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->buffer_size();
}

/*
 * Class:     mapnik_Map
 * Method:    getBasePath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Map_getBasePath
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return env->NewStringUTF(map->base_path().c_str());
}

/*
 * Class:     mapnik_Map
 * Method:    setBasePath
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setBasePath
  (JNIEnv *env, jobject mapobject, jstring basepathj)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring basepath(env, basepathj);
	map->set_base_path(basepath.stringz);
}

//// -- Layer class
/*
 * Class:     mapnik_Layer
 * Method:    alloc
 * Signature: (Ljava/lang/String;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Layer_alloc
  (JNIEnv *env, jclass c, jstring namej, jstring srsj)
{
	ASSERT_INITIALIZED;

	refjavastring name(env, namej);
	refjavastring srs(env, srsj);

	mapnik::layer* layer=new mapnik::layer(name.stringz, srs.stringz);
	return FROM_POINTER(layer);
}

/*
 * Class:     mapnik_Layer
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_dealloc
  (JNIEnv *env, jclass c, jlong ptr)
{
	if (ptr) {
		delete static_cast<mapnik::layer*>(TO_POINTER(ptr));
	}
}

/*
 * Class:     mapnik_Layer
 * Method:    getName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Layer_getName
  (JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return env->NewStringUTF(layer->name().c_str());
}

/*
 * Class:     mapnik_Layer
 * Method:    setName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setName
  (JNIEnv *env, jobject layerobj, jstring sj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	refjavastring s(env, sj);
	layer->set_name(s.stringz);
}

/*
 * Class:     mapnik_Layer
 * Method:    getTitle
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Layer_getTitle
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return env->NewStringUTF(layer->title().c_str());
}

/*
 * Class:     mapnik_Layer
 * Method:    setTitle
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setTitle
(JNIEnv *env, jobject layerobj, jstring sj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	refjavastring s(env, sj);
	layer->set_title(s.stringz);
}

/*
 * Class:     mapnik_Layer
 * Method:    getAbstract
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Layer_getAbstract
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return env->NewStringUTF(layer->abstract().c_str());
}

/*
 * Class:     mapnik_Layer
 * Method:    setAbstract
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setAbstract
(JNIEnv *env, jobject layerobj, jstring sj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	refjavastring s(env, sj);
	layer->set_abstract(s.stringz);
}

/*
 * Class:     mapnik_Layer
 * Method:    getSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Layer_getSrs
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return env->NewStringUTF(layer->srs().c_str());
}

/*
 * Class:     mapnik_Layer
 * Method:    setSrs
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setSrs
(JNIEnv *env, jobject layerobj, jstring sj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	refjavastring s(env, sj);
	layer->set_srs(s.stringz);
}

/*
 * Class:     mapnik_Layer
 * Method:    getStyles
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_Layer_getStyles
  (JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	std::vector<std::string> &styles(layer->styles());

	jobjectArray ary=env->NewObjectArray(
			styles.size(),
			CLASS_STRING,
			(jobject)0);

	for (unsigned i=0; i<styles.size(); i++) {
		env->SetObjectArrayElement(ary, i,
				env->NewStringUTF(styles[i].c_str()));
	}

	return ary;
}

/*
 * Class:     mapnik_Layer
 * Method:    setStyles
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setStyles
  (JNIEnv *env, jobject layerobj, jobjectArray ary)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	std::vector<std::string> &styles(layer->styles());
	styles.clear();

	if (ary) {
		jsize size=env->GetArrayLength(ary);
		for (jsize i=0; i<size; i++) {
			jobject element=env->GetObjectArrayElement(ary, i);
			refjavastring s(env, (jstring)element);
			styles.push_back(s.stringz);
		}
	}
}

/*
 * Class:     mapnik_Layer
 * Method:    getMinZoom
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Layer_getMinZoom
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return layer->getMinZoom();
}

/*
 * Class:     mapnik_Layer
 * Method:    setMinZoom
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setMinZoom
  (JNIEnv *env, jobject layerobj, jdouble z)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	layer->setMinZoom(z);
}

/*
 * Class:     mapnik_Layer
 * Method:    getMaxZoom
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Layer_getMaxZoom
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return layer->getMaxZoom();
}

/*
 * Class:     mapnik_Layer
 * Method:    setMaxZoom
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setMaxZoom
(JNIEnv *env, jobject layerobj, jdouble z)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	layer->setMaxZoom(z);
}

/*
 * Class:     mapnik_Layer
 * Method:    isActive
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isActive
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return (jboolean)layer->isActive();
}

/*
 * Class:     mapnik_Layer
 * Method:    setActive
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setActive
  (JNIEnv *env, jobject layerobj, jboolean b)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	layer->setActive((bool)b);
}


/*
 * Class:     mapnik_Layer
 * Method:    isQueryable
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isQueryable
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return (jboolean)layer->isQueryable();
}

/*
 * Class:     mapnik_Layer
 * Method:    setQueryable
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setQueryable
(JNIEnv *env, jobject layerobj, jboolean b)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	layer->setQueryable((bool)b);
}

/*
 * Class:     mapnik_Layer
 * Method:    isVisible
 * Signature: (D)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isVisible
  (JNIEnv *env, jobject layerobj, jdouble scale)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return layer->isVisible(scale);
}

/*
 * Class:     mapnik_Layer
 * Method:    isClearLabelCache
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isClearLabelCache
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return (jboolean)layer->clear_label_cache();
}

/*
 * Class:     mapnik_Layer
 * Method:    setClearLabelCache
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setClearLabelCache
(JNIEnv *env, jobject layerobj, jboolean b)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	layer->set_clear_label_cache((bool)b);
}

/*
 * Class:     mapnik_Layer
 * Method:    isCacheFeatures
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_Layer_isCacheFeatures
(JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	return (jboolean)layer->cache_features();
}

/*
 * Class:     mapnik_Layer
 * Method:    setCacheFeatures
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setCacheFeatures
(JNIEnv *env, jobject layerobj, jboolean b)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	layer->set_cache_features((bool)b);
}
