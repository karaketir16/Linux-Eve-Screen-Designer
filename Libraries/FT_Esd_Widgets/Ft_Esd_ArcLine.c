#include "Ft_Esd_Core.h"
#include "FT_Esd_Dl.h"
#include "Ft_Esd_ArcLine.h"

#include <math.h>

enum Ft_Esd_ArcLine_SectionType
{
	Partial = 1,
	Full = 0
};

extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;

extern void Ft_Esd_CircleLine_SetDSTAlpha();
extern void Ft_Esd_CircleLine_ClearDSTAlpha();

extern void Ft_Esd_CircleLine_Draw_Point(int x, int y, int radius);
extern void Ft_Esd_CircleLine_Draw_Point_Float(ft_float_t x, ft_float_t y, ft_float_t radius);
extern void Ft_Esd_CircleLine_Draw_Square(int x, int y, int side);

ft_bool_t Ft_Esd_ArcLine_Clamp_Float(ft_float_t *angle, ft_float_t min, ft_float_t max)
{
	if (angle == 0)
		return 0;
	ft_float_t old = *angle;
	*angle = (*angle < min) ? min : (*angle > max) ? max : *angle;
	return old != *angle;
};

void Ft_Esd_ArcLine_Draw_Arc_Left(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle);
void Ft_Esd_ArcLine_Draw_Arc_Right(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle);
void Ft_Esd_ArcLine_Draw_Arc_Above(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle);
void Ft_Esd_ArcLine_Draw_Arc_Below(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle);
void Ft_Esd_ArcLine_Draw_Line(int x0, int y0, int x1, int y1);
void Ft_Esd_ArcLine_GetTanglePoint(ft_float_t angle, ft_float_t radius, ft_float_t *x, ft_float_t *y, ft_bool_t isClockwise);

void Ft_Esd_ArcLine_Draw_Sectors(int sectorMask, int x, int y, int side);
void Ft_Esd_ArcLine_Draw_SectorArea(int x, int y, int radius, ft_bool_t isClockwise, int origin, int value);
void Ft_Esd_ArcLine_Draw_Sharp_Squares(int x[], int y[], int count, int side); // No Anti-aliasing at side

ESD_METHOD(Ft_Esd_ArcLine_Render_Func, Context = Ft_Esd_ArcLine)
ESD_PARAMETER(x, Type = int)
ESD_PARAMETER(y, Type = int)
ESD_PARAMETER(radius, Type = int)
ESD_PARAMETER(border, Type = int)
ESD_PARAMETER(color, Type = ft_argb32_t)
ESD_PARAMETER(isClockwise, Type = ft_bool_t)
ESD_PARAMETER(origin, Type = int)
ESD_PARAMETER(value, Type = int)
void Ft_Esd_ArcLine_Render_Func(Ft_Esd_ArcLine *context, int x, int y, int radius, int border, ft_argb32_t color, ft_bool_t isClockwise, int origin, int value);

