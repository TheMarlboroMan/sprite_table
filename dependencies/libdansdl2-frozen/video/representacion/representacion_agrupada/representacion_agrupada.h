#ifndef REPRESENTACION_AGRUPADA_H
#define REPRESENTACION_AGRUPADA_H

#include "../representacion.h"
#include "../../../herramientas/herramientas_sdl/herramientas_sdl.h"
#include <vector>

namespace DLibV
{

/*Tiene un vector de varias representaciones. Cuando se lo pasamos a la pantalla
las vuelca en el orden en que se han insertado. Las representacions deben
ser estáticas  o dinámicas, según el tipo de grupo. 

Una vez insertadas, el grupo toma posesión de las representaciones y se encarga 
de destruirlas al finalizar su tiempo de vida.*/

class Representacion_agrupada:public Representacion
{
	public:

				Representacion_agrupada(Punto, bool=true);
				Representacion_agrupada(const Representacion_agrupada&);
	virtual 		~Representacion_agrupada();
	void	 		insertar_representacion(Representacion *);

	void 			vaciar_grupo();

	unsigned int 		obtener_cuenta() const {return grupo.size();}

	virtual void 		ir_a(int x, int y);
	virtual Punto		obtener_posicion() const;
	virtual Rect		obtener_base_posicion_vista() const;

	std::vector<Representacion *>&	obtener_grupo() {return grupo;}

	virtual void		volcar(Pantalla&, const Camara&, bool=false);
	virtual void		volcar(Pantalla&, bool=false);

	private:

	void			volcado_interno(Pantalla&, Camara const *);

	Punto			posicion;
	bool			posee_las_representaciones;

	Representacion_agrupada& 	operator=(const Representacion_agrupada &);	//OJO: Es incopiable!.

	protected:

	std::vector<Representacion *> 	grupo;

	virtual void		volcado();
};

}

#endif
