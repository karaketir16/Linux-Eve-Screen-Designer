#include "Ft_Esd_Layout_Display.h"

static Ft_Esd_WidgetSlots s_Ft_Esd_Layout_Display__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_Layout_Display_Update,
	(void (*)(void *))Ft_Esd_Layout_Display_Render,
	(void (*)(void *))Ft_Esd_Layout_Display_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End,
};

void Ft_Esd_Layout_Display__Initializer(Ft_Esd_Layout_Display *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_Layout_Display_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_Layout_Display__Slots;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
}

void Ft_Esd_Layout_Display_Update(Ft_Esd_Layout_Display *context)
{
	if (context->Widget.Recalculate)
	{
		context->Widget.Recalculate = 0;
		Ft_Esd_Widget *child = context->Widget.First;
		while (child)
		{
			Ft_Esd_Widget *const next = child->Next;
			Ft_Esd_Widget_SetGlobalRect(child,
			    0, 0, Ft_Esd_Host->Parameters.Display.Width, Ft_Esd_Host->Parameters.Display.Height);
			child = next;
		}
	}
	Ft_Esd_Widget_IterateChildActiveSlotReverse((Ft_Esd_Widget *)context, FT_ESD_WIDGET_UPDATE);
}

void Ft_Esd_Layout_Display_Render(Ft_Esd_Layout_Display *context)
{
	Ft_Esd_Widget_IterateChildActiveValidSlotReverse((Ft_Esd_Widget *)context, FT_ESD_WIDGET_RENDER);
}

void Ft_Esd_Layout_Display_Idle(Ft_Esd_Layout_Display *context)
{
	Ft_Esd_Widget_IterateChildActiveSlotReverse((Ft_Esd_Widget *)context, FT_ESD_WIDGET_IDLE);
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_Layout_Display Instance;
} Ft_Esd_Layout_Display__ESD;

void *Ft_Esd_Layout_Display__Create__ESD()
{
	Ft_Esd_Layout_Display__ESD *context = (Ft_Esd_Layout_Display__ESD *)malloc(sizeof(Ft_Esd_Layout_Display__ESD));
	Ft_Esd_Layout_Display__Initializer(&context->Instance);
	context->Instance.Owner = context;
	return context;
}

void Ft_Esd_Layout_Display__Destroy__ESD(void *context)
{
	free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
