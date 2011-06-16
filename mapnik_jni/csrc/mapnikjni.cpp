#include <jni.h>
#include <mapnik/map.hpp>
#include <mapnik/load_map.hpp>
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
	CLASS_DATASOURCE_CACHE;

void throw_error(JNIEnv* env, const char* msg) {
	jclass clazz=env->FindClass("java/lang/Error");
	env->ThrowNew(clazz, msg);
}

#define ASSERT_INITIALIZED if (!initialized) throw_error(env, "Library not initialized")
#define TO_POINTER(ptr) ((void*)ptr)
#define FROM_POINTER(ptr) ((jlong)ptr)

// Pointer access macros
#define LOAD_MAP_POINTER(map) (static_cast<mapnik::Map*>((void*)(env->GetLongField(map, CLASS_MAP.ptr_field))))

bool init_class(JNIEnv* env, const char* name, classinfo_t& ci, bool has_parentref) {
	ci.ptr_field=0;
	ci.parentref_field=0;
	ci.java_class=env->FindClass(name);
	if (!ci.java_class) return false;

	ci.ptr_field=env->GetFieldID(ci.java_class, "ptr", "J");
	if (!ci.ptr_field) return false;

	if (has_parentref) {
		ci.parentref_field=env->GetFieldID(ci.java_class, "parentref", "Ljava/lang/Object;");
		if (!ci.parentref_field) return false;
	}

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
		init_class(env, "mapnik/Map", CLASS_MAP, false) ||
		init_class(env, "mapnik/DatasourceCache", CLASS_DATASOURCE_CACHE, false)
		)) {
		throw_error(env, "Error initializing native references");
	}

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
