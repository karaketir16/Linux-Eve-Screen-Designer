/**
* This source code ("the Software") is provided by Bridgetek Pte Ltd
* ("Bridgetek") subject to the licence terms set out
*   http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
* You must read the Licence Terms before downloading or using the Software.
* By installing or using the Software you agree to the Licence Terms. If you
* do not agree to the Licence Terms then do not download or use the Software.
*
* Without prejudice to the Licence Terms, here is a summary of some of the key
* terms of the Licence Terms (and in the event of any conflict between this
* summary and the Licence Terms then the text of the Licence Terms will
* prevail).
*
* The Software is provided "as is".
* There are no warranties (or similar) in relation to the quality of the
* Software. You use it at your own risk.
* The Software should not be used in, or for, any medical device, system or
* appliance. There are exclusions of Bridgetek liability for certain types of loss
* such as: special loss or damage; incidental loss or damage; indirect or
* consequential loss or damage; loss of income; loss of business; loss of
* profits; loss of revenue; loss of contracts; business interruption; loss of
* the use of money or anticipated savings; loss of information; loss of
* opportunity; loss of goodwill or reputation; and/or loss of, damage to or
* corruption of data.
* There is a monetary cap on Bridgetek's liability.
* The Software may have subsequently been amended by another user and then
* distributed by that other user ("Adapted Software").  If so that user may
* have additional licence terms that apply to those amendments. However, Bridgetek
* has no liability in relation to those amendments.
*/

#ifndef FT_ESD_WIDGET_H
#define FT_ESD_WIDGET_H

#include "Ft_Esd.h"
#include "Ft_Esd_Math.h"
#include "FT_Esd_Primitives.h"

ESD_CATEGORY(EsdWidgets, DisplayName = "ESD Widgets")
ESD_CATEGORY(EsdLayouts, DisplayName = "ESD Layouts")
ESD_CATEGORY(EsdLayoutUtilities, DisplayName = "Utilities", Category = EsdLayouts)
ESD_CATEGORY(EsdLayoutAdvanced, DisplayName = "Advanced", Category = EsdLayouts)

// Function pointer table for common rendering interface (32 bytes)
#define FT_ESD_WIDGET_INITIALIZE 0
#define FT_ESD_WIDGET_START 1
#define FT_ESD_WIDGET_ENABLE 2
#define FT_ESD_WIDGET_UPDATE 3
#define FT_ESD_WIDGET_RENDER 4
#define FT_ESD_WIDGET_IDLE 5
#define FT_ESD_WIDGET_DISABLE 6
#define FT_ESD_WIDGET_END 7
typedef struct Ft_Esd_WidgetSlots
{
	union
	{
		void (*Table[8])(void *context);
		struct
		{
			// Pointers to slot functions
			void (*Initialize)(void *context); // Initialize to default values
			void (*Start)(void *context); // Start any object functionality after value are initialized
			void (*Enable)(void *context); // Called between active/inactive switch
			void (*Update)(void *context);
			void (*Render)(void *context);
			void (*Idle)(void *context);
			void (*Disable)(void *context);
			void (*End)(void *context);
		};
	};

} Ft_Esd_WidgetSlots;

// Class ID for base widget
#define Ft_Esd_Widget_CLASSID 0x1EA612C1
ESD_SYMBOL(Ft_Esd_Widget_CLASSID, Type = esd_classid_t)

// Class ID for generated widgets using ESD_RENDER functions
#define Ft_Esd_Render_CLASSID 0x02976CC4
ESD_SYMBOL(Ft_Esd_Render_CLASSID, Type = esd_classid_t)

