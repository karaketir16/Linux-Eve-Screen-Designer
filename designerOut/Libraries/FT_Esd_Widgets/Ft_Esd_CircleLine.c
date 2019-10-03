#include "Ft_Esd.h"
#include "FT_Esd_Dl.h"
#include "Ft_Esd_CircleLine.h"

extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;

void Ft_Esd_CircleLine_SetDSTAlpha();
void Ft_Esd_CircleLine_ClearDSTAlpha();
void Ft_Esd_CircleLine_Draw_Point(int x, int y, int radius);
void Ft_Esd_CircleLine_Draw_Point_Float(ft_float_t x, ft_float_t y, ft_float_t radius);
void Ft_Esd_CircleLine_Draw_Square(int x, int y, int side);

ESD_METHOD(Ft_Esd_CircleLine_Render_Func, Context = Ft_Esd_CircleLine)
ESD_PARAMETER(x, Type = int)
ESD_PARAMETER(y, Type = int)
ESD_PARAMETER(radius, Type = int)
ESD_PARAMETER(border, Type = int)
ESD_PARAMETER(color, Type = ft_argb32_t)
void Ft_Esd_CircleLine_Render_Func(Ft_Esd_CircleLine *context, int x, int y, int radius, int border, ft_argb32_t color);

void Ft_Esd_CircleLine_Render_Func(Ft_Esd_CircleLine *context, int x, int y, int radius, int border, ft_argb32_t color)
{
	if (radius < 0)
		return;
	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));
	int innerRadius = radius - border;
	if (innerRadius < 0)
		innerRadius = 0;

	Ft_Esd_Primitive = 0;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SAVE_CONTEXT());

	Ft_Esd_Rect16 scissor = Ft_Esd_Rect16_Crop(context->Widget.GlobalRect, Ft_Esd_Dl_Scissor_Get());
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SCISSOR_XY(scissor.X, scissor.Y));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SCISSOR_SIZE(scissor.Width, scissor.Height));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, CLEAR_COLOR_RGB(0, 0, 0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, CLEAR_COLOR_A(0));
	//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, CLEAR(1, 1, 1));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(0, 0, 0, 1));

	Ft_Esd_CircleLine_ClearDSTAlpha();
	Ft_Esd_CircleLine_Draw_Square(x - radius - 1, y - radius - 1, (radius * 2) + 2);

	Ft_Esd_CircleLine_SetDSTAlpha();

	Ft_Esd_CircleLine_Draw_Point(x, y, radius);

	Ft_Esd_CircleLine_ClearDSTAlpha();

	if (innerRadius > 0)
	{
		Ft_Esd_CircleLine_Draw_Point(x, y, innerRadius);
	}

	// Finally draw final color
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(1, 1, 1, 0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));

	Ft_Esd_CircleLine_Draw_Square(x - radius, y - radius, radius * 2);

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, RESTORE_CONTEXT());
}

void Ft_Esd_CircleLine_SetDSTAlpha()
{
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BLEND_FUNC(ONE, ONE_MINUS_SRC_ALPHA));
}

void Ft_Esd_CircleLine_ClearDSTAlpha()
{
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BLEND_FUNC(ZERO, ONE_MINUS_SRC_ALPHA));
}

void Ft_Esd_CircleLine_Draw_Square(int x, int y, int side)
{
#if (EVE_MODEL >= EVE_FT810)
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(4));
#endif
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(RECTS));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, LINE_WIDTH(16));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F((x * 16), (y * 16)));
	x += side;
	y += side;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F((x * 16), (y * 16)));
	// Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
}

void Ft_Esd_CircleLine_Draw_Point(int x, int y, int radius)
{
	Ft_Esd_CircleLine_Draw_Point_Float((ft_float_t)x, (ft_float_t)y, (ft_float_t)radius);
}

// FIXME: Use ft_int16_f4_t as fixed point instead of float
void Ft_Esd_CircleLine_Draw_Point_Float(ft_float_t x, ft_float_t y, ft_float_t radius)
{
#if (EVE_MODEL >= EVE_FT810)
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(4));
#endif
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(FTPOINTS));
	int iR = (radius * 16.0f) + 0.5f;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, POINT_SIZE(iR));
	int iX = (x * 16.0f) + 0.5f;
	int iY = (y * 16.0f) + 0.5f;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(iX, iY));
	// Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
}
