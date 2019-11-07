/*
Ft_Esd_LinearRoller
C Source
*/
#include "Ft_Esd_LinearRoller.h"
#include "Ft_Esd_Theme.h"
#include "FT_Esd_Dl.h"
#include "FT_Esd_Primitives.h"
extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;

void Ft_Esd_LinearRoller_DrawHorizontalRoller(Ft_Esd_LinearRoller *context);
void Ft_Esd_LinearRoller_DrawVerticalRoller(Ft_Esd_LinearRoller *context);

ESD_METHOD(Ft_Esd_LinearRoller_GetGradientColor, Type = ft_argb32_t)
ESD_PARAMETER(colorA, Type = ft_argb32_t, Default = 0xFFFFFFFF)
ESD_PARAMETER(colorB, Type = ft_argb32_t, Default = 0x1F1F1F1F)
ESD_PARAMETER(scale, Type = ft_float_t, Default = 1)
ft_argb32_t Ft_Esd_LinearRoller_GetGradientColor(ft_argb32_t colorA, ft_argb32_t colorB, ft_float_t scale)
{
	//printf("c1: %d,  c2:%d,   scale: %f\n", colorA, colorB,scale);
	if (colorA == colorB)
		return colorA;
	if (scale < 0)
		return colorA;
	else if (scale > 1)
		return colorB;

	int a0 = ((colorA & 0xFF000000)) >> 24;
	int r0 = ((colorA & 0x00FF0000)) >> 16;
	int g0 = ((colorA & 0x0000FF00)) >> 8;
	int b0 = ((colorA & 0x000000FF));

	int a1 = ((colorB & 0xFF000000)) >> 24;
	int r1 = ((colorB & 0x00FF0000)) >> 16;
	int g1 = ((colorB & 0x0000FF00)) >> 8;
	int b1 = ((colorB & 0x000000FF));

	ft_float_t oppScale = 1.0f - scale;
	int a = (int)(a0 * oppScale) + (int)(a1 * scale);
	int r = (int)(r0 * oppScale) + (int)(r1 * scale);
	int g = (int)(g0 * oppScale) + (int)(g1 * scale);
	int b = (int)(b0 * oppScale) + (int)(b1 * scale);
	if (a > 255)
		a = 255;
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;
	//printf("argb0: %d,%d,%d,%d\n", a,r,g,b);

	ft_argb32_t color = (((((a << 8) + r) << 8) + g) << 8) + b;

	//printf("argb: %d,%d,%d,%d\n", a,r,g,b);
	return color;
}

ESD_METHOD(Ft_Esd_LinearRoller_Draw, Context = Ft_Esd_LinearRoller)
void Ft_Esd_LinearRoller_Draw(Ft_Esd_LinearRoller *context)
{
	Esd_Dl_SAVE_CONTEXT();
	Ft_Esd_Rect16 scissor = Esd_Dl_Scissor_Set(context->Widget.GlobalRect);
	if (context->Horizontal(context))
	{
		Ft_Esd_LinearRoller_DrawHorizontalRoller(context);
	}
	else
	{
		Ft_Esd_LinearRoller_DrawVerticalRoller(context);
	}
	Esd_Dl_Scissor_Reset(scissor);
	Esd_Dl_RESTORE_CONTEXT();
}

