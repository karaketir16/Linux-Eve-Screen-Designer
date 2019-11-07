#include "Ft_Esd_Layout_Linear.h"
#include "Ft_Esd_Layout_Stretch.h"

static Ft_Esd_WidgetSlots s_Ft_Esd_Layout_Linear__Slots = {
	(void (*)(void *))Ft_Esd_Widget_Initialize,
	(void (*)(void *))Ft_Esd_Widget_Start,
	(void (*)(void *))Ft_Esd_Widget_Enable,
	(void (*)(void *))Ft_Esd_Layout_Linear_Update,
	(void (*)(void *))Ft_Esd_Layout_Linear_Render,
	(void (*)(void *))Ft_Esd_Layout_Linear_Idle,
	(void (*)(void *))Ft_Esd_Widget_Disable,
	(void (*)(void *))Ft_Esd_Widget_End,
};

void Ft_Esd_Layout_Linear__Initializer(Ft_Esd_Layout_Linear *context)
{
	Ft_Esd_Widget__Initializer((Ft_Esd_Widget *)context);
	context->Widget.ClassId = Ft_Esd_Layout_Linear_CLASSID;
	context->Widget.Slots = &s_Ft_Esd_Layout_Linear__Slots;
	context->Widget.LocalWidth = 50;
	context->Widget.LocalHeight = 50;
	context->Margin = 0;
	context->Spacing = 4;
	context->Orientation = ESD_ORIENTATION_VERTICAL;
	context->Align = ESD_ALIGN_TOPFILL;
	context->Overflow = ESD_OVERFLOW_ALLOW;
	context->Scissor = 0;
	context->ChildClipping = ESD_CLIP_RENDER;
	context->AutoResize = FT_FALSE;
	context->LastValues = context->Values;
}

