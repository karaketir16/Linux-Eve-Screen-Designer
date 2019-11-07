

#include "Ft_Esd_Widget.h"
#include "FT_Esd_Dl.h"

static Ft_Esd_WidgetSlots s_Ft_Esd_Widget__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_Widget_Update,
	(void (*)(void *))Ft_Esd_Widget_Render,
	(void (*)(void *))Ft_Esd_Widget_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End
};

static Ft_Esd_Widget *s_Ft_Esd_Widget_FreeQueue = 0;

void Ft_Esd_Widget_IterateChildSlot(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		child->Slots->Table[slot](child);
		child = child->Parent ? child->Next : next;
	}
}

void Ft_Esd_Widget_IterateChildSlotReverse(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->Last;
	while (child)
	{
		Ft_Esd_Widget *const previous = child->Previous;
		child->Slots->Table[slot](child);
		child = child->Parent ? child->Previous : previous;
	}
}

void Ft_Esd_Widget_IterateChildActiveSlot(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		if (child->Active)
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Next : next;
	}
}

void Ft_Esd_Widget_IterateChildActiveSlotReverse(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->Last;
	while (child)
	{
		Ft_Esd_Widget *const previous = child->Previous;
		if (child->Active)
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Previous : previous;
	}
}

void Ft_Esd_Widget_IterateChildActiveValidSlot(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		if (child->Active && child->GlobalValid)
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Next : next;
	}
}

void Ft_Esd_Widget_IterateChildActiveValidSlotReverse(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->Last;
	while (child)
	{
		Ft_Esd_Widget *const previous = child->Previous;
		if (child->Active && child->GlobalValid)
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Previous : previous;
	}
}

void Ft_Esd_Widget_IterateChildVisibleSlot(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		if (child->Active && child->GlobalValid && Ft_Esd_Rect16_Intersects(child->GlobalRect, Ft_Esd_ScissorRect))
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Next : next;
	}
}

void Ft_Esd_Widget_IterateChildVisibleSlotReverse(Ft_Esd_Widget *context, int slot)
{
	Ft_Esd_Widget *child = context->Last;
	while (child)
	{
		Ft_Esd_Widget *const previous = child->Previous;
		if (child->Active && child->GlobalValid && Ft_Esd_Rect16_Intersects(child->GlobalRect, Ft_Esd_ScissorRect))
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Previous : previous;
	}
}

void Ft_Esd_Widget__Initializer(Ft_Esd_Widget *context)
{
	memset(context, 0, sizeof(Ft_Esd_Widget));
	context->Slots = &s_Ft_Esd_Widget__Slots;
	context->ClassId = Ft_Esd_Widget_CLASSID;
	Ft_Esd_Widget_Initialize(context);
}

void Ft_Esd_Widget_Initialize(struct Ft_Esd_Widget *context)
{
	// no-op
}

void Ft_Esd_Widget_Start(struct Ft_Esd_Widget *context)
{
	Ft_Esd_Widget_IterateChildSlot(context, FT_ESD_WIDGET_START);
}

void Ft_Esd_Widget_Enable(struct Ft_Esd_Widget *context)
{
	Ft_Esd_Widget_IterateChildActiveSlot(context, FT_ESD_WIDGET_ENABLE);
}

void Ft_Esd_Widget_Update(struct Ft_Esd_Widget *context)
{
	if (context->Recalculate)
	{
		context->Recalculate = 0;
		Ft_Esd_Widget *child = context->First;
		while (child)
		{
			Ft_Esd_Widget *const next = child->Next;
			if (child->Active)
			{
				Ft_Esd_Widget_SetGlobalRect(child,
				    context->GlobalX,
				    context->GlobalY,
				    context->GlobalWidth,
				    context->GlobalHeight);
			}
			child = next;
		}
	}
	Ft_Esd_Widget_IterateChildActiveSlotReverse(context, FT_ESD_WIDGET_UPDATE);
}

void Ft_Esd_Widget_Render(struct Ft_Esd_Widget *context)
{
	Ft_Esd_Widget_IterateChildActiveValidSlotReverse(context, FT_ESD_WIDGET_RENDER);
}

void Ft_Esd_Widget_Idle(struct Ft_Esd_Widget *context)
{
	Ft_Esd_Widget_IterateChildActiveSlotReverse(context, FT_ESD_WIDGET_IDLE);
}

void Ft_Esd_Widget_Disable(struct Ft_Esd_Widget *context)
{
	Ft_Esd_Widget_IterateChildActiveSlot(context, FT_ESD_WIDGET_DISABLE);
}

