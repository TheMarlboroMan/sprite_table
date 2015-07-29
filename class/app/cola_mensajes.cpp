#include "cola_mensajes.h"

std::string Cola_mensajes::recuperar_cola()
{
	std::string resultado;
	for(auto& a : cola) resultado+=a+"\n";
	cola.clear();
	return resultado;
}
