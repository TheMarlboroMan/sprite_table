#include "controlador_tabla.h"
#include "../recursos.h"
#include <sstream>
#include <algorithm>

using namespace DLibV;
using namespace DLibH;

Controlador_tabla::Controlador_tabla(Director_estados &DI, Cola_mensajes& CM,
	Pantalla& p, 
	Frames& f,
	const DLibV::Fuente_TTF& fuente,
	unsigned int cw, unsigned int ch):
	Controlador_base(DI, CM),
		fuente(fuente),
		actual(nullptr),
		estado(t_estados::CAMARA),
		frames(f),
		camara({0, 0, cw, ch}, {0, 0}),
		rep_imagen(Gestor_texturas::obtener(Recursos_graficos::RT_IMAGEN)),
		rep_icono(Gestor_texturas::obtener(Recursos_graficos::RT_ICONOS), {0,0,32,32}),
		rep_centrar(Gestor_texturas::obtener(Recursos_graficos::RT_ICONOS), {0,0,32,32}),
		rep_status_actual(fuente, DLibV::rgba8(255,255,255,255), ""),
		rep_mensaje_actual(fuente, DLibV::rgba8(255,255,255,255), ""),
		caja_fondo(Representacion_primitiva_poligono::tipo::relleno, {0, (int)p.acc_h()-64, p.acc_w(), 64}, DLibV::rgba8(0,0,0,255)),
//		linea_absurda(0, 0, 0, 0, 255, 0, 0),
		info_zoom(1, cw, ch),
		mostrar_numeros(true)
{
	int wa=rep_imagen.acc_posicion().w, wb=p.acc_w();
	int ha=rep_imagen.acc_posicion().h, hb=p.acc_h();

	camara.establecer_limites(0, 0, std::max(wa, wb), std::max(ha, hb)); 
	rep_icono.ir_a(cw-64, 32);
	rep_icono.establecer_recorte({0, 0, 32, 32});
	rep_icono.establecer_modo_blend(DLibV::Representacion::blends::alpha);

	rep_centrar.ir_a(0, 0);
	rep_centrar.establecer_recorte({128, 0, 32, 32});
	rep_centrar.establecer_modo_blend(DLibV::Representacion::blends::alpha);

	rep_status_actual.ir_a(32, ch-48);
	rep_mensaje_actual.ir_a(32, ch-16);

	caja_fondo.establecer_alpha(128);

	reset_actual();
}

Controlador_tabla::~Controlador_tabla()
{

}

void Controlador_tabla::reset_actual()
{
	if(frames.size()) actual=&frames.frames[0];
}

void Controlador_tabla::loop(Input_base& input, float delta)
{
	if(input.es_senal_salida() || input.es_input_down(Input::I_ESCAPE))
	{
		abandonar_aplicacion();
		romper_loop();
	}
	else
	{
		int mx=0, my=0;

		if(input.es_input_down(Input::I_GRABAR))
		{
			solicitar_cambio_estado(Director_estados::t_estados::EXPORTAR);
			return;
		}
		else if(input.es_input_down(Input::I_CARGAR))
		{
			solicitar_cambio_estado(Director_estados::t_estados::IMPORTAR);
			return;
		}
		else if(input.es_input_down(Input::I_PRESENTACION))
		{
			solicitar_cambio_estado(Director_estados::t_estados::PRESENTACION);
			return;
		}

		if(input.es_input_down(Input::I_TAB)) ciclo_estado();
		if(input.es_input_down(Input::I_ESPACIO)) mostrar_numeros=!mostrar_numeros;

		if(input.es_input_pulsado(Input::I_PASO))
		{
			if(input.es_input_down(Input::I_IZQUIERDA)) mx=-1;
			else if(input.es_input_down(Input::I_DERECHA)) mx=1;

			if(input.es_input_down(Input::I_ARRIBA)) my=-1;
			else if(input.es_input_down(Input::I_ABAJO)) my=1;
		}
		else
		{
			if(input.es_input_pulsado(Input::I_IZQUIERDA)) mx=-1;
			else if(input.es_input_pulsado(Input::I_DERECHA)) mx=1;

			if(input.es_input_pulsado(Input::I_ARRIBA)) my=-1;
			else if(input.es_input_pulsado(Input::I_ABAJO)) my=1;
		}

		procesar_input(mx, my);

		if(input.es_input_down(Input::I_CLICK_I))
		{
			auto pos=input.acc_posicion_raton();
			camara.transformar_posicion_raton({pos.x, pos.y});

//			linea_absurda.establecer_puntos(0, 0, pos.x, pos.y);

			seleccionar_por_posicion(pos.x, pos.y);
		}

		if(input.es_input_down(Input::I_ZOOM)) ciclo_zoom();
		if(input.es_input_down(Input::I_DUPLICAR)) duplicar();

		if(input.es_input_down(Input::I_NUEVO)) insertar_nuevo();
		else if(input.es_input_down(Input::I_AV_PAG)) ciclo_frames(1);
		else if(input.es_input_down(Input::I_RE_PAG)) ciclo_frames(-1);
		else if(input.es_input_down(Input::I_DEL)) eliminar();
	}
}

