#Sprite sheet editor.

Graphical interface to create rectangular sections on a sprite sheet. Those rectangular sections are later saved to a text file. The format of said text file is compatible with other tools in these repositories.

#Building

Make sure to have:

	libsdl2-dev (plus ttf, image and mixer extensions)

Download, build and install the following repos from github.com/themarlboroman

- libdansdl2
- libdansdl2-tools
- tools
- lm
- dfw

#Help

#History.

- As of November 5th 2015 the code has been patched with a compatibility layer to support many radical changes to the libdansdl2. Should the code be updated, the compatibility layer will be removed.
- As of July 2016 the code has been updated to drop SDL2 graphic capabilities and now uses OpenGL. There should be a few glitches here and there.
- As of July 2017 the code does not compile with the current branches of libdansdl2 and herramientas_proyecto due to translation of code and changes in the core functionality.
- 26/07/2017: Bugs:
	- Fixed camera bug: camera was limited to image size when image size was yet unknown.
	- Fixed stupid zoom bug. Turns out it wasn't opengl. It was me and something about atlas vs complete sampling in libdansdl2.

