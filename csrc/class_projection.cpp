

/// -- Projection class
/*
 * Class:     mapnik_Projection
 * Method:    alloc
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Projection_alloc
  (JNIEnv *env, jclass c, jstring paramsj)
{
	refjavastring params(env, paramsj);
	mapnik::projection* prj=new mapnik::projection(params.stringz);
	return FROM_POINTER(prj);
}

/*
 * Class:     mapnik_Projection
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Projection_dealloc
  (JNIEnv *env, jclass c, jlong ptr)
{
	delete static_cast<mapnik::projection*>(TO_POINTER(ptr));
}

/*
 * Class:     mapnik_Projection
 * Method:    getParams
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Projection_getParams
  (JNIEnv *env, jobject prjobject)
{
	mapnik::projection* prj=LOAD_PROJECTION_POINTER(prjobject);
	return env->NewStringUTF(prj->params().c_str());
}

/*
 * Class:     mapnik_Projection
 * Method:    getExpanded
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Projection_getExpanded
  (JNIEnv *env, jobject prjobject)
{
	mapnik::projection* prj=LOAD_PROJECTION_POINTER(prjobject);
	return env->NewStringUTF(prj->expanded().c_str());
}

/*
 * Class:     mapnik_Projection
 * Method:    forward
 * Signature: (Lmapnik/Coord;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Projection_forward
  (JNIEnv *env, jobject prjobject, jobject coord)
{
	if (!coord) return;
	mapnik::projection* prj=LOAD_PROJECTION_POINTER(prjobject);
	double x=env->GetDoubleField(coord, FIELD_COORD_X),
			y=env->GetDoubleField(coord, FIELD_COORD_Y);
	prj->forward(x, y);
	env->SetDoubleField(coord, FIELD_COORD_X, x);
	env->SetDoubleField(coord, FIELD_COORD_Y, y);
}

/*
 * Class:     mapnik_Projection
 * Method:    inverse
 * Signature: (Lmapnik/Coord;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Projection_inverse
	(JNIEnv *env, jobject prjobject, jobject coord)
{
	if (!coord) return;
	mapnik::projection* prj=LOAD_PROJECTION_POINTER(prjobject);
	double x=env->GetDoubleField(coord, FIELD_COORD_X),
			y=env->GetDoubleField(coord, FIELD_COORD_Y);
	prj->inverse(x, y);
	env->SetDoubleField(coord, FIELD_COORD_X, x);
	env->SetDoubleField(coord, FIELD_COORD_Y, y);
}

