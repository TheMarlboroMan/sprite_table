#include "representacion_bitmap.h"

using namespace DLibV;
Representacion_bitmap::Representacion_bitmap()
	:Representacion_grafica()
{
	actualizar_posicion_vista_rotacion();
}

Representacion_bitmap::Representacion_bitmap(const Textura * p_rec, Rect pos, Rect rec, sampling s)
	:Representacion_grafica(pos, rec, s)
{
	establecer_textura(p_rec);
	
	if(!pos.w) posicion.w=recorte.w;
	if(!pos.h) posicion.h=recorte.h;

	actualizar_posicion_vista_rotacion();
}

Representacion_bitmap::Representacion_bitmap(const Representacion_bitmap& o)
	:Representacion_grafica(o)
{

}

Representacion_bitmap& Representacion_bitmap::operator=(const Representacion_bitmap & p_otra)
{
	Representacion_grafica::operator=(p_otra);
	return *this;
}