void Controlador_tabla::ciclo_zoom()
{
	++info_zoom.zoom;
	if(info_zoom.zoom==6) info_zoom.zoom=1;
	camara.mut_zoom(info_zoom.zoom);
}

void Controlador_tabla::dibujar(Pantalla& pantalla)
{
	//La alternativa a usar la cámara para el zoom, el problema es que las medidas de la cámara
	//ya no son útiles para determinar lo que cae dentro y lo que no. 
	//SDL_RenderSetScale(pantalla.acc_renderer(), info_zoom.zoom, info_zoom.zoom);

	pantalla.limpiar(DLibV::rgba8(0, 0, 0, 255));

	rep_imagen.volcar(pantalla, camara);
	for(auto& f : frames.frames)
		dibujar_frame(pantalla, f, actual && actual->acc_id()==f.acc_id());

	caja_fondo.volcar(pantalla);

	std::stringstream ss;
	ss<<"["<<camara.acc_x()<<","<<camara.acc_y()<<"] ";
	if(actual) ss<<actual->acc_id()<<" : "<<actual->acc_x()<<","<<actual->acc_y()<<" ["<<actual->acc_w()<<","<<actual->acc_h()<<"] ["<<actual->acc_desp_x()<<","<<actual->acc_desp_y()<<"]";
	rep_status_actual.asignar(ss.str());
	rep_status_actual.volcar(pantalla);
	rep_mensaje_actual.volcar(pantalla);

	rep_icono.volcar(pantalla);

	if(estado==t_estados::CENTRAR && actual)
	{
		rep_centrar.establecer_posicion(actual->acc_x()+actual->acc_desp_x(), actual->acc_y()+actual->acc_desp_y(), 32, 32);
		rep_centrar.volcar(pantalla, camara);
	}
}

void Controlador_tabla::dibujar_frame(Pantalla& pantalla, Frame& f, bool actual)
{
	f.generar_representacion(); //Sólo lo hará si es necesario.
	f.color_caja(actual);
	f.acc_caja().volcar(pantalla, camara);
	if(mostrar_numeros) f.acc_txt().volcar(pantalla, camara);
}

/*
void Controlador_tabla::dibujar_caja(Pantalla& pantalla, SDL_Rect c, unsigned int r, unsigned int g, unsigned int b)
{
	Representacion_primitiva_caja_dinamica caja(c, r, g, b);
	caja.establecer_alpha(128);
	caja.volcar(pantalla, camara);
}
*/
void Controlador_tabla::ciclo_estado()
{
	switch(estado)
	{
		case t_estados::CAMARA: estado=t_estados::MOVER; break;
		case t_estados::MOVER: estado=t_estados::REDIMENSIONAR; break;
		case t_estados::REDIMENSIONAR: estado=t_estados::CENTRAR; break;
		case t_estados::CENTRAR: estado=t_estados::CAMARA; break;
	}

	int x=0;

	switch(estado)
	{
		case t_estados::CAMARA: x=0; break;
		case t_estados::MOVER: x=32; break;
		case t_estados::REDIMENSIONAR: x=64; break;
		case t_estados::CENTRAR: x=96; break;
	}	

	rep_icono.establecer_recorte({x, 0, 32, 32});
}

