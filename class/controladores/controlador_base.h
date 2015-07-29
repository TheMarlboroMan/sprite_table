#ifndef CONTROLADOR_BASE
#define CONTROLADOR_BASE

#include "../framework/interface_controlador.h"
#include "director_estados.h"
#include "../app/cola_mensajes.h"

class Controlador_base:public Interface_controlador
{
	private:

	Director_estados& DI;
	Cola_mensajes& COLA_M;
	bool salir;
	bool rompe_loop;

	protected:

	void abandonar_aplicacion() {salir=true;}

	void insertar_mensaje(const std::string&m) {COLA_M.insertar_mensaje(m);}
 
	public:

	void solicitar_cambio_estado(Director_estados::t_estados v) 
	{
		rompe_loop=true;
		DI.solicitar_cambio_estado(v);
	}

	virtual bool es_abandonar_aplicacion() const {return salir;}
	virtual bool es_romper_loop() const {return rompe_loop;}

	Controlador_base(Director_estados &d, Cola_mensajes& cm): DI(d), COLA_M(cm), salir(false), rompe_loop(false)
	{}

	void restaurar_loop() {rompe_loop=false;}
	void romper_loop() {rompe_loop=true;}
};

#endif