// Base class used for all widgets (56 bytes)
typedef struct Ft_Esd_Widget
{
	// The owner context used for callbacks
	void *Owner;

	// Hash of class name for identification purpose
	esd_classid_t ClassId;

	// Local positioning within the layout. Set by the user or by the widget itself. Must call Ft_Esd_Widget_PostLocalRect after changing while inside a layout, or more safely call Ft_Esd_Widget_SetRect to change this
	union
	{
		struct
		{
			ft_int16_t LocalX, LocalY, LocalWidth, LocalHeight;
		};
		Ft_Esd_Rect16 LocalRect;
	};

	// Global positioning on the screen. Set by the parent layout. Must call Ft_Esd_Widget_PostLocalRect after changing
	union
	{
		struct
		{
			ft_int16_t GlobalX, GlobalY, GlobalWidth, GlobalHeight;
		};
		Ft_Esd_Rect16 GlobalRect;
	};

	// Pointers to slot functions
	Ft_Esd_WidgetSlots *Slots;

	// Parent layout
	struct Ft_Esd_Widget *Parent;

	// Linked list of sibling objects
	struct Ft_Esd_Widget *Previous;
	struct Ft_Esd_Widget *Next;

	// Linked list of child objects
	struct Ft_Esd_Widget *First;
	struct Ft_Esd_Widget *Last;

	// Container widget for user added entries to the layout
	struct Ft_Esd_Widget *Container;

	// Flags
	union
	{
		struct
		{
			ft_bool_t Active : 1; // Widget is actively being displayed, call Ft_Esd_Widget_SetActive to change this
			ft_bool_t Recalculate : 1; // Widget layout needs to be recalculated. Set to 1 when user changes LocalRect on child object. Set to 1 when parent changes GlobalRect. Always recalculates all child GlobalRect variables
			ft_bool_t Root : 1; // Widget with this flag set is controlled manually and cannot be added into any layout
			ft_bool_t Instanced : 1; // Widget is allocated dynamically in memory and needs to be deleted
			ft_bool_t GlobalValid : 1; // Global rectangle has been set by the parent layout
		};
		ft_uint32_t Flags;
	};

} Ft_Esd_Widget;

ESD_TYPE(Ft_Esd_Widget *, Native = Pointer, Edit = None)

// Call after changing LocalRect variables
void Ft_Esd_Widget_PostLocalRect(Ft_Esd_Widget *context);

// Call after changing GlobalRect variables
void Ft_Esd_Widget_PostGlobalRect(Ft_Esd_Widget *context);

