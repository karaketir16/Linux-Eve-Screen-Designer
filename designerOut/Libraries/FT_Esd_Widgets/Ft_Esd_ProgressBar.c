#include "Ft_Esd.h"
#include "Ft_Esd_ProgressBar.h"

#include "Ft_Esd_Math.h"
#include "Ft_Esd_Elements.h"
#include "FT_CoPro_Cmds.h"
#include "Ft_Esd_CoCmd.h"

ESD_METHOD(Ft_Esd_ProgressBar_CoCmd, Context = Ft_Esd_ProgressBar)
ESD_PARAMETER(options, Type = Ft_CoPro_Opt, Default = 0)
ESD_PARAMETER(val, Type = ft_uint16_t, Default = 0)
ESD_PARAMETER(range, Type = ft_uint16_t, Default = 0)
void Ft_Esd_ProgressBar_CoCmd(Ft_Esd_ProgressBar *context, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range)
{
	ft_int16_t x = context->Widget.GlobalX, y = context->Widget.GlobalY;
	ft_int16_t w = context->Widget.GlobalWidth;
	ft_int16_t h = context->Widget.GlobalHeight;
	Ft_Esd_Adjust_Geometry(&x, &y, &w, &h, (h << 2), (w << 2), (h >> 2), (w >> 2));

	if ((context->Widget.GlobalWidth > 1) && (context->Widget.GlobalWidth > 1))
		Ft_Gpu_CoCmd_Progress(Ft_Esd_GetHost(), x, y, w, h, options, val, range);
}
