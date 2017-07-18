#ifndef REPRESENTACION_BITMAP
#define REPRESENTACION_BITMAP

#include "../representacion_grafica.h"

namespace DLibV
{

class Representacion_bitmap:public Representacion_grafica
{
	public:

	Representacion_bitmap();
	Representacion_bitmap(const Textura *, Rect={0,0,0,0}, Rect={0,0,0,0}, sampling=sampling::atlas);
	Representacion_bitmap(const Representacion_bitmap&);
	Representacion_bitmap& operator=(const Representacion_bitmap &);

	void establecer_recurso_sin_recortar(Textura const * r){Representacion_grafica::establecer_textura(r);}
};

} //Fin namespace DLibV

#endif
