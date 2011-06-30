/*
 * Class:     mapnik_Renderer
 * Method:    renderAgg
 * Signature: (Lmapnik/Map;Lmapnik/Image;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Renderer_renderAgg
  (JNIEnv *env, jclass c, jobject mapobj, jobject imobj)
{
	PREAMBLE;
	if (!mapobj || !imobj) {
		throw_runtime_exception(env, "Illegal arguments in call");
		return;
	}

	mapnik::Map* map=LOAD_MAP_POINTER(mapobj);
	mapnik::image_32* im=LOAD_IMAGE_POINTER(imobj);

	mapnik::agg_renderer<mapnik::image_32> ren(*map, *im);
	ren.apply();

	TRAILER_VOID;
}
