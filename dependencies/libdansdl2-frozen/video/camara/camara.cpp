#include "camara.h"

using namespace DLibV;

Camara::Camara(Rect foco, Punto pos):
	info_volcado{pos.x, pos.y, foco.x, foco.y, (int)foco.w, (int)foco.h, 1.0}, 
	caja_foco(foco),
	posicion(pos),
	limitada(false), limite_min_x(0), limite_min_y(0),
	limite_max_x(0), limite_max_y(0)
{
	sincronizar_cajas();
	caja_pos.w=caja_foco.w;
	caja_pos.h=caja_foco.h;
}

void Camara::sincronizar_cajas()
{
	caja_pos.x=posicion.x;
	caja_pos.y=posicion.y;

	info_volcado.pos_x=posicion.x;
	info_volcado.pos_y=posicion.y;
	info_volcado.rel_x=caja_foco.x;
	info_volcado.rel_y=caja_foco.y;
	info_volcado.vista_w=caja_foco.w;
	info_volcado.vista_h=caja_foco.h;
}

/*Mueve la posición a la que apunta la cámara en la pantalla. Se usan las
coordenadas provistas.*/

void Camara::enfocar_a(Punto p)
{
	if(limitada)
	{
		auto procesar=[](int pos, int dimension, int limite_min, int limite_max, int &blanco)
		{
			int fin=pos + dimension;
			if(pos >= limite_min && fin  <= limite_max) blanco=pos;
			else if(pos < limite_min) blanco=limite_min;
			else if(fin > limite_max) blanco=limite_max-dimension;
		};

		if((int)caja_foco.w <= limite_max_x - limite_min_x) procesar(p.x, caja_foco.w, limite_min_x, limite_max_x, caja_foco.x);
		if((int)caja_foco.h <= limite_max_y - limite_min_y) procesar(p.y, caja_foco.h, limite_min_y, limite_max_y, caja_foco.y);
	}
	else
	{
		caja_foco.x=p.x;
		caja_foco.y=p.y;
	}

	sincronizar_cajas();
}

/*Mueve la posición a la que apunta la cámara sumando las cantidades provistas
en los parámetros.*/

void Camara::movimiento_relativo(int p_x, int p_y)
{
	enfocar_a({caja_foco.x+p_x, caja_foco.y+p_y});
}

/*Establece los límites a los que la cámara puede apuntar: es decir, los límites
máximos para la cámara dentro del "mundo".*/

void Camara::establecer_limites(int p_min_limite_x, int p_min_limite_y, int p_max_limite_x, int p_max_limite_y)
{
	limitada=true;
//	this->limite_max_x=p_max_limite_x-this->caja_foco.w;
//	this->limite_max_y=p_max_limite_y-this->caja_foco.h;
	limite_max_x=p_max_limite_x; //-this->caja_pos.w;
	limite_max_y=p_max_limite_y; //-this->caja_pos.h;
	limite_min_x=p_min_limite_x;
	limite_min_y=p_min_limite_y;
}

void Camara::limpiar_limite()
{
	limitada=false;
	limite_max_x=0;
	limite_max_y=0;
	limite_min_x=0;
	limite_min_y=0;
}

Punto Camara::transformar_posicion_raton(Punto p) const
{
	Punto res(p);
	res.x=caja_foco.x + (p.x * (caja_foco.w / (float) caja_pos.w));
	res.y=caja_foco.y + (p.y * (caja_foco.h / (float) caja_pos.h));
	return res;
}

//Un valor de 2 significa que todo es 2 veces más grande, ergo la caja de foco es dos veces más pequeña.
void Camara::mut_zoom(double v)
{
	if(v < 0.01) v=0.01;
	info_volcado.zoom=v;
	caja_foco.w=caja_pos.w / v;
	caja_foco.h=caja_pos.h / v;
}
