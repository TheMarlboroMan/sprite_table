#include "representacion_grafica.h"
#include <algorithm>
#include "../../../herramientas/poligono_2d/poligono_2d.h"

using namespace DLibV;

Representacion_grafica::Representacion_grafica(Rect pos, Rect rec, sampling ts)
	:Representacion(), textura(nullptr), 
	pincel{0,0}, tipo_sampling(ts), posicion(pos), recorte(rec)
{

}

Representacion_grafica::Representacion_grafica(ColorRGBA color, Rect pos, Rect rec, sampling ts)
	:Representacion(color), textura(nullptr),
	pincel{0,0}, tipo_sampling(ts), posicion(pos), recorte(rec)
{

}

Representacion_grafica::Representacion_grafica(const Representacion_grafica& o)
	:Representacion(o) ,textura(o.textura),
	pincel(o.pincel), puntos(o.puntos), 
	final_ptex(o.final_ptex),
	tipo_sampling(o.tipo_sampling),
	posicion(o.posicion), 
	recorte(o.recorte)
{

}

Representacion_grafica& Representacion_grafica::operator=(const Representacion_grafica& o)
{
	Representacion::operator=(o);
	textura=o.textura;
	posicion=o.posicion;
	recorte=o.recorte;
	pincel=o.pincel;
	puntos=o.puntos;
	final_ptex=o.final_ptex;
	tipo_sampling=o.tipo_sampling;

	return *this;
}

void Representacion_grafica::recorte_a_medidas_textura()
{
	establecer_recorte(0,0, textura->acc_w(), textura->acc_h());
	posicion.w=recorte.w;
	posicion.h=recorte.h;
}

