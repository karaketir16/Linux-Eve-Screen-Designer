#include "Ft_Esd.h"
#include "Ft_Esd_ArcSlider.h"

extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;

void Ft_Esd_ArcSlider_Update_TouchTracker_NoWrap(Ft_Esd_ArcSlider *context)
{
	int step = (context->TrackAngle - context->StartTrackAngle) / 1024;
	int currAngle = context->StartAngle;
	int newAngle = currAngle;
	ft_int16_t max = context->ESD_Partial_Ring.MaxAngle(context->ESD_Partial_Ring.Owner);
	ft_int16_t min = 0;
	while (step > 180)
	{
		step -= 360;
	}
	while (step < -180)
	{
		step += 360;
	}

	int adjustAngle = (!!context->Clockwise(context->Owner)) ? (currAngle + step) : (currAngle - step);
	if (adjustAngle > max)
	{
		adjustAngle = max;
		context->IsOutOfBound = FT_TRUE;
	}
	else if (adjustAngle < min)
	{
		adjustAngle = min;
		context->IsOutOfBound = FT_TRUE;
	}
	else if (currAngle != adjustAngle)
	{
		context->IsOutOfBound = FT_FALSE;
	}
	//if (step != 0) 	//eve_printf_debug(" Start: %d, Pre: %d, Track: %d, StartTrack: %d, ", context->StartAngle, context->PreAngle, context->TrackAngle, context->StartTrackAngle);
	context->NewAngle = newAngle;
	context->ClampedNewAngle = adjustAngle;
	context->StartAngle = context->ClampedNewAngle;
	context->StartTrackAngle += step * 1024;
	//if (step != 0) 	//eve_printf_debug(" step: %d, new: %d, adjust: %d ", step, newAngle, adjustAngle, context->PreAngle);
}

ESD_METHOD(Ft_Esd_ArcSlider_Update_TouchTracker, Context = Ft_Esd_ArcSlider)
ESD_PARAMETER(tag, Type = ft_int16_t, Default = 255)
void Ft_Esd_ArcSlider_Update_TouchTracker(Ft_Esd_ArcSlider *context, ft_int16_t tag)
{
	// ...
	void *owner = context->Owner;
	int x = context->Widget.GlobalX + (context->Widget.GlobalWidth >> 1);
	int y = context->Widget.GlobalY + (context->Widget.GlobalHeight >> 1);
	//eve_printf_debug("\n x: %d, y: %d tag: %d", x, y, tag);
	Ft_Gpu_CoCmd_Track(Ft_Esd_Host, x, y, 1, 1, tag);

	ft_uint32_t tmp = Ft_Gpu_Hal_Rd32(Ft_Esd_Host, REG_TRACKER);
	int tagId = tmp & 0xFF;

	if (tagId == tag)
	{
		int angle = ((tmp >> (16 + 4)) * 90 + (90 << 10)) % (360 << 10);
		context->TrackAngle = angle;
		//eve_printf_debug(" angle: %d,[%d] tagRd: %d ", angle, tmp >> (16 + 4), tagId);
		if (context->IsTracking)
		{
			Ft_Esd_ArcSlider_Update_TouchTracker_NoWrap(context);
		}
		else
		{
			context->IsTracking = FT_TRUE;
			context->StartTrackAngle = context->TrackAngle;
			if (context->ClampedNewAngle < 0)
				context->ClampedNewAngle += 360;
			else if (context->ClampedNewAngle > 360)
				context->ClampedNewAngle -= 360;
			context->StartAngle = context->ClampedNewAngle;
			//eve_printf_debug(" >> Start: %d, track: %d\n", context->StartAngle, context->StartTrackAngle);
		}
	}
}
