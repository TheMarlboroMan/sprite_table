#ifndef REPRESENTACION_H
#define REPRESENTACION_H

#include <SDL2/SDL.h>
#include "../color/color.h"
#include "../rect/rect.h"
#include "../camara/camara.h"
#include "../info_volcado/info_volcado.h"
#include "../pantalla/pantalla.h"
#include "../../herramientas/poligono_2d/poligono_2d.h"

namespace DLibV
{

struct Representacion_transformacion
{
	float 				angulo_rotacion;
	DLibH::Punto_2d<float>		centro_rotacion;

	Representacion_transformacion():
		angulo_rotacion(0.f), centro_rotacion(0.f, 0.f)
	{}

	bool 				es_transformacion() const {return angulo_rotacion!=0.f;}
	bool 				es_cambia_centro_rotacion() const {return centro_rotacion.x!=0.f || centro_rotacion.y!=0.f;}

	void 				reiniciar()
	{
		angulo_rotacion=0.f;
		centro_rotacion.x=0.f;
		centro_rotacion.y=0.f;
	}
};

class Representacion
{
	public:

	enum class 		blends{nada, alpha};
	enum 			alpha{alpha_min=0, alpha_max=255};

				Representacion();
				Representacion(ColorRGBA);
				Representacion(const Representacion&);
	Representacion& 	operator=(const Representacion &);
	virtual 		~Representacion() {}

	bool 			es_en_toma(const Rect& rect) const {return rect.es_en_colision_con(posicion_vista, true);}
	const Rect&		acc_posicion_vista() const {return posicion_vista;}
	Rect 			copia_posicion_vista() {return posicion_vista;}

	void 			hacer_invisible() {this->visible=false;}
	void 			hacer_visible() {this->visible=true;}
	void 			intercambiar_visibilidad() {this->visible=!this->visible;}
	void 			cambiar_visibilidad(bool p_valor) {this->visible=p_valor;}
	bool 			es_visible() const {return this->visible;}

	//Virtuales por el caso especial de las agrupadas...
	virtual void		volcar(Pantalla&, const Camara&, bool=false);
	virtual void		volcar(Pantalla&, bool=false);

	void			mut_rgba(ColorRGBA v) {rgba=v;}
	void 			establecer_alpha(unsigned int v) {rgba.a=colorfi(v);}
	void 			establecer_color(unsigned int pr, unsigned int pg, unsigned int pb)
	{
		rgba.r=colorfi(pr);
		rgba.g=colorfi(pg);
		rgba.b=colorfi(pb);
	}

	ColorRGBA		acc_rgba() const {return rgba;}
	int			acc_alpha() const {return colorif(rgba.a);}
	int 			acc_color_r() const {return colorif(rgba.r);}
	int 			acc_color_g() const {return colorif(rgba.g);}
	int		 	acc_color_b() const {return colorif(rgba.b);}

	void 			establecer_modo_blend(blends v) {this->modo_blend=v;}
	blends		 	acc_modo_blend() const {return this->modo_blend;}

	virtual void 		reiniciar_transformacion() {transformacion.reiniciar();}

	void 			transformar_rotar(float v);
	void 			transformar_cancelar_rotar();
	void 			transformar_centro_rotacion(float x, float y);
	void 			transformar_centro_rotacion_cancelar();
	virtual bool 		es_transformada() const {return transformacion.es_transformacion();}

	//Estas hay que definirlas.
	void 			ir_a(Punto p) {ir_a(p.x, p.y);}
	virtual void 		ir_a(int x, int y)=0;
	virtual Punto		obtener_posicion() const=0;
	virtual Rect		obtener_base_posicion_vista() const=0;
	void			debug_trazar_caja() const;

	private:

	Representacion_transformacion transformacion;

	bool 			visible;
	blends		 	modo_blend;
	ColorRGBA		rgba;

	Rect	 		posicion_vista;

	protected:
	
	const Representacion_transformacion& acc_transformacion_rotacion() const {return transformacion;}
	void 			actualizar_posicion_vista_rotacion();
	Rect			calcular_posicion_vista_rotacion() const;
	virtual void 		volcado()=0;
	void			transformacion_pre_render(const Info_volcado);
};

} //Fin namespace DLibV

#endif
