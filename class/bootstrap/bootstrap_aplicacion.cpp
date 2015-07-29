#include "bootstrap_aplicacion.h"

#include "../controladores/controlador_tabla.h"
#include "../controladores/controlador_exportar.h"
#include "../controladores/controlador_importar.h"
#include "../controladores/controlador_presentacion.h"
#include "../app/frame.h"
#include "../app/cola_mensajes.h"

using namespace App;

void App::loop_aplicacion(Kernel_app& kernel)
{
	kernel.mut_mostrar_fps(false);

	Frames frames;
	Director_estados DI;
	Cola_mensajes COLA_M;

	Controlador_tabla C_TABLA(DI, COLA_M, kernel.acc_pantalla(), frames, kernel.acc_w(), kernel.acc_h());
	Controlador_exportar C_EXPORTAR(DI, COLA_M, frames, kernel.acc_ruta_destino());
	Controlador_importar C_IMPORTAR(DI, COLA_M, frames, kernel.acc_ruta_destino());
	Controlador_presentacion C_PRESENTACION(DI, COLA_M, frames, kernel.acc_w(), kernel.acc_h());

	Interface_controlador * IC=&C_TABLA;

	//Comprobamos que el fichero existe e intentamos cargar los datos.
	std::ifstream fichero(kernel.acc_ruta_destino().c_str());
	if(fichero)
	{
		fichero.close();
		C_IMPORTAR.importar();
		C_TABLA.reset_actual();
	}
	else
	{
		COLA_M.insertar_mensaje("Creando nuevo fichero "+kernel.acc_ruta_destino());
	}

	while(kernel.loop(*IC))
	{
		if(COLA_M.hay_cola())
		{
			C_TABLA.asignar_mensaje(COLA_M.recuperar_cola());
		}

		if(DI.es_cambio_estado())
		{
			switch(DI.acc_estado_actual())
			{
				case Director_estados::t_estados::TABLA: break;
				case Director_estados::t_estados::EXPORTAR: break;
				case Director_estados::t_estados::IMPORTAR: C_TABLA.reset_actual(); break;
				case Director_estados::t_estados::PRESENTACION: C_TABLA.restaurar_posicion_ids(); break;
			}

			switch(DI.acc_estado_deseado())
			{
				case Director_estados::t_estados::TABLA: IC=&C_TABLA; break;
				case Director_estados::t_estados::EXPORTAR: IC=&C_EXPORTAR; break;
				case Director_estados::t_estados::IMPORTAR: IC=&C_IMPORTAR; break;
				case Director_estados::t_estados::PRESENTACION: 
					IC=&C_PRESENTACION; 
					C_PRESENTACION.preparar_presentacion();
				break;
			}

			DI.confirmar_cambio_estado();
			C_TABLA.restaurar_loop();	//Ok, no es lo más ortodoxo pero nos vale.
		}
	};
}
