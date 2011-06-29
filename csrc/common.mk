JAVA_CMD ?= java
CFLAGS ?= -O3
JAVA_CFLAGS ?= $(shell $(JAVA_CMD) -jar java-config.jar --cflags)
MAPNIK_CFLAGS = $(shell mapnik-config --cflags)
MAPNIK_LIBS = $(shell mapnik-config --libs) $(shell pkg-config --cflags cairomm-1.0)

