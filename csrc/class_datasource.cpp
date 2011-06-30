// -- Datasource
/*
 * Class:     mapnik_Datasource
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Datasource_dealloc
  (JNIEnv *env, jclass c, jlong ptr)
{
	mapnik::datasource_ptr* dspinned=
			static_cast<mapnik::datasource_ptr*>(
					TO_POINTER(ptr));
	delete dspinned;

}


/*
 * Class:     mapnik_Datasource
 * Method:    getParameters
 * Signature: ()Lmapnik/Parameters;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_getParameters
  (JNIEnv *env, jobject dsobj)
{
	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	const mapnik::parameters& params((*dsp)->params());

	jobject paramobject=env->NewObject(CLASS_PARAMETERS, CTOR_PARAMETERS);

	for (mapnik::param_map::const_iterator iter=params.begin(); iter!=params.end(); iter++) {
		jstring key=env->NewStringUTF(iter->first.c_str());
		boost::apply_visitor(translate_parameter_visitor(env, paramobject, key), iter->second);
	}

	return paramobject;
}

/*
 * Class:     mapnik_Datasource
 * Method:    getType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Datasource_getType
  (JNIEnv *env, jobject dsobj)
{
	PREAMBLE;
	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	return (*dsp)->type();
	TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    bind
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Datasource_bind
  (JNIEnv *env, jobject dsobj)
{
	PREAMBLE;
	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	(*dsp)->bind();
	TRAILER_VOID;
}

/*
 * Class:     mapnik_Datasource
 * Method:    getEnvelope
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_getEnvelope
  (JNIEnv *env, jobject dsobj)
{
	PREAMBLE;
	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	return box2dFromNative(env, (*dsp)->envelope());
	TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    features
 * Signature: (Lmapnik/Query;)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_features
  (JNIEnv *env, jobject dsobj, jobject queryobj)
{
	PREAMBLE;
	if (!queryobj) return 0;

	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	mapnik::query* query=LOAD_QUERY_POINTER(queryobj);

	mapnik::featureset_ptr fs=(*dsp)->features(*query);
	mapnik::featureset_ptr* fspinned=new mapnik::featureset_ptr(fs);

	jobject ret=env->AllocObject(CLASS_FEATURESET.java_class);
	env->SetLongField(ret, CLASS_FEATURESET.ptr_field, FROM_POINTER(fspinned));
	env->SetLongField(ret, FIELD_FEATURESET_FEATURE_PTR, 0l);
	return ret;

	TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    featuresAtPoint
 * Signature: (Lmapnik/Coord;)Lmapnik/FeatureSet;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_featuresAtPoint
  (JNIEnv *env, jobject dsobj, jobject ptobj)
{
	PREAMBLE;
	if (!ptobj) return 0;

	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	mapnik::coord2d pt(
		env->GetDoubleField(ptobj, FIELD_COORD_X),
		env->GetDoubleField(ptobj, FIELD_COORD_Y)
		);

	mapnik::featureset_ptr fs=(*dsp)->features_at_point(pt);
	mapnik::featureset_ptr* fspinned=new mapnik::featureset_ptr(fs);

	jobject ret=env->AllocObject(CLASS_FEATURESET.java_class);
	env->SetLongField(ret, CLASS_FEATURESET.ptr_field, FROM_POINTER(fspinned));
	return ret;

	TRAILER(0);
}

/*
 * Class:     mapnik_Datasource
 * Method:    getDescriptor
 * Signature: ()Lmapnik/LayerDescriptor;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Datasource_getDescriptor
  (JNIEnv *env, jobject dsobj)
{
	PREAMBLE;
	mapnik::datasource_ptr* dsp=LOAD_DATASOURCE_POINTER(dsobj);
	mapnik::layer_descriptor desc=(*dsp)->get_descriptor();

	jobject ret=env->NewObject(CLASS_LAYERDESCRIPTOR, CTOR_LAYERDESCRIPTOR);
	env->SetObjectField(ret, FIELD_LAYERDESCRIPTOR_NAME, env->NewStringUTF(desc.get_name().c_str()));
	env->SetObjectField(ret, FIELD_LAYERDESCRIPTOR_ENCODING, env->NewStringUTF(desc.get_encoding().c_str()));

	std::vector<mapnik::attribute_descriptor> &descriptors(desc.get_descriptors());
	for (std::vector<mapnik::attribute_descriptor>::iterator iter=descriptors.begin(); iter!=descriptors.end(); iter++) {
		jobject attr=env->NewObject(CLASS_ATTRIBUTEDESCRIPTOR, CTOR_ATTRIBUTEDESCRIPTOR);
		env->SetObjectField(attr, FIELD_ATTRIBUTEDESCRIPTOR_NAME, env->NewStringUTF(iter->get_name().c_str()));
		env->SetIntField(attr, FIELD_ATTRIBUTEDESCRIPTOR_TYPE, iter->get_type());
		env->SetBooleanField(attr, FIELD_ATTRIBUTEDESCRIPTOR_PRIMARYKEY, (jboolean)iter->is_primary_key());
		env->SetIntField(attr, FIELD_ATTRIBUTEDESCRIPTOR_SIZE, iter->get_size());
		env->SetIntField(attr, FIELD_ATTRIBUTEDESCRIPTOR_PRECISION, iter->get_precision());

		env->CallVoidMethod(ret, METHOD_LAYERDESCRIPTOR_ADDDESCRIPTOR, attr);
	}

	return ret;
	TRAILER(0);
}

