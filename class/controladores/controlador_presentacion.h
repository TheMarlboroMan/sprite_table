#ifndef CONTROLADOR_PRESENTACION_H
#define CONTROLADOR_PRESENTACION_H

/* Simplemente presenta todos los frames en fila...*/

#include "controlador_base.h"
#include "../app/frame.h"
#include "../recursos.h"

class Controlador_presentacion:public Controlador_base
{
	private:

	Frames& frames;	
	DLibV::Camara camara;

	struct Info_presentacion
	{
		int x, y, w, h;
		Frame& frame;

		Info_presentacion(int px, int py, int pw, int ph, Frame& pf):
			x(px), y(py), w(pw), h(ph), frame(pf)
		{}
	};

	std::vector<Info_presentacion> info_presentacion;

	public:
	
	Controlador_presentacion(Director_estados &d, Cola_mensajes& cm, Frames& f, unsigned int w, unsigned int h);

	virtual void preloop(Input_base& input, float delta) {}
	virtual void postloop(Input_base& input, float delta) {}
	virtual void loop(Input_base& input, float delta);
	virtual void dibujar(DLibV::Pantalla& pantalla);

	void preparar_presentacion();
};

#endif
