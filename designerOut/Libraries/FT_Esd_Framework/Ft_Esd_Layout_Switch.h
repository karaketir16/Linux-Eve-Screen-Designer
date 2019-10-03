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

#ifndef FT_ESD_LAYOUT_SWITCH_H
#define FT_ESD_LAYOUT_SWITCH_H

#include "Ft_Esd_Widget.h"

#define Ft_Esd_Layout_Switch_CLASSID 0x66221688
ESD_SYMBOL(Ft_Esd_Layout_Switch_CLASSID, Type = esd_classid_t)

// Layout which only keeps the topmost widget active, and automatically disables any other child widgets
ESD_WIDGET(Ft_Esd_Layout_Switch, DisplayName = "Switch Page", Icon = ":/icons/layers-stack-arrange.png", Include = "Ft_Esd_Layout_Switch.h", Category = EsdLayouts, Width = 50, Height = 50, Callback, Layout, BackToFront)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};

	// Show spinner when switching
	ESD_VARIABLE(Spinner, Type = ft_bool_t, Default = 1, Public)
	ft_bool_t Spinner;

	// Limit the contents of the layout to be rendered within the layout boundaries only
	//ESD_VARIABLE(Scissor, Type = ft_bool_t, Default = 0, Public)
	//ft_bool_t Scissor;

	// Automatically resize the layout to the local size of it's contents. Occurs with one frame delay due to design constraints
	ESD_VARIABLE(AutoResize, Type = ft_bool_t, Default = 0, Public)
	ft_bool_t AutoResize;

	Ft_Esd_Widget *Current;
	Ft_Esd_Widget *Next;

} Ft_Esd_Layout_Switch;

void Ft_Esd_Layout_Switch__Initializer(Ft_Esd_Layout_Switch *context);

ESD_SLOT(Update)
void Ft_Esd_Layout_Switch_Update(Ft_Esd_Layout_Switch *context);

ESD_SLOT(Render)
void Ft_Esd_Layout_Switch_Render(Ft_Esd_Layout_Switch *context);

ESD_SLOT(Idle)
void Ft_Esd_Layout_Switch_Idle(Ft_Esd_Layout_Switch *context);

#endif /* FT_ESD_LAYOUT_SWITCH_H */

/* end of file */
