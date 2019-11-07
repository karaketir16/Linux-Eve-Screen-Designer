

#include "Ft_Esd_Layout_Tag.h"
#include "FT_Esd_Dl.h"

ft_uint8_t Ft_Esd_Layout_Tag_Tag__Default(void *context) { return 255; }

static Ft_Esd_WidgetSlots s_Ft_Esd_Layout_Tag__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_Widget_Update,
	(void (*)(void *))Ft_Esd_Layout_Tag_Render,
	(void (*)(void *))Ft_Esd_Widget_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End,
};

void Ft_Esd_Layout_Tag__Initializer(Ft_Esd_Layout_Tag *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_Layout_Tag_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_Layout_Tag__Slots;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
	context->Tag = Ft_Esd_Layout_Tag_Tag__Default;
}

void Ft_Esd_Layout_Tag_Render(Ft_Esd_Layout_Tag *context)
{
	void *owner = context->Owner;
	Ft_Esd_Dl_TAG(context->Tag(owner));
	Ft_Esd_Widget_Render(context);
	Ft_Esd_Dl_TAG(255);
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_Layout_Tag Instance;
	ft_uint8_t Tag;
} Ft_Esd_Layout_Tag__ESD;

ft_uint8_t Ft_Esd_Layout_Tag__Get_Tag__ESD(void *context) { return ((Ft_Esd_Layout_Tag__ESD *)context)->Tag; }
void Ft_Esd_Layout_Tag__Set_Tag__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Tag__ESD *)context)->Tag = value; }

void *Ft_Esd_Layout_Tag__Create__ESD()
{
	Ft_Esd_Layout_Tag__ESD *context = (Ft_Esd_Layout_Tag__ESD *)malloc(sizeof(Ft_Esd_Layout_Tag__ESD));
	Ft_Esd_Layout_Tag__Initializer(&context->Instance);
	context->Instance.Owner = context;
	context->Tag = 255;
	context->Instance.Tag = Ft_Esd_Layout_Tag__Get_Tag__ESD;
	return context;
}

void Ft_Esd_Layout_Tag__Destroy__ESD(void *context)
{
	free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
