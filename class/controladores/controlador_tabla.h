#ifndef CONTROLADOR_TABLA
#define CONTROLADOR_TABLA

#include "controlador_base.h"
#include "../app/frame.h"
#include <memory>

class Controlador_tabla:public Controlador_base
{
	//////////////
	//Definiciones
	private:

	enum class t_estados {CAMARA, MOVER, REDIMENSIONAR};

	//////////////
	//Propiedades
	private:

	Frame * actual;
	t_estados estado;
	Frames& frames;
	DLibV::Camara camara;
	DLibV::Representacion_bitmap_dinamica rep_imagen;
	DLibV::Representacion_bitmap_estatica rep_icono;
	DLibV::Representacion_texto_auto_estatica rep_status_actual;
	DLibV::Representacion_texto_auto_estatica rep_mensaje_actual;
	DLibV::Representacion_primitiva_caja_dinamica caja_fondo;
//	DLibV::Representacion_primitiva_linea_estatica linea_absurda;

	struct Info_zoom
	{
		unsigned int zoom;
		unsigned int w;
		unsigned int h;

		Info_zoom(unsigned int z, unsigned int pw, unsigned int ph):
			zoom(z), w(pw), h(ph)
		{}
	}info_zoom;

	///////////////
	//Métodos internos.

	private:

	void procesar_input(int x, int y);
	void mover_actual(int x, int y);
	void redimensionar_actual(int x, int y);
	void insertar_nuevo();
	void ciclo_estado();
	void ciclo_frames(int dir);
	void ciclo_zoom();
	void eliminar();
	void duplicar();
	void seleccionar_por_posicion(int x, int y);
//	void dibujar_caja(DLibV::Pantalla& pantalla, SDL_Rect c, unsigned int r, unsigned int g, unsigned int b);
	void dibujar_frame(DLibV::Pantalla& pantalla, Frame& f, bool actual);

	///////////////
	//Interface pública.
	public:
	
	Controlador_tabla(Director_estados &DI, Cola_mensajes& CM, DLibV::Pantalla& pantalla, Frames& f, unsigned int cw, unsigned int ch); //, DLibV::Pantalla&);
	~Controlador_tabla();

	virtual void preloop(Input_base& input, float delta) {}
	virtual void postloop(Input_base& input, float delta) {}
	virtual void loop(Input_base& input, float delta);
	virtual void dibujar(DLibV::Pantalla& pantalla);

	void reset_actual();
	void restaurar_posicion_ids();
	void asignar_mensaje(const std::string& m);
};

#endif
