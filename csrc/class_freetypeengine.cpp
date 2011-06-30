/*
 * Class:     mapnik_FreetypeEngine
 * Method:    registerFont
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FreetypeEngine_registerFont
  (JNIEnv *env, jclass c, jstring filenamej)
{
	PREAMBLE;
	if (!filenamej) return false;
	refjavastring filename(env, filenamej);

	return mapnik::freetype_engine::register_font(filename.stringz);

	TRAILER(false);
}

/*
 * Class:     mapnik_FreetypeEngine
 * Method:    registerFonts
 * Signature: (Ljava/lang/String;Z)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FreetypeEngine_registerFonts
  (JNIEnv *env, jclass c, jstring dirj, jboolean recurse)
{
	PREAMBLE;
	if (!dirj) return false;
	refjavastring dir(env, dirj);

	return mapnik::freetype_engine::register_fonts(dir.stringz);

	TRAILER(false);
}

/*
 * Class:     mapnik_FreetypeEngine
 * Method:    isFontFile
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_mapnik_FreetypeEngine_isFontFile
  (JNIEnv *env, jclass c, jstring filenamej)
{
	PREAMBLE;
	if (!filenamej) return false;
	refjavastring filename(env, filenamej);

	return mapnik::freetype_engine::is_font_file(filename.stringz);

	TRAILER(false);
}

