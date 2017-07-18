#Sprite sheet editor.

Graphical interface to create rectangular sections on a sprite sheet. Those rectangular sections are later saved to a text file. The format of said text file is compatible with other tools in these repositories.

Save early, save often. Crashes happen.

#Building

This project depends on libdansdl2.a and herramientas_proyecto, both available on https://github.com/TheMarlboroMan/

In order to compile, the branches master-es of both libdansdl2.a and herramientas_proyecto must be selected and then built. The project can be built next. 

As of today, it would be easier to start from scratch than to try and adapt the code to work with current branches. Since it still works and tools are available to build it, it will remain like this.

#Help

Application is invoked from the command line with:

	./a.out img=path_to_image out=path_to_output [res=WxH]

Like this:

	./a.out img=ejemplos/MiniKnightSet.png out=mynewout.dat res=400x400

There is no in-application help, so:

	ESC -> Exit.
	Cursor keys -> move.
	TAB -> Change mode (camera, move, resize, adjust origin).
	SPACE -> Toggle show-hide index.
	RE.PAG -> Next cut.
	AV.PAG -> Previous cut.
	DEL -> Remove selected cut.
	L SHIFT -> Pixel precision movement.
	L CTRL -> Move by snap parameter value.
	N -> New cut.
	S -> Save (to output).
	L -> Load (output, if not saved).
	D -> Duplicate cut.
	Z -> Zoom in (L SHIFT to zoom out)
	L SHIFT + Z -> Zoom out.
	P -> Presentation mode.

#History.

- As of November 5th 2015 the code has been patched with a compatibility layer to support many radical changes to the libdansdl2. Should the code be updated, the compatibility layer will be removed.
- As of July 2016 the code has been updated to drop SDL2 graphic capabilities and now uses OpenGL. There should be a few glitches here and there.
- As of July 2017 the code does not compile with the current branches of libdansdl2 and herramientas_proyecto due to translation of code and changes in the core functionality.

#Bugs

- Due to opengl rendering the zoom function is sketchy and will not give pixel precision. You get used to it... Using linear texture filters in libdansdl2 works, for obvious reasons, but looks ugly.
- Not a bug, but there are many hacks here and there to compensate for bugs in the old version of libdansdl2.
