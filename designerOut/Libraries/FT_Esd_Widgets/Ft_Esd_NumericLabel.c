#include "Ft_Esd.h"
#include "Ft_Esd_NumericLabel.h"

#define MAX_Digit (11) // when negative, 10 digits only
#define Size_Buffer (MAX_Digit + 3) // null char required, 0x for hex

ESD_METHOD(Ft_Esd_NumericLabel_ComputePosition, Context = Ft_Esd_NumericLabel)
void Ft_Esd_NumericLabel_ComputePosition(Ft_Esd_NumericLabel *context)
{
	// ...
	void *owner = context->Owner;
	if (!context->Number)
		return;

	ft_uint16_t hOps = context->AlignX(owner);
	ft_bool_t isRight = hOps & OPT_ALIGN_RIGHT;
	ft_bool_t isCenter = hOps & OPT_ALIGN_CENTER;
	//eve_printf_debug(" isRight: %d  isCenter: %d ", isRight, isCenter);
	context->Option = 0;

	if (!isRight && !isCenter)
	{
		context->X = context->Widget.GlobalX;
	}
	else if (isRight)
	{
		context->Option = OPT_RIGHTX;
		context->X = context->Widget.GlobalX + context->Widget.GlobalWidth;
	}
	else //if (isCenter)
	{
		context->Option = OPT_CENTERX;
		context->X = context->Widget.GlobalX + (context->Widget.GlobalWidth >> 1);
	}

	ft_uint16_t vOps = context->AlignY(owner);
	ft_bool_t isVBottom = vOps & OPT_ALIGN_BOTTOM;
	ft_bool_t isVCenter = vOps & OPT_ALIGN_CENTER;
	ft_bool_t isVTop = !isVBottom & !isVCenter;
	if (isVTop)
	{
		context->Y = context->Widget.GlobalY;
	}
	else if (isVBottom)
	{
		context->Y = context->Widget.GlobalY + context->Widget.GlobalHeight;
	}
	else //if (isVCenter)
	{
		context->Y = context->Widget.GlobalY + (context->Widget.GlobalHeight >> 1);
		context->Option |= OPT_CENTERY;
	}
}

ESD_METHOD(Ft_Esd_NumericLabel_UpdateNumberString, Context = Ft_Esd_NumericLabel)
void Ft_Esd_NumericLabel_UpdateNumberString(Ft_Esd_NumericLabel *context)
{
	void *owner = context->Owner;

	if (context->NDigit > MAX_Digit)
	{
		context->NDigit = MAX_Digit;
	}
	else if (context->NDigit < 0)
	{
		context->NDigit = 0;
	}

	int value = context->Value(owner);
	ft_bool_t isHex = context->IsHexDisplay(owner);
	if (context->EnablePadding(owner) && context->NDigit > 0)
	{
		if (context->IsZeroLeading(owner))
		{ //  Zero Padding
			//eve_printf_debug((isHex) ? "0x%0*X\n" : "%0*d\n", context->NDigit, value);
			eve_sprintf(context->Number, (isHex) ? "0x%0*X" : "%0*d", context->NDigit, value);
		}
		else
		{ // Space Padding
			//eve_printf_debug((isHex) ? "0x% *X\n": "% *d\n", context->NDigit, value);
			eve_sprintf(context->Number, (isHex) ? "0x% *X" : "% *d", context->NDigit, value);
		}
	}
	else
	{
		//eve_printf_debug((isHex) ? "0x%X\n" : "%d\n", value);
		eve_sprintf(context->Number, (isHex) ? "0x%X" : "%d", value);
	}
}

ESD_METHOD(Ft_Esd_NumericLabel_Start_Signal, Context = Ft_Esd_NumericLabel)
void Ft_Esd_NumericLabel_Start_Signal(Ft_Esd_NumericLabel *context)
{
	// ...
	context->Number = malloc(Size_Buffer);
	//eve_printf_debug("allocated\n");
}

ESD_METHOD(Ft_Esd_NumericLabel_End_Signal, Context = Ft_Esd_NumericLabel)
void Ft_Esd_NumericLabel_End_Signal(Ft_Esd_NumericLabel *context)
{
	// ...
	free(context->Number);
	context->Number = 0;
	//eve_printf_debug("free\n");
}
