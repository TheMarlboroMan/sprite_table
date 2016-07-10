#ifndef PROYECTO_SDL2_DERIVADA_CARGADOR_RECURSOS_H
#define PROYECTO_SDL2_DERIVADA_CARGADOR_RECURSOS_H

#include "../base/cargador_recursos_base.h"

class Cargador_recursos:public Cargador_recursos_base
{
	private:

	std::string textura;

	protected: 

	std::vector<std::string> obtener_entradas_audio() const 
	{
		std::vector<std::string> resultado;
		return resultado;
	}

	std::vector<std::string> obtener_entradas_musica() const
	{
		std::vector<std::string> resultado;
		return resultado;
	}

	std::vector<std::string> obtener_entradas_texturas() const
	{
		std::string linea1 {"1	data/graficos/iconos.png"};
		std::string linea2 {"10	"+textura};
		std::vector<std::string> resultado {linea1, linea2};
		return resultado;
	}

	std::vector<std::string> obtener_entradas_superficies() const {return obtener_entradas_desde_ruta(std::string("data/recursos/superficies.txt"));}

	public:

	Cargador_recursos():Cargador_recursos_base() {}

	void establecer_textura(const std::string& v) {textura=v;}
};

#endif
