
#include "Ft_Esd_DefaultTheme.h"
#include "Ft_Esd_Theme.h"

#include "Ft_Esd_Core.h"

Ft_Esd_Theme Ft_Esd_DefaultTheme = {
	.ClearColor = 0x212121,
	.BackColor = 0x151515,
	.TextColor = 0xFFFFFF,
	.ButtonTextColor = 0xFFFFFF,
	.DefaultColor = 0x6B6B6B,
	.PrimaryColor = 0xFF7F3F
};

static Ft_Esd_Theme *s_DefaultTheme = &Ft_Esd_DefaultTheme;

void Ft_Esd_Theme_SetCurrent(Ft_Esd_Theme *theme)
{
	if (!theme)
		theme = &Ft_Esd_DefaultTheme;

	s_DefaultTheme = theme;
	Esd_CurrentContext->ClearColor = theme->ClearColor;
}

Ft_Esd_Theme *Ft_Esd_Theme_GetCurrent()
{
	return s_DefaultTheme;
}

/* Nothing beyond this */
