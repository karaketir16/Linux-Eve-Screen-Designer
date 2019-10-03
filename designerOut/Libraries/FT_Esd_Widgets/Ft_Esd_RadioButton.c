#include "Ft_Esd.h"
#include "Ft_Esd_RadioButton.h"

static Ft_Esd_RadioButton *Ft_Esd_RadioButton_GlobalChecked = 0;

ESD_METHOD(Ft_Esd_RadioButton_Check_Signal, Context = Ft_Esd_RadioButton)
void Ft_Esd_RadioButton_Check_Signal(Ft_Esd_RadioButton *context)
{
	Ft_Esd_RadioGroup *rg = context->RadioGroup(context->Owner);
	if (rg)
		rg->Checked = context;
	else
		Ft_Esd_RadioButton_GlobalChecked = context;
}

ESD_METHOD(Ft_Esd_RadioButton_IsChecked, Context = Ft_Esd_RadioButton, Type = ft_bool_t)
ft_bool_t Ft_Esd_RadioButton_IsChecked(Ft_Esd_RadioButton *context)
{
	Ft_Esd_RadioGroup *rg = context->RadioGroup(context->Owner);
	if (rg)
		return rg->Checked == context;
	else
		return Ft_Esd_RadioButton_GlobalChecked == context;
}
