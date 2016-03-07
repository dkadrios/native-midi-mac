# README #

## Sample Adobe AIR Native Extension for Windows and Mac OS X using AIR SDK 20 ##

* Demonstrates how to build a Native Extension that works with the 64 bit version of Adobe AIR on Mac OS X.
* CMake script included in src/ folder to build XCode and VS projects

## Tested with ##
* AIR SDK 20
* XCode 7, Visual Studio 14 2015
* CMake 3.4

## Build Steps ##
* Set up AIR SDK at /AIRSDK (mac) C:\AIRSDK (win), or update sdk path inside xcode and build scripts
* Generate XCode / VS project with CMake (to get started with CMake: http://jonasjohanssongamedev.tumblr.com/post/55020271753/setting-up-a-cross-platform-c-environment)
* Build Framework: open XCode / VS project, build target INSTALL
* Build ANE: under Mac OS X open Terminal, run build.sh from build/ folder

## Testing ##
* Run test.sh / test.bat from test/ folder


## References ##

* http://blogs.adobe.com/flashplayer/2015/12/air-64-bit-on-mac-osx.html
* http://help.adobe.com/en_US/air/extensions/WSf268776665d7970d-2e74ffb4130044f3619-7fff.html
* http://rhuno.com/flashblog/2012/04/30/tutorial-flash-and-c-native-extension/
* http://riawanderer.com/2012/03/27/working-with-air-native-extensions-on-the-mac/