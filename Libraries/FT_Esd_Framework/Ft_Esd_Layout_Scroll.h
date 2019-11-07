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

#ifndef FT_ESD_LAYOUT_SCROLL_H
#define FT_ESD_LAYOUT_SCROLL_H

#include "Ft_Esd_Widget.h"

#define Ft_Esd_Layout_Scroll_CLASSID 0xE08B661F
ESD_SYMBOL(Ft_Esd_Layout_Scroll_CLASSID, Type = esd_classid_t)

// Layout which positions it's child widgets according to the specified alignment and scroll distance
ESD_WIDGET(Ft_Esd_Layout_Scroll, DisplayName = "Scroll Layout", Icon = ":/icons/ui-scroll-pane.png", Include = "Ft_Esd_Layout_Scroll.h", Category = EsdLayouts, Width = 50, Height = 50, Callback, Layout, BackToFront)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};

	ESD_VARIABLE(MinX, Type = ft_int16_t, Default = 0, ReadOnly)
	ft_int16_t MinX;

	ESD_VARIABLE(MaxX, Type = ft_int16_t, Default = 0, ReadOnly)
	ft_int16_t MaxX;

	ESD_VARIABLE(MinY, Type = ft_int16_t, Default = 0, ReadOnly)
	ft_int16_t MinY;

	ESD_VARIABLE(MaxY, Type = ft_int16_t, Default = 0, ReadOnly)
	ft_int16_t MaxY;

	union
	{
		struct
		{
			// Horizontal scroll value
			ESD_VARIABLE(ScrollX, Type = ft_int16_t, Default = 0, Public)
			ft_int16_t ScrollX;

			// Vertical scroll value
			ESD_VARIABLE(ScrollY, Type = ft_int16_t, Default = 0, Public)
			ft_int16_t ScrollY; // 32

			ESD_VARIABLE(Align, Type = Esd_Align, Default = ESD_ALIGN_TOPFILL, Public)
			ft_uint8_t Align; // 48
		};

		ft_uint64_t Values;
	};

	// Limit the contents of the layout to be rendered within the layout boundaries only
	ESD_VARIABLE(Scissor, Type = ft_bool_t, Default = 0, Public)
	ft_bool_t Scissor;

	ESD_WRITER(ScrollXChanged, Type = ft_int16_t)
	void (*ScrollXChanged)(void *context, ft_int16_t value);

	ESD_WRITER(ScrollYChanged, Type = ft_int16_t)
	void (*ScrollYChanged)(void *context, ft_int16_t value);

	union
	{
		struct
		{
			ft_int16_t LastScrollX;
			ft_int16_t LastScrollY; // 32
		};
		ft_int64_t LastValues;
	};

} Ft_Esd_Layout_Scroll;

void Ft_Esd_Layout_Scroll__Initializer(Ft_Esd_Layout_Scroll *context);

ESD_SLOT(Update)
void Ft_Esd_Layout_Scroll_Update(Ft_Esd_Layout_Scroll *context);

ESD_SLOT(Render)
void Ft_Esd_Layout_Scroll_Render(Ft_Esd_Layout_Scroll *context);

ESD_SLOT(Idle)
void Ft_Esd_Layout_Scroll_Idle(Ft_Esd_Layout_Scroll *context);

ESD_OUTPUT(RangeX, Type = ft_uint16_t)
ft_uint16_t Ft_Esd_Layout_Scroll_RangeX(Ft_Esd_Layout_Scroll *context);

ESD_OUTPUT(RangeY, Type = ft_uint16_t)
ft_uint16_t Ft_Esd_Layout_Scroll_RangeY(Ft_Esd_Layout_Scroll *context);

ESD_OUTPUT(Width, Type = ft_int16_t)
ft_int16_t Ft_Esd_Layout_Scroll_Width(Ft_Esd_Layout_Scroll *context);

ESD_OUTPUT(Height, Type = ft_int16_t)
ft_int16_t Ft_Esd_Layout_Scroll_Height(Ft_Esd_Layout_Scroll *context);

#endif /* FT_ESD_LAYOUT_SCROLL_H */

/* end of file */
