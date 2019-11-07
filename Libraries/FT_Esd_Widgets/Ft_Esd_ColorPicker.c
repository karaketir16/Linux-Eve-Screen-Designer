#include "Ft_Esd.h"
#include "Ft_Esd_ColorPicker.h"
#define _USE_MATH_DEFINES 1
#include <math.h>

ESD_FUNCTION(getSat, Type = double, DisplayName = "Get Saturation", Category = Ft_Esd_ColorPicker)
ESD_PARAMETER(x0, Type = int)
ESD_PARAMETER(y0, Type = int)
ESD_PARAMETER(x1, Type = int)
ESD_PARAMETER(y1, Type = int)
ESD_PARAMETER(r, Type = double)
double getSat(int x0, int y0, int x1, int y1, double r)
{
	double sizeX = pow((x1 - x0), 2);
	double sizeY = pow((y1 - y0), 2);
	return sqrt(sizeX + sizeY) / r;
}

ESD_FUNCTION(getHue, Type = double, DisplayName = "Get Hue", Category = Ft_Esd_ColorPicker)
ESD_PARAMETER(x0, Type = int)
ESD_PARAMETER(y0, Type = int)
ESD_PARAMETER(x1, Type = int)
ESD_PARAMETER(y1, Type = int)
double getHue(int x0, int y0, int x1, int y1)
{
	double angleInRadian, angleInDegree;
	if (x0 == x1)
	{
		if (y1 < y0)
			return 90;
		else if (y1 > y0)
			return 270;
		else
			return 0;
	}
	else if (x1 > x0)
	{
		if (y1 > y0)
		{
			angleInRadian = atan2((y1 - y0), (x1 - x0));
			angleInDegree = 360 - (angleInRadian * 180 / M_PI);
		}
		else
		{
			angleInRadian = atan2((y0 - y1), (x1 - x0));
			angleInDegree = angleInRadian * 180 / M_PI;
		}
	}
	else
	{
		if (y1 > y0)
		{
			angleInRadian = atan2((y1 - y0), (x0 - x1));
			angleInDegree = 180 + (angleInRadian * 180 / M_PI);
		}
		else
		{
			angleInRadian = atan2((y0 - y1), (x0 - x1));
			angleInDegree = 180 - (angleInRadian * 180 / M_PI);
		}
	}
	return angleInDegree;
}

ESD_FUNCTION(hsvToRgb, Type = ft_argb32_t, DisplayName = "HSV to RGB", Category = Ft_Esd_ColorPicker)
ESD_PARAMETER(h, Type = double)
ESD_PARAMETER(s, Type = double)
ESD_PARAMETER(v, Type = double)
ft_argb32_t hsvToRgb(double h, double s, double v)
{
	double r, g, b;
	int R, G, B;
	h = h / 360.0;
	v = v / 255.0;
	int i = (int)(h * 6);
	double f = h * 6 - i;
	double p = v * (1 - s);
	double q = v * (1 - f * s);
	double t = v * (1 - (1 - f) * s);

	switch (i % 6)
	{
	case 0:
		r = v, g = t, b = p;
		break;
	case 1:
		r = q, g = v, b = p;
		break;
	case 2:
		r = p, g = v, b = t;
		break;
	case 3:
		r = p, g = q, b = v;
		break;
	case 4:
		r = t, g = p, b = v;
		break;
	case 5:
		r = v, g = p, b = q;
		break;
	}
	R = (int)(r * 255.0);
	G = (int)(g * 255.0);
	B = (int)(b * 255.0);
	return (ft_argb32_t)((0xFF << 24) + (R << 16) + (G << 8) + B);
}
