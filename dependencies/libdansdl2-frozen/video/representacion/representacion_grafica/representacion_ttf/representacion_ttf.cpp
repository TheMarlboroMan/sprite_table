#include "representacion_ttf.h"

using namespace DLibV;

Representacion_TTF::Representacion_TTF(const Fuente_TTF& fuente, ColorRGBA color, std::string texto)
	:Representacion_grafica(color, Rect{0,0,0,0}, Rect{0,0,0,0}, sampling::completo), 
	fuente(&fuente),
	cadena(texto)
{
	generar_textura();
	actualizar_posicion_vista_rotacion();
}

Representacion_TTF::Representacion_TTF(const Representacion_TTF& o)
	:Representacion_grafica(o), 
	fuente(o.fuente),
	cadena(o.cadena)
{
	generar_textura();
}

Representacion_TTF::~Representacion_TTF()
{
	liberar_textura();
}

Representacion_TTF& Representacion_TTF::operator=(const Representacion_TTF& o)
{
	Representacion_grafica::operator=(o);
	fuente=o.fuente;
	cadena=o.cadena;
	generar_textura();

	return *this;
}

void Representacion_TTF::generar_textura()
{	
	//Si el texto tiene newlines no va a funcionar: tenemos que montar
	//nosotros el código. Empezamos por partir el texto en varias líneas...

	auto explotar=[](const std::string& cadena, char p_delimitador)
	{
		std::vector<std::string> resultado;
		std::string temp;

		for(const char& c : cadena) 
		{
			if(c==p_delimitador) 
			{	
				resultado.push_back(temp);
				temp="";
			}
			else
			{
				temp+=c;
			}
		}

		resultado.push_back(temp);
		return resultado;
	};

	std::vector<std::string> lineas=explotar(cadena, '\n');

	//Y ahora medimos el que sería el tamaño total de la superficie del texto.
	int total_h=0, h=0, w=0, tw=0;

	for(const std::string& c : lineas)
	{
		TTF_SizeUTF8(const_cast<TTF_Font*>(fuente->acc_fuente()), c.c_str(), &tw, &h);
		if(tw > w) w=tw;
	}

	total_h=(lineas.size() * h);
	// TTF_FontLineSkip(const_cast<TTF_Font*>(fuente->acc_fuente()))) + h;

	//Podemos preparar una superficie de ese tamaño... Vamos a sacar una
	//superficie primero para obtener el formato... Es una mierda pero
	//me vale.
	auto col=acc_rgba();
	SDL_Color sdl_col{(Uint8)colorif(col.r), (Uint8)colorif(col.g), (Uint8)colorif(col.b)};

	SDL_Surface * s=TTF_RenderUTF8_Blended
			(const_cast<TTF_Font*>(fuente->acc_fuente()), "a", sdl_col);
	std::unique_ptr<DLibV::Lienzo> lienzo(DLibV::Lienzo::generar_nuevo(w, total_h, s->format->BitsPerPixel, s->format->Rmask, s->format->Gmask, s->format->Bmask, s->format->Amask));
	SDL_FreeSurface(s);

	//Y ahora, por cada línea, crear una superficie y pegarla en el lienzo...
	int y=0;

	for(std::string& c : lineas)
	{
		reemplazar(c, "\t", "    ");
		const char * cad=c.size() ? c.c_str() : " \0";

		SDL_Surface * s=TTF_RenderUTF8_Blended
			(const_cast<TTF_Font*>(fuente->acc_fuente()), cad, sdl_col);

		if(!s)
		{
			throw std::runtime_error("Error preparando Representacion_TTF : "+std::string(TTF_GetError())+" : "+c);
		}
		else
		{
			SDL_Rect pos{0, y, 0, 0}; 
			SDL_BlitSurface(s, nullptr, lienzo->acc_superficie(), &pos);
			SDL_FreeSurface(s);
			y+=h;
		}		
	}

	if(!ptr_textura())
	{
		DLibV::Textura * tex=new DLibV::Textura(*lienzo);
		establecer_textura(tex);
	}
	else
	{
		ptr_textura()->reemplazar(*lienzo);
	}
	
	const auto &ref_tex=ptr_textura();
	establecer_modo_blend(Representacion::blends::alpha);
	establecer_recorte({0,0, ref_tex->acc_w(), ref_tex->acc_h()});
	establecer_posicion(0, 0, ref_tex->acc_w(), ref_tex->acc_h(), FRECT_W|FRECT_H);	//Esto debemos llamarlo aquí, de lo contrario se queda con ancho y alto 0, dando problemas con las cámaras.
}

void Representacion_TTF::modificar_fuente(const Fuente_TTF& f)
{
	fuente=&f;
	liberar_textura();
	generar_textura();
}

void Representacion_TTF::asignar(const char c)
{
	//Hay que joderse...
	std::string temp("");
	temp+=c;
	interno_asignar(temp);
}

void Representacion_TTF::asignar(const char * c)
{
	interno_asignar(c);
}

void Representacion_TTF::asignar(const std::string& c)
{
	interno_asignar(c);
}

void Representacion_TTF::interno_asignar(const std::string& c)
{
	if(c!=cadena)
	{
		//No se limpia la textura: se va a reutilizar.
		cadena=c;
		liberar_calculos();
		generar_textura();
	}
}

void Representacion_TTF::reemplazar(std::string& sujeto, const std::string& busca, const std::string& reemplaza)
{
	size_t pos = 0, l=reemplaza.length();
	while ((pos = sujeto.find(busca, pos)) != std::string::npos) 
	{
		sujeto.replace(pos, busca.length(), reemplaza);
		pos += l;
	}
}
