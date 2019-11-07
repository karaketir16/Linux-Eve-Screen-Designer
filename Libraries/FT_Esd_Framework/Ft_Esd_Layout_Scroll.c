#include "Ft_Esd_Layout_Scroll.h"
#include "FT_Esd_Dl.h"

static Ft_Esd_WidgetSlots s_Ft_Esd_Layout_Scroll__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_Layout_Scroll_Update,
	(void (*)(void *))Ft_Esd_Layout_Scroll_Render,
	(void (*)(void *))Ft_Esd_Layout_Scroll_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End,
};

void Ft_Esd_Layout_Scroll__Initializer(Ft_Esd_Layout_Scroll *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_Layout_Scroll_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_Layout_Scroll__Slots;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
	context->MinX = 0;
	context->MaxX = 0;
	context->MinY = 0;
	context->MaxY = 0;
	context->ScrollX = 0;
	context->ScrollY = 0;
	context->Align = ESD_ALIGN_TOPFILL;
	context->Scissor = FT_FALSE;
	context->ScrollXChanged = (void *)Ft_Esd_Noop;
	context->ScrollYChanged = (void *)Ft_Esd_Noop;
	context->LastValues = context->Values;
}

void Ft_Esd_Layout_Scroll_Recalculate(Ft_Esd_Layout_Scroll *context)
{
	Ft_Esd_Widget *child;
	child = context->Widget.First;
	ft_int16_t maxWidth = 0, maxHeight = 0;
	while (child)
	{
		if (child->Active)
		{
			if (child->LocalWidth > maxWidth)
				maxWidth = child->LocalWidth;
			if (child->LocalHeight > maxHeight)
				maxHeight = child->LocalHeight;
		}
		child = child->Next;
	}
	ft_int16_t globalX = context->Widget.GlobalX;
	ft_int16_t globalY = context->Widget.GlobalY;
	ft_int16_t globalWidth = context->Widget.GlobalWidth;
	ft_int16_t globalHeight = context->Widget.GlobalHeight;
	ft_uint8_t alignX = ESD_ALIGN_HORIZONTAL(context->Align);
	ft_uint8_t alignY = ESD_ALIGN_VERTICAL(context->Align);
	ft_int16_t scrollableX = maxWidth - globalWidth;
	ft_int16_t scrollableY = maxHeight - globalHeight;
	if (scrollableX > 0)
	{
		switch (alignX)
		{
		case ESD_ALIGN_LEFTX:
			context->MinX = 0;
			context->MaxX = scrollableX;
			break;
		case ESD_ALIGN_CENTERX:
			context->MinX = -(scrollableX >> 1);
			context->MaxX = context->MinX + scrollableX;
			break;
		case ESD_ALIGN_RIGHTX:
			context->MinX = -scrollableX;
			context->MaxX = 0;
			break;
		case ESD_ALIGN_FILLX:
			context->MinX = 0;
			context->MaxX = 0;
			break;
		}
	}
	else
	{
		context->MinX = 0;
		context->MaxX = 0;
	}
	if (scrollableY > 0)
	{
		switch (alignY)
		{
		case ESD_ALIGN_TOPY:
			context->MinY = 0;
			context->MaxY = scrollableY;
			break;
		case ESD_ALIGN_CENTERY:
			context->MinY = -(scrollableY >> 1);
			context->MaxY = context->MinY + scrollableY;
			break;
		case ESD_ALIGN_BOTTOMY:
			context->MinY = -scrollableY;
			context->MaxY = 0;
			break;
		case ESD_ALIGN_FILLY:
			context->MinY = 0;
			context->MaxY = 0;
			break;
		}
	}
	else
	{
		context->MinY = 0;
		context->MaxY = 0;
	}
	child = context->Widget.First;
	ft_int16_t scrollX = min(max(context->MinX, context->ScrollX), context->MaxX);
	ft_int16_t scrollY = min(max(context->MinY, context->ScrollY), context->MaxY);
	context->ScrollX = scrollX;
	context->ScrollY = scrollY;
	while (child)
	{
		if (child->Active)
		{
			ft_int16_t childX, childY, childWidth, childHeight;
			switch (alignX)
			{
			case ESD_ALIGN_LEFTX:
				childX = globalX - scrollX;
				childWidth = child->LocalWidth;
				break;
			case ESD_ALIGN_CENTERX:
				childX = globalX - scrollX + ((globalWidth - child->LocalWidth) >> 1);
				childWidth = child->LocalWidth;
				break;
			case ESD_ALIGN_RIGHTX:
				childX = globalX - scrollX + (globalWidth - child->LocalWidth);
				childWidth = child->LocalWidth;
				break;
			case ESD_ALIGN_FILLX:
				childX = globalX;
				childWidth = globalWidth;
				break;
			}
			switch (alignY)
			{
			case ESD_ALIGN_TOPY:
				childY = globalY - scrollY;
				childHeight = child->LocalHeight;
				break;
			case ESD_ALIGN_CENTERY:
				childY = globalY - scrollY + ((globalHeight - child->LocalHeight) >> 1);
				childHeight = child->LocalHeight;
				break;
			case ESD_ALIGN_BOTTOMY:
				childY = globalY - scrollY + (globalHeight - child->LocalHeight);
				childHeight = child->LocalHeight;
				break;
			case ESD_ALIGN_FILLY:
				childY = globalY;
				childHeight = globalHeight;
				break;
			}
			Ft_Esd_Widget_ScrollGlobalRect(child, childX, childY, childWidth, childHeight);
		}
		child = child->Next;
	}
}

