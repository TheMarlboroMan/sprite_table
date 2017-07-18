#ifndef REPRESENTACION_PRIMITIVA_H
#define REPRESENTACION_PRIMITIVA_H

#include "../representacion.h"
#include "../../color/color.h"
#include "../../../herramientas/herramientas_sdl/herramientas_sdl.h"

/*Esto será una clase abstracta que podremos usar para volcar
en una pantalla... De por si no hay nada de código aquí, solo
los esqueletos de lo que vamos a usar más adelante.
*/

namespace DLibV
{

class Representacion_primitiva:public Representacion
{
	public:

			Representacion_primitiva(ColorRGBA);
			Representacion_primitiva(const Representacion_primitiva&);
			Representacion_primitiva& operator=(const Representacion_primitiva&);
	virtual 	~Representacion_primitiva() {}

	//Estas hay que definirlas.
	virtual void 	ir_a(int x, int y)=0;
	virtual Punto	obtener_posicion() const=0;
	virtual Rect	obtener_base_posicion_vista() const=0;

	protected:

	void 		preparar_color();
	virtual void 	volcado()=0;
};

} //Fin namespace DLibV

#endif
