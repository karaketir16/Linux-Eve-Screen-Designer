#include "Ft_Esd.h"
#include "Ft_Esd_TouchArea.h"
#include "Ft_Esd_TouchTag.h"

ESD_METHOD(Ft_Esd_TouchArea_IsInBounds, Context = Ft_Esd_TouchArea, Type = ft_bool_t)
ft_bool_t Ft_Esd_TouchArea_IsInBounds(Ft_Esd_TouchArea *context)
{
	ft_int16_t x = Ft_Esd_TouchTag_TouchX(0);
	int boundX = context->X(context->Owner);
	if (x >= boundX && x < (boundX + context->Width(context->Owner)))
	{
		ft_int16_t y = Ft_Esd_TouchTag_TouchY(0);
		int boundY = context->Y(context->Owner);
		if (y > boundY && y < (boundY + context->Height(context->Owner)))
		{
			return 1;
		}
	}
	return 0;
}

ESD_METHOD(Ft_Esd_TouchArea_Update_TouchTag, Context = Ft_Esd_TouchArea)
void Ft_Esd_TouchArea_Update_TouchTag(Ft_Esd_TouchArea *context)
{
	Ft_Esd_TouchTag_Update(0);
}
