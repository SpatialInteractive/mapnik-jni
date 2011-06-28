mapnik-jni
----------

Bindings to use mapnik from Java.  The mapnik coding style can sometimes be pretty hard-core C++ and I have not been successful in the past in building systematic Java bindings that are robust and mirror the functionality.  I bit the bullet this time for a project I'm working on and just hand coded them.  The first part of the project involves introspecting a map and enumerating features.  These are the parts I've built out.  The rendering bits aren't there yet but should come quickly enough.

The following classes have been more or less implemented on the Java side:

* AttributeDescriptor
* Box2d
* Color
* Coord
* Datasource
* DatasourceCache
* FeatureSet (includes Feature)
* FeatureTypeStyle
* Geometry
* Layer
* LayerDescriptor
* Map
* Parameters
* Projection
* Query

Once I get this a little further along, I need to talk to the mapnik list about this.  It really would do better as part of the core mapnik project.  Also, if it were part of the main mapnik shared library it would add negligible code size as compared to building it standalone.  If anyone runs across this and wishes to talk to the list about it before I get a chance, feel free.

Building
--------
Assuming that mapnik2 (aka mapnik trunk) has been installed in a default location (ie. /usr/local), just running "ant" from the root should work on either Linux or OSX.  Your jar and shared library will be deposited into build/dist.

Problems Loading Input Plugins
------------------------------
On Linux, attempting to load the input plugins in a stock installation reports an error relating
to undefined symbols (depending on which plugin you load).  The problem is that all plugins depend
on symbols defined in libmapnik2.so but they do not explicitly depend on libmapnik2.so.  In a typical
situation where it is an executable which depends on libmapnik2.so which then loads the plugins, the
linker links the plugins against the full library.  However, for some reason that I can't quite figure,
when libmapnik2.so is the dependency of a jni shared library that is dlopen'd, only the symbols referenced
by the jni lib are available for resolution by subsequent libraries that are dlopen'd.  This situation
can be corrected by either LD_PRELOADing libmapnik2.so when the process launches or fixing the input
plugins so that they declare a dependency on libmapnik2.so.  The script fixup-input-plugins.sh, while a gruesome
hack does this.  Drop it in the directory containing your .input files and run it.  It will rename the
originals and create a new shared library for each that links to the original and libmapnik2.so.

I need to talk to the list about this, but for now, at least it works.


