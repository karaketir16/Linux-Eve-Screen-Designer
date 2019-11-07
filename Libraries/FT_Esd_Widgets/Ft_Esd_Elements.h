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

#ifndef FT_ESD_ELEMENTS_H
#define FT_ESD_ELEMENTS_H

#include "Ft_Esd.h"
#include "Ft_Esd_Theme.h"

// Panel element, sunken, follows the same alignment and 3D shading effect as CMD_PROGRESS and CMD_SLIDER
ESD_RENDER(Ft_Esd_Elements_PanelSunken, DisplayName = "Ft_Esd_Elements_PanelSunken", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(color, Type = ft_rgb32_t, Default = "#151515")
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(width, Type = ft_int16_t, Default = 80, Min = 0, Max = 4095)
ESD_PARAMETER(height, Type = ft_int16_t, Default = 60, Min = 0, Max = 4095)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 4, Min = 0, Max = 255)
void Ft_Esd_Elements_PanelSunken(ft_rgb32_t color, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height, ft_int16_t radius);

// Panel element, raised, follows the same alignment and 3D shading effect as CMD_BUTTON
ESD_RENDER(Ft_Esd_Elements_PanelRaised, DisplayName = "Ft_Esd_Elements_PanelRaised", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(color, Type = ft_rgb32_t, Default = "#6b6b6b")
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(width, Type = ft_int16_t, Default = 80, Min = 0, Max = 4095)
ESD_PARAMETER(height, Type = ft_int16_t, Default = 60, Min = 0, Max = 4095)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 4, Min = 0, Max = 255)
void Ft_Esd_Elements_PanelRaised(ft_rgb32_t color, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height, ft_int16_t radius);

// Circle element, sunken, follows the same alignment and 3D shading effect as CMD_CLOCK
ESD_RENDER(Ft_Esd_Elements_CircleSunken, DisplayName = "Ft_Esd_Elements_CircleSunken", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(color, Type = ft_argb32_t, Default = "#FF151515")
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 20, Min = 0, Max = 255)
void Ft_Esd_Elements_CircleSunken(ft_argb32_t color, ft_int16_t x, ft_int16_t y, ft_int16_t radius);

// Circle element, raised
ESD_RENDER(Ft_Esd_Elements_CircleRaised, DisplayName = "Ft_Esd_Elements_CircleRaised", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(color, Type = ft_argb32_t, Default = "#FF6b6b6b")
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 20, Min = 0, Max = 255)
void Ft_Esd_Elements_CircleRaised(ft_argb32_t color, ft_int16_t x, ft_int16_t y, ft_int16_t radius);

// Circle element, flat
ESD_RENDER(Ft_Esd_Elements_CircleFlat, DisplayName = "Ft_Esd_Elements_CircleFlat", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(color, Type = ft_argb32_t, Default = "#FF151515")
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 20, Min = 0, Max = 255)
void Ft_Esd_Elements_CircleFlat(ft_argb32_t color, ft_int16_t x, ft_int16_t y, ft_int16_t radius);

ESD_FUNCTION(Ft_Esd_Char_RightTriangle, Type = char *, Category = _GroupHidden, Inline)
inline static char *Ft_Esd_Char_RightTriangle() { return "\x10"; }

ESD_FUNCTION(Ft_Esd_Char_LeftTriangle, Type = char *, Category = _GroupHidden, Inline)
inline static char *Ft_Esd_Char_LeftTriangle() { return "\x11"; }

ESD_FUNCTION(Ft_Esd_Char_UpTriangle, Type = char *, Category = _GroupHidden, Inline)
inline static char *Ft_Esd_Char_UpTriangle() { return "\x1E"; }

ESD_FUNCTION(Ft_Esd_Char_DownTriangle, Type = char *, Category = _GroupHidden, Inline)
inline static char *Ft_Esd_Char_DownTriangle() { return "\x1F"; }

// Panel element, with "raised" option
ESD_RENDER(Ft_Esd_Elements_Panel, DisplayName = "Ft_Esd_Elements_Panel", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(theme, Type = Ft_Esd_Theme *, Default = Ft_Esd_Theme_GetCurrent)
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(width, Type = ft_int16_t, Default = 80, Min = 0, Max = 4095)
ESD_PARAMETER(height, Type = ft_int16_t, Default = 60, Min = 0, Max = 4095)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 4, Min = 0, Max = 255)
ESD_PARAMETER(raised, Type = ft_bool_t, Default = 0)
void Ft_Esd_Elements_Panel(Ft_Esd_Theme *theme, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height, ft_int16_t radius, ft_bool_t raised);

// Panel element, with "raised" option
ESD_RENDER(Ft_Esd_Elements_Panel_Color, DisplayName = "Ft_Esd_Elements_Panel_Color", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(color, Type = ft_argb32_t, Default = 0xFFFFFFFF)
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(width, Type = ft_int16_t, Default = 80, Min = 0, Max = 4095)
ESD_PARAMETER(height, Type = ft_int16_t, Default = 60, Min = 0, Max = 4095)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 4, Min = 0, Max = 255)
ESD_PARAMETER(raised, Type = ft_bool_t, Default = 0)
void Ft_Esd_Elements_Panel_Color(ft_argb32_t color, ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height, ft_int16_t radius, ft_bool_t raised);

// TODO: Backward compatibility feature
// Example: ESD_RENAME(Ft_Esd_Elements_PanelOld, To = Ft_Esd_Elements_PanelNew)

// Gradient Panel element, with "raised" option
ESD_RENDER(Ft_Esd_Elements_Gradient_Panel, DisplayName = "Ft_Esd_Elements_Gradient_Panel", Icon = ":/icons/ui-panel.png", Category = EsdElements)
ESD_PARAMETER(x, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(y, Type = ft_int16_t, Default = 0, Min = -2048, Max = 2047)
ESD_PARAMETER(width, Type = ft_int16_t, Default = 80, Min = 0, Max = 4095)
ESD_PARAMETER(height, Type = ft_int16_t, Default = 60, Min = 0, Max = 4095)
ESD_PARAMETER(radius, Type = ft_int16_t, Default = 4, Min = 0, Max = 255)
ESD_PARAMETER(color1, Type = ft_rgb32_t, Default = "#2F2F2F2F")
ESD_PARAMETER(color2, Type = ft_rgb32_t, Default = "#FFFFFFFF")
ESD_PARAMETER(direction, Type = ft_int16_t, Default = 90, Min = -720, Max = 720)
ESD_PARAMETER(raised, Type = ft_bool_t, Default = 0)
void Ft_Esd_Elements_Gradient_Panel(ft_int16_t x, ft_int16_t y, ft_int16_t width, ft_int16_t height, ft_int16_t radius, ft_rgb32_t color1, ft_rgb32_t color2, ft_int16_t direction, ft_bool_t raised);

// Geometry adjustment for slider and progressbar
void Ft_Esd_Adjust_Geometry(ft_int16_t *x, ft_int16_t *y, ft_int16_t *w, ft_int16_t *h,
    ft_uint16_t wMin, ft_uint16_t hMin, ft_uint16_t wMargin, ft_uint16_t hMargin);

#endif /* FT_ESD_ELEMENTS_H */

/* end of file */
