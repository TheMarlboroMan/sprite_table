#ifndef COLOR_LIBDANSDL2_H
#define COLOR_LIBDANSDL2_H

namespace DLibV
{

struct ColorRGBA
{
	float r, g, b, a;
};

ColorRGBA rgba8(int, int, int, int);
float colorfi(int);
int colorif(float);

}

#endif