void Ft_Esd_LinearRoller_DrawHorizontalRoller(Ft_Esd_LinearRoller *context)
{
	Ft_Esd_Widget *owner = context->Owner;
	int width = context->Widget.GlobalWidth * 256;
	if (width < 1)
		return;

	int height = context->Widget.GlobalHeight;
	if (height < 1)
		return;

	int gx = context->Widget.GlobalX;
	int gy = context->Widget.GlobalY;
	int gyc = gy + (height / 2);

	int length = context->Length(owner) << 8;
	int major = context->Major;
	int minor = context->Minor;

	context->Tick = ((double)context->Widget.GlobalWidth) / context->Length(owner);
	ft_float_t tick = (context->Tick * 16.0f);

	int max = length >> 8;
	int half = max >> 1;
	int minTick = context->MinValue(owner); // min
	int maxTick = context->MaxValue(owner); // max
	ft_bool_t isTrim = context->IsTrimToRange(owner);
	int minDisplayTick = (isTrim) ? minTick : minTick - half;
	int maxDisplayTick = (isTrim) ? maxTick : maxTick + half;
	int range = maxDisplayTick - minDisplayTick;

	int tickStart = context->StartValue + minTick;
	int tickEnd = tickStart + max;
	context->centerValue = (tickStart + tickEnd + 1) >> 1;
	context->maxValue = tickStart;
	context->minValue = tickEnd;

	ft_bool_t displayTick = context->IsDisplayTick(owner);
	ft_bool_t displayMinor = context->IsDisplayMinor(owner);

	//printf("minTick: %d, maxTick: %d\n minDisplayTick: %d, maxDisplayTick: %d\n\n", minTick, maxTick, minDisplayTick, maxDisplayTick);
	//printf("tickStart: %d, tickEnd: %d, center: %d\n\n", tickStart, tickEnd, context->centerValue);

	if (context->IsDisplayAxis(owner))
	{
		//printf("0 Axis line: %d %d , %d , %d\n",gx, gyc, gx + context->Widget.GlobalWidth, gyc, gyc);
		int x0 = (gx * 16);
		int x1 = x0 + (context->Widget.GlobalWidth * 16);
		int y0 = gyc * 16;
		int y1 = y0;
		//printf("Axis line: %d %d , %d , %d\n",x0, y0, x1, y1);
		Ft_Esd_Render_LineF(x0, y0, min(x1, 16383), y1, 4, context->ColorAxis);
	}

	for (int tickValue = tickStart; tickValue <= tickEnd; tickValue++)
	{
		int lineLength = height;
		int lineWidth = 0;
		ft_bool_t isMajor = 0;
		ft_bool_t isNeedTrim = context->IsTrimToRange(owner);

		if (tickValue % major == 0) // Draw major
		{
			lineLength *= context->MajorLength(owner);
			lineWidth = context->MajorWidth(owner);
			isMajor = FT_TRUE;
		}
		else if (displayMinor && (tickValue % minor == 0)) // Draw minor
		{
			lineLength *= context->MinorLength(owner);
			lineWidth = context->MinorWidth(owner);
		}
		else if (displayTick) // Draw tick
		{
			lineLength *= context->TickLength(owner);
			lineWidth = context->TickWidth(owner);
		}

		if (lineWidth > 0)
		{
			isNeedTrim = isTrim && (tickValue < minDisplayTick || tickValue > maxDisplayTick);
			ft_bool_t showTrim = context->ShowTrimRange(owner);
			ft_argb32_t color = Ft_Esd_LinearRoller_GetGradientColor(context->ColorMin, context->ColorMax, ((ft_float_t)tickValue - minTick) / range);
			if (isNeedTrim && !showTrim)
			{
				//printf("trimmed %d\n", tickValue);
				continue;
			}
			else if (isNeedTrim && showTrim)
			{
				color = Ft_Esd_LinearRoller_GetGradientColor(color, 0xFF000000, 0.85f);
			}

			int x = ((tickValue - tickStart) * tick);
			//printf("Line: %d, %d. %d\n", lineLength, lineWidth, x);
			int x0 = (gx * 16) + (x);
			int x1 = x0;
			int halfLineLength = lineLength >> 1;
			int y0 = (gyc + halfLineLength) * 16;
			int y1 = (gyc - halfLineLength) * 16;
			//printf("%d, %d to %d, %d\n", x0, y0, x1, y1);
			Ft_Esd_Render_LineF(x0, y0, x1, y1, lineWidth << 3, color);
			if (isMajor && !isNeedTrim)
			{
				Ft_Esd_Dl_COLOR_A((color & 0xFF000000) >> 24);
				Ft_Esd_Dl_COLOR_RGB(color);
				ft_uint8_t font = Ft_Esd_Dl_RomFont_Setup(context->Font);
				int padding = context->LabelPadding(owner);
				Ft_Esd_Cmd_Number(x0 / 16, (y0 / 16) + padding, font, OPT_SIGNED | OPT_CENTERX, tickValue);
				//printf("tick : %d\n", tickValue);
			}
		}
	}

	//Draw center line
	{
		int x0 = (gx * 16) + (width / 32);
		int x1 = x0;
		int height2 = height * 16 * context->MajorLength(owner) + 48;
		int y0 = gyc * 16 - height2 / 2;
		int y1 = y0 + height2;
		int width = context->MajorWidth(owner) + 2;
		int tickValue = context->centerValue;
		context->centerColor = Ft_Esd_LinearRoller_GetGradientColor(context->ColorMin, context->ColorMax, ((ft_float_t)tickValue - minTick) / range);
		Ft_Esd_Render_LineF(x0, y0, x1, y1, width << 3, context->centerColor);
	}
}