void Representacion_grafica::volcado()
{
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glBindTexture(GL_TEXTURE_2D, textura->acc_indice());
	glEnable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Alpha...
	const auto c=acc_rgba();
	switch(acc_modo_blend())
	{
		case Representacion::blends::nada:
			glDisable(GL_BLEND);
			glColor3f(c.r, c.g, c.b);
		break;
		case Representacion::blends::alpha:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(c.r, c.g, c.b, c.a);
		break;
	}
		
	if(!puntos.size() || final_ptex.size())
	{
		calcular_puntos();
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);

	glVertexPointer(2, GL_INT, 0, puntos.data());
	glTexCoordPointer(2, GL_FLOAT, 0, final_ptex.data());
	glDrawArrays(GL_QUADS, 0, puntos.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glDisable(GL_TEXTURE_2D);
}

//Calcula las posiciones de los vértices y los puntos de las texturas.
//Dado que las posiciones se calculan a partir de 0.0 no debería haber problemas
//al cambiar la posición.
//Los valores calculados se almacenan.

void Representacion_grafica::calcular_puntos()
{
	const Rect& pos=acc_posicion();
	const Rect& recor=acc_recorte();
	
	if(!pincel.w) pincel.w=pos.w;
	if(!pincel.h) pincel.h=pos.h;

	float w_tex=textura->acc_w(), h_tex=textura->acc_h();

	puntos.clear();
	final_ptex.clear();

	int itx=0;

	for(int x=0; x < (int)pos.w; x+=pincel.w)
	{
		int ity=0;
		const int dif_x=x+pincel.w > (int)pos.w ? pos.w - (itx * pincel.w)  : pincel.w;

		for(int y=0; y < (int)pos.h; y+=pincel.h)
		{
			const int dif_y=y+pincel.h > (int)pos.h ? pos.w - (ity * pincel.h) : pincel.h;

			Punto pts[]={{x, y}, {x+dif_x, y}, {x+dif_x, y+dif_y}, {x, y+dif_y}};

			//Samplear los puntos centrales de las cuatro esquinas. De momento no buscamos el centro del texel. 
			//El cálculo de ptex_fx y ptex_fy está sacando la proporción entre el pincel y la cantidad de espacio
			//que queda por dibujar (una simple regla de tres). La finalidad es mapear sólo el trozo de textura
			//necesario.
 
			GLfloat ptex_x=(GLfloat)recor.x,
				ptex_y=(GLfloat)recor.y, 
				ptex_fx=ptex_x+( ( (GLfloat)dif_x * (GLfloat)recor.w) / (GLfloat)pincel.w), 
				ptex_fy=ptex_y+( ( (GLfloat)dif_y * (GLfloat)recor.h) / (GLfloat)pincel.h);

			puntotex ptex[]={
				{ptex_x,	ptex_y},
				{ptex_fx,	ptex_y},
				{ptex_fx,	ptex_fy},
				{ptex_x,	ptex_fy}};

			//Invertir y resamplear puntos...
			if(transformacion.invertir_horizontal)
			{
				std::swap(ptex[0].x, ptex[1].x);
				std::swap(ptex[2].x, ptex[3].x);

				if(tipo_sampling==sampling::atlas) for(auto &p: ptex) p.x-=0.5f;
			}
			else 
			{
				if(tipo_sampling==sampling::atlas)
				{
					for(auto &p: ptex) p.x+=0.5f;
				}
			}

			if(transformacion.invertir_vertical)
			{
				std::swap(ptex[0].y, ptex[2].y);
				std::swap(ptex[1].y, ptex[3].y);

				if(tipo_sampling==sampling::atlas)
				{ 
					for(auto &p: ptex) p.y-=0.5f;
				}
			}
			else
			{
				if(tipo_sampling==sampling::atlas) 
				{
					for(auto &p: ptex) p.y+=0.5f;
				}
			}

			for(auto &p : ptex)
			{
				p.x/=w_tex; 
				p.y/=h_tex;
			}

			final_ptex.insert(std::end(final_ptex), ptex, ptex+4);
			puntos.insert(std::end(puntos), pts, pts+4);
			++ity;
		}
		++itx;
	}
}

//Eso sólo deberíamos llamarlo en aquellas para las cuales hemos creado una
//textura que poseen. Si es parte de un recurso que no es de su propiedad
//te vas a encontrar con un problema.

void Representacion_grafica::liberar_textura()
{
	if(this->textura)
	{
		delete this->textura;
		this->textura=nullptr;
	}
}

void Representacion_grafica::establecer_posicion(int p_x, int p_y, int p_w, int p_h, int p_flags)
{
	if(p_flags & FRECT_X) posicion.x=p_x;
	if(p_flags & FRECT_Y) posicion.y=p_y;
	if(p_flags & FRECT_W && p_w != -1) posicion.w=p_w;
	if(p_flags & FRECT_H && p_h != -1) posicion.h=p_h;
	actualizar_posicion_vista_rotacion();
}

void Representacion_grafica::establecer_posicion(Rect c)
{
	posicion=c;
	actualizar_posicion_vista_rotacion();
}

void Representacion_grafica::establecer_recorte(Rect p_caja)
{
	recorte=p_caja;
	liberar_calculos();
}

void Representacion_grafica::establecer_recorte(Sint16 p_x, Sint16 p_y, Uint16 p_w, Uint16 p_h, int p_flags)
{
	if(p_flags & FRECT_X) recorte.x=p_x;
	if(p_flags & FRECT_Y) recorte.y=p_y;
	if(p_flags & FRECT_W) recorte.w=p_w;
	if(p_flags & FRECT_H) recorte.h=p_h;
	liberar_calculos();
}

void Representacion_grafica::ir_a(int x, int y)
{
	posicion.x=x; 
	posicion.y=y;
	actualizar_posicion_vista_rotacion();
}

Punto Representacion_grafica::obtener_posicion() const
{
	return Punto{posicion.x, posicion.y};
}

Rect Representacion_grafica::obtener_base_posicion_vista() const
{
	return posicion;
}

void Representacion_grafica::liberar_calculos()
{
	pincel={0,0};
	puntos.clear();
	final_ptex.clear();
}
