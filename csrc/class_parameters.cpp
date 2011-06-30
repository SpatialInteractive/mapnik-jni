//// --- Parameters
static void translate_to_mapnik_parameters(JNIEnv *env, jobject javaparams, mapnik::parameters& mapnikparams)
{
	if (!javaparams) return;
	env->CallVoidMethod(javaparams, METHOD_PARAMETERS_COPY_TO_NATIVE, (jlong)(&mapnikparams));
}

class translate_parameter_visitor: public boost::static_visitor<>
{
	JNIEnv *env;
	jobject paramobject;
	jstring key;
public:
	translate_parameter_visitor(JNIEnv* aenv, jobject aparamobject, jstring akey): env(aenv), paramobject(aparamobject), key(akey) {
	}

	void operator()(const int& value) const {
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_INT, key, (jint)value);
	}

	void operator()(const std::string& value) const {
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_STRING,
				key,
				env->NewStringUTF(value.c_str()));
	}

	void operator()(const double& value) const {
		env->CallVoidMethod(paramobject, METHOD_PARAMETERS_SET_DOUBLE, key, (jdouble)value);
	}
};

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeInt
 * Signature: (JLjava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeInt
  (JNIEnv *env, jclass c, jlong ptr, jstring namej, jint value)
{
	refjavastring name(env, namej);
	mapnik::parameters *params=(mapnik::parameters*)(ptr);
	(*params)[name.stringz]=(int)value;
}

/*
 * Class:     mapnik_Parameters
 * Method:    setNativeString
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeString
  (JNIEnv *env, jclass c, jlong ptr, jstring namej, jstring valuej)
{
	refjavastring name(env, namej);
	refjavastring value(env, valuej);
	mapnik::parameters *params=(mapnik::parameters*)(ptr);
	(*params)[name.stringz]=value.stringz;
}


/*
 * Class:     mapnik_Parameters
 * Method:    setNativeDouble
 * Signature: (JLjava/lang/String;D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Parameters_setNativeDouble
  (JNIEnv *env, jclass c, jlong ptr, jstring namej, jdouble value)
{
	refjavastring name(env, namej);
	mapnik::parameters *params=(mapnik::parameters*)(ptr);
	(*params)[name.stringz]=(double)value;
}