void Ft_Esd_Widget_End(struct Ft_Esd_Widget *context)
{
	Ft_Esd_Widget_IterateChildSlot(context, FT_ESD_WIDGET_END);
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		if (child->Instanced)
		{
			Ft_Esd_Widget_Detach(child);
			Ft_Esd_Widget_Free(child);
		}
		child = next;
	}
}

void Ft_Esd_Widget_PostLocalRect(Ft_Esd_Widget *context)
{
	if (context->Parent)
		context->Parent->Recalculate = 1;
}

void Ft_Esd_Widget_PostGlobalRect(Ft_Esd_Widget *context)
{
	context->Recalculate = 1;
}

void Ft_Esd_Widget_Detach_Internal(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *const parent = context->Parent;
	if (!parent)
		return;
	if (context->Previous)
		context->Previous->Next = context->Next;
	else /* if (parent->First == context) */ // implied
		parent->First = context->Next;
	if (context->Next)
		context->Next->Previous = context->Previous;
	else /* if (parent->Last == context) */ // implied
		parent->Last = context->Previous;
	context->Parent = 0;
	context->Previous = 0;
	context->Next = 0;
	context->GlobalValid = FT_FALSE;
	parent->Recalculate = FT_TRUE;
}

void Ft_Esd_Widget_Detach(Ft_Esd_Widget *context)
{
	ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
	Ft_Esd_Widget_Detach_Internal(context);
	if (activeBefore)
		context->Slots->Disable(context);
}

void Ft_Esd_Widget_LocalInsertTop(Ft_Esd_Widget *context, Ft_Esd_Widget *parent)
{
	if (context->Root)
		return;
	ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
	if (context->Parent)
		Ft_Esd_Widget_Detach_Internal(context);
	if (!parent)
	{
		if (activeBefore)
			context->Slots->Disable(context);
		return;
	}
	Ft_Esd_Widget *const first = parent->First;
	context->Parent = parent;
	context->Previous = 0;
	context->Next = parent->First;
	if (first)
		first->Previous = context;
	parent->First = context;
	if (!parent->Last)
		parent->Last = context;
	parent->Recalculate = 1;
	if (!activeBefore && Ft_Esd_Widget_GetActive(context))
		context->Slots->Enable(context);
}

void Ft_Esd_Widget_LocalInsertBottom(Ft_Esd_Widget *context, Ft_Esd_Widget *parent)
{
	if (context->Root)
		return;
	ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
	if (context->Parent)
		Ft_Esd_Widget_Detach(context);
	if (!parent)
	{
		if (activeBefore)
			context->Slots->Disable(context);
		return;
	}
	Ft_Esd_Widget *const last = parent->Last;
	context->Parent = parent;
	context->Previous = last;
	context->Next = 0;
	if (last)
		last->Next = context;
	parent->Last = context;
	if (!parent->First)
		parent->First = context;
	parent->Recalculate = 1;
	if (!activeBefore && Ft_Esd_Widget_GetActive(context))
		context->Slots->Enable(context);
}

Ft_Esd_Widget *Ft_Esd_Widget_GetContainer(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *container = context;
	while (container->Container && (container->Container != container))
		container = container->Container;
	return container;
}

void Ft_Esd_Widget_InsertTop(Ft_Esd_Widget *context, Ft_Esd_Widget *parent)
{
	Ft_Esd_Widget_LocalInsertTop(context, Ft_Esd_Widget_GetContainer(parent));
}

void Ft_Esd_Widget_InsertBottom(Ft_Esd_Widget *context, Ft_Esd_Widget *parent)
{
	Ft_Esd_Widget_LocalInsertBottom(context, Ft_Esd_Widget_GetContainer(parent));
}

void Ft_Esd_Widget_SetActive(Ft_Esd_Widget *context, ft_bool_t active)
{
	if (context->Active != active)
	{
		ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
		context->Active = active;
		ft_bool_t activeAfter = Ft_Esd_Widget_GetActive(context);
		if (activeBefore != activeAfter)
		{
			if (activeAfter)
				context->Slots->Enable(context);
			else
				context->Slots->Disable(context);
		}
		if (context->Parent)
			context->Parent->Recalculate = FT_TRUE;
		context->GlobalValid = FT_FALSE;
	}
}

ft_bool_t Ft_Esd_Widget_GetActive(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *widget = context;
	while (widget->Parent)
	{
		if (!widget->Active)
			return 0;
		widget = widget->Parent;
	}
	return widget->Root && widget->Active;
}

