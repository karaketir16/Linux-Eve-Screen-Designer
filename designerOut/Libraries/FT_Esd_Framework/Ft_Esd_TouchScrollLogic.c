#include "Ft_Esd.h"
#include "Ft_Esd_TouchScrollLogic.h"

#define DAccFactor (0.92f)
#define FactorNewValue (0.2f)
#define FactorOldValue (0.8f)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Compute2ndOrder(double *newValue, double *oldValue, double *d, double *oldD, double *acc);
static void SetRecord(double *current, double *previous);
static void SetDifference(double *dif, double *valueNew, double *valueOld);
static void LimitValue(double *value, const int min, const int max);
static void HandleXTouchChange(Ft_Esd_TouchScrollLogic *context);
static void HandleYTouchChange(Ft_Esd_TouchScrollLogic *context);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

ESD_METHOD(Ft_Esd_TouchScrollLogic_TouchUpdate_Then, Context = Ft_Esd_TouchScrollLogic)
//ESD_PARAMETER(xOffsetStart, Type = int, Default = 0)
//ESD_PARAMETER(yOffsetStart, Type = int, Default = 0)
void Ft_Esd_TouchScrollLogic_TouchUpdate_Then(Ft_Esd_TouchScrollLogic *context)
{
	void *owner = context->Owner;
	//eve_printf_debug("Switch: X %d, Y %d\n",context->EnableXScroll(owner), context->EnableYScroll(owner));
	if (context->EnableXScroll(owner))
		HandleXTouchChange(context);
	if (context->EnableYScroll(owner))
		HandleYTouchChange(context);
}

ESD_METHOD(Ft_Esd_TouchScrollLogic_Check_X_acceleration_Then, Context = Ft_Esd_TouchScrollLogic)
void Ft_Esd_TouchScrollLogic_Check_X_acceleration_Then(Ft_Esd_TouchScrollLogic *context)
{
	void *owner = context->Owner;
	int accRounded = (int)context->speedXAvg;
	if (accRounded != 0)
	{
		SetRecord(&context->dX, &context->OldDx);
		double newAcc = context->speedXAvg * DAccFactor;
		double newDx = newAcc + context->dX;
		context->speedXAvg = newAcc;
		context->dX = newDx;
		double newXRounded = (context->dX + context->lastOffsetX);
		LimitValue(&newXRounded, context->MinX(owner), context->MaxX(owner));
		ft_bool_t changed = context->offsetX != newXRounded;
		context->offsetX = (int)newXRounded;
		if (changed)
			context->isTouchChanged |= 1;
	}
}

ESD_METHOD(Ft_Esd_TouchScrollLogic_Check_Y_acceleration_Then, Context = Ft_Esd_TouchScrollLogic)
void Ft_Esd_TouchScrollLogic_Check_Y_acceleration_Then(Ft_Esd_TouchScrollLogic *context)
{
	void *parent = context->Owner;
	int accRounded = (int)context->speedYAvg;
	if (accRounded != 0)
	{
		SetRecord(&context->dY, &context->OldDy);
		double newAcc = context->speedYAvg * DAccFactor;
		double newDy = newAcc + context->dY;
		context->speedYAvg = newAcc;
		context->dY = newDy;
		double newYRounded = (context->dY + context->lastOffsetY);
		LimitValue(&newYRounded, context->MinY(parent), context->MaxY(parent));
		ft_bool_t changed = context->offsetY != newYRounded;
		context->offsetY = (int)newYRounded;
		if (changed)
			context->isTouchChanged |= 1;
	}
}

static void HandleXTouchChange(Ft_Esd_TouchScrollLogic *context)
{
	void *parent = context->Owner;
	// Handle X
	double newX = Ft_Esd_TouchArea_TouchX(&context->Touch_Area);
	//eve_printf_debug("(%f, %f, %f, %f, %f)\n", newX, context->lastX, context->dX, context->OldDx, context->aX);
	Compute2ndOrder(&newX, &context->lastX, &(context->dX), &(context->OldDx), &(context->aX));
	context->speedXAvg = context->aX * FactorNewValue + context->speedXAvg * FactorOldValue;
	//eve_printf_debug("(%f, %f, %f, %f, %f)\n", newX, context->lastX, context->dX, context->OldDx, context->aX);
	//eve_printf_debug("lastOffsetX : %f\n", context->lastOffsetX);
	//eve_printf_debug("dX : %f\n", context->dX);

	double newXRounded = (context->dX + context->lastOffsetX);
	//eve_printf_debug("raw Offset X : %f\n", newXRounded);
	LimitValue(&newXRounded, context->MinX(parent), context->MaxX(parent));
	//eve_printf_debug(" > %f, %d, %d\n", newXRounded, context->MinX(parent), context->MaxX(parent));
	int dif = abs((int)(newXRounded - context->offsetX));
	if (dif > context->OffsetThreshold(parent))
	{
		context->offsetX = (int)newXRounded;
		//eve_printf_debug("Offset X : %f\n\n\n", context->offsetX);
		if (dif > 0)
			context->isTouchChanged |= 1;
	}
}

static void HandleYTouchChange(Ft_Esd_TouchScrollLogic *context)
{
	void *parent = context->Owner;
	// Handle Y
	double newY = Ft_Esd_TouchArea_TouchY(&context->Touch_Area);
	//eve_printf_debug("(%f, %f, %f, %f, %f)\n", newY, context->lastY, context->dY, context->OldDx, context->aY);
	Compute2ndOrder(&newY, &context->lastY, &(context->dY), &(context->OldDx), &(context->aY));
	context->speedYAvg = context->aY * FactorNewValue + context->speedYAvg * FactorOldValue;
	//eve_printf_debug("(%f, %f, %f, %f, %f)\n", newY, context->lastY, context->dY, context->OldDx, context->aY);
	//eve_printf_debug("lastOffsetY : %f\n", context->lastOffsetY);
	//eve_printf_debug("dY : %f\n", context->dY);
	double newYRounded = (context->dY + context->lastOffsetY);
	//eve_printf_debug("raw Offset Y : %f\n", newYRounded);

	LimitValue(&newYRounded, context->MinY(parent), context->MaxY(parent));
	int dif = abs((int)(newYRounded - context->offsetY));
	if (dif > context->OffsetThreshold(parent))
	{
		context->offsetY = (int)newYRounded;
		//eve_printf_debug("Offset Y : %f\n\n\n", context->offsetY);
		if (dif > 0)
			context->isTouchChanged |= 1;
	}
}

static void Compute2ndOrder(double *newValue, double *oldValue, double *d, double *oldD, double *acc)
{
	SetRecord(d, oldD);
	SetDifference(d, newValue, oldValue);
	SetDifference(acc, d, oldD);
}

static void SetRecord(double *current, double *previous)
{
	*previous = *current;
}

static void SetDifference(double *dif, double *valueNew, double *valueOld)
{
	*dif = *valueNew - *valueOld;
}

static void LimitValue(double *value, const int min, const int max)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}
