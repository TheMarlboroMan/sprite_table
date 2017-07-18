#include "herramientas_sdl.h"

using namespace DLibH;

void Herramientas_SDL::error()
{
	std::cout<<SDL_GetError();
}

bool Herramientas_SDL::iniciar_SDL(Uint32 p_flags)
{
	return SDL_Init(p_flags) >= 0 && TTF_Init()==0;
}

void Herramientas_SDL::apagar_SDL()
{
 	TTF_Quit();
	SDL_Quit();
}

bool Herramientas_SDL::arrancar_sistema_SDL(Uint32 p_flags)
{
	return SDL_InitSubSystem(p_flags)==0;
}

bool Herramientas_SDL::es_sistema_arrancado_SDL(Uint32 p_flags)
{
	return SDL_WasInit(p_flags)!=0;
}

void Herramientas_SDL::detener_sistema_SDL(Uint32 p_flags)
{
	SDL_QuitSubSystem(p_flags);
}
