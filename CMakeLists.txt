cmake_minimum_required(VERSION 3.5.1)

project(sprite_table)

option(BUILD_DEBUG "Build a debug release" OFF)
option(BUILD_APPIMAGE "Build for appimage" OFF)
option(BUILD_REGULAR "Regular build" OFF)
option(WITH_LEXICAL_NORMAL "If true, std::filesystem::lexically_normal() will be used in the file browser. Else, an approximation with realpath will be tried" TRUE)

#derive the kind of build...
if(${BUILD_APPIMAGE})

	set(BUILD_VERSION "AppImage")
elseif(${BUILD_REGULAR})

	set(BUILD_VERSION "bin")
else()

	message(FATAL_ERROR "A build type regular/appimage must be chosen")
endif()

if(${BUILD_DEBUG})

	set(CMAKE_BUILD_TYPE Debug)
	set(BUILD_VERSION="${BUILD_VERSION}.debug")
else()

	set(CMAKE_BUILD_TYPE Release)
endif()

#application version
set(MAJOR_VERSION 1)
set(MINOR_VERSION 1)
set(PATCH_VERSION 5)
set(BUILD_VERSION "\"${BUILD_VERSION}\"") #add some quotes to pacify the macro preprocessor

if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

	add_definitions(-DMAJOR_VERSION=${MAJOR_VERSION})
	add_definitions(-DMINOR_VERSION=${MINOR_VERSION})
	add_definitions(-DPATCH_VERSION=${PATCH_VERSION})
	add_definitions(-DBUILD_VERSION=${BUILD_VERSION})
else()

	add_compile_definitions(MAJOR_VERSION=${MAJOR_VERSION})
	add_compile_definitions(MINOR_VERSION=${MINOR_VERSION})
	add_compile_definitions(PATCH_VERSION=${PATCH_VERSION})
	add_compile_definitions(BUILD_VERSION=${BUILD_VERSION})
endif()

message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${BUILD_VERSION}")

#compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#flags for bin type and behaviour
if(${BUILD_APPIMAGE})

	if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

		add_definitions(-DAS_APPIMAGE=1)
	else()

		add_compile_definitions(AS_APPIMAGE=1)
	endif()
elseif(${BUILD_REGULAR})

	if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

		add_definitions(-DAS_REGULAR=1)
	else()

		add_compile_definitions(AS_REGULAR=1)
	endif()
endif()

if(${BUILD_DEBUG})

	if(${CMAKE_VERSION} VERSION_LESS "3.12.0")

		add_definitions(-DLIBDANSDL2_DEBUG)
	else()

		add_compile_definitions(LIBDANSDL2_DEBUG)
	endif()
endif()

if(${WITH_LEXICAL_NORMAL})
	if(${CMAKE_VERSION} VERSION_LESS "3.12.0")

		add_definitions(-DWITH_LEXICAL_NORMAL)
	else()

		add_compile_definitions(WWITH_LEXICAL_NORMAL)
	endif()
endif()

#sources, dependencies...
include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_library(lm SHARED IMPORTED)
add_library(tools SHARED IMPORTED)
add_library(ldtools SHARED IMPORTED)
add_library(dansdl2 SHARED IMPORTED)
add_library(dfw SHARED IMPORTED)

#lib locations...
set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)

if(${BUILD_DEBUG})

	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2_debug.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools_debug.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw_debug.so)
else()

	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)
endif()

#Virtual library, just for good measure.
add_library(applib STATIC ${SOURCE})
set_target_properties(applib PROPERTIES OUTPUT_NAME applib)

#Configure the executable, we want it into the "build directory" and we want all resources copied
#to the build directory. If we need to make an appdir, we will move our stuff around
add_executable(sprite_table src/main.cpp)
set_target_properties(sprite_table PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
add_custom_command(TARGET sprite_table POST_BUILD COMMAND cp -r ../resources/* ./build)
target_link_libraries(sprite_table applib dfw tools dansdl2 ldtools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)

