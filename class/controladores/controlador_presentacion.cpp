#include "controlador_presentacion.h"

using namespace DLibV;
using namespace DLibH;

Controlador_presentacion::Controlador_presentacion(Director_estados &d, Cola_mensajes& cm, Frames& f, unsigned int pw, unsigned int ph)
	:Controlador_base(d, cm), frames(f), 
	camara({0, 0, pw, ph}, {0,0})
{

}

void Controlador_presentacion::preparar_presentacion()
{
	const int ini_x=0, ini_y=0, margen_y_id=16, margen_x=4, max_x=camara.acc_caja_foco().w;
	int x=ini_x, y=ini_y, w=0, h=0;
	int max_h=0;

	for(auto& f : frames.frames)
	{
		//Medir el frame. Buscar la altura mayor para poder hacer el salto Y cuando sea preciso.
		/*x=f.acc_x(), y=f.acc_y(), */w=f.acc_w(), h=f.acc_h();
		if(h > max_h) max_h=h;

		//Con respecto al ancho, si no cabe en la línea, saltar a la siguiente línea de y
		//excepto que sea lo único que hay en la línea, en ese caso lo ponemos tal cual.

		if(x+w > max_x)	
		{
			if(x!=ini_x)
			{
				x=ini_x;
				y+=max_h+margen_y_id;
				max_h=0;
			}
		}

		//Colocar id del en el márgen inferior.
		f.acc_txt().ir_a(x, y+h+2);

		//Rellenar info presentación con el puntero al frame.
		info_presentacion.push_back(Info_presentacion(x, y, w, h, f));

		x+=w+margen_x;
	}

	camara.establecer_limites(0, 0, camara.acc_caja_foco().w, y); 
}

void Controlador_presentacion::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE) || input.es_input_down(Input::I_PRESENTACION))
	{
		solicitar_cambio_estado(Director_estados::t_estados::TABLA);
	}
	else
	{
		int y=0;
		if(input.es_input_pulsado(Input::I_ARRIBA)) y=-1;
		else if(input.es_input_pulsado(Input::I_ABAJO)) y=1;

		if(y) camara.movimiento_relativo(0, y);
	}
}	

void Controlador_presentacion::dibujar(DLibV::Pantalla& pantalla)
{
	pantalla.limpiar(DLibV::rgba8(0, 0, 0, 255));

	//Recorrer la representación interna de los frames. El texto con el 
	//id ya lo hemos preparado antes.

	Representacion_bitmap rep_bmp(Gestor_texturas::obtener(Recursos_graficos::RT_IMAGEN));

	for(auto& i : info_presentacion)
	{
		Representacion_primitiva_caja rep_caja(Representacion_primitiva_poligono::tipo::relleno, {i.x-1, i.y-1, (unsigned int)i.w+2, (unsigned int)i.h+2}, rgba8(255, 255, 255, 255));
		rep_caja.volcar(pantalla, camara);

		rep_bmp.establecer_posicion(i.x, i.y, i.w, i.h);
		rep_bmp.establecer_recorte(i.frame.acc_x(), i.frame.acc_y(), i.frame.acc_w(), i.frame.acc_h());
		rep_bmp.volcar(pantalla, camara);
		
		i.frame.acc_txt().volcar(pantalla, camara);
	}
}
