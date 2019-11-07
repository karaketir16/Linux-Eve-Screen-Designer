/*
PolygonWidget
C Source
*/
#include "Ft_Esd_Elements.h"

#include "Ft_Esd.h"
#include "Ft_Esd_Widget.h"
#include "FT_Esd_Primitives.h"
#include "Ft_Esd_PolygonWidget.h"

#define WIDTH 800.00
#define HEIGHT 480.00
ft_int32_t polyWidth = 250;
ft_int32_t polyHeight = 250;

#define MAX_VERTEX 8
ft_int32_t X[MAX_VERTEX] = { -1, -1, -1, -1, -1, -1, -1, -1 };
ft_int32_t Y[MAX_VERTEX] = { -1, -1, -1, -1, -1, -1, -1, -1 };

void DrawPolygon(ft_argb32_t color, ft_bool_t fillOrBorder)
{
#if (EVE_MODEL >= EVE_FT810)
	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));

	Esd_Dl_COLOR_ARGB(color);
	Ft_Esd_Primitive = 0;
	Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, SAVE_CONTEXT());

	if (fillOrBorder)
	{
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, COLOR_MASK(0, 0, 0, 0));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, STENCIL_OP(KEEP, INVERT));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, STENCIL_FUNC(ALWAYS, 255, 255));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, BEGIN(EDGE_STRIP_B));
	}
	else
	{
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, BEGIN(LINE_STRIP));
	}

	Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, VERTEX_FORMAT(3));

	for (int i = 0; i < MAX_VERTEX; ++i)
	{
		if ((X[i] < 0) || (Y[i] < 0))
			continue;

		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, VERTEX2F(X[i] << 3, Y[i] << 3));
	}

	Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, VERTEX2F(X[0] << 3, Y[0] << 3));
	Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, END());

	if (fillOrBorder)
	{
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, COLOR_MASK(1, 1, 1, 1));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, STENCIL_FUNC(EQUAL, 255, 255));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, BEGIN(EDGE_STRIP_R));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, VERTEX2F(0, 0));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, VERTEX2F(0, 511 << 4));
		Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, END());
	}

	Ft_Gpu_Hal_WrCmd32(Ft_Esd_Host, RESTORE_CONTEXT());
#endif
}

ESD_METHOD(Ft_Esd_PolygonWidget_DrawPolygon, Context = Ft_Esd_PolygonWidget)
ESD_PARAMETER(xPos, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(yPos, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(width, Type = ft_int32_t, Default = 250)
ESD_PARAMETER(height, Type = ft_int32_t, Default = 250)
ESD_PARAMETER(x1, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y1, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x2, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y2, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x3, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y3, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x4, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y4, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x5, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y5, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x6, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y6, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x7, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y7, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(x8, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(y8, Type = ft_int32_t, Default = 0)
ESD_PARAMETER(borderWidth, Type = ft_int16_t, Default = 0)
ESD_PARAMETER(borderColor, Type = ft_argb32_t, Default = 0x00000000)
ESD_PARAMETER(fillColor, Type = ft_argb32_t, Default = 0xFFFFFF00)
ft_void_t Ft_Esd_PolygonWidget_DrawPolygon(Ft_Esd_PolygonWidget *context,
    ft_int32_t xPos, ft_int32_t yPos,
    ft_int32_t width, ft_int32_t height,
    ft_int32_t x1, ft_int32_t y1,
    ft_int32_t x2, ft_int32_t y2,
    ft_int32_t x3, ft_int32_t y3,
    ft_int32_t x4, ft_int32_t y4,
    ft_int32_t x5, ft_int32_t y5,
    ft_int32_t x6, ft_int32_t y6,
    ft_int32_t x7, ft_int32_t y7,
    ft_int32_t x8, ft_int32_t y8,
    ft_int16_t borderWidth,
    ft_argb32_t borderColor,
    ft_argb32_t fillColor)
{
	Esd_Dl_LINE_WIDTH(borderWidth << 3);

	X[0] = x1 < 0 ? x1 : (x1 + xPos);
	X[1] = x2 < 0 ? x2 : (x2 + xPos);
	X[2] = x3 < 0 ? x3 : (x3 + xPos);
	X[3] = x4 < 0 ? x4 : (x4 + xPos);
	X[4] = x5 < 0 ? x5 : (x5 + xPos);
	X[5] = x6 < 0 ? x6 : (x6 + xPos);
	X[6] = x7 < 0 ? x7 : (x7 + xPos);
	X[7] = x8 < 0 ? x8 : (x8 + xPos);

	Y[0] = y1 < 0 ? y1 : (y1 + yPos);
	Y[1] = y2 < 0 ? y2 : (y2 + yPos);
	Y[2] = y3 < 0 ? y3 : (y3 + yPos);
	Y[3] = y4 < 0 ? y4 : (y4 + yPos);
	Y[4] = y5 < 0 ? y5 : (y5 + yPos);
	Y[5] = y6 < 0 ? y6 : (y6 + yPos);
	Y[6] = y7 < 0 ? y7 : (y7 + yPos);
	Y[7] = y8 < 0 ? y8 : (y8 + yPos);

	DrawPolygon(fillColor, FT_TRUE);

	if (0 != borderWidth)
	{
		DrawPolygon(borderColor, FT_FALSE);
	}
}

/* end of file */
