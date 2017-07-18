#ifndef HERRAMIENTAS_DLIBDAN_H
#define HERRAMIENTAS_DLIBDAN_H

#include <cstring> //Para memset
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <cmath>
#include <cctype>
#include <vector>
#include <cstdio>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159
#endif

namespace DLibH
{

class Herramientas
{
	private:

	Herramientas() 
	{
		//MUhaahahaaaaaahaahaa!
	}

	public:

	template<typename T>
	static float grados_a_radianes(T val)
	{
	    return (val * M_PI) / 180.0;
	}

	template<typename T>
	static float radianes_a_grados(T val)
	{
	    return ( val * 180.0) / M_PI;
	}

	//Sólo válido para líneas rectas, of course.
	template<typename T>
	static bool segmentos_superpuestos(T aini, T afin, T bini, T bfin, bool roce_es_colision=false)
	{
	 	//Comparamos si está fuera y lo negamos...
	        if(roce_es_colision)
	        {
	                return !( (bfin < aini) || (bini > afin) );
	        }
	        else
	        {
	                return !( (bfin <= aini) || (bini >= afin) );
	        }
	}

	static float angulo_360(float f);
	static float angulo_entre_puntos(float, float, float, float);
	static float porcentaje(float, float);

	template <typename T, typename U> static bool rectangulos_superpuestos(T, T, U, U, T, T, U, U, bool=false);
	template <typename T, typename U> static bool rectangulos_superpuestos(T, T, U, U, T, T, U, U, T&, T&, U&, U&, bool=false);
	template <typename T, typename U> static void posicion_dimension_segmentos(T, T, T, T, T&, U&);
	template <typename T, typename U> static void posicion_dimension_segmentos_solucion_parcial(T, T, T, T, T&, U&);
	template <typename T, typename U> static void posicion_dimension_segmentos_solucion_completa(T, T, T, T, T&, U&);

	//TODO: Hacer esto con templates...
	static bool caja_en_caja(int, int, unsigned int, unsigned int, int, int, unsigned int, unsigned int);
	static bool punto_en_caja(int, int, unsigned int, unsigned int, int, int);

};

}

#include "herramientas.templates.cpp"

#endif