void Ft_Esd_Widget_SetX(Ft_Esd_Widget *context, ft_int16_t x)
{
	if (context->LocalX != x)
	{
		context->LocalX = x;
		Ft_Esd_Widget_PostLocalRect(context);
	}
}

void Ft_Esd_Widget_SetY(Ft_Esd_Widget *context, ft_int16_t y)
{
	if (context->LocalY != y)
	{
		context->LocalY = y;
		Ft_Esd_Widget_PostLocalRect(context);
	}
}

void Ft_Esd_Widget_SetWidth(Ft_Esd_Widget *context, ft_int16_t width)
{
	if (context->LocalWidth != width)
	{
		context->LocalWidth = width;
		Ft_Esd_Widget_PostLocalRect(context);
	}
}

void Ft_Esd_Widget_SetHeight(Ft_Esd_Widget *context, ft_int16_t height)
{
	if (context->LocalHeight != height)
	{
		context->LocalHeight = height;
		Ft_Esd_Widget_PostLocalRect(context);
	}
}

void Ft_Esd_Widget_SetRect(Ft_Esd_Widget *context, Ft_Esd_Rect16 rect)
{
	if (context->LocalX != rect.X
	    || context->LocalY != rect.Y
	    || context->LocalWidth != rect.Width
	    || context->LocalHeight != rect.Height)
	{
		context->LocalRect = rect;
		Ft_Esd_Widget_PostLocalRect(context);
	}
}

void Ft_Esd_Widget_SetGlobalRect(Ft_Esd_Widget *context, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height)
{
	if (x != context->GlobalX
	    || y != context->GlobalY
	    || width != context->GlobalWidth
	    || height != context->GlobalHeight)
	{
		context->GlobalX = x;
		context->GlobalY = y;
		context->GlobalWidth = width;
		context->GlobalHeight = height;
		Ft_Esd_Widget_PostGlobalRect(context);
	}
	context->GlobalValid = FT_TRUE;
}

void Ft_Esd_Widget_RecurseScrollGlobalRect(Ft_Esd_Widget *context, ft_int16_t xd, ft_int16_t yd)
{
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		if (child->Active)
		{
			child->GlobalX += xd;
			child->GlobalY += yd;
			Ft_Esd_Widget_RecurseScrollGlobalRect(child, xd, yd);
		}
		child = next;
	}
}

void Ft_Esd_Widget_ScrollGlobalRect(Ft_Esd_Widget *context, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height)
{
	if (x != context->GlobalX
	    || y != context->GlobalY
	    || width != context->GlobalWidth
	    || height != context->GlobalHeight)
	{
		ft_int16_t xd = x - context->GlobalX;
		ft_int16_t yd = y - context->GlobalY;
		context->GlobalX = x;
		context->GlobalY = y;
		context->GlobalWidth = width;
		context->GlobalHeight = height;
		Ft_Esd_Widget_PostGlobalRect(context);
		Ft_Esd_Widget_RecurseScrollGlobalRect(context, xd, yd);
	}
	context->GlobalValid = FT_TRUE;
}

void Ft_Esd_Widget_GetGlobalRect(Ft_Esd_Widget *context, ft_int16_t *x, ft_int16_t *y, ft_int16_t *width, ft_int16_t *height)
{
	*x = context->GlobalX;
	*y = context->GlobalY;
	*width = context->GlobalWidth;
	*height = context->GlobalHeight;
}

void Ft_Esd_Widget_Free(Ft_Esd_Widget *context)
{
	// eve_printf_debug("Request free\n");
	if (context->Parent)
	{
#ifndef NDBEUG
		eve_printf_debug("Unable to free widget with parent\n");
#endif
		return; // ERROR
	}
	memset(context, 0, sizeof(Ft_Esd_Widget)); // Safety wipe
	context->Slots = &s_Ft_Esd_Widget__Slots;
	context->Container = s_Ft_Esd_Widget_FreeQueue; // Abuse container pointer here
	s_Ft_Esd_Widget_FreeQueue = context;
}

void Ft_Esd_Widget_ProcessFree()
{
	Ft_Esd_Widget *current = s_Ft_Esd_Widget_FreeQueue;
	s_Ft_Esd_Widget_FreeQueue = 0;
	while (current)
	{
		Ft_Esd_Widget *next = current->Container;
		// eve_printf_debug("Free widget\n");
		free(current);
		current = next;
	}
}

ft_bool_t Ft_Esd_Widget_IsVisible(Ft_Esd_Widget *context)
{
	return Ft_Esd_Rect16_Intersects(context->GlobalRect, Ft_Esd_ScissorRect);
}

/* end of file */
