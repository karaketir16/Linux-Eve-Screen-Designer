#include "Ft_Esd.h"
#include "Ft_Esd_Widget.h"
#include "FT_Esd_Dl.h"
#include "Ft_Esd_TouchTag.h"
#include "Ft_Esd_TouchArea.h"
#include "Ft_Esd_ScrollPanel.h"

#include <stdlib.h>

extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;

#define FT_ESD_SCROLLPANEL_SCISSOR 2

ESD_METHOD(Ft_Esd_ScrollPanel_Update_Scroll, Context = Ft_Esd_ScrollPanel)
void Ft_Esd_ScrollPanel_Update_Scroll(Ft_Esd_ScrollPanel *context)
{
	if (context->Touching)
	{
		ft_int16_t touchX = context->TouchX;
		ft_int16_t touchY = context->TouchY;
		ft_int16_t diffX = touchX - context->PreviousTouchX;
		ft_int16_t diffY = touchY - context->PreviousTouchY;

		context->ScrollX = context->PreviousScrollX - diffX;
		context->ScrollY = context->PreviousScrollY - diffY;
		context->ScrollXChanged(context->Owner, context->ScrollX);
		context->ScrollYChanged(context->Owner, context->ScrollY);
		if (diffY > 5 || diffY < -5 || diffX > 5 || diffX < -5)
		{
			// Cancel any ongoing button presses
			Ft_Esd_TouchTag_SuppressCurrentTags();
		}
	}
}

/* end of file */
