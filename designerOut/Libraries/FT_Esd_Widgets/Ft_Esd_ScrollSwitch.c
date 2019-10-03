#include "Ft_Esd.h"
#include "Ft_Esd_Math.h"
#include "Ft_Esd_ScrollSwitch.h"

void Ft_Esd_Spinner_Popup();

enum Ft_Esd_ScrollSwitch_State_Enum
{
	_IDLE = 0,
	_Changing = 1,
	_ToLeft = 2,
	_ToRight = 4,
	_Recovering = 8,
	_Shifting = 16,
	_Loading = 32,
	_All = 255
};

ESD_METHOD(Ft_Esd_ScrollSwitch_SS_Initialise, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_SS_Initialise(Ft_Esd_ScrollSwitch *context);

ESD_METHOD(Ft_Esd_ScrollSwitch_Touch_Down, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_Touch_Down(Ft_Esd_ScrollSwitch *context);

ESD_METHOD(Ft_Esd_ScrollSwitch_Touch_Up, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_Touch_Up(Ft_Esd_ScrollSwitch *context);

ESD_METHOD(Ft_Esd_ScrollSwitch_Update_Handle, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_Update_Handle(Ft_Esd_ScrollSwitch *context);

ESD_METHOD(Ft_Esd_ScrollSwitch_PreUpdate_Widget, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_PreUpdate_Widget(Ft_Esd_ScrollSwitch *context);

ESD_METHOD(Ft_Esd_ScrollSwitch_Update_Widget, Context = Ft_Esd_ScrollSwitch)
ESD_PARAMETER(newWidget, DisplayName = "Widget", Type = Ft_Esd_Widget *)
void Ft_Esd_ScrollSwitch_Update_Widget(Ft_Esd_ScrollSwitch *context, Ft_Esd_Widget *newWidget);

ESD_METHOD(Ft_Esd_ScrollSwitch_Recover_Position, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_Recover_Position(Ft_Esd_ScrollSwitch *context);

ESD_METHOD(Ft_Esd_ScrollSwitch_Shift_Position, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_Shift_Position(Ft_Esd_ScrollSwitch *context);

void Load_NewPage(Ft_Esd_ScrollSwitch *context)
{
	if (context->State & _Loading)
	{
		void *owner = context->Owner;
		//printf("Start Loading %d\n", context->State);
		if (context->Spinner(context->Owner))
			Ft_Esd_Spinner_Popup();
		context->NewPage(owner);
		Ft_Esd_Widget *newWidget = context->New(owner);
		Ft_Esd_ScrollSwitch_Update_Widget(context, newWidget);
		context->State &= ~_Loading;
		//printf("Done Loading %d\n", context->State);
	}
}

int Ft_Esd_ScrollSwitch_GetUnit(Ft_Esd_ScrollSwitch *context)
{
	int width = context->Widget.LocalWidth;
	return (width + context->Spacing);
}

int Ft_Esd_ScrollSwitch_GetStartX(int index, Ft_Esd_ScrollSwitch *context)
{
	int unit = Ft_Esd_ScrollSwitch_GetUnit(context);
	if (context->Looping)
	{
		index = Ft_Esd_Int16_ClampedValue(index, -1, context->Count + 1);
		return (index + 1) * unit;
	}
	else
	{
		index = Ft_Esd_Int16_ClampedValue(index, 0, context->Count - 1);
		return context->Spacing + index * unit;
	}
}

int Ft_Esd_ScrollSwitch_GetSpeed(int dx, int scale)
{
	scale = Ft_Esd_Int16_ClampedValue(scale, 1, 16);
	int max = ((abs(dx / 20) + 1) << 1) * scale;
	int step = (dx > 0) ? min(max, dx) : max(-max, dx);
	return step;
}

void Ft_Esd_ScrollSwitch_Update_Handle(Ft_Esd_ScrollSwitch *context)
{
	void *owner = context->Owner;

	int state = context->State;
	ft_bool_t isTouching = context->Touching;
	ft_bool_t isChanging = state & _Changing;
	ft_bool_t isRecovering = state & _Recovering;
	ft_bool_t isShifting = state & _Shifting;
	//printf("c %d, r %d, s %d\n", isChanging, isRecovering, isShifting);

	if (isRecovering)
	{
		context->ScrollPanel.TouchScroll = FT_FALSE;

		//printf("to isRecovering %d\n", context->State );
		Ft_Esd_ScrollSwitch_Recover_Position(context);
	}
	else if (isShifting)
	{
		//printf("Shifting %d\n", context->State );
		context->ScrollPanel.TouchScroll = FT_FALSE;

		Ft_Esd_ScrollSwitch_Shift_Position(context);
		int newState = context->State;
		ft_bool_t newIsShifting = newState & _Shifting;
		if (!newIsShifting)
		{
			//printf("Finished Shifting %d\n", context->State );
			Load_NewPage(context);
		}
	}
	else
		context->ScrollPanel.TouchScroll = FT_TRUE;
}

void Ft_Esd_ScrollSwitch_Recover_Position(Ft_Esd_ScrollSwitch *context)
{
	if (!(context->State & _Recovering))
		return;
	if (context->Touching)
		return;
	if (!context->Current)
		return;

	int newTargetX = Ft_Esd_ScrollSwitch_GetStartX(context->Index, context);
	int dx = (newTargetX - context->ScrollX);
	int step = Ft_Esd_ScrollSwitch_GetSpeed(dx, context->SwitchSpeed);

	//printf("Ft_Esd_ScrollSwitch_Recover_Position %d + %d -> %d\n", context->ScrollXStart, step, context->ScrollX + step);
	context->NewScrollX = context->ScrollX + step;
	if (step == dx)
	{
		context->NewScrollX = newTargetX;
		context->State &= ~(_Changing | _Recovering);
		//printf("Ft_Esd_ScrollSwitch_Recover_Position Ends %d\n", context->State );
	}
	//context->ScrollXStart = context->NewScrollX;
}

void Ft_Esd_ScrollSwitch_Shift_Position(Ft_Esd_ScrollSwitch *context)
{
	if (!(context->State & _Shifting))
	{
		//printf("Ft_Esd_ScrollSwitch_Shift_Position skip %d\n", context->State );
		return;
	}
	if (context->Overflow)
	{
		//Ft_Esd_Widget_SetActive(&context->ScrollPanel, FT_FALSE);
		context->ScrollPanel.Widget.GlobalValid = FT_FALSE;
		context->Current->GlobalValid = FT_FALSE;
		//context->ScrollPanel.Widget.Recalculate = FT_TRUE;
		context->Touching = 0;
		context->ScrollX = context->NewScrollX;
		int dx = Ft_Esd_ScrollSwitch_GetUnit(context) * context->Overflow; //(context->ScrollXTarget - context->NewScrollX);
		int step = Ft_Esd_ScrollSwitch_GetSpeed(dx, context->SwitchSpeed);
		context->NewScrollX = context->ScrollXStart + step;
		//printf("%d Start: %d, target: %d, x: %d, new: %d\n", context->Overflow, context->ScrollXStart, context->ScrollXTarget, context->ScrollX, context->NewScrollX);
		context->Overflow = 0;
		context->ScrollPanel.ScrollX = context->NewScrollX;
		//Ft_Esd_Widget_SetActive(&context->ScrollPanel, FT_TRUE);

		return;
	}

	//printf("Start: %d, target: %d, x: %d, new: %d\n", context->ScrollXStart, context->ScrollXTarget, context->ScrollX, context->NewScrollX);
	int newTargetX = context->ScrollXTarget;
	int dx = (newTargetX - context->ScrollX);
	if (abs(dx) > Ft_Esd_ScrollSwitch_GetUnit(context))
	{ // Readjust start X if it is too far
		context->Current->GlobalValid = FT_FALSE;
		context->ScrollX = context->ScrollXStart;
		return;
	}

	int step = Ft_Esd_ScrollSwitch_GetSpeed(dx, context->SwitchSpeed);
	//printf("Ft_Esd_ScrollSwitch_Shift_Position %d + %d -> %d\n", context->ScrollX, step, context->ScrollX + step);

	if (dx == step)
	{
		context->NewScrollX = newTargetX;
		context->State &= ~_Shifting;
		//printf("Ft_Esd_ScrollSwitch_Shift_Position Ends %d\n", context->State );
	}
	else
	{
		context->NewScrollX = context->ScrollX + step;
		context->ScrollPanel.ScrollX = context->NewScrollX;
	}
}

void Ft_Esd_ScrollSwitch_Touch_Down(Ft_Esd_ScrollSwitch *context)
{
	context->ScrollXStart = context->ScrollX;
	context->State &= ~_Changing;
}

void Ft_Esd_ScrollSwitch_Touch_Up(Ft_Esd_ScrollSwitch *context)
{
	if (context->State & _Changing)
		return;
	context->Overflow = 0;

	//context->ScrollPanel.Widget->Width(&context->ScrollPanel);
	int width = context->Widget.LocalWidth;
	int minDx = (width / (1 + 2.0f * context->Sensitivity));
	int dx = context->ScrollX - context->ScrollXStart;
	int index = context->Index;
	Ft_Esd_Widget *removingWidget = NULL;

	context->State &= ~(_ToRight | _ToLeft);

	if (dx > minDx && context->Index < context->Count)
	{
		index = Ft_Esd_Int16_ClampedValue((context->Index % context->Count) + 1, 0, context->Count);
		context->State |= _ToRight | _Changing;
		if (!context->Looping && index == context->Count)
			index--;
	}
	else if (dx < -minDx && context->Index > -1)
	{
		index = Ft_Esd_Int16_ClampedValue((context->Index % context->Count) - 1, -1, context->Count - 1);
		context->State |= _ToLeft | _Changing;
		if (!context->Looping && index == -1)
			index++;
	}

	if (index == context->Index)
	{
		context->State |= _Recovering;
		context->ScrollXStart = context->ScrollX;
		//printf("Set Recover %d\n", context->ScrollX);
	}
	else
	{
		if ((context->State & _ToRight) && context->Previous != NULL)
		{
			removingWidget = context->Previous;
		}
		else if ((context->State & _ToLeft) && context->Next != NULL)
		{
			removingWidget = context->Next;
		}

		if (context->Current->Previous == removingWidget)
			context->Current->Previous = NULL;
		else if (context->Current->Next == removingWidget)
			context->Current->Next = NULL;

		if (removingWidget == context->Current)
			removingWidget = NULL;

		if (removingWidget != NULL)
		{
			//if (removingWidget->Instanced == 1) // To be revisited: not able to check if it is lazy allocated
			{
				Ft_Esd_Widget_Detach(removingWidget);
				Ft_Esd_Widget_SetActive(removingWidget, FT_FALSE);
				Ft_Esd_Widget_Disable(removingWidget);
				//printf("flags: %x, %d\n", removingWidget->Flags, removingWidget->Instanced);
			}
			//context->RemovingWidget = removingWidget;
		}

		context->NewIndex = (context->Index - (context->Index - index) * 2);
		if (context->Looping)
		{
			if (index == context->Count)
			{
				//printf("Reseting Max\n");
				index = 0;
				context->NewIndex = index + 1;
				int dScrollX = context->ScrollX - context->Current->LocalX;
				int dScrollXStart = context->ScrollXStart - context->Current->LocalX;
				context->Current->LocalX = Ft_Esd_ScrollSwitch_GetStartX(-1, context);
				context->Next->LocalX = Ft_Esd_ScrollSwitch_GetStartX(0, context);
				//printf(" x: %d %d\n",context->ScrollX, context->Current->LocalX + dScrollX );

				context->ScrollX = context->Current->LocalX + dScrollX;
				context->ScrollXStart = context->Current->LocalX + dScrollXStart;
				context->Overflow = 1;
			}
			else if (index == -1)
			{
				//printf("Reseting Min\n");
				index = context->Count - 1;
				context->NewIndex = context->Count - 2;
				int dScrollX = context->ScrollX - context->Current->LocalX;
				int dScrollXStart = context->ScrollXStart - context->Current->LocalX;
				context->Current->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Count, context);
				context->Previous->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Count - 1, context);
				//printf(" x: %d %d\n",context->ScrollX, context->Previous->LocalX + dScrollX );

				context->ScrollX = context->Current->LocalX + dScrollX;
				context->ScrollXStart = context->Current->LocalX + dScrollXStart;
				context->Overflow = -1;
			}
		}
		if (context->Looping)
		{
			if (context->NewIndex < 0)
				context->NewIndex += context->Count;
			if (context->NewIndex >= context->Count)
				context->NewIndex %= context->Count;
		}
		else
		{
			if (context->NewIndex < 0)
				context->NewIndex = index;
			if (context->NewIndex >= context->Count)
				context->NewIndex = index;
		}
		//context->NewIndex = context->NewIndex % context->Count;

		//printf("Index %d -> %d (new %d), ScrollXTarget %d\n", context->Index, index, context->NewIndex, Ft_Esd_ScrollSwitch_GetStartX(index, context));
		context->Index = index;
		context->ScrollXTarget = Ft_Esd_ScrollSwitch_GetStartX(context->Index, context);
		//printf("Touch up done %d", context->State);
	}
}
void Ft_Esd_ScrollSwitch_PreUpdate_Widget(Ft_Esd_ScrollSwitch *context)
{
	if (!(context->State & _Changing))
		return;
	context->State &= ~_Loading;

	//printf("Ft_Esd_ScrollSwitch_PreUpdate_Widget %d\n", context->State);
	if ((context->State & _ToRight) && (context->Next != NULL))
	{
		context->Previous = context->Current;
		context->Current = context->Next;
		context->Next = NULL;
		context->State |= _Loading;
	}
	else if ((context->State & _ToLeft) && (context->Previous != NULL))
	{
		context->Next = context->Current;
		context->Current = context->Previous;
		context->Previous = NULL;
		context->State |= _Loading;
	}

	context->Widget.Recalculate = FT_TRUE;
	context->ScrollXStart = context->ScrollX;
	context->State |= _Shifting;
	//printf("Ft_Esd_ScrollSwitch_PreUpdate_Widget Done %d\n", context->State);
}

void Ft_Esd_ScrollSwitch_Update_Widget(Ft_Esd_ScrollSwitch *context, Ft_Esd_Widget *newWidget)
{
	if (!(context->State & _Changing))
		return;

	if (!(context->State & _Loading) || (context->State & _Shifting))
		return;
	//printf("Ft_Esd_ScrollSwitch_Update_Widget\n");

	if (context->State & _ToRight)
	{
		context->Next = context->New(context->Owner);

		if (context->Next && context->Next != context->Current)
		{
			//Ft_Esd_Widget_SortToBottom(context->Next);
			//Ft_Esd_Widget_SetActive(context->Next, FT_TRUE);
			context->Next->GlobalValid = FT_FALSE;
			context->Next->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Index + 1, context);
			context->Next->LocalY = 0;
			//context->Next->GlobalValid = FT_FALSE;
			//printf("New widget x: %d [next]\n", context->Next->LocalX);
		}
	}
	else if (context->State & _ToLeft)
	{
		context->Previous = context->New(context->Owner);

		if (context->Previous && context->Previous != context->Current)
		{
			//Ft_Esd_Widget_SortToTop(context->Previous);

			context->Previous->GlobalValid = FT_FALSE;
			context->Previous->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Index - 1, context);
			context->Previous->LocalY = 0;
			//context->Previous->GlobalValid = FT_FALSE;
			//printf("New widget x: %d [previous]\n", context->Previous->LocalX);
		}
	}
	context->Widget.Recalculate = FT_TRUE;
	context->State &= ~(_Changing | _Loading);
}

void Ft_Esd_ScrollSwitch_SS_Initialise(Ft_Esd_ScrollSwitch *context)
{
	int unit = Ft_Esd_ScrollSwitch_GetUnit(context);

	if (context->Looping)
		context->FixedPositioning.Widget.LocalWidth = (unit) * (context->Count + 2) - context->Spacing;
	else
		context->FixedPositioning.Widget.LocalWidth = (unit) * (context->Count) + context->Spacing;
	//printf("Width= %d, %d\n", context->FixedPositioning.Widget.LocalWidth, context->Count + 2);

	context->Previous = context->InitialPrevious(context->Owner);
	context->Current = context->InitialCurrent(context->Owner);
	context->Next = context->InitialNext(context->Owner);
	context->Index = context->InitialIndex(context->Owner);

	//printf("Previous: %x\n", context->Previous);
	//printf("Current: %x\n", context->Current);
	//printf("Next: %x\n", context->Next);

	context->Previous->LocalY = 0;
	context->Current->LocalY = 0;
	context->Next->LocalY = 0;
	context->Previous->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Index - 1, context);
	context->Current->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Index, context);
	context->Next->LocalX = Ft_Esd_ScrollSwitch_GetStartX(context->Index + 1, context);

	//context->ScrollX = context->Current->LocalX;
	context->ScrollXTarget = context->Current->LocalX;
	//context->NewScrollX = context->Current->LocalX;
	context->State = (_Changing | _Recovering);
}

ESD_METHOD(Ft_Esd_ScrollSwitch_Render_Signal, Context = Ft_Esd_ScrollSwitch)
void Ft_Esd_ScrollSwitch_Render_Signal(Ft_Esd_ScrollSwitch *context)
{
	int minX = context->Widget.GlobalX;
	int maxX = context->Widget.GlobalX + context->Widget.GlobalWidth;
	if (!context->Touching)
	{
		if (context->Previous != NULL)
		{
			if (context->Previous->GlobalX + context->Widget.GlobalWidth < minX)
			{
				//printf("Skip Previous %d %d\n", context->Previous->GlobalX, context->Previous->GlobalWidth);
				context->Previous->GlobalValid = FT_FALSE;
			}
		}

		if (context->Next != NULL)
		{
			if (context->Next->GlobalX > maxX)
			{
				//printf("Skip Next %d %d\n", context->Next->GlobalX, context->Next->GlobalWidth);
				context->Next->GlobalValid = FT_FALSE;
			}
		}
	}
	else
	{
		if (context->Previous != NULL)
		{
			context->Previous->GlobalValid = (context->Previous->GlobalX + context->Widget.GlobalWidth >= minX);
		}
		if (context->Next != NULL)
		{
			context->Next->GlobalValid = (context->Next->GlobalX <= maxX);
		}
	}
	//context->Current->GlobalValid = FT_TRUE; // only render current
	//printf("\n");
	// ...
}
