#include "controlador_exportar.h"
#include <fstream>

using namespace DLibH;

extern Log_base LOG;

Controlador_exportar::Controlador_exportar(Director_estados &DI, Cola_mensajes& CM, Frames& f, const std::string& ruta)
	:Controlador_base(DI, CM), frames(f), ruta(ruta)
{

}

Controlador_exportar::~Controlador_exportar()
{

}

void Controlador_exportar::loop(Input_base& input, float delta)
{
	//Técnicamente no necesitamos un controlador para hacer esto, pero es divertido probar.
	std::ofstream salida(ruta.c_str());

	if(!salida)
	{
		LOG<<"ERROR: Imposible abrir el fichero en "<<ruta<<" para guardar datos"<<std::endl;
		insertar_mensaje("ERROR: Imposible abrir el fichero en "+ruta+" para guardar datos");
	}
	else
	{
		for(auto& f : frames.frames) salida<<f;
		insertar_mensaje("Guardados "+std::to_string(frames.size())+" frames en el fichero");
		LOG<<"Guardados "<<std::to_string(frames.size())<<" frames en el fichero"<<std::endl;
	}

	solicitar_cambio_estado(Director_estados::t_estados::TABLA);
}

void Controlador_exportar::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(DLibV::rgba8(128, 128, 128, 255));
}