void Ft_Esd_Layout_Scroll_Update(Ft_Esd_Layout_Scroll *context)
{
	if (context->Widget.Recalculate || (context->Values != context->LastValues))
	{
		context->Widget.Recalculate = FT_FALSE;
		ft_int16_t lastScrollX = context->LastScrollX;
		ft_int16_t lastScrollY = context->LastScrollY;
		context->LastValues = context->Values;
		Ft_Esd_Layout_Scroll_Recalculate(context);
		ft_int16_t scrollX = context->ScrollX;
		ft_int16_t scrollY = context->ScrollY;
		if (scrollX != lastScrollX)
		{
			context->ScrollXChanged(context->Owner, scrollX);
		}
		if (scrollY != lastScrollY)
		{
			context->ScrollYChanged(context->Owner, scrollY);
		}
	}
	Ft_Esd_Widget_IterateChildActiveSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_UPDATE);
	if (context->Widget.Recalculate)
	{
		context->Widget.Recalculate = FT_FALSE;
		ft_int16_t lastScrollX = context->ScrollX;
		ft_int16_t lastScrollY = context->ScrollX;
		Ft_Esd_Layout_Scroll_Recalculate(context);
		ft_int16_t scrollX = context->ScrollX;
		ft_int16_t scrollY = context->ScrollY;
		if (scrollX != lastScrollX)
		{
			context->ScrollXChanged(context->Owner, scrollX);
		}
		if (scrollY != lastScrollY)
		{
			context->ScrollYChanged(context->Owner, scrollY);
		}
	}
}

void Ft_Esd_Layout_Scroll_Render(Ft_Esd_Layout_Scroll *context)
{
	Ft_Esd_Rect16 scissor;
	if (context->Scissor)
	{
		scissor = Ft_Esd_Dl_Scissor_Set(context->Widget.GlobalRect);
	}

	Ft_Esd_Widget_IterateChildActiveValidSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_RENDER);

	if (context->Scissor)
	{
		Ft_Esd_Dl_Scissor_Reset(scissor);
	}
}

void Ft_Esd_Layout_Scroll_Idle(Ft_Esd_Layout_Scroll *context)
{
	Ft_Esd_Widget_IterateChildActiveSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_IDLE);
}

ft_int16_t Ft_Esd_Layout_Scroll_Width(Ft_Esd_Layout_Scroll *context)
{
	return context->Widget.GlobalWidth;
}

ft_int16_t Ft_Esd_Layout_Scroll_Height(Ft_Esd_Layout_Scroll *context)
{
	return context->Widget.GlobalHeight;
}

ft_uint16_t Ft_Esd_Layout_Scroll_RangeX(Ft_Esd_Layout_Scroll *context)
{
	return context->MaxX - context->MinX;
}

ft_uint16_t Ft_Esd_Layout_Scroll_RangeY(Ft_Esd_Layout_Scroll *context)
{
	return context->MaxY - context->MinY;
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_Layout_Scroll Instance;
} Ft_Esd_Layout_Scroll__ESD;

void *Ft_Esd_Layout_Scroll__Create__ESD()
{
	Ft_Esd_Layout_Scroll__ESD *context = (Ft_Esd_Layout_Scroll__ESD *)malloc(sizeof(Ft_Esd_Layout_Scroll__ESD));
	Ft_Esd_Layout_Scroll__Initializer(&context->Instance);
	context->Instance.Owner = context;
	return context;
}

ft_int16_t Ft_Esd_Layout_Scroll__Get_MinX__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MinX; }
void Ft_Esd_Layout_Scroll__Set_MinX__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MinX = value; }
ft_int16_t Ft_Esd_Layout_Scroll__Get_MaxX__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MaxX; }
void Ft_Esd_Layout_Scroll__Set_MaxX__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MaxX = value; }
ft_int16_t Ft_Esd_Layout_Scroll__Get_MinY__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MinY; }
void Ft_Esd_Layout_Scroll__Set_MinY__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MinY = value; }
ft_int16_t Ft_Esd_Layout_Scroll__Get_MaxY__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MaxY; }
void Ft_Esd_Layout_Scroll__Set_MaxY__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.MaxY = value; }
ft_int16_t Ft_Esd_Layout_Scroll__Get_ScrollX__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.ScrollX; }
void Ft_Esd_Layout_Scroll__Set_ScrollX__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.ScrollX = value; }
ft_int16_t Ft_Esd_Layout_Scroll__Get_ScrollY__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.ScrollY; }
void Ft_Esd_Layout_Scroll__Set_ScrollY__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.ScrollY = value; }
ft_uint8_t Ft_Esd_Layout_Scroll__Get_Align__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.Align; }
void Ft_Esd_Layout_Scroll__Set_Align__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.Align = value; }
ft_bool_t Ft_Esd_Layout_Scroll__Get_Scissor__ESD(void *context) { return ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.Scissor; }
void Ft_Esd_Layout_Scroll__Set_Scissor__ESD(void *context, ft_bool_t value) { ((Ft_Esd_Layout_Scroll__ESD *)context)->Instance.Scissor = value; }

void Ft_Esd_Layout_Scroll__Destroy__ESD(void *context)
{
	free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
