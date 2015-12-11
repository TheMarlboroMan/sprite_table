#include "kernel.h"

Kernel_app::Kernel_app(Herramientas_proyecto::Controlador_argumentos& carg)
	:Kernel_base(carg), w(0), h(0), ruta_destino("")
{
}

Kernel_app::~Kernel_app()
{

}

void Kernel_app::obtener_resolucion()
{
	using namespace Herramientas_proyecto;

	try
	{
		auto val_res=acc_controlador_argumentos().valor_argumento("res");
		auto ex=explotar(val_res, 'x');

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
	catch(Controlador_argumentos_exception& e)
	{
		w=800;
		h=600;
	}
}

void Kernel_app::inicializar()
{
	auto& CARG=acc_controlador_argumentos();
	size_t cantidad=CARG.obtener_cantidad();
	
	if(cantidad < 3 || cantidad > 4)
	{
		throw Kernel_excepcion("ERROR: ejecutable img=ruta_imagen out=ruta_fichero [res=800x600]"
"\n"
"\n"
"ESC -> Salir\n"
"Cursores -> Mover\n"
"TAB -> Cambiar modo (cámara, mover, redimensionar)\n"
"ESPACIO -> Ocultar / mostrar índices\n"
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
		cargador_recursos.establecer_textura(CARG.valor_argumento("img"));
		ruta_destino=CARG.valor_argumento("out");
		obtener_resolucion();
	}

	Kernel_base::inicializar();
}
