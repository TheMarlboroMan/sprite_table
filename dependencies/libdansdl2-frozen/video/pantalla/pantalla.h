#ifndef PANTALLA_H
#define PANTALLA_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#ifdef WINCOMPIL
#include <GL/glext.h>
#endif
#include <iostream>
#include <string>
//#include <vector>
#include "../utilidades_graficas_sdl/utilidades_graficas_sdl.h"
#include "../camara/camara.h"
#include "../color/color.h"
//#include "../representacion/representacion.h"

namespace DLibV
{

class Pantalla
{
	public:

	enum modos_ventana {
		M_VENTANA, //La ventana se creará como ventana.
		M_PANTALLA_COMPLETA_RESOLUCION, //La ventana pasa a fullscreen con cambio de resolución incluido.
		M_PANTALLA_COMPLETA_SIMULADA, //La pantalla pasa a fullscreen simulando el cambio de resolución.
		M_MAX_MODO};

	//Interfaz pública.

					Pantalla(int=640, int=400, short unsigned int=M_VENTANA);
					~Pantalla();

	SDL_Window * 			acc_ventana() const {return this->ventana;}
	unsigned int 			acc_w() const {return w;}
	unsigned int 			acc_h() const {return h;}
	void 				establecer_posicion(int, int);
	void  				establecer_titulo(const char * p_cadena);
	void 				establecer_titulo(const std::string&);
	unsigned short int 		acc_modo_ventana() const {return modo_ventana;}

	void 				inicializar(int, int, int=SDL_WINDOW_OPENGL);
	void 				establecer_modo_ventana(unsigned int);
	void 				establecer_medidas_logicas(int, int);
	void 				limpiar(const ColorRGBA&);
	void 				actualizar();

	void 				asignar_camara(const Camara&);
	bool				es_con_camara() const {return camara_actual!=nullptr;}

	void 				establecer_clip(Camara const&);
	void 				establecer_clip(Rect);
	void 				reiniciar_clip();

	const Rect& 			acc_simulacro_caja() {return this->simulacro_caja;}
	const Info_volcado		acc_info_volcado() const {return info_volcado;};

	private:

	//Propiedades...

	SDL_Window * 			ventana;
	SDL_GLContext 			context;
	Camara const *			camara_actual;
	Info_volcado			info_volcado;
	Rect 				simulacro_caja; //El simulacro de caja es un rectángulo con las medidas de la pantalla, para controlar lo que se muestra o no.

	int 				w,
					h; //Ancho y alto de la ventana...

	unsigned short int 		modo_ventana;
	bool 				comprobar_en_toma;

	int 				w_logico, //Ancho y alto del renderer.
					h_logico;
};

} //Fin namespace DLibV

#endif