void Ft_Esd_ArcLine_Render_Func(Ft_Esd_ArcLine *context, int x, int y, int radius, int border, ft_argb32_t color, ft_bool_t isClockwise, int origin, int value)
{
#if (EVE_MODEL >= EVE_FT810)
	if (radius <= 0)
		return;
	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));
	int innerRadius = radius - border;
	if (innerRadius < 0)
		innerRadius = 0;

	origin %= 360;
	int originSector = (origin / 90) % 4;
	int endAngle = origin + value;
	int endSector = (endAngle / 90) % 4;

	if (x + radius + radius < 0)
	{
		//printf("x too small  %d, %d, %d\n", x, y, radius);
		return;
	}
	//printf("x, y, radius %d, %d, %d\n", x, y, radius);
	//printf("innerRadius, endAngle, originSector %d, %d, %d\n", innerRadius, endAngle, originSector);

	Ft_Esd_Primitive = 0;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SAVE_CONTEXT()); // A

	Ft_Esd_Rect16 scissor = {
		.X = context->Widget.GlobalX - 1, .Y = context->Widget.GlobalY - 1, .Width = context->Widget.GlobalWidth + 2, .Height = context->Widget.GlobalHeight + 2
	};
	scissor = Ft_Esd_Rect16_Crop(scissor, Ft_Esd_Dl_Scissor_Get());
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SCISSOR_XY(scissor.X, scissor.Y));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SCISSOR_SIZE(scissor.Width, scissor.Height));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, CLEAR_COLOR_RGB(0, 0, 0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, CLEAR_COLOR_A(0));
	//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, CLEAR(1, 1, 1));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(0, 0, 0, 1));

	Ft_Esd_CircleLine_ClearDSTAlpha();
	Ft_Esd_CircleLine_Draw_Square(x - radius - 3, y - radius - 3, (radius * 2) + 6);

	Ft_Esd_CircleLine_SetDSTAlpha();

	Ft_Esd_CircleLine_Draw_Point(x, y, radius);

	Ft_Esd_CircleLine_ClearDSTAlpha();

	if (innerRadius > 0)
	{
		Ft_Esd_CircleLine_Draw_Point(x, y, innerRadius);
	}
	// Edge trip all un-neccessary parts

	if (!isClockwise)
		originSector = 3 - originSector;
	if (isClockwise)
	{
		//printf("handle originSector, %d\n", originSector);
		switch (originSector)
		{
		case 0:
			//printf("trim by edge trip Right, Clockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Right((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		case 1:
			//printf("trim by edge trip Below, Clockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Below((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		case 2:
			//printf("trim by edge trip Left, Clockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Left((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		case 3:
			//printf("trim by edge trip Above, Clockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Above((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		}
	}
	else
	{
		switch (originSector)
		{
		case 3:
			//printf("trim by edge trip Right, AntiClockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Right((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		case 2:
			//printf("trim by edge trip Below, AntiClockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Above((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		case 1:
			//printf("trim by edge trip Left, AntiClockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Left((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		case 0:
			//printf("trim by edge trip Above, AntiClockwise, %d\n", origin);
			Ft_Esd_ArcLine_Draw_Arc_Below((ft_float_t)origin, x, y, radius * 2, 0xFFFFFFFF, isClockwise, 0);
			break;
		}
	}

	if (!isClockwise)
		endSector = 3 - (endSector % 4);
	if ((value < 270) || (value >= 270 && originSector != endSector))
	{
		if (isClockwise)
		{
			//printf("handle endSector, %d\n", endSector);

			switch (endSector)
			{
			case 0:
				//printf("trim by edge trip Below, Clockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Below((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			case 1:
				//printf("trim by edge trip Left, Clockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Left((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			case 2:
				//printf("trim by edge trip Above, Clockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Above((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			case 3:
				//printf("trim by edge trip Right, Clockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Right((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			}
		}
		else
		{
			switch (endSector)
			{
			case 3:
				//printf("trim by edge trip Below, AntiClockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Above((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			case 2:
				//printf("trim by edge trip Left, AntiClockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Left((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			case 1:
				//printf("trim by edge trip Above, AntiClockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Below((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			case 0:
				//printf("trim by edge trip Right, AntiClockwise, %d\n", endAngle);
				Ft_Esd_ArcLine_Draw_Arc_Right((ft_float_t)360 - (endAngle % 360), x, y, radius * 2, 0xFFFFFFFF, !isClockwise, 0);
				break;
			}
		}
	}

	if (value < 360 && value > 0)
	{
		if (!context->ShowStartPoint)
		{
			// Start point as line
			ft_float_t x0 = x * 16, y0 = y * 16;
			ft_float_t x1 = x * 16, y1 = y * 16;
			Ft_Esd_ArcLine_GetTanglePoint(origin, (innerRadius + 0.8) * 16, &x0, &y0, isClockwise);
			Ft_Esd_ArcLine_GetTanglePoint(origin, (radius - 0.8) * 16, &x1, &y1, isClockwise);

			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(4));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, LINE_WIDTH(16));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(LINES));
			int _x = x0;
			int _y = y0;
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(_x, _y));
			_x = x1;
			_y = y1;
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(_x, _y));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
		}
		if (!context->ShowEndPoint)
		{
			//printf("Draw endpoint\n");
			// End point as line
			ft_float_t x0 = x * 16, y0 = y * 16;
			ft_float_t x1 = x * 16, y1 = y * 16;
			Ft_Esd_ArcLine_GetTanglePoint(endAngle, (innerRadius + 0.8) * 16, &x0, &y0, isClockwise);
			Ft_Esd_ArcLine_GetTanglePoint(endAngle, (radius - 0.8) * 16, &x1, &y1, isClockwise);

			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(4));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, LINE_WIDTH(16));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(LINES));
			int _x = x0;
			int _y = y0;
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(_x, _y));
			_x = x1;
			_y = y1;
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(_x, _y));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
		}
	}

	// Finally draw final color
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(1, 1, 1, 1));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));
	Ft_Esd_ArcLine_Draw_SectorArea(x, y, radius, isClockwise, origin, value);

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, RESTORE_CONTEXT()); // A

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SAVE_CONTEXT()); // B

	// Draw endpoints
	ft_float_t r0 = (radius + innerRadius) / 2.0f;

	if (context->ShowStartPoint)
	{
		ft_float_t x0 = x, y0 = y;
		ft_float_t x1 = x, y1 = y;

		Ft_Esd_ArcLine_GetTanglePoint(origin, r0, &x1, &y1, isClockwise);
		//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
		if (context->ShowPointShadow)
		{
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(185));
			Ft_Esd_ArcLine_GetTanglePoint(origin - 0.5f, r0 - 0.5f, &x0, &y0, isClockwise);
			//ft_float_t startR = (radius - innerRadius - 1) /2.0f;
			//Ft_Esd_ArcLine_GetTanglePoint(((origin +1 - 90 ) + 360) % 360, 2, &x1, &y1, isClockwise);
			if (x0 == x1 && y0 == y1)
			{
				x0 += (x1 > x);
				y0 += (y1 > y);
			}
			Ft_Esd_CircleLine_Draw_Point_Float(x0, y0, (radius - innerRadius - 1) / 2.0f);
		}

		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));
		Ft_Esd_CircleLine_Draw_Point_Float(x1, y1, (radius - innerRadius) / 2.0f);
	}

	if (context->ShowEndPoint)
	{
		ft_float_t x1 = x, y1 = y;

		Ft_Esd_ArcLine_GetTanglePoint(endAngle, r0, &x1, &y1, isClockwise);
		//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
		if (context->ShowPointShadow)
		{
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r >> 1, g >> 1, b >> 1));
			Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(125));
			ft_float_t x0 = x1, y0 = y1;
			Ft_Esd_ArcLine_GetTanglePoint(((endAngle + 90) + 360) % 360, 2, &x0, &y0, isClockwise);
			Ft_Esd_CircleLine_Draw_Point_Float(x0, y0, (radius - innerRadius - 1) / 2.0f); // reduce dark 	shadow point
		}

		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
		Ft_Esd_CircleLine_Draw_Point_Float(x1, y1, (radius - innerRadius) / 2.0f);
	}
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, RESTORE_CONTEXT()); // B

	if (endAngle > 360 && (endAngle % 90) > 0)
	{
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SAVE_CONTEXT()); // C

		Ft_Esd_Rect16 scissor = Ft_Esd_Rect16_Crop(context->Widget.GlobalRect, Ft_Esd_Dl_Scissor_Get());
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SCISSOR_XY(scissor.X, scissor.Y));
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SCISSOR_SIZE(scissor.Width, scissor.Height));

		//printf("Sec Round\n");
		//Ft_Esd_CircleLine_ClearDSTAlpha();
		//Ft_Esd_CircleLine_Draw_Square( x - radius -1, y - radius -1, (radius * 2) + 2);

		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(0, 0, 0, 1));
		Ft_Esd_CircleLine_SetDSTAlpha();

		Ft_Esd_CircleLine_Draw_Point(x, y, radius);

		Ft_Esd_CircleLine_ClearDSTAlpha();

		if (innerRadius > 0)
		{
			Ft_Esd_CircleLine_Draw_Point(x, y, innerRadius);
		}

		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(1, 1, 1, 1));
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BLEND_FUNC(DST_ALPHA, ONE_MINUS_DST_ALPHA));

		int endAngle = value + (origin % 360);
		int sector = (endAngle / 90) % 4;
		if (!isClockwise)
			sector = 3 - sector;
		//printf("Handle Last overwrite end Sector: %d, %d\n", sector, endAngle);
		if (originSector == sector)
			if (isClockwise)
			{
				switch (sector)
				{
				case 0:
					//printf("trim by edge trip Right, Clockwise, %d\n", endAngle - 360);
					Ft_Esd_ArcLine_Draw_Arc_Right((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				case 1:
					//printf("trim by edge trip Below, Clockwise, %d\n", endAngle - 360);
					Ft_Esd_ArcLine_Draw_Arc_Below((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				case 2:
					//printf("trim by edge trip Left, Clockwise, %d\n", endAngle - 360);
					Ft_Esd_ArcLine_Draw_Arc_Left((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				case 3:
					//printf("trim by edge trip Above, Clockwise, %d\n", endAngle - 360);
					Ft_Esd_ArcLine_Draw_Arc_Above((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				}
			}
			else
			{
				switch (originSector)
				{
				case 3:
					//printf("trim by edge trip Right, AntiClockwise, %d\n", origin);
					Ft_Esd_ArcLine_Draw_Arc_Right((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				case 2:
					//printf("trim by edge trip Below, AntiClockwise, %d\n", origin);
					Ft_Esd_ArcLine_Draw_Arc_Above((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				case 1:
					//printf("trim by edge trip Left, AntiClockwise, %d\n", origin);
					Ft_Esd_ArcLine_Draw_Arc_Left((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				case 0:
					//printf("trim by edge trip Above, AntiClockwise, %d\n", origin);
					Ft_Esd_ArcLine_Draw_Arc_Below((ft_float_t)endAngle - 360, x, y, radius * 2, color, isClockwise, 0);
					break;
				}
			}
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, RESTORE_CONTEXT()); // C

		if (value < 360 && value > 0)
		{
			if (!context->ShowEndPoint)
			{
				//printf("Draw endpoint 2\n");
				// End point as line
				ft_float_t x0 = x * 16, y0 = y * 16;
				ft_float_t x1 = x * 16, y1 = y * 16;
				Ft_Esd_ArcLine_GetTanglePoint(endAngle - 0.2, (innerRadius + 0.8) * 16, &x0, &y0, isClockwise);
				Ft_Esd_ArcLine_GetTanglePoint(endAngle - 0.2, (radius - 0.8) * 16, &x1, &y1, isClockwise);

				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, SAVE_CONTEXT()); // D

				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(255));
				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(4));
				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, LINE_WIDTH(16));
				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(LINES));
				int _x = x0;
				int _y = y0;
				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(_x, _y));
				_x = x1;
				_y = y1;
				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(_x, _y));
				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());

				Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, RESTORE_CONTEXT()); // D
			}
		}
	}

	//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, RESTORE_CONTEXT());
	//printf("\n\n");
#endif
}

void Ft_Esd_ArcLine_GetTanglePoint(ft_float_t angle, ft_float_t radius, ft_float_t *x, ft_float_t *y, ft_bool_t isClockwise)
{
	ft_float_t x0 = *x;
	ft_float_t y0 = *y;

	if (angle > 360)
		angle -= 360;
	else if (angle < 0)
		angle += 360;

	//printf(">Angle: %f , x,y,r,pr: %d,%d,%d,%d\n", angle, x,y,radius,pointRadius);
	ft_float_t dA = angle * 3.14159265 / 180.0f;
	ft_float_t dY = sin(dA) * (radius);
	ft_float_t dX = cos(dA) * (radius);
	*x = x0 + dX;
	*y = y0 + ((isClockwise) ? dY : -dY);
	//printf(">x,y: %d,%d\n", x,y);
	//DrawCirle(x, y, pointRadius >> 1, color, 0xFFFFFFFF);
}

void Ft_Esd_ArcLine_Draw_SectorArea(int x, int y, int radius, ft_bool_t isClockwise, int origin, int value)
{
	while (origin < 0)
		origin += 360;
	while (origin > 360)
		origin -= 360;

	int originSector = origin / 90;
	int endSector = (origin + value) / 90;

	int sectorMask = 0;
	for (int i = originSector; i <= endSector; i++)
	{
		sectorMask |= (isClockwise) ? (1 << i) : (1 << (7 - i)); // Clockwise: 0, 1, 2, 3;   AntiClockwise: 3, 2, 1, 0
	}
	//printf("sectorMask: %d\n", sectorMask);
	Ft_Esd_ArcLine_Draw_Sectors(sectorMask, x, y, radius);
}
void Ft_Esd_ArcLine_Draw_Sectors(int sectorMask, int x, int y, int side)
{
	sectorMask = (sectorMask | (sectorMask >> 4)) & 0xF;
	if (sectorMask == 0xF)
	{
		//printf(" all sectors: %d (%d %d %d)\n", sectorMask, x - side, y - side, side * 2);
		Ft_Esd_CircleLine_Draw_Square(x - side, y - side, side * 2);
		return;
	}

	for (int mask = 1; mask <= 0x8; mask = mask * 2)
	{
		if (sectorMask & mask)
		{
			switch (mask)
			{
			case 1:
				//printf("Draw sector 0\n");
				//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(255,0,0));
				Ft_Esd_CircleLine_Draw_Square(x + 1, y + 1, side);
				break;
			case 2:
				//printf("Draw sector 1\n");
				//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(0,255,0));
				Ft_Esd_CircleLine_Draw_Square(x - side - 2, y + 1, side);
				break;
			case 4:
				//printf("Draw sector 2\n");
				//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(0,0,255));
				Ft_Esd_CircleLine_Draw_Square(x - side - 2, y - side - 2, side);
				break;
			case 8:
				//printf("Draw sector 3\n");
				//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(255,255,0));
				Ft_Esd_CircleLine_Draw_Square(x + 1, y - side - 2, side);
				break;
			}
		}
	}
}
/*
void Ft_Esd_ArcLine_Draw_Sectors(int sectorMask, int x, int y, int side)
{
sectorMask = (sectorMask | (sectorMask >> 4)) & 0xF;

if (sectorMask == 0xF)
return Ft_Esd_CircleLine_Draw_Square( x - side, y - side, side * 2);
int xS[4], yS[4];
for(int i = 0; i < 4; ++i)
{
xS[i] = 0;
yS[i] = 0;
}

side += 1; //include one extra pix for border anti-aliasing effect
int n = 0;
for(int mask = 1; mask <= 0x8 ; mask = mask * 2)
{
if (sectorMask & mask)
{
switch(mask)
{
case 1:
//printf("Draw sector 0\n");
xS[n] = x;
yS[n++] = y;
break;
case 2:
//printf("Draw sector 1\n");
xS[n] = x - side;
yS[n++] = y;
break;
case 4:
//printf("Draw sector 2\n");
xS[n] = x - side;
yS[n++] = y - side;
break;
case 8:
//printf("Draw sector 3\n");
xS[n] = x;
yS[n++] = y - side;
break;
}
}
}
Ft_Esd_ArcLine_Draw_Sharp_Squares(xS, yS, n, side);
}
*/

void Ft_Esd_ArcLine_Draw_Sharp_Squares(int x[], int y[], int count, int side) // No Anti-aliasing at side
{
#if (EVE_MODEL >= EVE_FT810)
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(0));
#endif
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BITMAP_HANDLE(ESD_CO_SCRATCH_HANDLE));
#if (EVE_MODEL >= EVE_FT810)
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BITMAP_LAYOUT_H(0, 0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BITMAP_SIZE_H(0, 0));
#endif
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BITMAP_LAYOUT(L8, 1, 1));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BITMAP_SIZE(NEAREST, REPEAT, REPEAT, side, side));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(BITMAPS));
	//Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, LINE_WIDTH(16);
	for (int i = 0; i < count; i++)
	{
#if (EVE_MODEL >= EVE_FT810)
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x[i], y[i]));
#else
		Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2II(x[i], y[i], 0, 0));
