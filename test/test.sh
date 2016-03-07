#!/bin/sh

name="NativeMIDI";
testPath=$(dirname $0);

cd ${testPath}/extensions;
rm -rf ${name}ANE.ane;
mkdir ${name}ANE.ane;

cd ${name}ANE.ane;
unzip ../${name}.ane;


cd ${testPath};
cd ../..
echo 'pwd';
echo ${testPath};

pwd
"/AIRSDK/bin/adl" -profile extendedDesktop -extdir extensions ${name}Test-app.xml;
