#include <SDL2/SDL2_gfxPrimitives.h>
#include "representacion_primitiva_poligono.h"

using namespace DLibV;

Representacion_primitiva_poligono::Representacion_primitiva_poligono(tipo t, const std::vector<Punto>& pt, ColorRGBA c)
	:Representacion_primitiva(c), puntos(pt), original(pt[0]), tipo_relleno(t)
{ 
	//Guardarlos de forma que el primero sea 0.0.
	normalizar();

	actualizar_posicion_vista_rotacion();
}

Representacion_primitiva_poligono::Representacion_primitiva_poligono(const Representacion_primitiva_poligono& o)
	:Representacion_primitiva(o), puntos(o.puntos), original(o.original), tipo_relleno(o.tipo_relleno)
{

}

Representacion_primitiva_poligono& Representacion_primitiva_poligono::operator=(const Representacion_primitiva_poligono& o)
{
	Representacion_primitiva::operator=(o);
	puntos=o.puntos;
	original=o.original;
	tipo_relleno=o.tipo_relleno;
	return *this;
}

void Representacion_primitiva_poligono::normalizar()
{
	for(auto& pt : puntos)
	{
		pt.x-=original.x;
		pt.y-=original.y;
	}
}

void Representacion_primitiva_poligono::volcado()
{
	preparar_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(tipo_relleno==tipo::relleno ? GL_POLYGON : GL_LINE_LOOP, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

Rect Representacion_primitiva_poligono::obtener_base_posicion_vista() const
{
	int x=puntos[0].x+original.x, y=puntos[0].y+original.y, maxx=x, maxy=y;

	for(auto p : puntos)
	{
		//Añadir el valor original, porque estos puntos empiezan en 0.0.
		p.x+=original.x;
		p.y+=original.y;

		if(p.x < x) x=p.x;
		else if(p.x > maxx) maxx=p.x;

		if(p.y < y) y=p.y;
		else if(p.y > maxy) maxy=p.y;
	}

	return Rect{x, y, (unsigned int)maxx-x, (unsigned int)maxy-y};
}


void Representacion_primitiva_poligono::ir_a(int x, int y)
{
	original.x=x;
	original.y=y;
	actualizar_posicion_vista_rotacion();
}

//Position is the first point.
Punto Representacion_primitiva_poligono::obtener_posicion() const
{
	return Punto{original.x, original.y};
}
