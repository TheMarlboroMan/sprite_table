#include "kernel.h"

Kernel_app::Kernel_app(DLibH::Controlador_argumentos& carg)
	:Kernel_base(carg), w(0), h(0), ruta_destino("")
{
}

Kernel_app::~Kernel_app()
{

}

std::string Kernel_app::obtener_valor_argumento(const std::string& tipo, const std::vector<std::string>& args)
{
	std::string resultado;

	auto it=std::find_if(args.begin(), args.end(), [&tipo](const std::string& arg)
		{return arg.find(tipo)!=std::string::npos;});

	if(it==args.end())
	{
		throw Kernel_excepcion("ERROR: falta parámetro "+tipo+"."); 
	}
	else
	{
		auto ex=DLibH::Herramientas::explotar(*it, '=');
		if(ex.size()!=2)
		{
			throw Kernel_excepcion("ERROR: Parámetro "+tipo+" se usa como img=ruta");
		}
		else
		{
			resultado=ex[1];
		}
	}

	return resultado;
}

std::string Kernel_app::obtener_ruta_fichero_de_argumento(const std::string& tipo, const std::vector<std::string>& args)
{
	std::string ruta=obtener_valor_argumento(tipo, args);
	std::ifstream archivo(ruta.c_str());
	if(!archivo)
	{
		throw Kernel_excepcion("ERROR: "+ruta+" no es un fichero válido para el parámetro "+tipo+".");
	}

	return ruta;
}

void Kernel_app::obtener_resolucion(const std::vector<std::string>& args)
{
	auto val_res=obtener_valor_argumento("res=", args);
	auto ex=DLibH::Herramientas::explotar(val_res, 'x');

	if(ex.size()!=2)
	{
		throw Kernel_excepcion("ERROR: Resolución debe ser expresada como res=WxH");
	}
	else
	{
		w=std::atoi(ex[0].c_str());
		h=std::atoi(ex[1].c_str());
	}
}

void Kernel_app::inicializar()
{
	auto& CARG=acc_controlador_argumentos();

	if(CARG.obtener_cantidad()!=4)
	{
		throw Kernel_excepcion("ERROR: ejecutable img=ruta_imagen out=ruta_fichero res=800x600"
"\n"
"\n"
"ESC -> Salir\n"
"Cursores -> Mover\n"
"TAB -> Cambiar modo (cámara, mover, redimensionar)\n"
"RE.PAG -> Ir a frame anterior\n"
"AV.PAG -> Ir a frame siguiente\n"
"DEL -> Eliminar frame seleccionado\n"
"L SHIFT -> Con cursores, mover pixel a pixel\n"
"N -> Crear nuevo frame en posición de la cámara\n"
"S -> Guardar\n"
"L -> Cargar\n"
"D -> Duplicar\n"
"Z -> Zoom\n"
"P -> Modo presentación\n"
); 
	}
	else
	{
		auto& args=CARG.acc_argumentos();

		//Localizar ficheros y resolución...
		cargador_recursos.establecer_textura(obtener_ruta_fichero_de_argumento("img=", args));
		ruta_destino=obtener_valor_argumento("out=", args);
		obtener_resolucion(args);
	}

	Kernel_base::inicializar();
}
