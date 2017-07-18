#include "imagen.h"

using namespace DLibV;

//Toma propiedad del puntero a partir de este momento..

Imagen::Imagen(SDL_Surface * p_superficie)
	:Superficie(p_superficie)
{

}

Imagen::Imagen(const std::string& p_ruta, SDL_Window * ventana)
	:Superficie(Utilidades_graficas_SDL::cargar_imagen(p_ruta.c_str(), ventana))
{

}

Imagen::Imagen(const std::string& p_ruta)
	:Superficie(Utilidades_graficas_SDL::cargar_imagen(p_ruta.c_str()))
{

}

Imagen& Imagen::operator=(const Imagen& p_otro)
{
	Superficie::operator=(p_otro);
	return *this;
}

Imagen::Imagen(const Imagen& p_otro)
{
	Superficie::operator=(p_otro);
}


Imagen::~Imagen()
{

}
