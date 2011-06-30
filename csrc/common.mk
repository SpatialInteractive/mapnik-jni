JAVA_CMD ?= java
CFLAGS ?= -O3
JAVA_CFLAGS ?= $(shell $(JAVA_CMD) -jar java-config.jar --cflags)
MAPNIK_CFLAGS = $(shell mapnik-config --cflags)
MAPNIK_LIBS = $(shell mapnik-config --libs) $(shell pkg-config --cflags cairomm-1.0)

SOURCE_DEPENDS = mapnikjni.h mapnikjni.cpp globals.cpp \
	class_parameters.cpp \
	class_map.cpp \
	class_layer.cpp \
	class_datasource.cpp \
	class_datasourcecache.cpp \
	class_featuretypestyle.cpp \
	class_projection.cpp \
	class_query.cpp \
	class_featureset.cpp \
	class_geometry.cpp \
	class_image.cpp \
	class_renderer.cpp