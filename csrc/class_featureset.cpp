/// -- FeatureSet class
/*
 * Class:     mapnik_FeatureSet
 * Method:    dealloc
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_mapnik_FeatureSet_dealloc
  (JNIEnv *env, jclass c, jlong fsptr, jlong fptr)
{
	if (fsptr)
		delete static_cast<mapnik::featureset_ptr*>(TO_POINTER(fsptr));
	if (fptr)
		delete static_cast<mapnik::feature_ptr*>(TO_POINTER(fptr));
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    _next
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FeatureSet__1next
	(JNIEnv *env, jobject fsobj)
{
	PREAMBLE;
	mapnik::featureset_ptr* fsp=LOAD_FEATURESET_POINTER(fsobj);
	mapnik::feature_ptr* fp=LOAD_FEATURE_POINTER(fsobj);
	if (fp) delete fp;

	mapnik::feature_ptr next=(*fsp)->next();
	if (!next) {
		// End
		env->SetLongField(fsobj, FIELD_FEATURESET_FEATURE_PTR, 0);
		return false;
	} else {
		// Another
		fp=new mapnik::feature_ptr(next);
		env->SetLongField(fsobj, FIELD_FEATURESET_FEATURE_PTR, FROM_POINTER(fp));
		return true;
	}

	TRAILER(false);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    _loadGeometries
 * Signature: ()[Lmapnik/Geometry;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_FeatureSet__1loadGeometries
	(JNIEnv *env, jobject fsobj)
{
	PREAMBLE;
	mapnik::feature_ptr* fp=LOAD_FEATURE_POINTER(fsobj);
	if (!fp) {
		throw_runtime_exception(env, "No feature loaded");
		return 0;
	}

	unsigned count=(*fp)->num_geometries();
	jobjectArray ret=env->NewObjectArray(count, CLASS_GEOMETRY.java_class, (jobject)0);
	for (unsigned index=0; index<count; index++) {
		mapnik::geometry_type &g((*fp)->get_geometry(index));
		jobject gobject=env->AllocObject(CLASS_GEOMETRY.java_class);
		env->SetLongField(gobject, CLASS_GEOMETRY.ptr_field, FROM_POINTER(&g));
		env->SetObjectArrayElement(ret, index, gobject);
	}

	return ret;
	TRAILER(0);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    getId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_FeatureSet_getId
  (JNIEnv *env, jobject fsobj)
{
	PREAMBLE;
	mapnik::feature_ptr* fp=LOAD_FEATURE_POINTER(fsobj);
	if (!fp) {
		throw_runtime_exception(env, "No feature loaded");
		return 0;
	}

	return (*fp)->id();
	TRAILER(0);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    getEnvelope
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureSet_getEnvelope
(JNIEnv *env, jobject fsobj)
{
	PREAMBLE;
	mapnik::feature_ptr* fp=LOAD_FEATURE_POINTER(fsobj);
	if (!fp) {
		throw_runtime_exception(env, "No feature loaded");
		return 0;
	}

	return box2dFromNative(env, (*fp)->envelope());
	TRAILER(0);
}

/*
 * Class:     mapnik_FeatureSet
 * Method:    getPropertyNames
 * Signature: ()Ljava/util/Set;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureSet_getPropertyNames
(JNIEnv *env, jobject fsobj)
{
	PREAMBLE;
	mapnik::feature_ptr* fp=LOAD_FEATURE_POINTER(fsobj);
	if (!fp) {
		throw_runtime_exception(env, "No feature loaded");
		return 0;
	}

	jobject ret=env->NewObject(CLASS_HASHSET, CTOR_HASHSET);
	for (std::map<std::string,mapnik::value>::iterator iter=(*fp)->begin(); iter!=(*fp)->end(); iter++) {
		std::string const& name(iter->first);
		env->CallBooleanMethod(ret, METHOD_HASHSET_ADD, env->NewStringUTF(name.c_str()));
	}

	return ret;
	TRAILER(0);
}

class value_to_java: public boost::static_visitor<jobject> {
	JNIEnv* env;
public:
	value_to_java(JNIEnv* aenv): env(aenv) {
	}

	jobject operator()(int value) const {
		return env->CallStaticObjectMethod(CLASS_INTEGER, METHOD_INTEGER_VALUEOF, value);
	}

	jobject operator()(double value) const {
		return env->CallStaticObjectMethod(CLASS_DOUBLE, METHOD_DOUBLE_VALUEOF, value);
	}

	jobject operator()(std::string const& value) const {
		return env->NewStringUTF(value.c_str());
	}

	jobject operator()(icu::UnicodeString const& value) const {
		return env->NewString(value.getBuffer(), value.length());
	}

	jobject operator()(mapnik::value_null const& value) const {
		return 0;
	}
};

/*
 * Class:     mapnik_FeatureSet
 * Method:    getProperty
 * Signature: (Ljava/lang/String;)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureSet_getProperty
  (JNIEnv *env, jobject fsobj, jstring namej)
{
	PREAMBLE;
	mapnik::feature_ptr* fp=LOAD_FEATURE_POINTER(fsobj);
	if (!fp) {
		throw_runtime_exception(env, "No feature loaded");
		return 0;
	}

	refjavastring name(env,namej);
	std::map<std::string,mapnik::value>::iterator iter=(*fp)->props().find(name.stringz);
	if (iter==(*fp)->end()) return 0;

	// Convert the value
	mapnik::value_base const& value=iter->second.base();
	return boost::apply_visitor(value_to_java(env), value);
	TRAILER(0);
}

