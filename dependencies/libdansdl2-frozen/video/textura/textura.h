#ifndef TEXTURA_SDL_H
#define TEXTURA_SDL_H

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include <SDL2/SDL.h>
#include "../superficie/superficie.h"

namespace DLibV
{

class Textura
{
	public:
			Textura(const Superficie&);
			Textura(const Textura&)=delete;
			~Textura();
	Textura& 	operator=(const Textura& t)=delete;

	unsigned int 	acc_w() const {return w;}
	unsigned int 	acc_h() const {return h;}
	GLuint		acc_indice() const {return indice;}
	void		reemplazar(const Superficie&);

	private:

	void		cargar(const SDL_Surface *);

	GLuint		indice;
	int 		mode, 
			w,
	 		h;
};

}

#endif
