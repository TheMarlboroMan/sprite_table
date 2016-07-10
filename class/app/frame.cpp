#include "frame.h"
#include <templates/parches_compat.h>

using namespace std;
using namespace DLibV;
using namespace DLibH;
//using namespace Aplicacion;

void Frame::actualizar_representacion()
{
	rep_caja.reset(new Representacion_primitiva_caja(Representacion_primitiva_poligono::tipo::relleno, DLibV::Rect{(int)x, (int)y, w, h}, DLibV::rgba8(255, 64, 64,64)));
	rep_caja->establecer_modo_blend(Representacion::blends::alpha);

	rep_txt_id->ir_a(x+3, y+3);
	rep_txt_id->asignar(compat::to_string(id));
}

void Frame::generar_representacion()
{
	if(!rep_txt_id && !rep_caja)
	{
		rep_txt_id=std::unique_ptr<Representacion_TTF>(new Representacion_TTF(*fuente, DLibV::rgba8(255,255,255,255), compat::to_string(id) ));
		rep_txt_id->ir_a(x+3, y+3);

		rep_caja.reset(new Representacion_primitiva_caja(Representacion_primitiva_poligono::tipo::relleno, DLibV::Rect{(int)x, (int)y, w, h}, DLibV::rgba8(255,64, 64,64)));
		rep_caja->establecer_modo_blend(Representacion::blends::alpha);
	}
}

void Frame::color_caja(bool seleccionada)
{
	if(seleccionada) rep_caja->mut_rgba(DLibV::rgba8(0, 0, 255, 64));
	else rep_caja->mut_rgba(DLibV::rgba8(255, 0, 64, 64));
}

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
