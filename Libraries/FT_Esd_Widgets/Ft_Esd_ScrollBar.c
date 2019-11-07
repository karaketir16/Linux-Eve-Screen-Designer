#include "Ft_Esd.h"
#include "Ft_Esd_ScrollBar.h"

#include "Ft_Esd_Math.h"
#include "Ft_Esd_Elements.h"
#include "FT_CoPro_Cmds.h"
#include "Ft_Esd_CoCmd.h"

void Ft_Esd_ScrollBar_Geometry(ft_int16_t *x, ft_int16_t *y, ft_int16_t *w, ft_int16_t *h)
{
	Ft_Esd_Adjust_Geometry(x, y, w, h, (*h << 2), (*w << 2), (*h >> 2), (*w >> 2));
}

ESD_METHOD(Ft_Esd_ScrollBar_Update_Signal, Context = Ft_Esd_ScrollBar)
void Ft_Esd_ScrollBar_Update_Signal(Ft_Esd_ScrollBar *context)
{
	if (Ft_Esd_TouchTag_Touching(&context->TouchTag))
	{
		ft_int16_t x = context->Widget.GlobalX, y = context->Widget.GlobalY;
		ft_int16_t w = context->Widget.GlobalWidth;
		ft_int16_t h = context->Widget.GlobalHeight;
		Ft_Esd_ScrollBar_Geometry(&x, &y, &w, &h);

		ft_int16_t touchX = Ft_Esd_TouchTag_TouchX(&context->TouchTag) - context->Widget.GlobalX;
		ft_int16_t touchY = Ft_Esd_TouchTag_TouchY(&context->TouchTag) - context->Widget.GlobalY;
		ft_int16_t minRange = context->Min(context->Owner);
		ft_int16_t maxRange = context->Max(context->Owner);
		ft_int16_t range = maxRange - minRange;
		ft_bool_t isHorizontal = w >= h;
		ft_int32_t width = ((isHorizontal) ? w : h);
		ft_int32_t touchDelta = (isHorizontal) ? touchX - context->PreviousX : touchY - context->PreviousY;
		ft_int32_t valueDelta = (touchDelta * (range + context->Size(context->Owner))) / width;
		ft_int32_t newValue = context->PreviousValue + valueDelta;

		context->Value = Ft_Esd_Int32_ClampedValue(newValue, minRange, maxRange);
		context->Changed(context->Owner, context->Value);
	}
}

ESD_METHOD(Ft_Esd_ScrollBar_CoCmd, Context = Ft_Esd_ScrollBar)
ESD_PARAMETER(options, Type = Ft_CoPro_Opt, Default = 0)
ESD_PARAMETER(val, Type = ft_uint16_t, Default = 0)
ESD_PARAMETER(size, Type = ft_uint16_t, Default = 0)
ESD_PARAMETER(range, Type = ft_uint16_t, Default = 0)
void Ft_Esd_ScrollBar_CoCmd(Ft_Esd_ScrollBar *context, ft_uint16_t options, ft_uint16_t val, ft_uint16_t size, ft_uint16_t range)
{
	ft_int16_t x = context->Widget.GlobalX, y = context->Widget.GlobalY;
	ft_int16_t w = context->Widget.GlobalWidth;
	ft_int16_t h = context->Widget.GlobalHeight;
	Ft_Esd_ScrollBar_Geometry(&x, &y, &w, &h);

	if (w > 0 && h > 0)
	{
		if (!range)
		{
			range = 1;
			size = 1;
		}

		Ft_Gpu_CoCmd_Scrollbar(Ft_Esd_GetHost(),
		    x, y, w, h, options, val, size, range);
	}
}