void Ft_Esd_Widget_IterateChildSlot(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildSlotReverse(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildActiveSlot(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildActiveSlotReverse(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildActiveValidSlot(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildActiveValidSlotReverse(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildVisibleSlot(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildVisibleSlotReverse(Ft_Esd_Widget *context, int slot);
void Ft_Esd_Widget_IterateChildClippedSlot(Ft_Esd_Widget *context, int slot, ft_bool_t (*visible)(Ft_Esd_Widget *));
void Ft_Esd_Widget_IterateChildClippedSlotReverse(Ft_Esd_Widget *context, int slot, ft_bool_t (*visible)(Ft_Esd_Widget *));

void Ft_Esd_Widget__Initializer(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Initialize(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Start(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Enable(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Update(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Render(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Idle(Ft_Esd_Widget *context);
void Ft_Esd_Widget_Disable(Ft_Esd_Widget *context);
void Ft_Esd_Widget_End(Ft_Esd_Widget *context);

// Detach the widget from the current parent layout. Implicitly disables the widget
ESD_FUNCTION(Ft_Esd_Widget_Detach, DisplayName = "Detach", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_Detach(Ft_Esd_Widget *context);

void Ft_Esd_Widget_LocalInsertTop(Ft_Esd_Widget *context, Ft_Esd_Widget *parent);
void Ft_Esd_Widget_LocalInsertAbove(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling);
void Ft_Esd_Widget_LocalInsertBelow(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling);
void Ft_Esd_Widget_LocalInsertBottom(Ft_Esd_Widget *context, Ft_Esd_Widget *parent);

// Sort the widget to the top within the current parent layout
ESD_FUNCTION(Ft_Esd_Widget_SortToTop, DisplayName = "Sort to Top", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_SortToTop(Ft_Esd_Widget *context);

// Sort the widget above it's current sibling within the current parent layout
ESD_FUNCTION(Ft_Esd_Widget_SortHigher, DisplayName = "Sort Higher", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_SortHigher(Ft_Esd_Widget *context);

// Sort the widget below it's current sibling within the current parent layout
ESD_FUNCTION(Ft_Esd_Widget_SortLower, DisplayName = "Sort Lower", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_SortLower(Ft_Esd_Widget *context);

// Sort the widget to the bottom within the current parent layout
ESD_FUNCTION(Ft_Esd_Widget_SortToBottom, DisplayName = "Sort to Bottom", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_SortToBottom(Ft_Esd_Widget *context);

// Returns FT_TRUE if the widget is the topmost widget within the current parent layout
ESD_FUNCTION(Ft_Esd_Widget_IsTop, Type = ft_bool_t, DisplayName = "Is Top", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
ft_bool_t Ft_Esd_Widget_IsTop(Ft_Esd_Widget *context);

// Returns FT_TRUE if the widget is the bottom widget within the current parent layout
ESD_FUNCTION(Ft_Esd_Widget_IsBottom, Type = ft_bool_t, DisplayName = "Is Bottom", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
ft_bool_t Ft_Esd_Widget_IsBottom(Ft_Esd_Widget *context);

// Layouting function to get the actual container widget which will be used by a widget as layout parent for child widgets
Ft_Esd_Widget *Ft_Esd_Widget_GetContainer(Ft_Esd_Widget *context);

// Insert a widget in the top position of the specified parent layout
ESD_FUNCTION(Ft_Esd_Widget_InsertTop, DisplayName = "Insert at Top", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
ESD_PARAMETER(parent, DisplayName = "Parent", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_InsertTop(Ft_Esd_Widget *context, Ft_Esd_Widget *parent);

// Insert a widget above the specified sibling widget in the sibling widget's parent layout
ESD_FUNCTION(Ft_Esd_Widget_InsertAbove, DisplayName = "Insert Above", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
ESD_PARAMETER(sibling, DisplayName = "Sibling", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_InsertAbove(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling);

// Insert a widget below the specified sibling widget in the sibling widget's parent layout
ESD_FUNCTION(Ft_Esd_Widget_InsertBelow, DisplayName = "Insert Below", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
ESD_PARAMETER(sibling, DisplayName = "Sibling", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_InsertBelow(Ft_Esd_Widget *context, Ft_Esd_Widget *sibling);

// Insert a widget in the bottom position of the specified parent layout
ESD_FUNCTION(Ft_Esd_Widget_InsertBottom, DisplayName = "Insert at Bottom", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, DisplayName = "Widget", Type = Ft_Esd_Widget *)
ESD_PARAMETER(parent, DisplayName = "Parent", Type = Ft_Esd_Widget *)
void Ft_Esd_Widget_InsertBottom(Ft_Esd_Widget *context, Ft_Esd_Widget *parent);

// Change a widget's active state
void Ft_Esd_Widget_SetActive(Ft_Esd_Widget *context, ft_bool_t active);
ft_bool_t Ft_Esd_Widget_GetActive(Ft_Esd_Widget *context);

// Change a widget's relative positioning
void Ft_Esd_Widget_SetX(Ft_Esd_Widget *context, ft_int16_t x);
void Ft_Esd_Widget_SetY(Ft_Esd_Widget *context, ft_int16_t y);
void Ft_Esd_Widget_SetWidth(Ft_Esd_Widget *context, ft_int16_t width);
void Ft_Esd_Widget_SetHeight(Ft_Esd_Widget *context, ft_int16_t height);
void Ft_Esd_Widget_SetRect(Ft_Esd_Widget *context, Ft_Esd_Rect16 rect);

// Set GlobalRect on a widget. Called by layouting widgets on child widgets during Recalculate
void Ft_Esd_Widget_SetGlobalRect(Ft_Esd_Widget *context, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height);

// Scroll GlobalRect on a widget. Called by scrolling layouting widgets on child widgets during Recalculate. Uses recursive positioning update to scroll to bypass full recalculation
void Ft_Esd_Widget_ScrollGlobalRect(Ft_Esd_Widget *context, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height);

// Get GlobalRect from a widget. Called by the designer simulation. Equivalent to GlobalX etc
void Ft_Esd_Widget_GetGlobalRect(Ft_Esd_Widget *context, ft_int16_t *x, ft_int16_t *y, ft_int16_t *width, ft_int16_t *height);

// Safe way to free a widget. Must already be deactivated and ended. Uses a queue to free while not iterating through slots
void Ft_Esd_Widget_Free(Ft_Esd_Widget *context);

// Check if the widget is within the current screen scissor area
ESD_FUNCTION(Ft_Esd_Widget_IsVisible, Type = ft_bool + t, DisplayName = "Is Visible", Category = EsdLayoutUtilities)
ESD_PARAMETER(context, Type = Ft_Esd_Widget *)
ft_bool_t Ft_Esd_Widget_IsVisible(Ft_Esd_Widget *context);

ESD_FUNCTION(Ft_Esd_Widget_GetGlobalX, Type = ft_int16_t, DisplayName = "Get Global X", Category = EsdLayoutUtilities, Macro)
ESD_PARAMETER(context, Type = Ft_Esd_Widget *)
#define Ft_Esd_Widget_GetGlobalX(context) (((Ft_Esd_Widget *)context)->GlobalX)

ESD_FUNCTION(Ft_Esd_Widget_GetGlobalY, Type = ft_int16_t, DisplayName = "Get Global Y", Category = EsdLayoutUtilities, Macro)
ESD_PARAMETER(context, Type = Ft_Esd_Widget *)
#define Ft_Esd_Widget_GetGlobalY(context) (((Ft_Esd_Widget *)context)->GlobalY)

ESD_FUNCTION(Ft_Esd_Widget_GetGlobalWidth, Type = ft_int16_t, DisplayName = "Get Global Width", Category = EsdLayoutUtilities, Macro)
ESD_PARAMETER(context, Type = Ft_Esd_Widget *)
#define Ft_Esd_Widget_GetGlobalWidth(context) (((Ft_Esd_Widget *)context)->GlobalWidth)

ESD_FUNCTION(Ft_Esd_Widget_GetGlobalHeight, Type = ft_int16_t, DisplayName = "Get Global Height", Category = EsdLayoutUtilities, Macro)
ESD_PARAMETER(context, Type = Ft_Esd_Widget *)
#define Ft_Esd_Widget_GetGlobalHeight(context) (((Ft_Esd_Widget *)context)->GlobalHeight)

ESD_ENUM(Esd_AutoResize, Type = ft_uint8_t)
#define ESD_AUTORESIZE_NONE (0)
#define ESD_AUTORESIZE_WIDTH (1)
#define ESD_AUTORESIZE_HEIGHT (2)
#define ESD_AUTORESIZE_BOTH (ESD_AUTORESIZE_WIDTH | ESD_AUTORESIZE_HEIGHT)
ESD_END()

ESD_ENUM(Esd_Overflow, Type = ft_uint8_t)
#define ESD_OVERFLOW_ALLOW (0)
#define ESD_OVERFLOW_CLIP (1)
#define ESD_OVERFLOW_FILL (2)
// #define ESD_OVERFLOW_ALLOWX_ALLOWY
ESD_END()

ESD_ENUM(Esd_Clip, Type = ft_uint8_t, Flags)
#define ESD_CLIP_RENDER (1)
#define ESD_CLIP_UPDATE (2)
#define ESD_CLIP_IDLE (4)
ESD_END()

ESD_ENUM(Esd_Visible, Type = ft_uint8_t)
#define ESD_VISIBLE_NEVER (0)
#define ESD_VISIBLE_WHENNEEDED (1)
#define ESD_VISIBLE_ALWAYS (2)
ESD_END()

ESD_ENUM(Esd_CircularGradientType, Type = ft_uint8_t)
#define ESD_PYTHAGOREAN (0)
#define ESD_GAUSSIAN (1)
ESD_END()

#endif /* FT_ESD_WIDGET_H */

/* end of file */
