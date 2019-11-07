#include "Ft_Esd_Layout_Fill.h"

static Ft_Esd_WidgetSlots s_Ft_Esd_Layout_Fill__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_Widget_Update,
	(void (*)(void *))Ft_Esd_Widget_Render,
	(void (*)(void *))Ft_Esd_Widget_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End,
};

void Ft_Esd_Layout_Fill__Initializer(Ft_Esd_Layout_Fill *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_Layout_Fill_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_Layout_Fill__Slots;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_Layout_Fill Instance;
} Ft_Esd_Layout_Fill__ESD;

void *Ft_Esd_Layout_Fill__Create__ESD()
{
	Ft_Esd_Layout_Fill__ESD *context = (Ft_Esd_Layout_Fill__ESD *)malloc(sizeof(Ft_Esd_Layout_Fill__ESD));
	Ft_Esd_Layout_Fill__Initializer(&context->Instance);
	context->Instance.Owner = context;
	return context;
}

void Ft_Esd_Layout_Fill__Destroy__ESD(void *context)
{
	free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