#endif
	}
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
}

void Ft_Esd_ArcLine_Draw_Line(int x0, int y0, int x1, int y1)
{
	/*
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(4));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, LINE_WIDTH(16);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(LINES);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F((x0 * 16), (y0 * 16)));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F((x1 * 16), (y1 * 16)));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
	*/
}

void Ft_Esd_ArcLine_Draw_Arc_Left(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle)
{
#if (EVE_MODEL >= EVE_FT810)
	enum Ft_Esd_ArcLine_SectionType type = Full;
	if (!isClockwise)
	{
		angle = 360 - angle - refAngle;
		if (angle < -180)
		{
			angle += 360;
		}
		//printf("Draw_Arc_Left angle %f\n", angle);
	}

	//if (angle < -180) angle += 360;
	if (angle == 180.0f)
		return;
	ft_float_t dA = angle - 180.0f;

	if ((isClockwise && dA <= 0) || (!isClockwise && dA >= 0))
	{ // No Draw required
		//printf("No draw Draw_Arc_Left.%f\n", dA);
		return;
	}
	else if (isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, 0, 90))
		type = Partial;
	else if (!isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, -90, 0))
		type = Partial;

	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, POINT_SIZE(radius * 16));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(EDGE_STRIP_L));
	//x = x + 1;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));

	ft_float_t dY = 0;
	ft_float_t dX = 0;
	if (type == Full)
	{
		dY = (isClockwise) ? radius : -radius;
	}
	else
	{
		dA = dA * 3.14159265 / 180.0f;
		dY = sin(dA) * (radius);
		dX = cos(dA) * (radius);
	}
	//printf("EDGE_STRIP_L from\n x: %d ; y: %d\n", x, y);
	int x0 = x, y0 = y;
	x = (isClockwise) ? x - (int)dX : x - (int)dX;
	y = (isClockwise) ? y - (int)dY : y - (int)dY + 1;
	//printf("x: %d ; y: %d\n", x, y);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
	//printf("\n", x, y);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(0, 0, 0));
	Ft_Esd_ArcLine_Draw_Line(x0, y0, x, y);
