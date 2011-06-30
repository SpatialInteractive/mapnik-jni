//// --- Map class members
/*
 * Class:     mapnik_Map
 * Method:    alloc
 * Signature: (IILjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Map_alloc__IILjava_lang_String_2
  (JNIEnv *env, jclass c, jint width, jint height, jstring srsj)
{
	ASSERT_INITIALIZED;
	refjavastring srs(env, srsj);

	return FROM_POINTER(new mapnik::Map(width, height, srs.stringz));
}

/*
 * Class:     mapnik_Map
 * Method:    alloc
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_mapnik_Map_alloc__
  (JNIEnv *env, jclass c)
{
	ASSERT_INITIALIZED;
	return FROM_POINTER(new mapnik::Map());
}

/*
 * Class:     mapnik_Map
 * Method:    dealloc
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_dealloc
  (JNIEnv *env, jclass c, jlong ptr)
{
	if (ptr) {
		delete static_cast<mapnik::Map*>(TO_POINTER(ptr));
	}
}

/*
 * Class:     mapnik_Map
 * Method:    loadMap
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_loadMap
  (JNIEnv *env, jobject mapobject, jstring filenamej, jboolean strict)
{
	try {
		mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
		refjavastring filename(env, filenamej);
		mapnik::load_map(*map, filename.stringz, (bool)strict);
	} catch (std::exception& e) {
		throw_java_exception(env, e);
		return;
	}
}

/*
 * Class:     mapnik_Map
 * Method:    loadMapString
 * Signature: (Ljava/lang/String;ZLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_loadMapString
  (JNIEnv *env, jobject mapobject, jstring strj, jboolean strict, jstring basepathj)
{
	try {
		mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
		refjavastring str(env, strj);
		refjavastring basepath(env, basepathj);
		mapnik::load_map_string(*map, str.stringz, (bool)strict, basepath.stringz);
	} catch (std::exception& e) {
		throw_java_exception(env, e);
		return;
	}
}

/*
 * Class:     mapnik_Map
 * Method:    getLayerCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getLayerCount
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->layer_count();
}

/*
 * Class:     mapnik_Map
 * Method:    getLayer
 * Signature: (I)Lmapnik/Layer;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getLayer
  (JNIEnv *env, jobject mapobject, jint index)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	std::vector<mapnik::layer> &layers(map->layers());
	if (index<0 || ((unsigned)index)>layers.size()) {
		return 0;
	}

	mapnik::layer* layer_copy=new mapnik::layer(layers[index]);
	jobject layer=env->AllocObject(CLASS_LAYER.java_class);
	env->SetLongField(layer, CLASS_LAYER.ptr_field, FROM_POINTER(layer_copy));
	return layer;
}

/*
 * Class:     mapnik_Map
 * Method:    setLayer
 * Signature: (ILmapnik/Layer;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setLayer
  (JNIEnv *env, jobject mapobject, jint index, jobject layerobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	std::vector<mapnik::layer> &layers(map->layers());
	if (index<0 || ((unsigned)index)>layers.size()) {
		return;
	}

	if (!layerobject) return;
	mapnik::layer* layer=
			static_cast<mapnik::layer*>(TO_POINTER(env->GetLongField(layerobject, CLASS_LAYER.ptr_field)));
	layers[index]=*layer;
}


/*
 * Class:     mapnik_Map
 * Method:    removeLayer
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_removeLayer
  (JNIEnv *env, jobject mapobject, jint index)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->removeLayer(index);
}

/*
 * Class:     mapnik_Map
 * Method:    addLayer
 * Signature: (Lmapnik/Layer;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_addLayer
  (JNIEnv *env, jobject mapobject, jobject layerobject)
{
	if (!layerobject) return;

	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	mapnik::layer* layer=
			static_cast<mapnik::layer*>(TO_POINTER(env->GetLongField(layerobject, CLASS_LAYER.ptr_field)));

	map->addLayer(*layer);
}

/*
 * Class:     mapnik_Map
 * Method:    removeAllLayers
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_removeAllLayers
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	std::vector<mapnik::layer> &layers(map->layers());
	layers.clear();
}

/*
 * Class:     mapnik_Map
 * Method:    getWidth
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getWidth
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->width();
}

/*
 * Class:     mapnik_Map
 * Method:    getHeight
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getHeight
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->height();
}

/*
 * Class:     mapnik_Map
 * Method:    setWidth
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setWidth
  (JNIEnv *env, jobject mapobject, jint width)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_width(width);
}

/*
 * Class:     mapnik_Map
 * Method:    setHeight
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setHeight
  (JNIEnv *env, jobject mapobject, jint height)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_height(height);
}

/*
 * Class:     mapnik_Map
 * Method:    resize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_resize
  (JNIEnv *env, jobject mapobject, jint width, jint height)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->resize(width, height);
}

/*
 * Class:     mapnik_Map
 * Method:    getSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Map_getSrs
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return env->NewStringUTF(map->srs().c_str());
}

/*
 * Class:     mapnik_Map
 * Method:    setSrs
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setSrs
  (JNIEnv *env, jobject mapobject, jstring srsj)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring srs(env, srsj);

	map->set_srs(srs.stringz);
}

/*
 * Class:     mapnik_Map
 * Method:    setBufferSize
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setBufferSize
  (JNIEnv *env, jobject mapobject, jint s)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_buffer_size(s);
}

/*
 * Class:     mapnik_Map
 * Method:    getBufferSize
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_mapnik_Map_getBufferSize
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return (jint)map->buffer_size();
}

/*
 * Class:     mapnik_Map
 * Method:    getBasePath
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Map_getBasePath
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return env->NewStringUTF(map->base_path().c_str());
}

/*
 * Class:     mapnik_Map
 * Method:    setBasePath
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setBasePath
  (JNIEnv *env, jobject mapobject, jstring basepathj)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring basepath(env, basepathj);
	map->set_base_path(basepath.stringz);
}

/*
 * Class:     mapnik_Map
 * Method:    getStyleNames
 * Signature: ()Ljava/util/Collection;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getStyleNames
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	jobject ret=env->NewObject(CLASS_HASHSET, CTOR_HASHSET);
	for (mapnik::Map::style_iterator iter=map->begin_styles(); iter!=map->end_styles(); iter++) {
		jstring name=env->NewStringUTF(iter->first.c_str());
		env->CallBooleanMethod(ret, METHOD_HASHSET_ADD, name);
	}

	return ret;
}

/*
 * Class:     mapnik_Map
 * Method:    getStyle
 * Signature: (Ljava/lang/String;)Lmapnik/FeatureTypeStyle;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getStyle
  (JNIEnv *env, jobject mapobject, jstring namej)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring name(env, namej);
	std::string namestring(name.stringz);

	boost::optional<mapnik::feature_type_style const&> style=map->find_style(namestring);
	if (!style) return 0;

	mapnik::feature_type_style* stylepinned=new mapnik::feature_type_style(style.get());

	jobject ret=env->AllocObject(CLASS_FEATURE_TYPE_STYLE.java_class);
	env->SetLongField(ret, CLASS_FEATURE_TYPE_STYLE.ptr_field, FROM_POINTER(stylepinned));
	return ret;
}

/*
 * Class:     mapnik_Map
 * Method:    addStyle
 * Signature: (Ljava/lang/String;Lmapnik/FeatureTypeStyle;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_addStyle
  (JNIEnv *env, jobject mapobject, jstring namej, jobject styleobject)
{
	if (!styleobject) return;

	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring name(env, namej);
	mapnik::feature_type_style* style=static_cast<mapnik::feature_type_style*>(
			TO_POINTER(env->GetLongField(styleobject, CLASS_FEATURE_TYPE_STYLE.ptr_field))
			);
	map->insert_style(name.stringz, *style);
}

/*
 * Class:     mapnik_Map
 * Method:    removeStyle
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_removeStyle
  (JNIEnv *env, jobject mapobject, jstring namej)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	refjavastring name(env, namej);
	map->remove_style(name.stringz);
}

/*
 * Class:     mapnik_Map
 * Method:    getMaximumExtent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getMaximumExtent
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	boost::optional<mapnik::box2d<double> > extent=map->maximum_extent();
	if (!extent) return 0;

	return box2dFromNative(env, extent.get());
}

/*
 * Class:     mapnik_Map
 * Method:    setMaximumExtent
 * Signature: (Lmapnik/Box2d;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setMaximumExtent
  (JNIEnv *env, jobject mapobject, jobject extentj)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	if (!extentj) return;

	map->set_maximum_extent(
			box2dToNative(env, extentj));
}

/*
 * Class:     mapnik_Map
 * Method:    getCurrentExtent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getCurrentExtent
	(JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	mapnik::box2d<double> extent=map->get_current_extent();
	return box2dFromNative(env, extent);
}

/*
 * Class:     mapnik_Map
 * Method:    getBufferedExtent
 * Signature: ()Lmapnik/Box2d;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getBufferedExtent
	(JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	mapnik::box2d<double> extent=map->get_buffered_extent();
	return box2dFromNative(env, extent);
}

/*
 * Class:     mapnik_Map
 * Method:    zoom
 * Signature: (D)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_zoom
  (JNIEnv *env, jobject mapobject, jdouble z)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->zoom(z);
}

/*
 * Class:     mapnik_Map
 * Method:    zoomToBox
 * Signature: (Lmapnik/Box2d;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_zoomToBox
  (JNIEnv *env, jobject mapobject, jobject box)
{
	if (!box) return;

	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->zoom_to_box(box2dToNative(env, box));
}

/*
 * Class:     mapnik_Map
 * Method:    zoomAll
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_zoomAll
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->zoom_all();
}

/*
 * Class:     mapnik_Map
 * Method:    pan
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_pan
  (JNIEnv *env, jobject mapobject, jint x, jint y)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->pan(x, y);
}

/*
 * Class:     mapnik_Map
 * Method:    panAndZoom
 * Signature: (IID)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_panAndZoom
  (JNIEnv *env, jobject mapobject, jint x, jint y, jdouble z)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->pan_and_zoom(x, y, z);
}

/*
 * Class:     mapnik_Map
 * Method:    getScale
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Map_getScale
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return map->scale();
}

/*
 * Class:     mapnik_Map
 * Method:    getScaleDenominator
 * Signature: ()D
 */
