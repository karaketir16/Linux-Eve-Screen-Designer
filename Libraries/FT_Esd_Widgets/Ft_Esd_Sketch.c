
#include "Ft_Esd.h"
#include "Ft_Esd_Sketch.h"

#include "FT_Esd_Primitives.h"

#include "Ft_Esd_CoCmd.h"

#include <stdio.h>

extern Ft_Gpu_Hal_Context_t *Ft_Esd_Host;
extern Ft_Esd_GpuAlloc *Ft_Esd_GAlloc;

ESD_METHOD(Ft_Esd_Sketch_Resize_Bitmap, Context = Ft_Esd_Sketch)
void Ft_Esd_Sketch_Resize_Bitmap(Ft_Esd_Sketch *context)
{
	// Get old bitmap options
	Ft_Esd_BitmapInfo *bitmapInfo = &context->BitmapInfo;
	ft_uint32_t oldAddr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
	Ft_Esd_BitmapInfo oldBitmapInfo;
	if (oldAddr != GA_INVALID)
	{
		oldBitmapInfo = *bitmapInfo;
	}

	// Set bitmap options
	ft_int16_t realWidth = max(0, max(context->Widget.GlobalWidth, context->MinWidth));
	ft_int16_t realHeight = max(0, max(context->Widget.GlobalHeight, context->MinHeight));
	bitmapInfo->Width = max(0, context->Widget.GlobalWidth);
	bitmapInfo->Height = max(0, context->Widget.GlobalHeight);
	bitmapInfo->Format = L8;
	bitmapInfo->Stride = realWidth;
	bitmapInfo->Size = (realWidth * realHeight);

	// Drop any previous memory allocation
	// Memory management clears the previous allocation after frame swap (should not clear memory while bitmap is still on display)
	bitmapInfo->GpuHandle.Id = MAX_NUM_ALLOCATIONS;
	bitmapInfo->GpuHandle.Seq = 0;
	bitmapInfo->BitmapHandle = ~0;

	// Set cell
	context->BitmapCell.Info = bitmapInfo;
	context->BitmapCell.Cell = 0;

	// Allocate or re-use previous allocation
	if (bitmapInfo->Size <= 0)
		return;

	ft_int16_t oldRealHeight;
	ft_int16_t oldRealWidth;
	ft_uint32_t addr = GA_INVALID;
	if (oldAddr != GA_INVALID)
	{
		oldRealHeight = oldBitmapInfo.Size / oldBitmapInfo.Stride;
		oldRealWidth = oldBitmapInfo.Stride;
		if (oldRealWidth == realWidth && oldRealHeight == realHeight)
		{
			bitmapInfo->GpuHandle = oldBitmapInfo.GpuHandle;
			addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
		}
	}
	if (addr == GA_INVALID)
	{
		bitmapInfo->GpuHandle = Ft_Esd_GpuAlloc_Alloc(Ft_Esd_GAlloc, bitmapInfo->Size, GA_GC_FLAG);
		addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
	}

	// Failed allocation
	if (addr == GA_INVALID)
	{
		// Revert to old
		if (oldAddr != GA_INVALID)
		{
			*bitmapInfo = oldBitmapInfo;
		}

		// Abort
		return;
	}

	// Copy or erase
	if (oldAddr == GA_INVALID)
	{
		Ft_Gpu_CoCmd_MemZero(Ft_Esd_Host, addr, bitmapInfo->Size);
	}
	else
	{
		int minHeight = min(oldRealHeight, realHeight);
		int minStride = min(oldRealWidth, realWidth);
		for (int i = 0; i < minHeight; ++i)
		{
			Ft_Gpu_CoCmd_MemCpy(Ft_Esd_Host,
			    addr + (i * realWidth),
			    oldAddr + (i * oldRealWidth),
			    minStride);
			if (realWidth > oldRealWidth)
			{
				Ft_Gpu_CoCmd_MemZero(Ft_Esd_Host,
				    addr + (i * realWidth) + oldRealWidth,
				    realWidth - oldRealWidth);
			}
		}
		for (int i = minHeight; i < realHeight; ++i)
		{
			Ft_Gpu_CoCmd_MemZero(Ft_Esd_Host,
			    addr + (i * realWidth),
			    realWidth);
		}
	}
}