void Ft_Esd_Layout_Linear_Update(Ft_Esd_Layout_Linear *context)
{
	if (context->LastValues != context->Values)
	{
		context->LastValues = context->Values;
		context->Widget.Recalculate = FT_TRUE;
	}
	if (context->Widget.Recalculate)
	{
		// eve_printf_debug("Recalculate Linear\n");

		context->Widget.Recalculate = 0;

		const ft_int16_t margin = max(0, min(min(context->Margin, // Margin must be within the rectangle and positive
		                                         context->Widget.GlobalWidth >> 1),
		                                     context->Widget.GlobalHeight >> 1));
		const ft_int16_t outerX = context->Widget.GlobalX + margin;
		const ft_int16_t outerY = context->Widget.GlobalY + margin;
		const ft_int16_t outerWidth = max(0, context->Widget.GlobalWidth - (margin << 1));
		const ft_int16_t outerHeight = max(0, context->Widget.GlobalHeight - (margin << 1));
		const ft_int16_t spacing = max(0, context->Spacing); // Spacing must be positive

		ft_uint8_t alignX = ESD_ALIGN_HORIZONTAL(context->Align);
		ft_uint8_t alignY = ESD_ALIGN_VERTICAL(context->Align);

		if (context->Orientation & ESD_ORIENTATION_HORIZONTAL)
		{
			Ft_Esd_Widget *child;

			ft_int16_t childWidthTotal = 0;
			ft_int16_t childCount = 0;
			ft_int16_t stretchCount = 0;

			child = context->Widget.First;
			while (child)
			{
				if (child->Active)
				{
					if (child->ClassId == Ft_Esd_Layout_Stretch_CLASSID)
					{
						stretchCount += ((Ft_Esd_Layout_Stretch *)child)->Weight;
					}
					else
					{
						childWidthTotal += max(0, child->LocalWidth);
						++childCount;
					}
				}
				child = child->Next;
			}

			const ft_int32_t childWidthTotalInclSpacing = childWidthTotal + ((childCount - 1) * spacing);
			ft_int32_t remainingStretchWidth = outerWidth - childWidthTotalInclSpacing;
			ft_int32_t remainingStretchCount = stretchCount; // (remainingStretchWidth > 0) ? stretchCount : 0;

			if (remainingStretchWidth > 0)
			{
				// When remaining width, and stretches detected, simply force basic align
				if (stretchCount && alignX == ESD_ALIGN_FILLX)
					alignX = ESD_ALIGN_LEFTX;
			}
			else if (remainingStretchWidth < 0)
			{
				// Only fill when overflowing and overflow is set to fill, otherwise force basic align
				if (context->Overflow == ESD_OVERFLOW_FILL)
					alignX = ESD_ALIGN_FILLX;
				else if (alignX == ESD_ALIGN_FILLX)
					alignX = ESD_ALIGN_LEFTX;
			}

			ft_int16_t currentX = outerX;

			switch (alignX)
			{
			case ESD_ALIGN_CENTERX:
				currentX += (remainingStretchWidth >> 1);
				break;
			case ESD_ALIGN_RIGHTX:
				currentX += remainingStretchWidth;
				break;
			case ESD_ALIGN_FILLX:
				// Proportional
				stretchCount = 0;
				remainingStretchWidth = outerWidth - ((childCount - 1) * spacing);
				remainingStretchCount = childWidthTotal;
				break;
			}

			child = context->Widget.First;
			while (child)
			{
				if (child->Active)
				{
					ft_int16_t childY, childHeight;
					switch (alignY)
					{
					case ESD_ALIGN_TOPY:
						childY = outerY;
						childHeight = child->LocalHeight;
						break;
					case ESD_ALIGN_CENTERY:
						childY = outerY + ((outerHeight - child->LocalHeight) >> 1);
						childHeight = child->LocalHeight;
						break;
					case ESD_ALIGN_BOTTOMY:
						childY = outerY + (outerHeight - child->LocalHeight);
						childHeight = child->LocalHeight;
						break;
					case ESD_ALIGN_FILLY:
						childY = outerY;
						childHeight = outerHeight;
						break;
					}
					if (context->Overflow == ESD_OVERFLOW_FILL && childHeight > outerHeight)
					{
						childHeight = outerHeight;
					}
					if (child->ClassId == Ft_Esd_Layout_Stretch_CLASSID)
					{
						if (stretchCount)
						{
							ft_int32_t weight = ((Ft_Esd_Layout_Stretch *)child)->Weight;
							ft_int16_t stretchWidth = remainingStretchWidth * weight / remainingStretchCount;
							remainingStretchWidth -= stretchWidth;
							remainingStretchCount -= weight;
							Ft_Esd_Widget_SetGlobalRect(child,
							    currentX,
							    childY,
							    max(0, (stretchWidth - spacing)),
							    childHeight);
							currentX += stretchWidth;
						}
						else
						{
							Ft_Esd_Widget_SetGlobalRect(child,
							    currentX,
							    childY,
							    0,
							    childHeight);
						}
					}
					else if (alignX == ESD_ALIGN_FILLX)
					{
						ft_int32_t weight = child->LocalWidth;
						ft_int16_t stretchWidth = remainingStretchWidth * weight / remainingStretchCount;
						remainingStretchWidth -= stretchWidth;
						remainingStretchCount -= weight;
						Ft_Esd_Widget_SetGlobalRect(child,
						    currentX,
						    childY,
						    max(0, stretchWidth),
						    childHeight);
						currentX += stretchWidth + spacing;
					}
					else
					{
						Ft_Esd_Widget_SetGlobalRect(child,
						    currentX,
						    childY,
						    max(0, child->LocalWidth),
						    childHeight);
						currentX += child->GlobalWidth + spacing;
					}
					if (context->Overflow == ESD_OVERFLOW_CLIP)
					{
						child->GlobalValid &= Ft_Esd_Rect16_IsInsideHorizontal(child->GlobalRect, context->Widget.GlobalRect);
					}
				}
				child = child->Next;
			}

			if ((context->AutoResize & ESD_AUTORESIZE_WIDTH)
			    && (context->Overflow == ESD_OVERFLOW_ALLOW))
			{
				// Generally self resize is useful when part of scroll panel
				Ft_Esd_Widget_SetWidth((Ft_Esd_Widget *)context, childWidthTotalInclSpacing + (context->Margin * 2));
			}
		}
		else
		{
			Ft_Esd_Widget *child;

			ft_int16_t childHeightTotal = 0;
			ft_int16_t childCount = 0;
			ft_int16_t stretchCount = 0;

			child = context->Widget.First;
			while (child)
			{
				if (child->Active)
				{
					if (child->ClassId == Ft_Esd_Layout_Stretch_CLASSID)
					{
						stretchCount += ((Ft_Esd_Layout_Stretch *)child)->Weight;
					}
					else
					{
						childHeightTotal += max(0, child->LocalHeight);
						++childCount;
					}
				}
				child = child->Next;
			}

			const ft_int32_t childHeightTotalInclSpacing = childHeightTotal + ((childCount - 1) * spacing);
			ft_int32_t remainingStretchHeight = outerHeight - childHeightTotalInclSpacing;
			ft_int32_t remainingStretchCount = stretchCount; // (remainingStretchHeight > 0) ? stretchCount : 0;

			if (remainingStretchHeight > 0)
			{
				// When remaining width, and stretches detected, simply force basic align
				if (stretchCount && alignY == ESD_ALIGN_FILLY)
					alignY = ESD_ALIGN_TOPY;
			}
			else if (remainingStretchHeight < 0)
			{
				// Only fill when overflowing and overflow is set to fill, otherwise force basic align
				if (context->Overflow == ESD_OVERFLOW_FILL)
					alignY = ESD_ALIGN_FILLY;
				else if (alignY == ESD_ALIGN_FILLY)
					alignY = ESD_ALIGN_TOPY;
			}

			ft_int16_t currentY = outerY;

			switch (alignY)
			{
			case ESD_ALIGN_CENTERY:
				currentY += (remainingStretchHeight >> 1);
				break;
			case ESD_ALIGN_BOTTOMY:
				currentY += remainingStretchHeight;
				break;
			case ESD_ALIGN_FILLY:
				// Proportional
				stretchCount = 0;
				remainingStretchHeight = outerHeight - ((childCount - 1) * spacing);
				remainingStretchCount = childHeightTotal;
				break;
			}

			child = context->Widget.First;
			while (child)
			{
				if (child->Active)
				{
					ft_int16_t childX, childWidth;
					switch (alignX)
					{
					case ESD_ALIGN_LEFTX:
						childX = outerX;
						childWidth = child->LocalWidth;
						break;
					case ESD_ALIGN_CENTERX:
						childX = outerX + ((outerWidth - child->LocalWidth) >> 1);
						childWidth = child->LocalWidth;
						break;
					case ESD_ALIGN_RIGHTX:
						childX = outerX + (outerWidth - child->LocalWidth);
						childWidth = child->LocalWidth;
						break;
					case ESD_ALIGN_FILLX:
						childX = outerX;
						childWidth = outerWidth;
						break;
					}
					if (context->Overflow == ESD_OVERFLOW_FILL && childWidth > outerWidth)
					{
						childWidth = outerWidth;
					}
					if (child->ClassId == Ft_Esd_Layout_Stretch_CLASSID)
					{
						if (stretchCount)
						{
							ft_int32_t weight = ((Ft_Esd_Layout_Stretch *)child)->Weight;
							ft_int16_t stretchHeight = remainingStretchHeight * weight / remainingStretchCount;
							remainingStretchHeight -= stretchHeight;
							remainingStretchCount -= weight;
							Ft_Esd_Widget_SetGlobalRect(child,
							    outerX,
							    currentY,
							    outerWidth,
							    max(0, (stretchHeight - spacing)));
							currentY += stretchHeight;
						}
						else
						{
							Ft_Esd_Widget_SetGlobalRect(child,
							    childX,
							    currentY,
							    childWidth,
							    0);
						}
					}
					else if (alignY == ESD_ALIGN_FILLY)
					{
						ft_int32_t weight = child->LocalHeight;
						ft_int16_t stretchHeight = remainingStretchHeight * weight / remainingStretchCount;
						remainingStretchHeight -= stretchHeight;
						remainingStretchCount -= weight;
						Ft_Esd_Widget_SetGlobalRect(child,
						    childX,
						    currentY,
						    childWidth,
						    max(0, stretchHeight));
						currentY += stretchHeight + spacing;
					}
					else
					{
						Ft_Esd_Widget_SetGlobalRect(child,
						    childX,
						    currentY,
						    childWidth,
						    max(0, child->LocalHeight));
						currentY += child->GlobalHeight + spacing;
					}
					if (context->Overflow == ESD_OVERFLOW_CLIP)
					{
						child->GlobalValid &= Ft_Esd_Rect16_IsInsideVertical(child->GlobalRect, context->Widget.GlobalRect);
					}
				}
				child = child->Next;
			}

			if ((context->AutoResize & ESD_AUTORESIZE_HEIGHT)
			    && (context->Overflow == ESD_OVERFLOW_ALLOW))
			{
				// Generally self resize is useful when part of scroll panel
				Ft_Esd_Widget_SetHeight((Ft_Esd_Widget *)context, childHeightTotalInclSpacing + (context->Margin * 2));
			}
		}
	}
	if (context->ChildClipping & ESD_CLIP_UPDATE)
	{
		Ft_Esd_Widget_IterateChildVisibleSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_UPDATE);
	}
	else
	{
		Ft_Esd_Widget_IterateChildActiveSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_UPDATE);
	}
}