void Controlador_tabla::procesar_input(int x, int y)
{
	switch(estado)
	{
		case t_estados::CAMARA: camara.movimiento_relativo(x, y); break;
		case t_estados::MOVER: mover_actual(x, y); break;
		case t_estados::REDIMENSIONAR: redimensionar_actual(x, y); break;
		case t_estados::CENTRAR: centrar_actual(x, y); break;
	}
}

void Controlador_tabla::insertar_nuevo()
{
	unsigned int id=frames.nuevo_id();
	unsigned int x=static_cast<unsigned int>(camara.acc_x());
	unsigned int y=static_cast<unsigned int>(camara.acc_y());

	frames.push_back(Frame(fuente, id, x, y ,32 ,32 , 0, 0));

	auto it=std::find_if(std::begin(frames.frames), std::end(frames.frames), [id](const Frame& f) {return f.acc_id()==id;});

	if(it != frames.frames.end())
	{
		actual=&*it;
	}

	insertar_mensaje("Se ha creado el frame "+std::to_string(actual->acc_id()));
}

void Controlador_tabla::duplicar()
{
	if(actual)
	{
		unsigned int id=frames.nuevo_id();
		frames.push_back(Frame{fuente, id, actual->acc_x(), actual->acc_y(), actual->acc_w(), actual->acc_h(), 0, 0});
		actual=&*(frames.frames.rbegin());
		insertar_mensaje("Se ha creado el frame "+std::to_string(actual->acc_id()));
	}
}

void Controlador_tabla::ciclo_frames(int dir)
{
	if(actual)
	{
		unsigned int id=actual->acc_id();
		auto it=std::find_if(std::begin(frames.frames), std::end(frames.frames), [id](const Frame& f) {return f.acc_id()==id;});	

		if(dir > 0)
		{
			++it;
			if(it==frames.frames.end())
			{
				it=frames.frames.begin();
			}

			actual=&*it;
		}
		else
		{
			if(it==frames.frames.begin())
			{
				it=frames.frames.end();
			}

			--it;
			actual=&*it;
		}

	}
}

void Controlador_tabla::eliminar()
{
	if(!actual) return;

	unsigned int id=actual->acc_id();
	auto it=std::find_if(std::begin(frames.frames), std::end(frames.frames), [id](const Frame& f) {return f.acc_id()==id;});
	it=frames.erase(it);		

	if(!frames.size())
	{
		actual=nullptr;
	}
	else 
	{
		if(it==frames.frames.end()) actual=nullptr;
		else actual=&*it;
	}
}

void Controlador_tabla::seleccionar_por_posicion(int x, int y)
{
	for(auto& f : frames.frames)
	{
		if(f.es_en_posicion(x, y))
		{
			actual=&f;
			break;
		}
	}
}

void Controlador_tabla::mover_actual(int x, int y)
{
	if(!actual) return;
	actual->mover(x, y);
}

void Controlador_tabla::centrar_actual(int x, int y)
{
	if(!actual) return;
	actual->centrar(x, y);
}

void Controlador_tabla::redimensionar_actual(int x, int y)
{
	if(!actual) return;
	actual->redimensionar(x, y);
}

void Controlador_tabla::asignar_mensaje(const std::string& m)
{
	rep_mensaje_actual.asignar(m);
}

void Controlador_tabla::restaurar_posicion_ids()
{
	for(auto& f : frames.frames)
	{
		f.actualizar_representacion();
	}
}
