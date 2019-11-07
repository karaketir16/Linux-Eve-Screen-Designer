#include "Ft_Esd.h"
#include "Ft_Esd_LabelButton.h"

ESD_METHOD(Ft_Esd_LabelButton_GetOptionUpdatePosition, Context = Ft_Esd_LabelButton, Type = Ft_CoPro_Opt)
ft_uint16_t Ft_Esd_LabelButton_GetOptionUpdatePosition(Ft_Esd_LabelButton *context)
{
	ft_uint16_t op = 0;
	void *owner = context->Owner;

	ft_uint16_t hOps = context->AlignX(owner);
	ft_bool_t isHRight = hOps & OPT_ALIGN_RIGHT;
	ft_bool_t isHCenter = hOps & OPT_ALIGN_CENTER;
	ft_bool_t isHLeft = !isHRight & !isHCenter;

	if (isHLeft)
	{
		context->X = context->Widget.GlobalX;
	}
	else if (isHRight)
	{
		context->X = context->Widget.GlobalX + context->Widget.GlobalWidth;
		op |= OPT_RIGHTX;
	}
	else //if (isHCenter)
	{
		context->X = context->Widget.GlobalX + ((context->Widget.GlobalWidth + 1) >> 1);
		op |= OPT_CENTERX;
	}

	ft_uint16_t vOps = context->AlignY(owner);
	ft_bool_t isVBottom = vOps & OPT_ALIGN_BOTTOM;
	ft_bool_t isVCenter = vOps & OPT_ALIGN_CENTER;
	ft_bool_t isVTop = !isVBottom & !isVCenter;
	if (isVTop)
	{
		context->Y = context->Widget.GlobalY;
	}
	else if (isVBottom)
	{
		context->Y = context->Widget.GlobalY + context->Widget.GlobalHeight - context->FontHeight;
	}
	else //if (isVCenter)
	{
		context->Y = context->Widget.GlobalY + ((context->Widget.GlobalHeight + 1) >> 1);
		op |= OPT_CENTERY;
	}

	return op;
}
