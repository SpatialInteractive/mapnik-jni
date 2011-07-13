#!/bin/bash
td=$(dirname $0)
exec java -classpath $td/build/dist/mapnik-jni.jar:$td/build/classes-tools "$@"
