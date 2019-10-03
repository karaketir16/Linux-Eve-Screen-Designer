/*
This file is automatically generated
DO NOT MODIFY BY HAND
Ft_Esd_Slider
Header
*/

#ifndef Ft_Esd_Slider__H
#define Ft_Esd_Slider__H

#include "Ft_Esd.h"
#include "Ft_Esd_Math.h"
#include "Ft_Esd_Theme.h"
#include "Ft_Esd_TouchTag.h"
#include "Ft_Esd_Widget.h"

#ifndef ESD_LOGIC
#define ESD_LOGIC(name, ...)
#define ESD_ACTOR(name, ...)
#define ESD_WIDGET(name, ...)
#define ESD_PAGE(name, ...)
#define ESD_APPLICATION(name, ...)
#define ESD_INPUT(name, ...)
#define ESD_OUTPUT(name, ...)
#define ESD_SLOT(name, ...)
#define ESD_SIGNAL(name, ...)
#define ESD_VARIABLE(name, ...)
#define ESD_WRITER(name, ...)
#endif

#define Ft_Esd_Slider_CLASSID 0x30E54FFB
ESD_SYMBOL(Ft_Esd_Slider_CLASSID, Type = esd_classid_t)

/* EVE builtin slider bar widget */
ESD_WIDGET(Ft_Esd_Slider, Include = "Ft_Esd_Slider.h", Callback, DisplayName = "ESD Slider", Category = EsdWidgets, Icon = ":/icons/ui-seek-bar.png", X = 50, Y = 50, Width = 160, Height = 20)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};
	ESD_INPUT(Theme, Type = Ft_Esd_Theme *, Default = Ft_Esd_Theme_GetCurrent)
	Ft_Esd_Theme *(* Theme)(void *context);
	ESD_INPUT(Alpha, Type = ft_uint8_t, Min = 0, Max = 255, SingleStep = 1, Default = 255)
	ft_uint8_t(* Alpha)(void *context);
	ESD_VARIABLE(PreviousValue, Type = ft_int16_t, Private)
	ft_int16_t PreviousValue;
	ESD_WRITER(Changed, Type = int)
	void(* Changed)(void *context, int value);
	ESD_VARIABLE(PreviousX, Type = ft_int16_t, Private)
	ft_int16_t PreviousX;
	/* Current slider value, must be between 0 and Range inclusive */
	ESD_VARIABLE(Value, Type = ft_int16_t, Public)
	ft_int16_t Value;
	ESD_VARIABLE(PreviousY, Type = ft_int16_t, Private)
	ft_int16_t PreviousY;
	/* Inclusive range of the slider value */
	ESD_INPUT(Min, Type = ft_int16_t)
	ft_int16_t(* Min)(void *context);
	/* Inclusive range of the slider value */
	ESD_INPUT(Max, Type = ft_int16_t, Default = 100)
	ft_int16_t(* Max)(void *context);
	Ft_Esd_TouchTag TouchTag;
} Ft_Esd_Slider;

void Ft_Esd_Slider__Initializer(Ft_Esd_Slider *context);

ESD_SLOT(Start)
void Ft_Esd_Slider_Start(Ft_Esd_Slider *context);

ESD_SLOT(End)
void Ft_Esd_Slider_End(Ft_Esd_Slider *context);

ESD_SLOT(Render)
void Ft_Esd_Slider_Render(Ft_Esd_Slider *context);

ESD_SLOT(Update)
void Ft_Esd_Slider_Update(Ft_Esd_Slider *context);

#endif /* Ft_Esd_Slider__H */

/* end of file */
