#include "frame.h"

using namespace std;
using namespace DLibV;
using namespace DLibH;
//using namespace Aplicacion;

void Frame::actualizar_representacion()
{
	rep_caja->establecer_posicion(x, y, w, h);
	rep_txt_id->establecer_posicion(x+3, y+3);
	rep_txt_id->asignar(std::to_string(id));
}

void Frame::generar_representacion(const SDL_Renderer * renderer, const DLibV::Superficie * superf)
{
	if(!rep_txt_id && !rep_caja)
	{
		rep_txt_id=std::unique_ptr<Representacion_texto_auto_dinamica>(new Representacion_texto_auto_dinamica(renderer, superf, std::to_string(id) ));
		rep_txt_id->establecer_posicion(x+3, y+3);

		rep_caja=std::unique_ptr<Representacion_primitiva_caja_dinamica>(new Representacion_primitiva_caja_dinamica(DLibH::Herramientas_SDL::nuevo_sdl_rect(x, y, w, h), 255, 64, 64));
		rep_caja->establecer_alpha(128);
	}
}

void Frame::color_caja(bool seleccionada)
{
	if(seleccionada) rep_caja->mut_rgb(64, 64, 255);
	else rep_caja->mut_rgb(255, 64, 64);
}

//std::ostream& operator<<(std::ostream& stream, const Aplicacion::Frame& f)
std::ostream& operator<<(std::ostream& stream, const Frame& f)
{
	stream<<f.id<<"\t"<<f.x<<"\t"<<f.y<<"\t"<<f.w<<"\t"<<f.h<<"\t"<<f.desp_x<<"\t"<<f.desp_y<<std::endl;
	return stream;
}

std::istream& operator>>(std::istream& stream, Frame& f)
{
	stream>>f.id>>f.x>>f.y>>f.w>>f.h>>f.desp_x>>f.desp_y;
	return stream;
}