void Ft_Esd_Layout_Linear_Render(Ft_Esd_Layout_Linear *context)
{
	if (context->ChildClipping & ESD_CLIP_RENDER)
	{
		Ft_Esd_Widget_IterateChildVisibleSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_RENDER);
	}
	else
	{
		Ft_Esd_Widget_IterateChildActiveValidSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_RENDER);
	}
}

void Ft_Esd_Layout_Linear_Idle(Ft_Esd_Layout_Linear *context)
{
	if (context->ChildClipping & ESD_CLIP_IDLE)
	{
		Ft_Esd_Widget_IterateChildVisibleSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_IDLE);
	}
	else
	{
		Ft_Esd_Widget_IterateChildActiveSlot((Ft_Esd_Widget *)context, FT_ESD_WIDGET_IDLE);
	}
}

#ifdef ESD_SIMULATION
#include <stdlib.h>

typedef struct
{
	Ft_Esd_Layout_Linear Instance;
} Ft_Esd_Layout_Linear__ESD;

void *Ft_Esd_Layout_Linear__Create__ESD()
{
	Ft_Esd_Layout_Linear__ESD *context = (Ft_Esd_Layout_Linear__ESD *)malloc(sizeof(Ft_Esd_Layout_Linear__ESD));
	Ft_Esd_Layout_Linear__Initializer(&context->Instance);
	context->Instance.Owner = context;
	return context;
}

