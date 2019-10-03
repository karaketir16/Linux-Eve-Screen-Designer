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

#ifndef FT_ESD_LAYOUT_FILL_H
#define FT_ESD_LAYOUT_FILL_H

#include "Ft_Esd_Widget.h"

#define Ft_Esd_Layout_Fill_CLASSID 0xBC16215B
ESD_SYMBOL(Ft_Esd_Layout_Fill_CLASSID, Type = esd_classid_t)

// Layout which positions all it's child widgets to the full size of the layout itself
ESD_WIDGET(Ft_Esd_Layout_Fill, DisplayName = "Fill Layout", Include = "Ft_Esd_Layout_Fill.h", Icon = ":/icons/layout-select-content.png", Category = EsdLayouts, Width = 50, Height = 50, Callback, Layout, BackToFront)
typedef struct
{
	union
	{
		void *Owner;
		Ft_Esd_Widget Widget;
	};
} Ft_Esd_Layout_Fill;

void Ft_Esd_Layout_Fill__Initializer(Ft_Esd_Layout_Fill *context);

#endif /* FT_ESD_LAYOUT_FILL_H */

/* end of file */
