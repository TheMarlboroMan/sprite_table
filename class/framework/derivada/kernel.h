#ifndef KERNEL_APP_SDL2
#define KERNEL_APP_SDL2

#include "../base/kernel_base.h"

#include "../derivada/cargador_recursos.h"
#include "../derivada/configuracion.h"
#include "../derivada/input.h"
#include "../derivada/localizador.h"

#include "../../recursos.h"

class Kernel_excepcion
{
	public:

	std::string mensaje;
	Kernel_excepcion(const std::string& m):mensaje(m) {}
};

class Kernel_app:public Kernel_base
{
	private:

	Cargador_recursos cargador_recursos;
	Configuracion configuracion;
	Localizador localizador;
	Input input;

	unsigned int w;
	unsigned int h;

	std::string ruta_destino;

	private:

	void obtener_resolucion();

	public:

	Kernel_app(DLibH::Controlador_argumentos& carg);
	~Kernel_app();

	virtual void inicializar();

	virtual void paso() {}
	virtual void preloop() {}
	virtual void postloop() {}
	virtual std::string acc_titulo_inicial() const {return std::string("Generador tabla sprites");}
	virtual bool acc_mostrar_cursor_inicial() const {return true;}
	virtual int acc_w_pantalla_inicial() const {return w;}
	virtual int acc_h_pantalla_inicial() const {return h;}
	virtual Input_base& acc_input() {return input;}
	virtual Configuracion_base& acc_configuracion() {return configuracion;}
	virtual Cargador_recursos_base& acc_cargador_recursos() {return cargador_recursos;}
	virtual unsigned int acc_recurso_fps() const {return Recursos_graficos::RS_FUENTE_BASE;}

	unsigned int acc_w() const {return w;}
	unsigned int acc_h() const {return h;}
	const std::string& acc_ruta_destino() const {return ruta_destino;}
};

#endif