ft_int16_t Ft_Esd_Layout_Linear__Get_Margin__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Margin; }
void Ft_Esd_Layout_Linear__Set_Margin__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Margin = value; }
ft_int16_t Ft_Esd_Layout_Linear__Get_Spacing__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Spacing; }
void Ft_Esd_Layout_Linear__Set_Spacing__ESD(void *context, ft_int16_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Spacing = value; }
ft_uint8_t Ft_Esd_Layout_Linear__Get_Orientation__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Orientation; }
void Ft_Esd_Layout_Linear__Set_Orientation__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Orientation = value; }
ft_uint8_t Ft_Esd_Layout_Linear__Get_Align__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Align; }
void Ft_Esd_Layout_Linear__Set_Align__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Align = value; }
ft_uint8_t Ft_Esd_Layout_Linear__Get_Overflow__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Overflow; }
void Ft_Esd_Layout_Linear__Set_Overflow__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Overflow = value; }
ft_bool_t Ft_Esd_Layout_Linear__Get_Scissor__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Scissor; }
void Ft_Esd_Layout_Linear__Set_Scissor__ESD(void *context, ft_bool_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.Scissor = value; }
ft_uint8_t Ft_Esd_Layout_Linear__Get_ChildClipping__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.ChildClipping; }
void Ft_Esd_Layout_Linear__Set_ChildClipping__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.ChildClipping = value; }
ft_uint8_t Ft_Esd_Layout_Linear__Get_AutoResize__ESD(void *context) { return ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.AutoResize; }
void Ft_Esd_Layout_Linear__Set_AutoResize__ESD(void *context, ft_uint8_t value) { ((Ft_Esd_Layout_Linear__ESD *)context)->Instance.AutoResize = value; }

void Ft_Esd_Layout_Linear__Destroy__ESD(void *context)
{
	free(context);
}

#endif /* ESD_SIMULATION */

/* end of file */
