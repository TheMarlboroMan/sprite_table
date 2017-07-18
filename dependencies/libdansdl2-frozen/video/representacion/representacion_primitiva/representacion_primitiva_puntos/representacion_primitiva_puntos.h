#ifndef PRIMITIVA_GRAFICA_PUNTOS_H
#define PRIMITIVA_GRAFICA_PUNTOS_H

#include "../representacion_primitiva.h"
#include <vector>

namespace DLibV
{
class Representacion_primitiva_puntos:
	public Representacion_primitiva
{
	public:
	
						Representacion_primitiva_puntos(ColorRGBA);
						Representacion_primitiva_puntos(const std::vector<Punto>&, ColorRGBA);
						Representacion_primitiva_puntos(Punto, ColorRGBA);
						Representacion_primitiva_puntos(const Representacion_primitiva_puntos&);
	virtual 				~Representacion_primitiva_puntos() {}
						Representacion_primitiva_puntos& operator=(const Representacion_primitiva_puntos&);

	void 					insertar(int x, int y) {insertar_interno({x, y}, true);}
	void					insertar(Punto p) {insertar_interno(p, true);}
	void					insertar(const std::vector<Punto>&);
	void					limpiar_puntos();

	virtual void 				ir_a(int x, int y);
	virtual Punto				obtener_posicion() const;
	virtual Rect				obtener_base_posicion_vista() const;

	protected: 

	virtual void				volcado();

	private:

	void					insertar_interno(Punto, bool=true);

	std::vector<Punto>			puntos;
	Punto					original;
};

}

#endif
