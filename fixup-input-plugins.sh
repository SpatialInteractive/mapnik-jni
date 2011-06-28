#!/bin/bash
cd $(dirname $0)

for input in *.input
do
	partial=$input.partial
	if ! [ -f $partial ]; then
		mv $input $partial
	fi

	g++ -shared -o $input $(pwd)/$partial ../../libmapnik2.so
done

