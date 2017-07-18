#include "color.h"

using namespace DLibV;

//Entra 0-255 sale 0-1
float DLibV::colorfi(int v)
{
	return (float)v / 255.f;
}

//Entra 0-1 sale 0-255
int DLibV::colorif(float v)
{
	return v * 255.f;
}

ColorRGBA DLibV::rgba8(int r, int g, int b, int a)
{
	return ColorRGBA{colorfi(r), colorfi(g), colorfi(b), colorfi(a)};
}
