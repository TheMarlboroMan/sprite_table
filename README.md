# Sprite sheet editor.

Graphical interface to create rectangular sections on a sprite sheet. Those rectangular sections are later saved to a text file. The format of said text file is compatible with other tools in these repositories.

## Building

Make sure to have:

	libsdl2-dev (plus ttf, image and mixer extensions)

Download, build and install the following repos from github.com/themarlboroman

- libdansdl2
- libdansdl2-tools
- tools
- lm
- dfw

You will need to define one of the possible build environments, either the AppImage build (if you have the necessary deps to build one) or the standalone build:

mkdir -p build
cd build
cmake .. -D BUILD_REGULAR=YES/NO
cmake .. -D BUILD_APPIMAGE=YES/NO

You can also opt to build a debug or production environment, which can greatly break the application depending on the libdansdl2 library that has been built!

cmake .. -D BUILD_DEBUG=YES/NO

Then just do the usual make dance.

## Help

Press f1 in the application main screen or run with -h.

## Command line parameters

-h shows the help file.
-f session_file: opens the session file
-i image_file: uses background image

## Configuration

Upon building, the configuration file will be in data/config.json. Values that
can be useful to change are:

- default_sprite_w: the default width of a new sprite.
- default_sprite_h: the default height of a new sprite.
- movement_factor: how many pixels per tic are moved in an arrow keypress.
- window_w_px: width of the window
- window_w_logical: logical width of the window. If you are so inclined, you
	could set it to half of the window width and have a zoomed view. In most
	cases, this must be kept in sync with the above value.
- window_h_px: height of the window
- window_h_logical: logical height of the window

## History.

- As of July 2020 the project has been rewritten.
- As of November 5th 2015 the code has been patched with a compatibility layer to support many radical changes to the libdansdl2. Should the code be updated, the compatibility layer will be removed.
- As of July 2016 the code has been updated to drop SDL2 graphic capabilities and now uses OpenGL. There should be a few glitches here and there.
- As of July 2017 the code does not compile with the current branches of libdansdl2 and herramientas_proyecto due to translation of code and changes in the core functionality.
- 26/07/2017: Bugs:
	- Fixed camera bug: camera was limited to image size when image size was yet unknown.
	- Fixed stupid zoom bug. Turns out it wasn't opengl. It was me and something about atlas vs complete sampling in libdansdl2.
