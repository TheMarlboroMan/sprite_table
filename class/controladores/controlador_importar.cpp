#include "controlador_importar.h"
#include <fstream>

using namespace DLibH;

extern Log_base LOG;

Controlador_importar::Controlador_importar(Director_estados &DI, Cola_mensajes& CM, Frames& f, const std::string& ruta)
	:Controlador_base(DI, CM),
		frames(f), ruta(ruta)
{

}

Controlador_importar::~Controlador_importar()
{

}

void Controlador_importar::loop(Input_base& input, float delta)
{
	importar();
	solicitar_cambio_estado(Director_estados::t_estados::TABLA);
}

void Controlador_importar::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(128, 128, 128, 255);
}

void Controlador_importar::importar()
{
	std::ifstream fichero(ruta.c_str());
	if(!fichero)
	{
		insertar_mensaje("ERROR: Imposible localizar el fichero "+ruta+" para importar información");
		LOG<<"ERROR: Imposible localizar el fichero "<<ruta<<" para importar información"<<std::endl;
	}
	else
	{
		frames.clear();

		while(true)
		{
			Frame f;
			fichero>>f;
			if(fichero.eof()) break;
			frames.push_back(std::move(f));
		}

		insertar_mensaje("Cargados "+std::to_string(frames.size())+" frames del fichero");
		LOG<<"Cargados "<<frames.size()<<" frames del fichero"<<std::endl;

		fichero.close();
	}
}
