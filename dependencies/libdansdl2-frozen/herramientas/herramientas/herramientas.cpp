#include "herramientas.h"

using namespace DLibH;

float Herramientas::angulo_entre_puntos(float x1, float y1, float x2, float y2)
{
	float theta = atan2(y2 - y1, x2 - x1);
	theta += M_PI/2.0; //0 apunta ahora hacia el norte...
	float angulo=theta * 180 / M_PI; //Convertir a grados.
	if(angulo < 0) angulo+=360;
	return angulo;
}

float Herramientas::porcentaje(float p_parte, float p_total)
{
	if(p_total==0) return 0;
	else return (p_parte * 100) / p_total;
}

bool Herramientas::punto_en_caja(int cx, int cy, unsigned int cw, unsigned int ch, int px, int py)
{
        return  (cx <= px && (int)(cx+cw) >= px)
        &&
        (cy <= py && (int)(cy+ch) >= py);
}

bool Herramientas::caja_en_caja(int pqx, int pqy, unsigned int pqw, unsigned int pqh, int grx, int gry, unsigned int grw, unsigned int grh)
{
	bool resultado=false;

	int grande_xf=grx+grw,
	pequena_xf=pqx+pqw,
	grande_yf=gry+grh,
	pequena_yf=pqy+pqh;

	resultado=(grx <= pqx
	&& grande_xf >= pequena_xf
	&& gry <= pqy
	&& grande_yf >= pequena_yf);

	return resultado;
}

float Herramientas::angulo_360(float a)
{
	//Convertir a formato 360...
	if(a < 0.f)
	{
		a=180.f + 180.f + a;
	}

	return a;
}
