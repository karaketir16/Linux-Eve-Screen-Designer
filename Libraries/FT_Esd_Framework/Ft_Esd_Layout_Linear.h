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

#ifndef FT_ESD_LAYOUT_LINEAR_H
#define FT_ESD_LAYOUT_LINEAR_H

#include "Ft_Esd_Widget.h"

#define Ft_Esd_Layout_Linear_CLASSID 0xF3E88321
ESD_SYMBOL(Ft_Esd_Layout_Linear_CLASSID, Type = esd_classid_t)

// Layout which positions it's child widgets along one direction, one after the other
ESD_WIDGET(Ft_Esd_Layout_Linear, DisplayName = "Linear Layout", Icon = ":/icons/layout-3.png", Include = "Ft_Esd_Layout_Linear.h", Category = EsdLayouts, Width = 50, Height = 50, Callback, Layout)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};

	union
	{
		struct
		{
			ESD_VARIABLE(Margin, Type = ft_int16_t, Default = 0, Public)
			ft_int16_t Margin; // 16

			ESD_VARIABLE(Spacing, Type = ft_int16_t, Default = 4, Public)
			ft_int16_t Spacing; // 32

			// Layout orientation
			ESD_VARIABLE(Orientation, Type = Esd_Orientation, Default = ESD_ORIENTATION_VERTICAL, Public)
			ft_uint8_t Orientation; // 40

			ESD_VARIABLE(Align, Type = Esd_Align, Default = ESD_ALIGN_TOPFILL, Public)
			ft_uint8_t Align; // 48

			ESD_VARIABLE(Overflow, Type = Esd_Overflow, Default = ESD_OVERFLOW_ALLOW, Public)
			ft_uint8_t Overflow; // 56

			ESD_VARIABLE(Scissor, Type = ft_bool_t, Default = 0, Public)
			ft_bool_t Scissor : 1; // 57

			// ESD_VARIABLE(Reverse, Type = ft_bool_t, Default = 0, Public)
			// ft_bool_t Reverse : 1; // 58
		};

		ft_uint64_t Values;
	};

	ESD_VARIABLE(ChildClipping, Type = Esd_Clip, Default = ESD_CLIP_RENDER, Public)
	ft_uint8_t ChildClipping;

	// Automatically resize the layout to the total local size of it's contents. Useful when child layout of scroll panel. Occurs with one frame delay due to design constraints
	ESD_VARIABLE(AutoResize, Type = Esd_AutoResize, Default = ESD_AUTORESIZE_NONE, Public)
	ft_uint8_t AutoResize;

	ft_int64_t LastValues;

} Ft_Esd_Layout_Linear;

void Ft_Esd_Layout_Linear__Initializer(Ft_Esd_Layout_Linear *context);

ESD_SLOT(Update)
void Ft_Esd_Layout_Linear_Update(Ft_Esd_Layout_Linear *context);

ESD_SLOT(Render)
void Ft_Esd_Layout_Linear_Render(Ft_Esd_Layout_Linear *context);

ESD_SLOT(Idle)
void Ft_Esd_Layout_Linear_Idle(Ft_Esd_Layout_Linear *context);

#endif /* FT_ESD_LAYOUT_LINEAR_H */

/* end of file */
