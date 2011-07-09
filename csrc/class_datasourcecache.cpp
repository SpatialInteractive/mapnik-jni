//// -- DatasourceCache
/*
 * Class:     mapnik_DatasourceCache
 * Method:    pluginNames
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_mapnik_DatasourceCache_pluginNames
  (JNIEnv *env, jclass c)
{
	PREAMBLE;
	std::vector<std::string> names(mapnik::datasource_cache::plugin_names());
	jobjectArray ary=env->NewObjectArray(
		names.size(),
		CLASS_STRING,
		(jobject)0
		);

	for (unsigned i=0; i<names.size(); i++) {
		env->SetObjectArrayElement(ary, i,
			env->NewStringUTF(names[i].c_str()));
	}

	return ary;
	TRAILER(0);
}

/*
 * Class:     mapnik_DatasourceCache
 * Method:    pluginDirectories
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_DatasourceCache_pluginDirectories
  (JNIEnv *env, jclass c)
{
	PREAMBLE;
	std::string s(mapnik::datasource_cache::plugin_directories());
	return env->NewStringUTF(s.c_str());
	TRAILER(0);
}

/*
 * Class:     mapnik_DatasourceCache
 * Method:    registerDatasources
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_DatasourceCache_registerDatasources
  (JNIEnv *env, jclass c, jstring sj)
{
	PREAMBLE;
	refjavastring path(env, sj);
	mapnik::datasource_cache::register_datasources(path.stringz);
	TRAILER_VOID;
}

/*
 * Class:     mapnik_DatasourceCache
 * Method:    create
 * Signature: (Lmapnik/Parameters;Z)Lmapnik/Datasource;
 */
JNIEXPORT jobject JNICALL Java_mapnik_DatasourceCache_create
	(JNIEnv *env, jclass c, jobject paramsmap, jboolean bind)
{
	PREAMBLE;
	mapnik::parameters params;
	translate_to_mapnik_parameters(env, paramsmap, params);

	mapnik::datasource_ptr ds=mapnik::datasource_cache::create(params, (bool)bind);
	if (!ds) return 0;

	mapnik::datasource_ptr *dspinned=new mapnik::datasource_ptr(ds);
	jobject ret=env->NewObject(CLASS_DATASOURCE, CTOR_NATIVEOBJECT);
	env->SetLongField(ret, FIELD_PTR, FROM_POINTER(dspinned));
	return ret;
	TRAILER(0);
}

