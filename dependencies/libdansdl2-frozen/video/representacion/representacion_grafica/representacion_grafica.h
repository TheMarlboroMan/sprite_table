#ifndef REPRESENTACION_GRAFICA_H
#define REPRESENTACION_GRAFICA_H

#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include "../representacion.h"
#include "../../textura/textura.h"
#include "../../../herramientas/log_base/log_base.h"

/*
Es la clase base de todas las representaciones que usan una textura
(encapsulada en un recurso) para ser representado. 

Viene acompañada de una estructura de transformación para su rotación.
*/

namespace DLibV
{

struct Representacion_grafica_transformacion
{
	bool invertir_horizontal;
	bool invertir_vertical;

	Representacion_grafica_transformacion():
		invertir_horizontal(false), invertir_vertical(false)
	{}

	bool es_transformacion() const {return invertir_horizontal || invertir_vertical;}

	void reiniciar()
	{
		invertir_horizontal=false;
		invertir_vertical=false;
	}
};

class Representacion_grafica:public Representacion
{
	public:

	enum 			FLAGS_RECT{FRECT_X=1, FRECT_Y=2, FRECT_W=4, FRECT_H=8};
	enum class		sampling{
					atlas, 	//Añade medio píxel al sampling.
					completo}; //No añade nada.

				Representacion_grafica(Rect={0,0,0,0}, Rect={0,0,0,0}, sampling=sampling::completo);
				Representacion_grafica(ColorRGBA, Rect={0,0,0,0}, Rect={0,0,0,0}, sampling=sampling::completo);
				Representacion_grafica(const Representacion_grafica&);
				Representacion_grafica& operator=(const Representacion_grafica &);
	virtual 		~Representacion_grafica() {}

	Textura * 		ptr_textura() const {return textura;}

	int			acc_w_textura() const {return textura!=nullptr ? textura->acc_w() : -1;}
	int			acc_h_textura() const {return textura!=nullptr ? textura->acc_h() : -1;}

	void 			transformar_invertir_horizontal(bool v) {transformacion.invertir_horizontal=v;}
	void 			transformar_invertir_vertical(bool v) {transformacion.invertir_vertical=v;}
	virtual void 		reiniciar_transformacion() {Representacion::reiniciar_transformacion(); transformacion.reiniciar();}
	virtual bool 		es_transformada() const {return Representacion::es_transformada() || transformacion.es_transformacion();}

	void			establecer_pincel(int w, int h) {pincel.w=w; pincel.h=h;}

	const Rect& 		acc_posicion() const {return posicion;}
	Rect 			acc_posicion() {return posicion;}
	void			establecer_posicion(int, int, int=-1, int=-1, int=15);
	void 			establecer_posicion(Rect);

	const Rect& 		acc_recorte() const {return recorte;}
	Rect 			acc_recorte() {return recorte;}
	void 			establecer_recorte(Sint16, Sint16, Uint16, Uint16, int=15);
	void 			establecer_recorte(Rect);
	void 			recorte_a_medidas_textura();
	void			establecer_tipo_sampling(sampling v) {liberar_calculos(); tipo_sampling=v;}
	sampling		acc_tipo_sampling() const {return tipo_sampling;}

	Representacion_grafica_transformacion& acc_transformacion() {return transformacion;}

	void 			establecer_textura(Textura const * p_textura) {textura=const_cast <Textura *> (p_textura);}

	virtual void 		ir_a(int x, int y);
	virtual Punto		obtener_posicion() const;
	virtual Rect		obtener_base_posicion_vista() const;

	private:

	Representacion_grafica_transformacion transformacion;
	void			calcular_puntos();

	Textura * textura;	//Este es el puntero a su superficie de memoria. Es un puntero y no una referencia para permitir cambiarlo.

	struct 	{int w, h;}	pincel;
	struct puntotex {GLfloat x, y;};

	std::vector<Punto>	puntos;
	std::vector<puntotex>	final_ptex;
	sampling		tipo_sampling;

	protected:

	Rect 			posicion; 	//Lugar en que se muestra de la pantalla.
	Rect	 		recorte;	//Considerando la dimensión total de la representación, la parte que mostramos.

	void			liberar_calculos();
	void 			liberar_textura();
	void 			anular_textura() {textura=nullptr;}

	virtual void 		volcado();
};

} //Fin namespace DLibV

#endif
