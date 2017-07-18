#ifndef HERRAMIENTAS_SDL_H
#define HERRAMIENTAS_SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include "../herramientas/herramientas.h"

namespace DLibH
{

class Herramientas_SDL
{
	private:
	
	Herramientas_SDL()
	{
		//Ahahahaaaaaa!!!.
	}

	public:

	static void error();
	static bool iniciar_SDL(Uint32=(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK));
        static void apagar_SDL();
	static bool arrancar_sistema_SDL(Uint32);
	static bool es_sistema_arrancado_SDL(Uint32);
	static void detener_sistema_SDL(Uint32);
};

}
#endif
