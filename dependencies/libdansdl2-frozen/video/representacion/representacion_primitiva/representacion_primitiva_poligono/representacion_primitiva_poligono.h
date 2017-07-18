#ifndef PRIMITIVA_GRAFICA_POLIGONO_H
#define PRIMITIVA_GRAFICA_POLIGONO_H

/*
*/

#include <vector>

#include "../representacion_primitiva.h"

namespace DLibV
{
class Representacion_primitiva_poligono
	:public Representacion_primitiva
{
	public:

	enum class			tipo{relleno, lineas};

					Representacion_primitiva_poligono(tipo, const std::vector<Punto>&, ColorRGBA);
					Representacion_primitiva_poligono(const Representacion_primitiva_poligono&);
					Representacion_primitiva_poligono& operator=(const Representacion_primitiva_poligono&);
	virtual 			~Representacion_primitiva_poligono() {}

	virtual void 			ir_a(int x, int y);
	virtual Punto			obtener_posicion() const;
	virtual Rect			obtener_base_posicion_vista() const;

	protected:

	virtual void			volcado();
	void				normalizar();

	std::vector<Punto>		puntos;
	Punto				original;	//Guarda el primer punto original sin estar en 0.0.
	tipo				tipo_relleno;
};

}

#endif
