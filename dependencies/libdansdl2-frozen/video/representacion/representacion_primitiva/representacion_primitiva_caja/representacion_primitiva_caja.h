#ifndef PRIMITIVA_GRAFICA_CAJA_H
#define PRIMITIVA_GRAFICA_CAJA_H

#include "../representacion_primitiva_poligono/representacion_primitiva_poligono.h"

//Simplemente un caso especial de un polígono.

namespace DLibV
{
class Representacion_primitiva_caja
	:public Representacion_primitiva_poligono
{
	public:
	
				Representacion_primitiva_caja(Representacion_primitiva_poligono::tipo, const Rect& p_pos, const ColorRGBA&);
				Representacion_primitiva_caja(const Representacion_primitiva_caja& p_otra);
				Representacion_primitiva_caja& operator=(const Representacion_primitiva_caja& p_otro);
	virtual 		~Representacion_primitiva_caja() {}

	void			establecer_dimensiones(int, int);
	void			establecer_posicion(const Rect&);
};

}

#endif
