#include "cola_sonido.h"

using namespace DLibA;

/* TODO: 
Lo ideal es que esto no sea sólo una cola de sonidos sino una cola de bloques
de sonido que incluyan cosas como el canal, el volumen o lo que sea. De momento
lo podemos dejar así pero me gustaría estudiarlo en el futuro.
*/

Cola_sonido::Cola_sonido()
{
	inicializar();
}

Cola_sonido::~Cola_sonido()
{
	vaciar();
}

/*
OJO: El mero de hecho de llamar a este método (que se hace sólo nada más 
construir un objeto de esta clase) implica que se inicializará el controlador
de audio sdl.
*/

void Cola_sonido::inicializar()
{
	controlador=Controlador_audio_SDL::obtener();
}

void Cola_sonido::vaciar()
{
	cola.clear();
}

//Los sonidos se insertan, preparados o no.

void Cola_sonido::insertar(const Estructura_sonido& p_e)
{
	cola.push_back(p_e);
}
	
Estructura_sonido Cola_sonido::obtener_primero()
{
	return cola.front();
}

void Cola_sonido::eliminar_primero()
{
	cola.erase(cola.begin());
}

void Cola_sonido::procesar_cola(bool descartar)
{
	if(controlador)
	{
		//Los sonidos que NO se pueden reproducir se descartan o se
		//insertan en la parte trasera de la cola de nuevo. 
		//Simplemente hacemos un bucle por los sonidos que había al 
		//principio y lo iteramos una vez.

/*		Estructura_sonido actual=NULL;
		unsigned int t=this->cola.size(), i=0;		
		
		for(; i<t; i++)
		{
			actual=this->cola.front();
			this->cola.pop();
  
			if(this->controlador->reproducir_sonido(actual) == -1 && !descartar)
			{
				this->cola.push(actual);
			}

			actual=NULL;
		}
*/

		if(cola.size())
		{
			for(auto& c : cola)
			{
				if(controlador->reproducir_sonido(c) == -1 && !descartar)
				{
//			this->cola.push(actual);
				}
				
			}
			cola.clear();
		}

	}
}