#endif
}

void Ft_Esd_ArcLine_Draw_Arc_Right(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle)
{
#if (EVE_MODEL >= EVE_FT810)
	enum Ft_Esd_ArcLine_SectionType type = Full;
	//printf("Draw_Arc_Right %d isClockwise\n", isClockwise);
	if (!isClockwise)
	{
		angle = 360 - angle - refAngle;
		if (angle < 0)
			angle += 360;
		//printf("Draw_Arc_Right angle %f\n", angle);
	}

	ft_float_t dA = (isClockwise) ? angle - 0 : angle - 360;
	//if (!isClockwise)
	//printf("Da. %f\n", dA);

	if (isClockwise && angle == 0)
		return;
	if (!isClockwise && angle == 360)
		return;

	if ((isClockwise && dA <= 0) || (!isClockwise && dA >= 0))
	{ // No Draw required
		//printf("No draw Draw_Arc_Right. %f\n", dA);
		return;
	}
	else if (isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, 0, 90))
		type = Partial;
	else if (!isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, -90, 0))
		type = Partial;

	//if (!isClockwise)  printf("Da %f\n", dA);
	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, POINT_SIZE(radius * 16));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(EDGE_STRIP_R));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));

	ft_float_t dY = 0;
	ft_float_t dX = 0;
	if (type == Full)
	{
		dY = (isClockwise) ? radius : -radius;
		//printf("full angle\n");
	}
	else
	{
		dA = dA * 3.14159265 / 180.0f;
		dY = sin(dA) * (radius);
		dX = cos(dA) * (radius);
	}
	int x0 = x, y0 = y;
	//printf("EDGE_STRIP_R from\n x: %d ; y: %d to ", x, y);
	x = (isClockwise) ? x + (int)dX : x + (int)dX;
	y = (isClockwise) ? y + (int)dY : y + (int)dY;
	//printf("x: %d ; y: %d\n", x, y);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(0, 0, 0));
	Ft_Esd_ArcLine_Draw_Line(x0, y0, x, y);

	//printf("\n");
