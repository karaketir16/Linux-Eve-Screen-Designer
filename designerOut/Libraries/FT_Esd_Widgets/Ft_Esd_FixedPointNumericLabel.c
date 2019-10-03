#include "Ft_Esd.h"
#include "Ft_Esd_FixedPointNumericLabel.h"

#define MAX_Digit (11) // when negative, 10 digits only
#define Size_Buffer (MAX_Digit + 3) // null char required, .

ESD_METHOD(Ft_Esd_FixedPointNumericLabel_ComputePosition, Context = Ft_Esd_FixedPointNumericLabel)
void Ft_Esd_FixedPointNumericLabel_ComputePosition(Ft_Esd_FixedPointNumericLabel *context)
{
	// ...
	void *owner = context->Owner;
	ft_uint16_t hOps = context->AlignX(owner);
	ft_bool_t isRight = hOps & OPT_ALIGN_RIGHT;
	ft_bool_t isCenter = hOps & OPT_ALIGN_CENTER;

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
		context->Y = context->Widget.GlobalY + context->Widget.GlobalHeight - context->FontHeight;
	}
	else //if (isVCenter)
	{
		context->Y = context->Widget.GlobalY + ((context->Widget.GlobalHeight + 1) >> 1);
		context->Option |= OPT_CENTERY;
	}
}

ESD_METHOD(Ft_Esd_FixedPointNumericLabel_UpdateNumberString, Context = Ft_Esd_FixedPointNumericLabel)
void Ft_Esd_FixedPointNumericLabel_UpdateNumberString(Ft_Esd_FixedPointNumericLabel *context)
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

	ft_float_t value = context->Value(owner);
	if (context->EnablePadding(owner) && context->NDigit > 0)
	{
		if (context->IsZeroLeading(owner))
		{ //  Zero Padding
			//eve_printf_debug("%0*.*f", context->NDigit, context->NDigitAfterDot, value);
			sprintf(context->Number, "%0*.*f", context->NDigit, context->NDigitAfterDot, value);
		}
		else
		{ // Space Padding
			//eve_printf_debug("% *.*f", context->NDigit, context->NDigitAfterDot, value);
			sprintf(context->Number, "% *.*f", context->NDigit, context->NDigitAfterDot, value);
		}
	}
	else
	{
		//eve_printf_debug("% *.*f\n", context->NDigit, context->NDigitAfterDot, value);
		sprintf(context->Number, "% *.*f", context->NDigit, context->NDigitAfterDot, value);
	}
}

ESD_METHOD(Ft_Esd_FixedPointNumericLabel_Start_Signal, Context = Ft_Esd_FixedPointNumericLabel)
void Ft_Esd_FixedPointNumericLabel_Start_Signal(Ft_Esd_FixedPointNumericLabel *context)
{
	// ...
	context->Number = malloc(Size_Buffer);
	//eve_printf_debug("allocated\n");
}

ESD_METHOD(Ft_Esd_FixedPointNumericLabel_End_Signal, Context = Ft_Esd_FixedPointNumericLabel)
void Ft_Esd_FixedPointNumericLabel_End_Signal(Ft_Esd_FixedPointNumericLabel *context)
{
	// ...
	free(context->Number);
	context->Number = 0;
	//eve_printf_debug("free\n");
}
