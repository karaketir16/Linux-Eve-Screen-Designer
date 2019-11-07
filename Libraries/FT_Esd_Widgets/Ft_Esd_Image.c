#include "Ft_Esd.h"
#include "Ft_Esd_Image.h"
#include "FT_Esd_Primitives.h"

ESD_METHOD(Ft_Esd_Image_Render_Bitmap, Context = Ft_Esd_Image)
void Ft_Esd_Image_Render_Bitmap(Ft_Esd_Image *context)
{
	Ft_Esd_BitmapCell bitmapCell = context->BitmapCell(context->Owner);
	if (!bitmapCell.Info)
		return;
	Ft_Esd_Size16 bitmapSize;
	bitmapSize.Width = (bitmapCell.Info->Width * context->ScaleX) >> 16;
	bitmapSize.Height = (bitmapCell.Info->Height * context->ScaleY) >> 16;
	Ft_Esd_Size16 scaledSize = Ft_Esd_Primitive_GetScaledSize(
	    context->Widget.GlobalRect.Size, bitmapSize, context->Scaling);
	Ft_Esd_Rect16 alignedRect = Ft_Esd_Primitive_GetAlignedRect(
	    context->Widget.GlobalRect.Size, scaledSize, context->Align);
	Ft_Esd_Render_BitmapFreeform(bitmapCell, context->Color, context->Widget.GlobalRect, alignedRect, context->MinAlpha(context->Owner));
	if (context->Scaling == ESD_SCALING_FIT)
	{
		if (context->AutoResize & ESD_AUTORESIZE_WIDTH)
		{
			ft_int32_f16_t ratio = (((ft_int32_f16_t)scaledSize.Width) << 16) / (ft_int32_f16_t)scaledSize.Height;
			Ft_Esd_Widget_SetWidth(&context->Widget, (((ft_int32_f16_t)context->Widget.GlobalHeight) * ratio) >> 16);
		}
		if (context->AutoResize & ESD_AUTORESIZE_HEIGHT)
		{
			ft_int32_f16_t ratio = (((ft_int32_f16_t)scaledSize.Height) << 16) / (ft_int32_f16_t)scaledSize.Width;
			Ft_Esd_Widget_SetHeight(&context->Widget, (((ft_int32_f16_t)context->Widget.GlobalWidth) * ratio) >> 16);
		}
	}
	else
	{
		if (context->AutoResize & ESD_AUTORESIZE_WIDTH)
			Ft_Esd_Widget_SetWidth(&context->Widget, scaledSize.Width);
		if (context->AutoResize & ESD_AUTORESIZE_HEIGHT)
			Ft_Esd_Widget_SetHeight(&context->Widget, scaledSize.Height);
	}
}
