#include "textura.h"
#include <vector>
#include <cstdint>

using namespace DLibV;

Textura::Textura(const Superficie& s):
	indice(0), mode(GL_RGB), w(0), h(0)
{
	cargar(s.acc_superficie());
}

Textura::~Textura()
{
	glDeleteTextures(1, &indice);
}

/*
Textura::Textura(const Textura& t)
	:indice(0), mode(t.mode), w(t.w), h(t.h)
{

	//TODO: This isn't working.
	glBindTexture(GL_TEXTURE_2D, t.indice);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, mode, 0, 0, w, h, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	std::vector<uint8_t> raw_img;

	//TODO: Test this with other sizes.
	int bpp=mode==GL_RGBA ? 4 : 3;
	raw_img.reserve((w*h)*bpp);

	glGetTexImage(GL_TEXTURE_2D, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());
	glGenTextures(1, &indice);
	glBindTexture(GL_TEXTURE_2D, indice);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());
}

Textura& Textura::operator=(const Textura& t)
{
	//TODO: This isn't working.
	mode=t.mode;
	w=t.w;
	h=t.h;

	glBindTexture(GL_TEXTURE_2D, t.indice);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, mode, 0, 0, w, h, 0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	std::vector<uint8_t> raw_img;

	//TODO: Test this with other sizes.
	int bpp=mode==GL_RGBA ? 4 : 3;
	raw_img.reserve((w*h)*bpp);

	glGetTexImage(GL_TEXTURE_2D, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());
	glGenTextures(1, &indice);
	glBindTexture(GL_TEXTURE_2D, indice);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, raw_img.data());

	return *this;
}
*/

void Textura::reemplazar(const Superficie& s)
{
	cargar(s.acc_superficie());
}

void Textura::cargar(const SDL_Surface * superficie)
{
	w=superficie->w;
	h=superficie->h;

	//Si el índice no existe se pide una nueva textura a OpenGL. En caso contrario
	//se trabaja sobre la original.
	if(!indice)
	{
		glGenTextures(1, &indice);
	}
	
	glBindTexture(GL_TEXTURE_2D, indice);

 	if(superficie->format->BytesPerPixel==4) mode=GL_RGBA;
	else mode=GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, w, h, 0, mode, GL_UNSIGNED_BYTE, superficie->pixels);
}
