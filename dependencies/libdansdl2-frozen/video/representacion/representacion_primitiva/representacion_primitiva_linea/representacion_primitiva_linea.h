#ifndef PRIMITIVA_GRAFICA_LINEA_H
#define PRIMITIVA_GRAFICA_LINEA_H

#include <array>
#include "../representacion_primitiva.h"

namespace DLibV
{
class Representacion_primitiva_linea:public Representacion_primitiva
{
	public:
	
					Representacion_primitiva_linea(int x1, int y1, int x2, int y2, ColorRGBA);
					Representacion_primitiva_linea(const Representacion_primitiva_linea&);
	virtual 			~Representacion_primitiva_linea() {}
					Representacion_primitiva_linea& operator=(const Representacion_primitiva_linea&);

	virtual void 			ir_a(int x, int y);
	virtual Punto			obtener_posicion() const;
	virtual Rect			obtener_base_posicion_vista() const;

	void 				establecer_puntos(int x1, int y1, int x2, int y2);

	protected:

	virtual void			volcado();

	private:

	std::array<Punto, 2>		puntos;
	Punto				original;
};

}

#endif
