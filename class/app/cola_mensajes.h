#ifndef COLA_MENSAJES_H
#define COLA_MENSAJES_H

#include <vector>
#include <string>

class Cola_mensajes
{
	private:

	std::vector<std::string> cola;

	public:

	std::string recuperar_cola();
	bool hay_cola() const {return cola.size();}
	void insertar_mensaje(const std::string& m) {cola.push_back(m);}

	Cola_mensajes()
	{}

};

#endif