void Ft_Esd_LinearRoller_DrawVerticalRoller(Ft_Esd_LinearRoller *context)
{
	Ft_Esd_Widget *owner = context->Owner;
	int width = context->Widget.GlobalWidth;
	if (width < 1)
		return;

	int height = context->Widget.GlobalHeight * 256;
	if (height < 1)
		return;

	int gx = context->Widget.GlobalX;
	int gy = context->Widget.GlobalY;
	int gxc = gx + (width / 2);

	int length = context->Length(owner) << 8;
	int major = context->Major;
	int minor = context->Minor;

	context->Tick = ((double)height) / length;
	ft_float_t tick = (context->Tick * 16.0f);

	int max = length >> 8;
	int half = max >> 1;
	int minTick = context->MinValue(owner); // min
	int maxTick = context->MaxValue(owner); // max
	ft_bool_t isTrim = context->IsTrimToRange(owner);
	int minDisplayTick = (isTrim) ? minTick : minTick - half;
	int maxDisplayTick = (isTrim) ? maxTick : maxTick + half;
	int range = maxDisplayTick - minDisplayTick;

	int tickStart = context->StartValue + minTick;
	int tickEnd = tickStart + max;
	context->centerValue = (tickStart + tickEnd + 1) >> 1;
	context->maxValue = tickStart;
	context->minValue = tickEnd;

	ft_bool_t displayTick = context->IsDisplayTick(owner);
	ft_bool_t displayMinor = context->IsDisplayMinor(owner);

	//printf("minTick: %d, maxTick: %d\n minDisplayTick: %d, maxDisplayTick: %d\n\n", minTick, maxTick, minDisplayTick, maxDisplayTick);
	//printf("tickStart: %d, tickEnd: %d\n\n", tickStart, tickEnd);

	if (context->IsDisplayAxis(owner))
	{
		int y0 = (gy * 16);
		int y1 = y0 + (context->Widget.GlobalHeight * 16);
		int x0 = gxc * 16;
		int x1 = gxc * 16;
		Ft_Esd_Render_LineF(x0, y0, x1, min(y1, 16383), 4, context->ColorAxis);
	}

	for (int tickValue = tickStart; tickValue <= tickEnd; tickValue++)
	{
		int lineLength = width;
		int lineWidth = 0;
		ft_bool_t isMajor = 0;
		ft_bool_t isNeedTrim = context->IsTrimToRange(owner);

		if (tickValue % major == 0) // Draw major
		{
			lineLength *= context->MajorLength(owner);
			lineWidth = context->MajorWidth(owner);
			isMajor = FT_TRUE;
		}
		else if (displayMinor && (tickValue % minor == 0)) // Draw minor
		{
			lineLength *= context->MinorLength(owner);
			lineWidth = context->MinorWidth(owner);
		}
		else if (displayTick) // Draw tick
		{
			lineLength *= context->TickLength(owner);
			lineWidth = context->TickWidth(owner);
		}

		if (lineWidth > 0)
		{
			isNeedTrim = isTrim && (tickValue < minDisplayTick || tickValue > maxDisplayTick);
			ft_bool_t showTrim = context->ShowTrimRange(owner);
			ft_argb32_t color = Ft_Esd_LinearRoller_GetGradientColor(context->ColorMin, context->ColorMax, ((ft_float_t)tickValue - minTick) / range);
			if (isNeedTrim && !showTrim)
			{
				//printf("trimmed %d\n", tickValue);
				continue;
			}
			else if (isNeedTrim && showTrim)
			{
				color = Ft_Esd_LinearRoller_GetGradientColor(color, 0xFF000000, 0.85f);
			}

			int y = ((tickValue - tickStart) * tick);
			//printf("Line: %d, %d. %d\n", lineLength, lineWidth, x);
			int y0 = (gy * 16) + (y);
			int y1 = y0;
			int halfLineLength = lineLength >> 1;
			int x0 = (gxc + halfLineLength) * 16;
			int x1 = (gxc - halfLineLength) * 16;
			//printf("%d, %d to %d, %d\n", x0, y0, x1, y1);
			Ft_Esd_Render_LineF(x0, y0, x1, y1, lineWidth << 3, color);
			if (isMajor && !isNeedTrim)
			{
				Ft_Esd_Dl_COLOR_A((color & 0xFF000000) >> 24);
				Ft_Esd_Dl_COLOR_RGB(color);
				ft_uint8_t font = Ft_Esd_Dl_RomFont_Setup(context->Font);
				int padding = context->LabelPadding(owner);
				Ft_Esd_Cmd_Number(x1 / 16 - padding, (y1 / 16), font, OPT_SIGNED | OPT_CENTERY, tickValue);
				//printf("tick : %d\n", tickValue);
			}
		}
	}

	//Draw center line
	{
		int y0 = (gy * 16) + (height / 32);
		int y1 = y0;
		int width2 = width * 16 * context->MajorLength(owner) + 48;
		int x0 = gxc * 16 - width2 / 2;
		int x1 = x0 + width2;
		int width = context->MajorWidth(owner) + 2;
		int tickValue = context->centerValue;
		context->centerColor = Ft_Esd_LinearRoller_GetGradientColor(context->ColorMin, context->ColorMax, ((ft_float_t)tickValue - minTick) / range);
		Ft_Esd_Render_LineF(x0, y0, x1, y1, width << 3, context->centerColor);
	}
}
