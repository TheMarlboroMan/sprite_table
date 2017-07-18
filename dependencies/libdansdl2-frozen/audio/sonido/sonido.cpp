#include "sonido.h"
#include <iostream>
#include <stdexcept>

using namespace DLibA;

Sonido::Sonido()
	:preparado(false)
{

}

Sonido::Sonido(const char * p_ruta)
	:preparado(false)
{
	cargar(p_ruta);
}

Sonido::~Sonido()
{
	liberar();
}

void Sonido::liberar()
{
	if(this->p_sonido)
	{
		Mix_FreeChunk(p_sonido);
		p_sonido=nullptr;
		preparado=false;
	}
}

void Sonido::cargar(const char * p_ruta)
{
	p_sonido=Mix_LoadWAV(p_ruta);
	ruta=p_ruta;

	if(!this->p_sonido)
	{
		throw std::runtime_error(std::string("Sonido::cargar() : sonido no cargado:")+p_ruta);
	}

	preparado=true;
}
