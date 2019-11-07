

#include "Ft_Esd_Widget.h"

void Ft_Esd_Widget_Detach_Internal(Ft_Esd_Widget *context);

void Ft_Esd_Widget_LocalInsertAbove(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling)
{
	if (context->Root)
		return;
	ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
	if (context->Parent)
		Ft_Esd_Widget_Detach_Internal(context);
	Ft_Esd_Widget *const parent = sibling->Parent;
	if (!parent)
	{
		if (activeBefore)
			context->Slots->Disable(context);
		return;
	}
	context->Previous = sibling->Previous;
	context->Next = sibling;
	context->Parent = parent;
	sibling->Previous = context;
	if (context->Previous)
		context->Previous->Next = context;
	if (parent->First == sibling)
		parent->First = context;
	parent->Recalculate = 1;
	if (!activeBefore && Ft_Esd_Widget_GetActive(context))
		context->Slots->Enable(context);
}

void Ft_Esd_Widget_LocalInsertBelow(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling)
{
	if (context->Root)
		return;
	ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
	if (context->Parent)
		Ft_Esd_Widget_Detach_Internal(context);
	Ft_Esd_Widget *const parent = sibling->Parent;
	if (!parent)
	{
		if (activeBefore)
			context->Slots->Disable(context);
		return;
	}
	context->Previous = sibling;
	context->Next = sibling->Next;
	context->Parent = parent;
	sibling->Next = context;
	if (context->Next)
		context->Next->Previous = context;
	if (parent->Last == sibling)
		parent->Last = context;
	parent->Recalculate = 1;
	if (!activeBefore && Ft_Esd_Widget_GetActive(context))
		context->Slots->Enable(context);
}

void Ft_Esd_Widget_SortToTop(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *const parent = context->Parent;
	if (!parent)
		return;
	ft_bool_t active = context->Active;
	ft_bool_t globalValid = context->GlobalValid;
	context->Active = FT_FALSE;
	Ft_Esd_Widget_Detach(context);
	Ft_Esd_Widget_LocalInsertTop(context, parent);
	context->Active = active;
	context->GlobalValid = globalValid;
}

void Ft_Esd_Widget_SortHigher(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *const previous = context->Previous;
	if (!previous)
		return;
	ft_bool_t active = context->Active;
	ft_bool_t globalValid = context->GlobalValid;
	context->Active = FT_FALSE;
	Ft_Esd_Widget_Detach(context);
	Ft_Esd_Widget_LocalInsertAbove(context, previous);
	context->Active = active;
	context->GlobalValid = globalValid;
}

void Ft_Esd_Widget_SortLower(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *const next = context->Next;
	if (!next)
		return;
	ft_bool_t active = context->Active;
	ft_bool_t globalValid = context->GlobalValid;
	context->Active = FT_FALSE;
	Ft_Esd_Widget_Detach(context);
	Ft_Esd_Widget_LocalInsertBelow(context, next);
	context->Active = active;
	context->GlobalValid = globalValid;
}

void Ft_Esd_Widget_SortToBottom(Ft_Esd_Widget *context)
{
	Ft_Esd_Widget *const parent = context->Parent;
	if (!parent)
		return;
	ft_bool_t active = context->Active;
	ft_bool_t globalValid = context->GlobalValid;
	context->Active = FT_FALSE;
	Ft_Esd_Widget_Detach(context);
	Ft_Esd_Widget_LocalInsertBottom(context, parent);
	context->Active = active;
	context->GlobalValid = globalValid;
}

void Ft_Esd_Widget_InsertAbove(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling)
{
	Ft_Esd_Widget_LocalInsertAbove(context, sibling);
}

void Ft_Esd_Widget_InsertBelow(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling)
{
	Ft_Esd_Widget_LocalInsertBelow(context, sibling);
}

ft_bool_t Ft_Esd_Widget_IsTop(Ft_Esd_Widget *context)
{
	return !context->Previous;
}

ft_bool_t Ft_Esd_Widget_IsBottom(Ft_Esd_Widget *context)
{
	return !context->Next;
}

void Ft_Esd_Widget_IterateChildClippedSlot(Ft_Esd_Widget *context, int slot, ft_bool_t (*visible)(Ft_Esd_Widget *))
{
	Ft_Esd_Widget *child = context->First;
	while (child)
	{
		Ft_Esd_Widget *const next = child->Next;
		if (visible(child))
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Next : next;
	}
}

void Ft_Esd_Widget_IterateChildClippedSlotReverse(Ft_Esd_Widget *context, int slot, ft_bool_t (*visible)(Ft_Esd_Widget *))
{
	Ft_Esd_Widget *child = context->Last;
	while (child)
	{
		Ft_Esd_Widget *const previous = child->Previous;
		if (visible(child))
			child->Slots->Table[slot](child);
		child = child->Parent ? child->Previous : previous;
	}
}

#ifdef ESD_SIMULATION
extern int Ft_Esd_DesignerMode;
void Ft_Esd_Widget_SetRoot(Ft_Esd_Widget *context, ft_bool_t root)
{
	root = root && !Ft_Esd_DesignerMode;
	if (context->Root != root)
	{
		ft_bool_t activeBefore = Ft_Esd_Widget_GetActive(context);
		context->Root = root;
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
#endif

/* end of file */
