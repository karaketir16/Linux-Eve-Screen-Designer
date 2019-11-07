#ifndef Ft_Esd_TouchTagRenderInterface__H
#define Ft_Esd_TouchTagRenderInterface__H

#include "Ft_Esd.h"
#include "Ft_Esd_TouchTag.h"
#include "Ft_Esd_Widget.h"

#ifndef ESD_LOGIC
#define ESD_LOGIC(name, ...)
#define ESD_ACTOR(name, ...)
#define ESD_WIDGET(name, ...)
#define ESD_UI(name, ...)
#define ESD_PAGE(name, ...)
#define ESD_APPLICATION(name, ...)
#define ESD_INPUT(name, ...)
#define ESD_OUTPUT(name, ...)
#define ESD_SLOT(name, ...)
#define ESD_SIGNAL(name, ...)
#define ESD_VARIABLE(name, ...)
#define ESD_WRITER(name, ...)
#endif

#define Ft_Esd_TouchTagRenderInterface_CLASSID 0x8F576E5E
ESD_SYMBOL(Ft_Esd_TouchTagRenderInterface_CLASSID, Type = esd_classid_t)

ESD_WIDGET(Ft_Esd_TouchTagRenderInterface, Include = "Ft_Esd_TouchTagRenderInterface.h", DisplayName = "Touch Tag Render", Icon = ":/icons/hand-point-090.png", Callback, Category = EsdUtilities, X = 0, Y = 0, Width = 400, Height = 300, Layout)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};
	ESD_SIGNAL(Rendering)
	void (*Rendering)(void *context);
	ESD_SIGNAL(Up)
	void (*Up)(void *context);
	ESD_SIGNAL(Down)
	void (*Down)(void *context);
	ESD_SIGNAL(Tap)
	void (*Tap)(void *context);
	Ft_Esd_TouchTag Touch_Tag;
} Ft_Esd_TouchTagRenderInterface;

void Ft_Esd_TouchTagRenderInterface__Initializer(Ft_Esd_TouchTagRenderInterface *context);

ESD_SLOT(Start)
void Ft_Esd_TouchTagRenderInterface_Start(Ft_Esd_TouchTagRenderInterface *context);

ESD_SLOT(Update)
void Ft_Esd_TouchTagRenderInterface_Update(Ft_Esd_TouchTagRenderInterface *context);

ESD_SLOT(End)
void Ft_Esd_TouchTagRenderInterface_End(Ft_Esd_TouchTagRenderInterface *context);

ESD_SLOT(Render)
void Ft_Esd_TouchTagRenderInterface_Render(Ft_Esd_TouchTagRenderInterface *context);

ESD_OUTPUT(Touching, Type = ft_bool_t)
ft_bool_t Ft_Esd_TouchTagRenderInterface_Touching(Ft_Esd_TouchTagRenderInterface *context);

ESD_OUTPUT(Inside, Type = ft_bool_t)
ft_bool_t Ft_Esd_TouchTagRenderInterface_Inside(Ft_Esd_TouchTagRenderInterface *context);

ESD_OUTPUT(TouchX, Type = int)
int Ft_Esd_TouchTagRenderInterface_TouchX(Ft_Esd_TouchTagRenderInterface *context);

ESD_OUTPUT(TouchY, Type = int)
int Ft_Esd_TouchTagRenderInterface_TouchY(Ft_Esd_TouchTagRenderInterface *context);

// Touch X delta position since last update, context may be NULL
ESD_OUTPUT(TouchXDelta, DisplayName = "Touch X Delta", Type = ft_int16_t)
ft_int16_t Ft_Esd_TouchTagRenderInterface_TouchXDelta(Ft_Esd_TouchTagRenderInterface *context);

// Touch Y delta position since last update, context may be NULL
ESD_OUTPUT(TouchYDelta, DisplayName = "Touch Y Delta", Type = ft_int16_t)
ft_int16_t Ft_Esd_TouchTagRenderInterface_TouchYDelta(Ft_Esd_TouchTagRenderInterface *context);

ESD_OUTPUT(Tag, Type = int)
int Ft_Esd_TouchTagRenderInterface_Tag(Ft_Esd_TouchTagRenderInterface *context);

#endif /* Ft_Esd_TouchTagRenderInterface__H */

/* end of file */