ESD_METHOD(Ft_Esd_Sketch_Alloc_Bitmap, Context = Ft_Esd_Sketch)
void Ft_Esd_Sketch_Alloc_Bitmap(Ft_Esd_Sketch *context)
{
	Ft_Esd_BitmapInfo *bitmapInfo = &context->BitmapInfo;

	if (bitmapInfo->Size <= 0)
		return;

	ft_uint32_t addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
	if (addr == GA_INVALID)
	{
		// Allocate new memory if needed
		bitmapInfo->GpuHandle = Ft_Esd_GpuAlloc_Alloc(Ft_Esd_GAlloc, bitmapInfo->Size, GA_GC_FLAG);
		addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);

		if (addr != GA_INVALID)
		{
			// Successfully allocated memory for this, we can write now
			Ft_Gpu_CoCmd_MemZero(Ft_Esd_Host, addr, bitmapInfo->Size);
		}
	}
}

ESD_METHOD(Ft_Esd_Sketch_Clear_Bitmap, Context = Ft_Esd_Sketch)
void Ft_Esd_Sketch_Clear_Bitmap(Ft_Esd_Sketch *context)
{
	Ft_Esd_BitmapInfo *bitmapInfo = &context->BitmapInfo;

	// Drop any previous memory allocation
	// Memory management clears the previous allocation after frame swap (should not clear memory while bitmap is still on display)
	bitmapInfo->GpuHandle.Id = MAX_NUM_ALLOCATIONS;
	bitmapInfo->GpuHandle.Seq = 0;
	bitmapInfo->BitmapHandle = ~0;
}

ESD_METHOD(Ft_Esd_Sketch_Get_Addr, Type = ft_uint32_t, Context = Ft_Esd_Sketch)
ft_uint32_t Ft_Esd_Sketch_Get_Addr(Ft_Esd_Sketch *context)
{
	Ft_Esd_BitmapInfo *bitmapInfo = &context->BitmapInfo;
	return Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
}

ESD_METHOD(Ft_Esd_Sketch_CoCmd_Sketch, Context = Ft_Esd_Sketch)
void Ft_Esd_Sketch_CoCmd_Sketch(Ft_Esd_Sketch *context)
{
	Ft_Esd_BitmapInfo *bitmapInfo = &context->BitmapInfo;

	// Get address of memory from the handle
	ft_uint32_t addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
	if (addr == GA_INVALID)
		return;

	// Enable SKETCH mode
	Ft_Esd_CoCmd_Sketch(context,
	    context->Widget.GlobalRect,
	    addr, L8);
}

ESD_METHOD(Ft_Esd_Sketch_CoCmd_Stop, Context = Ft_Esd_Sketch)
void Ft_Esd_Sketch_CoCmd_Stop(Ft_Esd_Sketch *context)
{
	// Disable SKETCH mode
	Ft_Esd_CoCmd_Stop(context);
}

ESD_METHOD(Ft_Esd_Sketch_Render_Bitmap, Context = Ft_Esd_Sketch)
void Ft_Esd_Sketch_Render_Bitmap(Ft_Esd_Sketch *context)
{
	Ft_Esd_BitmapInfo *bitmapInfo = &context->BitmapInfo;

	// Get address of memory from the handle
	ft_uint32_t addr = Ft_Esd_GpuAlloc_Get(Ft_Esd_GAlloc, bitmapInfo->GpuHandle);
	if (addr == GA_INVALID || !bitmapInfo->Height || !bitmapInfo->Width)
		return;

	// Render the bitmap with the bitmapinfo and our provided memory contents
	Ft_Esd_Render_Bitmap(context->Widget.GlobalX, context->Widget.GlobalY, context->BitmapCell, context->PenColor);
}
