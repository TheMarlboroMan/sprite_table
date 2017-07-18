#include "representacion.h"

#include <iostream>
#include <algorithm>

using namespace DLibV;

Representacion::Representacion():
	visible(true),
	modo_blend(blends::nada), 
	rgba{1.f, 1.f, 1.f, 1.f},
	posicion_vista{0,0,0,0}
{

}

Representacion::Representacion(ColorRGBA c):
	visible(true),
	modo_blend(blends::nada), 
	rgba(c),
	posicion_vista{0,0,0,0}
{

}

Representacion::Representacion(const Representacion& p_otra):
	visible(p_otra.visible), 
	modo_blend(blends::nada),
	rgba(p_otra.rgba),
	posicion_vista(p_otra.posicion_vista)
{

}

Representacion& Representacion::operator=(const Representacion& p_otra)
{
	visible=p_otra.visible;
	rgba=p_otra.rgba;
	posicion_vista=p_otra.posicion_vista;
	modo_blend=p_otra.modo_blend;
	return *this;
}

//El parámetro saltar_toma indica que es posible no comprobar si la 
//representación está en la toma. Tiene su sentido en las representaciones 
//agrupadas, que son problemáticas de comprobar cuando al propio grupo se le
//aplica una rotación (no se tiene un cuenta la misma a la hora de comprobar
//si está en toma). Para solucionarlo se pueden añadir varios parámetros a
//estas funciones y las dependientes (posición del grupo y rotación) para 
//calcular pero el código es menos limpio.

void Representacion::volcar(Pantalla& p_pantalla, const Camara& p_camara, bool saltar_toma)
{
	if(visible && (saltar_toma || es_en_toma(p_camara.acc_caja_foco()))) 
	{
		p_pantalla.asignar_camara(p_camara);
		transformacion_pre_render(p_camara.acc_info_volcado());
		volcado();
	}

	//Es importante que esto siempre esté presente...
	glLoadIdentity();
}

void Representacion::volcar(Pantalla& p_pantalla, bool saltar_toma)
{
	if(visible && (saltar_toma || es_en_toma(p_pantalla.acc_simulacro_caja())))
	{
		if(p_pantalla.es_con_camara())
		{
			p_pantalla.reiniciar_clip();
		}

		transformacion_pre_render(p_pantalla.acc_info_volcado());
		volcado();
	}

	//Es importante que esto siempre esté presente...
	glLoadIdentity();
}

void Representacion::debug_trazar_caja() const
{
	auto rect=calcular_posicion_vista_rotacion();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.f, 0.f, 0.f, 0.25f);
	glEnableClientState(GL_VERTEX_ARRAY);

	int 	fx=rect.x+rect.w,
		fy=rect.y+rect.h;

	std::vector<Punto> puntos{ {rect.x, rect.y},
			{fx, rect.y},
			{fx, fy},
			{rect.x, fy}};

	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_POLYGON, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY);
}

void Representacion::transformacion_pre_render(const Info_volcado iv)
{
	glMatrixMode(GL_MODELVIEW);
	
	auto pos=obtener_posicion();
	int 	x=iv.pos_x+pos.x-iv.rel_x, 
		y=iv.pos_y+pos.y-iv.rel_y;

	//En caso de que haya zoom compensamos el movimiento que causa la escala.
	//Básicamente estamos desplazando la mitad, dos tercios, tres cuartos...
	if(iv.zoom!=1.0)
	{
		glScaled(iv.zoom, iv.zoom, 1.0);
		//Puro empirismo.
		glTranslatef((iv.pos_x / iv.zoom) - iv.pos_x, (iv.pos_y / iv.zoom) - iv.pos_y, 0.f);
	}

	glTranslatef(x, y, 0.f);

	if(transformacion.angulo_rotacion != 0.f)
	{
		glTranslatef(transformacion.centro_rotacion.x, transformacion.centro_rotacion.y, 0.f);
		glRotatef(transformacion.angulo_rotacion, 0.f, 0.f, 1.f);
		glTranslatef(-transformacion.centro_rotacion.x, -transformacion.centro_rotacion.y, 0.f);
	}
}

//Se usa para darle un volumen a la posición, que de por si no tiene.
//La posición no tendría interés ninguno de no ser que la cámara la 
//considera para ver si dibujarla o no. Por defecto podemos pensar que 
//es el del recorte.

void Representacion::transformar_rotar(float v) 
{
	transformacion.angulo_rotacion=fmod(v, 360.f);
	actualizar_posicion_vista_rotacion();
}

void Representacion::transformar_cancelar_rotar() 
{
	transformacion.angulo_rotacion=0.f;
	actualizar_posicion_vista_rotacion();
}

void Representacion::transformar_centro_rotacion(float x, float y) 
{
	transformacion.centro_rotacion.x=x;
	transformacion.centro_rotacion.y=y;
	actualizar_posicion_vista_rotacion();
}

void Representacion::transformar_centro_rotacion_cancelar() 
{
	transformacion.centro_rotacion.x=0.f;
	transformacion.centro_rotacion.y=0.f;
	actualizar_posicion_vista_rotacion();
}

void Representacion::actualizar_posicion_vista_rotacion()
{
	if(!transformacion.es_transformacion())
	{
		posicion_vista=obtener_base_posicion_vista();
	}
	else
	{
		posicion_vista=calcular_posicion_vista_rotacion();
	}
}

Rect Representacion::calcular_posicion_vista_rotacion() const
{
	const auto p=obtener_base_posicion_vista();
	const auto pos=obtener_posicion();

	auto c=transformacion.centro_rotacion;

	DLibH::Poligono_2d_vertices<double> polig(
		{ 
			{(double)p.x, (double)p.y},
			{(double)(p.x+p.w), (double)p.y},
			{(double)(p.x+p.w), (double)(p.y+p.h)},
			{(double)p.x, (double)(p.y+p.h)},        
		}, {(double)c.x+pos.x, (double)c.y+pos.y});
		//Las rotaciones son "clockwise"... Las reales son "counter-clockwise"...
	float a=transformacion.angulo_rotacion;

	polig.rotar(a);

	//Sacar las medidas para la nueva caja...
	std::vector<double> xs={polig.vertice(0).x, polig.vertice(1).x, polig.vertice(2).x, polig.vertice(3).x};
	std::vector<double> ys={polig.vertice(0).y, polig.vertice(1).y, polig.vertice(2).y, polig.vertice(3).y};

	Rect res{0,0,0,0};
	res.x=*std::min_element(std::begin(xs), std::end(xs));
	res.y=*std::min_element(std::begin(ys), std::end(ys));
//	res.w=*std::max_element(std::begin(xs), std::end(xs))-posicion_vista.x;
//	res.h=*std::max_element(std::begin(ys), std::end(ys))-posicion_vista.y;
	res.w=*std::max_element(std::begin(xs), std::end(xs))-res.x;
	res.h=*std::max_element(std::begin(ys), std::end(ys))-res.y;

	return res;
}
