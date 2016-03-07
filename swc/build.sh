#!/bin/sh

name="NativeMIDI";

cd $(dirname $0);

"/AIRSDK/bin/acompc" -source-path src -include-classes fs.ane.${name} -swf-version=31 -output ../build/${name}.swc;
