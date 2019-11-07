#include "Ft_Esd.h"
#include "Ft_Esd_Timer.h"
#include <stdio.h>

ft_uint32_t Ft_Esd_GetMillis();

#define FT_ESD_TIMER_MAXNB 64
typedef struct
{
	ft_int32_t TimeMs;
	Ft_Esd_Timer *Timer;
} Ft_Esd_TimerEntry;
Ft_Esd_TimerEntry Ft_Esd_TimerEntries[FT_ESD_TIMER_MAXNB];
int Ft_Esd_TimerEntryNb = 0;

void Ft_Esd_Timer_RunGlobal(Ft_Esd_Timer *timer, ft_int32_t timeMs)
{
	if (Ft_Esd_TimerEntryNb < FT_ESD_TIMER_MAXNB)
	{
		int i = 0;
		for (i = 0; i < Ft_Esd_TimerEntryNb; ++i)
		{
			if ((timeMs - Ft_Esd_TimerEntries[i].TimeMs) >= 0)
			{
				break;
			}
		}
		for (int j = Ft_Esd_TimerEntryNb - 1; j >= i; --j)
		{
			Ft_Esd_TimerEntries[j + 1] = Ft_Esd_TimerEntries[j];
		}
		Ft_Esd_TimerEntries[i].Timer = timer;
		Ft_Esd_TimerEntries[i].TimeMs = timeMs;
		++Ft_Esd_TimerEntryNb;
	}
}

void Ft_Esd_Timer_UpdateGlobal()
{
	if (Ft_Esd_TimerEntryNb)
	{
		ft_int32_t timeMs = (ft_int32_t)Ft_Esd_GetMillis();
		do
		{
			int i = Ft_Esd_TimerEntryNb - 1;
			if ((timeMs - Ft_Esd_TimerEntries[i].TimeMs) >= 0)
			{
				Ft_Esd_Timer *timer = Ft_Esd_TimerEntries[i].Timer;
				timer->Fired(timer->Owner);
				--Ft_Esd_TimerEntryNb;
				if (timer->Repeat && timer->TimeoutMs)
				{
					Ft_Esd_Timer_RunGlobal(timer, timer->TimeoutMs + Ft_Esd_TimerEntries[i].TimeMs);
				}
			}
			else
			{
				break;
			}
		} while (Ft_Esd_TimerEntryNb);
	}
}

void Ft_Esd_Timer_CancelGlobal()
{
	Ft_Esd_TimerEntryNb = 0;
}

ESD_METHOD(Ft_Esd_Timer_End_Signal, Context = Ft_Esd_Timer)
void Ft_Esd_Timer_End_Signal(Ft_Esd_Timer *context)
{
	if (Ft_Esd_TimerEntryNb)
	{
		int i;
		for (i = 0; i < Ft_Esd_TimerEntryNb; ++i)
		{
			if (Ft_Esd_TimerEntries[i].Timer == context)
			{
				break;
			}
		}
		if (i < Ft_Esd_TimerEntryNb)
		{
			--Ft_Esd_TimerEntryNb;
			for (; i < Ft_Esd_TimerEntryNb; ++i)
			{
				Ft_Esd_TimerEntries[i] = Ft_Esd_TimerEntries[i + 1];
			}
		}
	}
}

ESD_METHOD(Ft_Esd_Timer_Run_Signal, Context = Ft_Esd_Timer)
void Ft_Esd_Timer_Run_Signal(Ft_Esd_Timer *context)
{
	Ft_Esd_Timer_End_Signal(context);
	Ft_Esd_Timer_RunGlobal(context, context->TimeoutMs + (ft_int32_t)Ft_Esd_GetMillis());
}
