cmake_minimum_required(VERSION 3.5.1)

project(sprite_table)

option(BUILD_DEBUG "Build a debug release" OFF)
option(BUILD_APPIMAGE "Build for appimage" OFF)
option(BUILD_REGULAR "Regular build" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#derive the kind of build...
if(${BUILD_APPIMAGE})

	message("will build as AppImage")
	set(BUILD_VERSION "appimage")
	if(${CMAKE_VERSION} VERSION_LESS "3.22.0") 

		add_definitions(-DAS_APPIMAGE=1)
	else()

		add_compile_definitions(AS_APPIMAGE=1)
	endif()

elseif(${BUILD_REGULAR})

	message("will build as regular app")
	set(BUILD_VERSION "bin")
	if(${CMAKE_VERSION} VERSION_LESS "3.22.0") 

		add_definitions(-DAS_REGULAR=1)
	else()

		add_compile_definitions(AS_REGULAR=1)
	endif()

else()

	message(FATAL_ERROR "A build type regular/appimage must be chosen")
endif()


if(${BUILD_DEBUG})

	set(CMAKE_BUILD_TYPE Debug)
	set(BUILD_VERSION="${BUILD_VERSION}.debug")
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

#add the framework dependencies
add_library(lm SHARED IMPORTED)
add_library(tools SHARED IMPORTED)
add_library(ldtools SHARED IMPORTED)
add_library(dansdl2 SHARED IMPORTED)
add_library(dfw SHARED IMPORTED)

#If the libraries are not installed, tell them where they are.
set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)

if(${BUILD_DEBUG})
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)
else()
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2_debug.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools_debug.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw_debug.so)
endif()

if(${CMAKE_VERSION} VERSION_LESS "3.22.0") 

	add_definitions(-DMAJOR_VERSION=1)
	add_definitions(-DMINOR_VERSION=1)
	add_definitions(-DPATCH_VERSION=0)
	add_definitions(-DBUILD_VERSION=\"${BUILD_VERSION}\")
else()

	add_compile_definitions(MAJOR_VERSION=1)
	add_compile_definitions(MINOR_VERSION=1)
	add_compile_definitions(PATCH_VERSION=0)
	add_compile_definitions(BUILD_VERSION=\"${BUILD_VERSION}\")
endif()

#Configure the executable, we want it into the "build directory" and we want all resources copied
#to the build directory. If we need to make an appdir, we will move our stuff around
add_executable(sprite_table src/main.cpp ${SOURCE})
set_target_properties(sprite_table PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
add_custom_command(TARGET sprite_table POST_BUILD COMMAND cp -r ../resources/* ./build)
