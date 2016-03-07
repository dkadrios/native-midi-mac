# Install script for directory: /Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src/..")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/build" TYPE DIRECTORY FILES "/Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src/Debug/NativeMIDI.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/AIRSDK/runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "/Library/Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/build" TYPE DIRECTORY FILES "/Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src/Release/NativeMIDI.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/AIRSDK/runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "/Library/Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/build" TYPE DIRECTORY FILES "/Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src/MinSizeRel/NativeMIDI.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/AIRSDK/runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "/Library/Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/build" TYPE DIRECTORY FILES "/Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src/RelWithDebInfo/NativeMIDI.framework" USE_SOURCE_PERMISSIONS)
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/AIRSDK/runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../runtimes/air/mac"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "@executable_path/../Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      execute_process(COMMAND /usr/bin/install_name_tool
        -add_rpath "/Library/Frameworks"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/build/NativeMIDI.framework/Versions/A/NativeMIDI")
      endif()
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/dkadrioski13/git-nebiru/ZenEdit2/NativeMidiMac/src/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
