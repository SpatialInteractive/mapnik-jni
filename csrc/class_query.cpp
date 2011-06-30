/// -- Query class
/*
 * Class:     mapnik_Query
 * Method:    alloc
 * Signature: (Lmapnik/Box2d;DDD)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Query_alloc
  (JNIEnv *env, jclass c, jobject bboxj, jdouble resx, jdouble resy, jdouble scale_denominator)
{
	PREAMBLE;
	mapnik::box2d<double> bbox(box2dToNative(env, bboxj));
	mapnik::query* q=new mapnik::query(bbox, mapnik::query::resolution_type(resx, resy), scale_denominator);

	return FROM_POINTER(q);
	TRAILER(0);
}

/*
 * Class:     mapnik_Query
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Query_dealloc
  (JNIEnv *env, jclass c, jlong ptr)
{
	delete static_cast<mapnik::query*>(TO_POINTER(ptr));
}

/*
 * Class:     mapnik_Query
 * Method:    addPropertyName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Query_addPropertyName
  (JNIEnv *env, jobject qobj, jstring namej)
{
	PREAMBLE;
	mapnik::query* q=LOAD_QUERY_POINTER(qobj);
	refjavastring name(env, namej);
	q->add_property_name(name.stringz);
	TRAILER_VOID;
}

