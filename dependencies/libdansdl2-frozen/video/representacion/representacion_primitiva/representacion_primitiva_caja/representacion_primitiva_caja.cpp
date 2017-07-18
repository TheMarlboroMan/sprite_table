#include "representacion_primitiva_caja.h"

using namespace DLibV;

Representacion_primitiva_caja::Representacion_primitiva_caja(Representacion_primitiva_poligono::tipo t,const Rect& p_pos, const ColorRGBA& c)
	:Representacion_primitiva_poligono(t, { 
		{p_pos.x, p_pos.y}, 
		{p_pos.x+(int)p_pos.w, p_pos.y}, 
		{p_pos.x+(int)p_pos.w, p_pos.y+(int)p_pos.h}, 
		{p_pos.x, p_pos.y+(int)p_pos.h}}, 
		c)
{

}

Representacion_primitiva_caja& Representacion_primitiva_caja::operator=(const Representacion_primitiva_caja& p_otro)
{
	Representacion_primitiva_poligono::operator=(p_otro);
	return *this;
}

Representacion_primitiva_caja::Representacion_primitiva_caja(const Representacion_primitiva_caja& p_otra)
	:Representacion_primitiva_poligono(p_otra) 
{

}

void Representacion_primitiva_caja::establecer_dimensiones(int w, int h)
{
	int 	nx=puntos[0].x+w,
		ny=puntos[0].y+h;

	puntos[1].x=nx;
	puntos[2].x=nx;
	puntos[2].y=ny;
	puntos[3].y=ny;

	actualizar_posicion_vista_rotacion();
}

void Representacion_primitiva_caja::establecer_posicion(const Rect& r)
{
	puntos.clear();

	original={r.x, r.y};

	int 	fx=r.x+r.w,
		fy=r.y+r.h;
		
	puntos.push_back({r.x, r.y});
	puntos.push_back({fx, r.y});
	puntos.push_back({fx, fy});
	puntos.push_back({r.x, fy});

	normalizar();
	actualizar_posicion_vista_rotacion();
}
