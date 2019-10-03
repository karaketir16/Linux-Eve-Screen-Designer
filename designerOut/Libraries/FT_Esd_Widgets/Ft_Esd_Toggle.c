#include "Ft_Esd.h"
#include "Ft_Esd_Toggle.h"

#include "FT_Esd_Dl.h"
#include "FT_Esd_Primitives.h"

extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;

ft_int16_t Ft_Esd_Toggle_RealHeight(Ft_Esd_Toggle *context)
{
	void *owner = context->Owner;
	ft_int16_t i_4 = context->Font(owner);
	ft_uint16_t left_17 = Ft_Esd_GetFontHeight(i_4);
	ft_int16_t right_16 = 5;
	ft_int16_t left_16 = left_17 * right_16;
	ft_int16_t right_17 = 4;
	return left_16 / right_17;
}

ESD_METHOD(Ft_Esd_Toggle_Render_CoCmd, Context = Ft_Esd_Toggle)
ESD_PARAMETER(tag, Type = ft_uint8_t, Default = 255)
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0) // SCREEN_SIZE
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0) // SCREEN_SIZE
ESD_PARAMETER(width, Type = ft_int16_t, Default = 60) // SCREEN_SIZE
ESD_PARAMETER(font, Type = ft_int16_t, Default = 21, Min = 0, Max = 31) // BITMAP_HANDLE
ESD_PARAMETER(options, Type = Ft_CoPro_Opt, Default = 0) // TODO: Option flags / Bit enum?
ESD_PARAMETER(state, Type = ft_uint16_t, Default = 0)
ESD_PARAMETER(s, Type = const ft_char8_t *, Default = "on\x00off")
void Ft_Esd_Toggle_Render_CoCmd(Ft_Esd_Toggle *context, ft_uint8_t tag, ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t font, ft_uint16_t options, ft_uint16_t state, const ft_char8_t *s)
{
#if (EVE_MODEL < EVE_BT815)
	// Fix for tag issue pre-BT815
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, TAG_MASK(0));
	Ft_Gpu_CoCmd_Toggle(Ft_Esd_Host, x, y, w, font, options, state, s);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, TAG_MASK(1));
	Ft_Esd_Dl_TAG(tag);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(0, 0, 0, 0));
	ft_int16_t realY = context->Widget.GlobalY + (context->Widget.GlobalHeight >> 1);
	Ft_Esd_Render_Line(x, realY, x + w, realY, Ft_Esd_Toggle_RealHeight(context), 0xffffffff);
	Ft_Gpu_CoCmd_SendCmd(Ft_Esd_Host, COLOR_MASK(1, 1, 1, 1));
	Ft_Esd_Dl_TAG(255);
#else
	// Just use TAG on Toggle itself
	Ft_Esd_Dl_TAG(tag);
	Ft_Gpu_CoCmd_Toggle(Ft_Esd_Host, x, y, w, font, options, state, s);
	Ft_Esd_Dl_TAG(255);
#endif
}
