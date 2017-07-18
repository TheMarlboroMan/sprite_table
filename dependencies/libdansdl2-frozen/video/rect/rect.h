#ifndef LIDBANSDL2_RECT_H
#define LIDBANSDL2_RECT_H

#include <SDL2/SDL.h>
#include "../../herramientas/caja/caja.h"

namespace DLibV
{
typedef DLibH::Caja<int, unsigned int> 	Rect;
typedef DLibH::Punto_2d<int>		Punto;
//Rect desde_sdl_rect(SDL_Rect);
}

#endif
