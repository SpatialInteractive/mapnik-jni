/// -- FeatureTypeStyle class
/*
 * Class:     mapnik_FeatureTypeStyle
 * Method:    alloc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_mapnik_FeatureTypeStyle_alloc
  (JNIEnv *env, jclass c)
{
	PREAMBLE;
	return FROM_POINTER(new mapnik::feature_type_style());
	TRAILER(0);
}

/*
 * Class:     mapnik_FeatureTypeStyle
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_FeatureTypeStyle_dealloc
  (JNIEnv *env, jobject, jlong ptr)
{
	PREAMBLE;
	mapnik::feature_type_style* style=static_cast<mapnik::feature_type_style*>(TO_POINTER(ptr));
	delete style;
	TRAILER_VOID;
}

/*
 * Class:     mapnik_FeatureTypeStyle
 * Method:    collectAttributes
 * Signature: ()Ljava/util/Set;
 */
JNIEXPORT jobject JNICALL Java_mapnik_FeatureTypeStyle_collectAttributes
  (JNIEnv *env, jobject styleobject)
{
	PREAMBLE;
	mapnik::feature_type_style* style=LOAD_FEATURE_TYPE_STYLE_POINTER(styleobject);
	const std::vector<mapnik::rule>& rules(style->get_rules());

	std::set<std::string> attrs;
	mapnik::attribute_collector collector(attrs);
	BOOST_FOREACH(mapnik::rule const& r, rules) {
		collector(r);
	}


	jobject ret=env->NewObject(CLASS_HASHSET, CTOR_HASHSET);
	for (std::set<std::string>::iterator iter=attrs.begin(); iter!=attrs.end(); iter++) {
		env->CallBooleanMethod(ret, METHOD_HASHSET_ADD, env->NewStringUTF(iter->c_str()));
	}

	return ret;
	TRAILER(0);
}

