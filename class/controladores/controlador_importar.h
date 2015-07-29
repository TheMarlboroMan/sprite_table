#ifndef CONTROLADOR_IMPORTAR_H
#define CONTROLADOR_IMPORTAR_H

#include "controlador_base.h"
#include "../app/frame.h"

class Controlador_importar:public Controlador_base
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
	
	Controlador_importar(Director_estados &DI, Cola_mensajes& CM, Frames& f, const std::string& ruta);
	~Controlador_importar();

	virtual void preloop(Input_base& input, float delta) {}
	virtual void postloop(Input_base& input, float delta) {}
	virtual void loop(Input_base& input, float delta);
	virtual void dibujar(DLibV::Pantalla& pantalla);

	void importar();
};

#endif
