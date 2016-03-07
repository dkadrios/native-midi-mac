cmake_minimum_required (VERSION 2.8.11)
project (NativeMIDI)
set(CMAKE_MACOSX_RPATH 1)
# Overwrite default install directory
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/..)

add_library(NativeMIDI SHARED NativeMIDI.cpp NativeMIDI.h)
include_directories(/AIRSDK/include)

if(APPLE)
    set(NativeMIDI_VER A)
    set_target_properties(NativeMIDI PROPERTIES
      FRAMEWORK TRUE
      FRAMEWORK_VERSION ${NativeMIDI_VER}
      PUBLIC_HEADER "NativeMIDI.h;"
      # Load dynamic libraries from these folders:
      INSTALL_RPATH "@executable_path/../runtimes/air/mac;@executable_path/../Frameworks;/Library/Frameworks;"
      DEBUG_POSTFIX -d
    )
    set(AIR_64_LIB /AIRSDK/runtimes/air/mac/Adobe\ AIR.framework/Versions/1.0/Adobe\ AIR_64) 
    target_link_libraries(NativeMIDI LINK_PUBLIC ${AIR_64_LIB})
endif(APPLE)

if(WIN32)
    set(AIR_LIB /AIRSDK/lib/win/FlashRuntimeExtensions.lib) 
    target_link_libraries(NativeMIDI LINK_PUBLIC ${AIR_LIB})
endif(WIN32)


install(TARGETS NativeMIDI
  FRAMEWORK DESTINATION build
  RUNTIME DESTINATION build

  # These are ignored on the Mac... and things are automatically placed in
  # their appropriate Framework sub-folder at build time. (And then the built
  # framework is copied recursively when it is installed.)
  PRIVATE_HEADER DESTINATION share/NativeMIDI-${NativeMIDI_VER}/PrivateHeaders
  PUBLIC_HEADER DESTINATION include/NativeMIDI-${NativeMIDI_VER}
  RESOURCE DESTINATION share/NativeMIDI-${NativeMIDI_VER}/Resources
  # But they are required to be present so that installing a framework on other
  # other platforms will install the pieces of the framework without having to
  # duplicate install rules for the pieces of the framework.
)

