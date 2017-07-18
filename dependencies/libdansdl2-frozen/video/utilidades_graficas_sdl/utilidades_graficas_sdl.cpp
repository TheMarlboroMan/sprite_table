#include "utilidades_graficas_sdl.h"
#include <stdexcept>

using namespace DLibV;


SDL_Surface * Utilidades_graficas_SDL::nueva_superficie_formato(SDL_Surface const * p_origen)
{
	SDL_Rect caja;
	caja.x=0;
	caja.y=0;
	caja.w=p_origen->w;
	caja.h=p_origen->h;

	return nueva_superficie_formato(p_origen, caja, 0, 0);
}

SDL_Surface * Utilidades_graficas_SDL::nueva_superficie_formato(SDL_Surface const * p_origen, const SDL_Rect& p_caja, Uint32 p_flags, Uint32 p_color)
{
	SDL_Surface * origen=const_cast <SDL_Surface *> (p_origen);
	SDL_Surface * copia=nullptr;

	copia=SDL_CreateRGBSurface(
//		origen->flags|p_flags,
		p_flags,
		p_caja.w, p_caja.h, origen->format->BitsPerPixel,
		origen->format->Rmask, origen->format->Gmask, origen->format->Bmask, origen->format->Amask);

	SDL_FillRect(copia, nullptr, p_color);

	return copia;
}

SDL_Surface * Utilidades_graficas_SDL::copiar_superficie(SDL_Surface const * p_origen)
{
	SDL_Rect p_caja;
	p_caja.x=0;
	p_caja.y=0;
	p_caja.w=p_origen->w;
	p_caja.h=p_origen->h;

	Uint32 p_flags=p_origen->flags;	
	Uint32 p_color=0;	

	return copiar_superficie(p_origen, p_caja, p_flags, p_color);
}

SDL_Surface * Utilidades_graficas_SDL::copiar_superficie(SDL_Surface const * p_origen, const SDL_Rect& p_caja, Uint32 p_flags, Uint32 p_color)
{
	SDL_Surface * copia=Utilidades_graficas_SDL::nueva_superficie_formato(p_origen, p_caja, p_flags, p_color);
	SDL_Surface * origen=const_cast <SDL_Surface *> (p_origen);
	SDL_Rect * caja=const_cast <SDL_Rect *> (&p_caja);

	if(copia)
	{
		SDL_BlitSurface(origen, caja, copia, nullptr);
	}

	return copia;
}

SDL_Surface * Utilidades_graficas_SDL::cargar_imagen(const char * cadena, const SDL_Window * ventana)
{
        SDL_Surface * temporal=IMG_Load(cadena);
        if (!temporal) 
        {
		throw std::runtime_error(std::string("Utilidades_graficas_SDL::cargar_imagen() : Imagen no cargada:")+cadena);
        }

	if(!ventana) 
	{
		SDL_Surface * optimizada=SDL_ConvertSurfaceFormat(temporal, SDL_PIXELFORMAT_RGBA8888, 0);
      	        SDL_FreeSurface(temporal);
		return optimizada;
	}
	else
	{
		SDL_Surface * optimizada=SDL_ConvertSurface(temporal, SDL_GetWindowSurface(const_cast<SDL_Window *>(ventana))->format, 0);
      	        SDL_FreeSurface(temporal);
		return optimizada;
	}
}

SDL_Surface * Utilidades_graficas_SDL::cargar_imagen(const char * cadena)
{
        SDL_Surface * temporal=IMG_Load(cadena);
        if (!temporal) 
        {
		throw std::runtime_error(std::string("Utilidades_graficas_SDL::cargar_imagen() : Imagen no cargada:")+cadena);
        }

	return temporal;
}

Uint32 Utilidades_graficas_SDL::SDL_GetPixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	
	/* Here p is the address to the pixel we want to retrieve */
	if(SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	if(SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);

	switch(bpp) 
	{
		case 1:
			return *p;
		break;

		case 2:
			return *(Uint16 *)p;
		break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
			else return p[0] | p[1] << 8 | p[2] << 16;
		break;

		case 4:
			return *(Uint32 *)p;
		break;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
		break;
	}
}
