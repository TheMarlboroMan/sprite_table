#include "pantalla.h"

using namespace DLibV;

Pantalla::Pantalla(int p_w, int p_h, unsigned short int p_m):
	ventana(nullptr), camara_actual(nullptr), 
	info_volcado{0,0,0,0,(int)p_w,(int)p_h,1.0},
	w(p_w), h(p_h), modo_ventana(p_m), w_logico(w), h_logico(h)
{
	simulacro_caja.w=0;
 	simulacro_caja.h=0;
	simulacro_caja.x=0;
	simulacro_caja.y=0;
}

Pantalla::~Pantalla()
{
	if(ventana) SDL_DestroyWindow(ventana);
	SDL_GL_DeleteContext(context); 
}


void Pantalla::establecer_titulo(const char * p_cadena)
{
	SDL_SetWindowTitle(ventana, p_cadena);
}

void Pantalla::establecer_titulo(const std::string& c)
{
	SDL_SetWindowTitle(ventana, c.c_str());
}

void Pantalla::limpiar(const ColorRGBA& c)
{
	glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Pantalla::actualizar()
{
	SDL_GL_SwapWindow(ventana);
}

void Pantalla::inicializar(int p_w, int p_h, int flags_ventana)
{
	w=p_w;
	h=p_h;

	if(!ventana)
	{	
		//All these attributes must be set BEFORE creating the window!!.
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		//This is very important...
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

		ventana=SDL_CreateWindow("", 
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w, h, flags_ventana); //Por defecto SDL_WINDOW_OPENGL

		context=SDL_GL_CreateContext(ventana);
		establecer_medidas_logicas(w, h);
	}
	else
	{
		//Cambia el tamaño de la ventana pero jode completamente el viewport.
		SDL_SetWindowSize(ventana, w, h);
	}

	info_volcado={0,0,0,0,w,h,1.0};
	simulacro_caja.w=w;
	simulacro_caja.h=h;
	simulacro_caja.x=0;
	simulacro_caja.y=0;
}

void Pantalla::establecer_medidas_logicas(int pw, int ph)
{
	//Establecer el modo a matriz de proyección y cargarla a 1.
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	w_logico=pw;
	h_logico=ph;
	glViewport(0.f, 0.f, w, h);

	//Izquierda, derecha, abajo, arriba, cerca y lejos...
	//Ahora el punto 0.0 es arriba a la izquierda. Ajustamos a 0.5
	//para que pille el centro del pixel.
	glOrtho(-0.5f, (float)w_logico-.5f, (float)h_logico-.5f, -0.5f, 1.f, -1.0f);

	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

void Pantalla::establecer_modo_ventana(unsigned int v)
{
//TODO... These SDL functions may very well fuck everything up as will mess with the viewport.
//	modo_ventana=v;
//	switch(modo_ventana)
//	{
//		case M_VENTANA: SDL_SetWindowFullscreen(ventana, 0); break;
//		case M_PANTALLA_COMPLETA_RESOLUCION: SDL_SetWindowFullscreen(ventana, SDL_WINDOW_FULLSCREEN_DESKTOP); break;
//		case M_PANTALLA_COMPLETA_SIMULADA: SDL_SetWindowFullscreen(ventana, SDL_WINDOW_FULLSCREEN); break;
//	}
}

void Pantalla::establecer_clip(Camara const& p_camara)
{
	establecer_clip(p_camara.acc_caja_pos());
}

void Pantalla::establecer_clip(Rect p_caja)
{
	//Reiniciar matrices...
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Activar test de stencil, establecer valor de limpieza y limpiar.

	glDepthMask(GL_FALSE); //Realmente no sé si esto hace algo.
	glEnable(GL_STENCIL_TEST); //Esto se activa aquí... Se desactivaría en algún otro lado.
	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);

	//Nunca renderizar... pero si reemplazar.

//	glColor4f(0.1f, 0.6f, 0.6f, 0.2f);
	glStencilFunc(GL_NEVER, 1, 1);
//	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

	int	fx=p_caja.x+p_caja.w, 
		fy=p_caja.y+p_caja.h;

	struct pt {int x, y;};
	std::vector<pt> puntos{ 
		{p_caja.x, p_caja.y}, 
		{fx, p_caja.y}, 
		{fx, fy}, 
		{p_caja.x, fy}};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, 0, puntos.data());
	glDrawArrays(GL_POLYGON, 0, puntos.size());
	glDisableClientState(GL_VERTEX_ARRAY); 

	//Ahora si, sólo renderizamos cuando el valor sea igual y
	//no cambiaremos el pixel.
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void Pantalla::establecer_posicion(int x, int y)
{
	SDL_SetWindowPosition(ventana, x, y);
}

void Pantalla::asignar_camara(const Camara& c)
{
	if(!camara_actual || &c != camara_actual)
	{
		establecer_clip(c);
		camara_actual=&c;
	}
}

void Pantalla::reiniciar_clip()
{
	glDisable(GL_STENCIL_TEST); 
	camara_actual=nullptr;
}
