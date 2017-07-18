#include "representacion_primitiva.h"

using namespace DLibV;

Representacion_primitiva::Representacion_primitiva(ColorRGBA c)
	:Representacion(c)
{

}

Representacion_primitiva::Representacion_primitiva(const Representacion_primitiva& p_otra)
	:Representacion(p_otra)
{

}

Representacion_primitiva& Representacion_primitiva::operator=(const Representacion_primitiva& p_otra)
{
	Representacion::operator=(p_otra);
	return *this;
}

void Representacion_primitiva::preparar_color()
{
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

	glDisable(GL_TEXTURE_2D);
}
