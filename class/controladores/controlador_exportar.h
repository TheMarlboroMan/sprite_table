#ifndef CONTROLADOR_EXPORTAR_H
#define CONTROLADOR_EXPORTAR_H

#include "controlador_base.h"
#include "../app/frame.h"
#include <memory>

class Controlador_exportar:public Controlador_base
{
	//////////////
	//Definiciones
	private:

	//////////////
	//Propiedades
	private:

	Frames& frames;
	std::string ruta;

	///////////////
	//Métodos internos.

	private:

	///////////////
	//Interface pública.
	public:
	
	Controlador_exportar(Director_estados &DI, Cola_mensajes& CM, Frames& f, const std::string& ruta);
	~Controlador_exportar();

	virtual void preloop(Input_base& input, float delta) {}
	virtual void postloop(Input_base& input, float delta) {}
	virtual void loop(Input_base& input, float delta);
	virtual void dibujar(DLibV::Pantalla& pantalla);
};

#endif
