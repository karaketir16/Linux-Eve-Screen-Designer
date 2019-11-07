#include "Ft_Esd_TouchTagRenderInterface.h"

#include "FT_Esd_Dl.h"

extern void Ft_Esd_Noop(void *context);

static Ft_Esd_WidgetSlots s_Ft_Esd_TouchTagRenderInterface__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_TouchTagRenderInterface_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_TouchTagRenderInterface_Update,
	(void (*)(void *))Ft_Esd_TouchTagRenderInterface_Render,
	(void (*)(void *))Ft_Esd_Widget_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_TouchTagRenderInterface_End,
};

static void Ft_Esd_TouchTagRenderInterface_Touch_Tag_Down__Signal(void *context);
static void Ft_Esd_TouchTagRenderInterface_Touch_Tag_Up__Signal(void *context);
static void Ft_Esd_TouchTagRenderInterface_Touch_Tag_Tap__Signal(void *context);

void Ft_Esd_TouchTagRenderInterface__Touch_Tag__Initializer(Ft_Esd_TouchTagRenderInterface *context)
{
	Ft_Esd_TouchTag *object = &context->Touch_Tag;
	Ft_Esd_TouchTag__Initializer(object);
	object->Owner = context;
	object->Down = Ft_Esd_TouchTagRenderInterface_Touch_Tag_Down__Signal;
	object->Up = Ft_Esd_TouchTagRenderInterface_Touch_Tag_Up__Signal;
	object->Tap = Ft_Esd_TouchTagRenderInterface_Touch_Tag_Tap__Signal;
}

void Ft_Esd_TouchTagRenderInterface__Initializer(Ft_Esd_TouchTagRenderInterface *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_TouchTagRenderInterface_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_TouchTagRenderInterface__Slots;
	context->Widget.LocalX = 0;
	context->Widget.LocalY = 0;
	context->Widget.LocalWidth = 400;
	context->Widget.LocalHeight = 300;
	context->Rendering = Ft_Esd_Noop;
	context->Up = Ft_Esd_Noop;
	context->Down = Ft_Esd_Noop;
	context->Tap = Ft_Esd_Noop;
	Ft_Esd_TouchTagRenderInterface__Touch_Tag__Initializer(context);
}

void Ft_Esd_TouchTagRenderInterface_Start(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	Ft_Esd_Widget_Start((Ft_Esd_Widget *)context);
	Ft_Esd_TouchTag_Start(&context->Touch_Tag);
}

void Ft_Esd_TouchTagRenderInterface_Update(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	Ft_Esd_TouchTag_Update(&context->Touch_Tag);
	Ft_Esd_Widget_Update((Ft_Esd_Widget *)context);
}

void Ft_Esd_TouchTagRenderInterface_End(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	Ft_Esd_TouchTag_End(&context->Touch_Tag);
	Ft_Esd_Widget_End((Ft_Esd_Widget *)context);
}

void Ft_Esd_TouchTagRenderInterface_Render(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	ft_uint8_t s = Ft_Esd_TouchTag_Tag(&context->Touch_Tag);
	Ft_Esd_Dl_TAG(s);
	context->Rendering(owner);
	ft_uint8_t s_1 = 255;
	Ft_Esd_Dl_TAG(s_1);
	Ft_Esd_Widget_Render((Ft_Esd_Widget *)context);
}

ft_bool_t Ft_Esd_TouchTagRenderInterface_Touching(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_Touching(&context->Touch_Tag);
}

ft_bool_t Ft_Esd_TouchTagRenderInterface_Inside(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_Inside(&context->Touch_Tag);
}

int Ft_Esd_TouchTagRenderInterface_TouchX(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_TouchX(&context->Touch_Tag);
}

int Ft_Esd_TouchTagRenderInterface_TouchY(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_TouchY(&context->Touch_Tag);
}

ft_int16_t Ft_Esd_TouchTagRenderInterface_TouchXDelta(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_TouchXDelta(&context->Touch_Tag);
}

ft_int16_t Ft_Esd_TouchTagRenderInterface_TouchYDelta(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_TouchYDelta(&context->Touch_Tag);
}

int Ft_Esd_TouchTagRenderInterface_Tag(Ft_Esd_TouchTagRenderInterface *context)
{
	void *owner = context->Owner;
	return Ft_Esd_TouchTag_Tag(&context->Touch_Tag);
}

void Ft_Esd_TouchTagRenderInterface_Touch_Tag_Down__Signal(void *c)
{
	Ft_Esd_TouchTagRenderInterface *context = (Ft_Esd_TouchTagRenderInterface *)c;
	void *owner = context->Owner;
	context->Down(owner);
}

void Ft_Esd_TouchTagRenderInterface_Touch_Tag_Up__Signal(void *c)
{
	Ft_Esd_TouchTagRenderInterface *context = (Ft_Esd_TouchTagRenderInterface *)c;
	void *owner = context->Owner;
	context->Up(owner);
}

void Ft_Esd_TouchTagRenderInterface_Touch_Tag_Tap__Signal(void *c)
{
	Ft_Esd_TouchTagRenderInterface *context = (Ft_Esd_TouchTagRenderInterface *)c;
	void *owner = context->Owner;
	context->Tap(owner);
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_TouchTagRenderInterface Instance;
} Ft_Esd_TouchTagRenderInterface__ESD;

void *Ft_Esd_TouchTagRenderInterface__Create__ESD()
{
	Ft_Esd_TouchTagRenderInterface__ESD *context = (Ft_Esd_TouchTagRenderInterface__ESD *)malloc(sizeof(Ft_Esd_TouchTagRenderInterface__ESD));
	Ft_Esd_TouchTagRenderInterface__Initializer(&context->Instance);
	context->Instance.Owner = context;
	return context;
}

void Ft_Esd_TouchTagRenderInterface__Destroy__ESD(void *context)
{
	free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
