#include "representacion_primitiva_linea.h"

using namespace DLibV;



Representacion_primitiva_linea::Representacion_primitiva_linea(int px1, int py1, int px2, int py2, ColorRGBA c)
	:Representacion_primitiva(c), original{px1, py1}
{
	establecer_puntos(px1, py1, px2, py2);
	actualizar_posicion_vista_rotacion();
}

Representacion_primitiva_linea::Representacion_primitiva_linea(const Representacion_primitiva_linea& p_otra)
	:Representacion_primitiva(p_otra), puntos(p_otra.puntos), original(p_otra.original)
{

}

Representacion_primitiva_linea& Representacion_primitiva_linea::operator=(const Representacion_primitiva_linea& p_otro)
{
	Representacion_primitiva::operator=(p_otro);
	puntos=p_otro.puntos;
	original=p_otro.original;
	return *this;
}

void Representacion_primitiva_linea::establecer_puntos(int px1, int py1, int px2, int py2)
{
	//Guardar como 0.0...
	original={px1, py1};
	puntos[0]={px1-original.x, py1-original.y};
	puntos[1]={px2-original.x, py2-original.y};
	actualizar_posicion_vista_rotacion();
}

Rect Representacion_primitiva_linea::obtener_base_posicion_vista() const
{
	int x, y, w, h;

	auto f=[](int v1, int v2, int& pos, int& medida)
	{
		if(v1 < v2)
		{
			pos=v1;
			medida=abs(v2-v1);
		}
		else
		{
			pos=v2;
			medida=abs(v1-v2);
		}
	};

	f(puntos[0].x+original.x, puntos[1].x+original.x, x, w);
	f(puntos[0].y+original.y, puntos[1].y+original.y, y, h);

	return Rect{x, y, (unsigned int)w, (unsigned int)h};
}

void Representacion_primitiva_linea::volcado()
{
	preparar_color();
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_LINES, 0, 2);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Representacion_primitiva_linea::ir_a(int x, int y)
{
	original.x=x;
	original.y=y;
	actualizar_posicion_vista_rotacion();
}

//El punto más arriba y más a la izquierda.
Punto Representacion_primitiva_linea::obtener_posicion() const
{
	int x, y;

	auto f=[](int v1, int v2, int& pos)
	{
		if(v1 < v2) pos=v1;
		else pos=v2;
	};

	f(puntos[0].x+original.x, puntos[1].x+original.x, x);
	f(puntos[0].y+original.y, puntos[1].y+original.y, y);

	return Punto{x, y};
}