#endif
}

void Ft_Esd_ArcLine_Draw_Arc_Above(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle)
{
#if (EVE_MODEL >= EVE_FT810)
	// Refactored. Angle is in the direction
	enum Ft_Esd_ArcLine_SectionType type = Full;
	//if (!isClockwise) angle -= refAngle;

	//LimitAngleRange(&angle, -360, 360);
	if (!isClockwise)
	{
		angle = 360 - angle - refAngle;
		//if (angle < 360) angle += 360;
		//printf("EDGE_STRIP_A angle %f\n", angle);
	}

	if (angle == 270)
		return;
	ft_float_t dA = angle - 270;
	//printf(" dA %f\n", dA);

	if ((isClockwise && dA <= 0) || (!isClockwise && dA >= 0))
	{ // No Draw required
		//printf("No draw Draw_Arc_Above. %f\n", dA);
		return;
	}
	else if (isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, 0, 90))
		type = Partial;
	else if (!isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, -90, 0))
		type = Partial;

	//printf(" clamped dA %f\n", dA);
	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, POINT_SIZE(radius * 16));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(EDGE_STRIP_A));
	//x = (isClockwise) ? x -1: x;
	//y = y + 1;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));

	ft_float_t dY = 0;
	ft_float_t dX = 0;
	if (type == Full)
	{
		dX = (isClockwise) ? radius : -radius;
		//printf("EDGE_STRIP_A Full\n", x, y);
	}
	else
	{
		dA = dA * 3.14159265 / 180.0f;
		dY = cos(dA) * (radius);
		dX = sin(dA) * (radius);
	}
	int x0 = x, y0 = y;
	//printf("EDGE_STRIP_A from\n x: %d ; y: %d\n", x, y);
	x = (isClockwise) ? x + (int)dX + 1 : x + (int)dX - 1;
	y = (isClockwise) ? y - (int)dY : y - (int)dY;
	//printf("x: %d ; y: %d\n", x, y);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(0, 0, 0));
	Ft_Esd_ArcLine_Draw_Line(x0, y0, x, y);
