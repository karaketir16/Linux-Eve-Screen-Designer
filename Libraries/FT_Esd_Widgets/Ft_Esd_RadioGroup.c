#include "Ft_Esd.h"
#include "Ft_Esd_RadioGroup.h"

ESD_TYPE(Ft_Esd_RadioGroup *, Native = Pointer, Edit = None)
ESD_TYPE(Ft_Esd_RadioButton *, Native = Pointer, Edit = None)

ESD_METHOD(Ft_Esd_RadioGroup_Reset_Signal, Context = Ft_Esd_RadioGroup)
void Ft_Esd_RadioGroup_Reset_Signal(Ft_Esd_RadioGroup *context)
{
	context->Checked = 0;
}

ESD_METHOD(Ft_Esd_RadioGroup_GetPointer, Context = Ft_Esd_RadioGroup, Type = Ft_Esd_RadioGroup *)
Ft_Esd_RadioGroup *Ft_Esd_RadioGroup_GetPointer(Ft_Esd_RadioGroup *context)
{
	return context;
}
