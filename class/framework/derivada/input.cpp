#include "input.h"

void Input::configurar()
{
	//El input I_ESCAPE es la tecla SDLK_ESCAPE. Simple.
	mapa_teclado.insert(std::make_pair(I_ESCAPE, SDL_SCANCODE_ESCAPE));

	mapa_teclado.insert(std::make_pair(I_ESPACIO, SDL_SCANCODE_SPACE));
	mapa_teclado.insert(std::make_pair(I_TAB, SDL_SCANCODE_TAB));

	mapa_teclado.insert(std::make_pair(I_IZQUIERDA, SDL_SCANCODE_LEFT));
	mapa_teclado.insert(std::make_pair(I_DERECHA, SDL_SCANCODE_RIGHT));
	mapa_teclado.insert(std::make_pair(I_ARRIBA, SDL_SCANCODE_UP));
	mapa_teclado.insert(std::make_pair(I_ABAJO, SDL_SCANCODE_DOWN));

	mapa_teclado.insert(std::make_pair(I_RE_PAG, SDL_SCANCODE_PAGEUP));
	mapa_teclado.insert(std::make_pair(I_AV_PAG, SDL_SCANCODE_PAGEDOWN));
	mapa_teclado.insert(std::make_pair(I_DEL, SDL_SCANCODE_DELETE));

	mapa_teclado.insert(std::make_pair(I_PASO, SDL_SCANCODE_LSHIFT));
	mapa_teclado.insert(std::make_pair(I_NUEVO, SDL_SCANCODE_N));
	mapa_teclado.insert(std::make_pair(I_GRABAR, SDL_SCANCODE_S));
	mapa_teclado.insert(std::make_pair(I_CARGAR, SDL_SCANCODE_L));
	mapa_teclado.insert(std::make_pair(I_ZOOM, SDL_SCANCODE_Z));
	mapa_teclado.insert(std::make_pair(I_DUPLICAR, SDL_SCANCODE_D));
	mapa_teclado.insert(std::make_pair(I_PRESENTACION, SDL_SCANCODE_P));
	mapa_teclado.insert(std::make_pair(I_SNAP, SDL_SCANCODE_LCTRL));

	mapa_raton.insert(std::make_pair(I_CLICK_I, SDL_BUTTON_LEFT));
	mapa_raton.insert(std::make_pair(I_CLICK_I, SDL_BUTTON_RIGHT));
}