JNIEXPORT jdouble JNICALL Java_mapnik_Map_getScaleDenominator
	(JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	return map->scale_denominator();
}

/*
 * Class:     mapnik_Map
 * Method:    getBackground
 * Signature: ()Lmapnik/Color;
 */
JNIEXPORT jobject JNICALL Java_mapnik_Map_getBackground
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	boost::optional<mapnik::color> const& color(map->background());
	if (!color) return 0;

	return colorFromNative(env, color.get());
}

/*
 * Class:     mapnik_Map
 * Method:    setBackground
 * Signature: (Lmapnik/Color;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setBackground
  (JNIEnv *env, jobject mapobject, jobject c)
{
	if (!c) return;
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	map->set_background(colorToNative(env, c));
}

/*
 * Class:     mapnik_Map
 * Method:    getBackgroundImage
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_mapnik_Map_getBackgroundImage
  (JNIEnv *env, jobject mapobject)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	boost::optional<std::string> const& image(map->background_image());
	if (!image) return 0;

	return env->NewStringUTF(image.get().c_str());
}

/*
 * Class:     mapnik_Map
 * Method:    setBackgroundImage
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_setBackgroundImage
  (JNIEnv *env, jobject mapobject, jstring filenamej)
{
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	if (!filenamej) return;
	refjavastring filename(env, filenamej);
	map->set_background_image(filename.stringz);
}

/*
 * Class:     mapnik_Map
 * Method:    saveMapToFile
 * Signature: (Ljava/lang/String;Z)V
 */
JNIEXPORT void JNICALL Java_mapnik_Map_saveMapToFile
  (JNIEnv *env, jobject mapobject, jstring filenamej, jboolean explicitDefaults)
{
	PREAMBLE;
	mapnik::Map* map=LOAD_MAP_POINTER(mapobject);
	if (!filenamej) {
		throw_runtime_exception(env, "Filename is required");
		return;
	}

	refjavastring filename(env, filenamej);
	mapnik::save_map(*map, filename.stringz, explicitDefaults);

	TRAILER_VOID;
}

/*
 * Class:     mapnik_Map
 * Method:    saveMapToBuffer
 * Signature: (Z)[B
 */
JNIEXPORT jbyteArray JNICALL Java_mapnik_Map_saveMapToBuffer
  (JNIEnv *, jobject, jboolean);