#endif
}

void Ft_Esd_ArcLine_Draw_Arc_Below(ft_float_t angle, int x, int y, int radius, ft_argb32_t color, ft_bool_t isClockwise, ft_float_t refAngle)
{
#if (EVE_MODEL >= EVE_FT810)
	enum Ft_Esd_ArcLine_SectionType type = Full;
	//if (!isClockwise) angle -= refAngle;

	//LimitAngleRange(&angle, -360, 360);
	if (!isClockwise)
	{
		angle = 360 - angle - refAngle;
		//if (angle < 360) angle += 360;
		//printf("EDGE_STRIP_B angle %f\n", angle);
	}
	if (angle == 90)
		return;
	//if (!isClockwise && angle == 270) return;

	//if (angle <= 0) angle += 360; // origin at 90, range 0 - 180. 0 cant be offset to 360 // anti clock wise will fail
	ft_float_t dA = angle - 90;
	//printf(" dA %f\n", dA);
	//ft_float_t dA = angle - 90.0f;

	if ((isClockwise && dA <= 0) || (!isClockwise && dA >= 90))
	{ // No Draw required
		//printf("No draw Draw_Arc_Below. %f\n", dA);
		return;
	}
	else if (isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, 0, 90))
		type = Partial;
	else if (!isClockwise && !Ft_Esd_ArcLine_Clamp_Float(&dA, -90, 0))
		type = Partial;
	//printf(" clamped dA %f\n", dA);

	int a = ((color & 0xFF000000)) >> 24;
	int r = ((color & 0x00FF0000)) >> 16;
	int g = ((color & 0x0000FF00)) >> 8;
	int b = ((color & 0x000000FF));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_A(a));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(r, g, b));

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, POINT_SIZE(radius * 16));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX_FORMAT(0));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, BEGIN(EDGE_STRIP_B));
	//y = y - 1;
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));

	ft_float_t dY = 0;
	ft_float_t dX = 0;
	if (type == Full)
	{
		dX = (isClockwise) ? radius : -radius;
		//printf("EDGE_STRIP_B Full\n", x, y);
	}
	else
	{
		dA = dA * 3.14159265 / 180.0f;
		dY = cos(dA) * (radius);
		dX = sin(dA) * (radius);
	}
	//if (direction != Clockwise)
	//	printf("EDGE_STRIP_B from  x: %d ; y: %d ", x, y);
	//printf("EDGE_STRIP_B direction: %d\n", (int)direction);
	//x = (isClockwise) ? x - (int)dX + 1: x - (int)dX + 3;
	int x0 = x, y0 = y;
	x = (isClockwise) ? x - (int)dX : x - (int)dX + 2;
	//x = (isClockwise) ? x - (int)dX : x - (int)dX + 1;

	y = (isClockwise) ? y + (int)dY : y + (int)dY;
	//if (direction != Clockwise)
	//	printf(" to x: %d ; y: %d\n", x, y);
	//printf("x: %d ; y: %d\n", x, y);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, VERTEX2F(x, y));
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, END());

	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_RGB(0, 0, 0));
	Ft_Esd_ArcLine_Draw_Line(x0, y0, x, y);
#endif
}
