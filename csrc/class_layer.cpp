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

/*
 * Class:     mapnik_Layer
 * Method:    getDatasource
 * Signature: ()Lmapnik/Datasource;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Layer_getDatasource
  (JNIEnv *env, jobject layerobj)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);
	mapnik::datasource_ptr ds=layer->datasource();
	if (!ds) return 0;

	mapnik::datasource_ptr* dspinned=new mapnik::datasource_ptr(ds);
	jobject ret=env->AllocObject(CLASS_DATASOURCE.java_class);
	env->SetLongField(ret, CLASS_DATASOURCE.ptr_field, FROM_POINTER(dspinned));

	return ret;
}

/*
 * Class:     mapnik_Layer
 * Method:    setDatasource
 * Signature: (Lmapnik/Datasource;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Layer_setDatasource
  (JNIEnv *env, jobject layerobj, jobject dsobject)
{
	mapnik::layer* layer=LOAD_LAYER_POINTER(layerobj);

	if (!dsobject) {
		layer->set_datasource(mapnik::datasource_ptr());
		return;
	}

	mapnik::datasource_ptr* dspinned=
			static_cast<mapnik::datasource_ptr*>(
					TO_POINTER(env->GetLongField(dsobject, CLASS_DATASOURCE.ptr_field)));
	layer->set_datasource(*dspinned);
}
