mapnik-jni
==========

Bindings to use mapnik from Java.  The mapnik coding style can sometimes be pretty hard-core C++ and I have not been successful in the past in building systematic Java bindings that are robust and mirror the functionality.  I bit the bullet this time for a project I'm working on and just hand coded them.  The mapnik C++ api has some pretty advanced functionality in places.  This binding tries to strike the 80-20 rule by providing access to the most used parts from Java.  There are several things that are not yet implemented but what is there is usable for programatically building/introspecting a map, enumerating features and rendering with AGG.

Prerequisites
-------------

* Mapnik2 (aka mapnik trunk).  In particular, to function on Linux, you must have a version that at least includes this fix: http://trac.mapnik.org/ticket/790
* Java 6 JDK
* Ant >= 1.7
* All dependencies for compiling Mapnik2 in your environment
* Linux or Mac OSX

Links
-----

* [JavaDocs](http://spatialinteractive.github.com/mapnik-jni/javadocs/index.html)
* [NanoMaps Server](https://github.com/SpatialInteractive/nanomaps-server) A tile server.  One of the first things I wrote with the Java bindings.
* [Mapnik Distiller](https://github.com/SpatialInteractive/mapnik-distiller) Map data optimizer.  Another bit that I wrote with the Java bindings.

Building
--------

* Make sure that the mapnik-config program is on your path and is the one you want to be linking against
* In the mapnik-jni directory, run "ant test"
* Your jar and shared library are in the build/dist directory.  Copy them to your project.  If the shared library is found in the same directory as the jar, then that is used with no further configuration.  Otherwise, the java.library.path and system default locations are searched depending on how you have your JVM setup.

Quick Start
-----------
If you have coded with the C++ api or the Python bindings, the Java api should feel familiar to you.  There are a few deviations:

* In Java, the Map class is called MapDefinition to avoid conflicts with java.util.Map
* Random global methods in Mapnik have generally been attached to the classes they operate on (ie. Map save/load, discovering style attributes, etc).  If you poke around with your favorite IDE, you should find things easily enough.
* Rendering methods are in the Renderer class

Here is some sample code:

	import mapnik.*;
	
	// You must call initialize once.
	// Calling it repeatedly is fine and has no effect.
	Mapnik.initialize();
	
	Box2d bounds=new Box2d(-180, -90, 180, 90);
	MapDefinition m=new MapDefinition();

	m.loadMap("mymapfile.xml", false);
	m.setSrs(Projection.LATLNG_PARAMS);
	m.resize(512, 512);
	m.zoomToBox(bounds);
	
	Image image=new Image(512, 512);
	Renderer.renderAgg(m, image);
	
	byte[] contents=image.saveToMemory("png");
	image.dispose();
	m.dispose();

	
Memory Management
-----------------
There are a few things to know about memory management:

* Most of the things you attach to a MapDefinition use pass-by-value semantics even though everything is obviously an object in Java.  For example, adding a Layer or Style copies the Layer or Style into the map.  The object you are left hanging on to is not "live."  A notable exception to this is the Datasource object.  These are pass by reference, just as they are in C++.
* Some very light weight mapnik objects such as Box2d, Color, LayerDescriptor and Coord are implemented in pure Java.  Under the covers these will be converted to and from the given native representation.  These are all more or less value types in C++ so this made sense to do it this way and saves wrapping an endless number of small objects around native pointers.
* Anything that descends from NativeObject is backed by at least one pointer to a C++ mapnik object.  This is all transparent to you unless if you want to be aware of it.  NativeObject has a "dispose()" method that you can use to prematurely deallocate the backing native memory.  Any attempt to access methods on the object after that point will throw an exception.  If you don't call dispose() everything will be cleaned up at finalization.  However, remember that most of these objects are like icebergs: from the JVM's perspective they occupy 8 bytes of memory (a pointer) but reference potentially very large blocks of non Java heap memory.  For long running systems I have found that just letting the garbage collector take care of things is a good way to run your system out of memory.  If you know you are done with something, particular a "big" object like a Map, FeatureSet, Datasource, Image, etc, just dispose() it right then and there and save yourself the hassle.
* If you are going to be enumerating features off of a datasource from the Java side, be aware that when you seek to the next Feature, any objects you got from the previous feature (ie. the feature itself, geometries, etc) are effectively disposed for you.  Don't try to access them.

Road Map
--------
I've been slowly adding classes and methods as I've needed them.  If you have need of something that's not here, either take a shot at adding it and send me a pull request or open an issue and I'll see if I can get to it.

Notwithstanding adding various small additions, the following bigger things need to be done:

* More and better unit testing.  What's there now is mainly focused on crash testing and doesn't have complete coverage for that.
* Integrate with Java Cairo bindings in order to enable Cairo rendering
* MetaWriters
* MemoryDatasource support in Java
* Wrap detailed style classes for full introspection and assembly of rules from the Java side


