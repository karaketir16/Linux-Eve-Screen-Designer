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

#include "EVE_HalImpl.h"
#include "EVE_Platform.h"
#include "FT_DataTypes.h"
#include <time.h>
#include <stdint.h>
#if defined(Linux_PLATFORM)

#include "colibri_spi.h"
#include "pinCtl.h"

/*********
** INIT **
*********/

EVE_HalPlatform g_HalPlatform;

/* Initialize HAL platform */
void EVE_HalImpl_initialize()
{
	g_HalPlatform.TotalDevices = 1;
}

/* Release HAL platform */
void EVE_HalImpl_release()
{
//	spi_uninit(SPIM);
}

/* Get the default configuration parameters */
void EVE_HalImpl_defaults(EVE_HalParameters *parameters)
{

}

/* Opens a new HAL context using the specified parameters */
bool EVE_HalImpl_open(EVE_HalContext *phost, EVE_HalParameters *parameters)
{
    pinCtl_pd_connect();
    pinCtl_cs_connect();

    pinCtl_pdSet(HIGH);
    SPI_init(SPI_PATH, SPI_CLOCK_SPEED);
    uint8_t dummyTx = 0;
    uint8_t dummyRx = 0;
    SPI_transfer(&dummyTx, &dummyRx, 1);

    pinCtl_pdSet(LOW);
    usleep(300*1000);

    pinCtl_pdSet(HIGH);
    usleep(300*1000);
    SPI_init(SPI_PATH, SPI_CLOCK_SPEED);
    /* Initialize the context valriables */
    phost->SpiDummyBytes = 1;//by default ft800/801/810/811 goes with single dummy byte for read
    phost->SpiChannels = 0;

    phost->Status = EVE_STATUS_OPENED;

}

/* Close a HAL context */
void EVE_HalImpl_close(EVE_HalContext *phost)
{

}

/* Idle. Call regularly to update frequently changing internal state */
void EVE_HalImpl_idle(EVE_HalContext *phost)
{

}

/*************
** TRANSFER **
*************/

void EVE_Hal_startTransfer(EVE_HalContext *phost, EVE_TRANSFER_T rw, uint32_t addr)
{
	eve_assert(phost->Status == EVE_STATUS_OPENED);

    pinCtl_csSet(LOW);
    if (rw == EVE_TRANSFER_READ)
    {
        ft_uint8_t tx[4];
        ft_uint8_t rx[4];


        tx[0] = (ft_uint8_t) (addr >> 16);
        tx[1] = (ft_uint8_t) (addr >> 8);
        tx[2] = (ft_uint8_t) (addr);

        tx[3] = 0; //Dummy Read byte



        SPI_transfer(tx, rx, sizeof(tx));
        phost->Status = FT_GPU_HAL_READING;
    }else{


        ft_uint8_t tx[3];
        ft_uint8_t rx[3];

        tx[0] = (0x80|(addr >> 16));
        tx[1] = addr >> 8;
        tx[2] = addr;

        SPI_transfer(tx, rx, sizeof(tx));
        phost->Status = FT_GPU_HAL_WRITING;
    }

}


void EVE_Hal_endTransfer(EVE_HalContext *phost)
{
    eve_assert(phost->Status == EVE_STATUS_READING || phost->Status == EVE_STATUS_WRITING);

    pinCtl_csSet(HIGH);
    phost->Status = EVE_STATUS_OPENED;
}

void EVE_Hal_flush(EVE_HalContext *phost)
{
	eve_assert(phost->Status == EVE_STATUS_OPENED);
	/* no-op */
}

static inline void rdBuffer(EVE_HalContext *phost, uint8_t *buffer, uint32_t size)
{
    while (size--) {
       (*buffer) = EVE_Hal_transfer8(phost,0);
       buffer++;
    }
}

static inline void wrBuffer(EVE_HalContext *phost, const uint8_t *buffer, uint32_t size)
{
    while (size--) {
       EVE_Hal_transfer8(phost,*buffer);
       buffer++;
    }
}

static inline uint8_t transfer8(EVE_HalContext *phost, uint8_t value)
{
    ft_uint8_t tx = value;
    ft_uint8_t rx = 0;

    if (phost->Status == EVE_STATUS_READING)
    {
        SPI_transfer(&tx, &rx, sizeof(tx));
        return rx;
    }
    else
    {
        SPI_transfer(&tx, &rx, sizeof(tx));
        return 0;
    }

}

uint8_t EVE_Hal_transfer8(EVE_HalContext *phost, uint8_t value)
{
    return transfer8(phost, value);
}

uint16_t EVE_Hal_transfer16(EVE_HalContext *phost, uint16_t value)
{
    uint8_t buffer[2];
    if (phost->Status == EVE_STATUS_READING)
    {
        rdBuffer(phost, buffer, 2);
        return (uint16_t)buffer[0]
            | (uint16_t)buffer[1] << 8;
    }
    else
    {
        buffer[0] = value & 0xFF;
        buffer[1] = value >> 8;
        wrBuffer(phost, buffer, 2);
        return 0;
    }
}

uint32_t EVE_Hal_transfer32(EVE_HalContext *phost, uint32_t value)
{
    uint8_t buffer[4];
    if (phost->Status == EVE_STATUS_READING)
    {
        rdBuffer(phost, buffer, 4);
        return (uint32_t)buffer[0]
            | (uint32_t)buffer[1] << 8
            | (uint32_t)buffer[2] << 16
            | (uint32_t)buffer[3] << 24;
    }
    else
    {
        buffer[0] = value & 0xFF;
        buffer[1] = (value >> 8) & 0xFF;
        buffer[2] = (value >> 16) & 0xFF;
        buffer[3] = value >> 24;
        wrBuffer(phost, buffer, 4);
        return 0;
    }
}

void EVE_Hal_transferMem(EVE_HalContext *phost, uint8_t *result, const uint8_t *buffer, uint32_t size)
{
	if (!size)
		return;

	if (result && buffer)
	{
		/* not implemented */
		eve_debug_break();
	}
	else if (result)
	{
        rdBuffer(phost, result, size);
	}
	else if (buffer)
	{
        wrBuffer(phost, buffer, size);
	}
}

void EVE_Hal_transferProgmem(EVE_HalContext *phost, uint8_t *result, eve_progmem_const uint8_t *buffer, uint32_t size)
{
	if (!size)
		return;

	if (result && buffer)
	{
		/* not implemented */
		eve_debug_break();
	}
	else if (result)
	{
		/* not implemented */
		eve_debug_break();
	}
	else if (buffer)
	{
		eve_assert(!((uintptr_t)buffer & 0x3)); // must be 32-bit aligned
		eve_assert(!(size & 0x3)); // must be 32-bit aligned
		eve_progmem_const uint32_t *buf32 = (eve_progmem_const uint32_t *)(void eve_progmem_const *)buffer;
		size >>= 2;
		while (size--)
		{
			uint32_t value = *(buf32++);
			wrBuffer(phost, (uint8_t *)(&value), 4);
		}
	}
}

uint32_t EVE_Hal_transferString(EVE_HalContext *phost, const char *str, uint32_t index, uint32_t size, uint32_t padMask)
{
	if (!size)
	{
		/* TODO: Support different padding options */
		eve_assert(padMask == 0x3);
		EVE_Hal_transfer32(phost, 0);
		return 4;
	}

	eve_assert(size <= EVE_CMD_STRING_MAX);
	uint32_t transferred = 0;
	if (phost->Status == EVE_STATUS_WRITING)
	{
		transferred += (uint32_t)strnlen(str, size) + 1;
		eve_assert(str[transferred - 1] == '\0');
		wrBuffer(phost, str, transferred);
		if (transferred & padMask)
		{
			uint32_t pad = 4 - (transferred & padMask);
			uint8_t padding[4] = { 0 };
			wrBuffer(phost, padding, pad);
			transferred += pad;
			eve_assert(!(transferred & 0x3));
		}
	}
	else
	{
		/* not implemented */
		eve_debug_break();
	}
	return transferred;
}

/************
** UTILITY **
************/

void EVE_Hal_hostCommand(EVE_HalContext *phost, uint8_t cmd)
{
	eve_assert(phost->Status == EVE_STATUS_OPENED);

    uint8_t hcmd[4] = { 0 };
    hcmd[0] = cmd;
    hcmd[1] = 0;
    hcmd[2] = 0;
    hcmd[3] = 0;

    ft_uint8_t rx[3];
    pinCtl_csSet(LOW);
    SPI_transfer(hcmd, rx, sizeof(hcmd));
    pinCtl_csSet(HIGH);
}

void EVE_Hal_hostCommandExt3(EVE_HalContext *phost, uint32_t cmd)
{
	eve_assert(phost->Status == EVE_STATUS_OPENED);

	uint8_t hcmd[4] = { 0 };
	hcmd[0] = cmd & 0xff;
	hcmd[1] = (cmd >> 8) & 0xff;
	hcmd[2] = (cmd >> 16) & 0xff;
	hcmd[3] = 0;

    ft_uint8_t rx[3];
    pinCtl_csSet(LOW);
    SPI_transfer(hcmd, rx, sizeof(hcmd));
    pinCtl_csSet(HIGH);

}

void EVE_Hal_powerCycle(EVE_HalContext *phost, bool up)
{
    if (up)
    {
        pinCtl_pdSet(LOW);
        EVE_sleep(20);

        pinCtl_pdSet(HIGH);
        EVE_sleep(20);
    }else
    {
        pinCtl_pdSet(HIGH);
        EVE_sleep(20);

        pinCtl_pdSet(LOW);
        EVE_sleep(20);
    }
}

void EVE_Hal_setSPI(EVE_HalContext *phost, EVE_SPI_CHANNELS_T numchnls, uint8_t numdummy)
{
#if (EVE_MODEL < EVE_FT810)
    return;
#else

    phost->SpiChannels = numchnls;
    phost->SpiDummyBytes = numdummy;

    ft_uint8_t writebyte = 0;
    /* error check */
    if((numchnls > FT_GPU_SPI_QUAD_CHANNEL) || (numdummy > FT_GPU_SPI_TWODUMMY) || (numdummy < FT_GPU_SPI_ONEDUMMY))
    {
        return ;//error
    }

    phost->SpiChannels = numchnls;
    writebyte = phost->SpiChannels;
    phost->SpiDummyBytes = numdummy;

    if(FT_GPU_SPI_TWODUMMY == phost->SpiDummyBytes)
    {
        writebyte |= FT_SPI_TWO_DUMMY_BYTE;
    }
    Ft_Gpu_Hal_Wr8(phost,REG_SPI_WIDTH,writebyte);
    /* set the parameters in hal context and also set into ft81x */
    return ;
#endif
}

uint32_t EVE_Hal_currentFrequency(EVE_HalContext *phost)
{
    ft_uint32_t t0, t1;
    ft_uint32_t addr = REG_CLOCK;
    ft_uint8_t spidata[4];
    ft_int32_t r = 15625;

    t0 = Ft_Gpu_Hal_Rd32(phost,REG_CLOCK); /* t0 read */
    usleep(r);
    t1 = Ft_Gpu_Hal_Rd32(phost,REG_CLOCK); /* t1 read */
    return ((t1 - t0) * 64);
}

/*********
** MISC **
*********/

static void initSdHost()
{

}

void EVE_Mcu_initialize()
{

}

void EVE_Mcu_release()
{
	/* no-op */
}

static int64_t s_TotalMilliseconds = 0;
static int64_t start;

void EVE_Millis_initialize()
{
    start = (int64_t) clock();
}

void EVE_Millis_release()
{

}


uint32_t EVE_millis()
{
    return ((int64_t) ((((double) ((int64_t)clock() - start)) / CLOCKS_PER_SEC) * 1000));
}


void EVE_sleep(uint32_t us)
{
    usleep(us * 1000);
}


bool EVE_UtilImpl_bootupDisplayGpio(EVE_HalContext *phost)
{
	return true;
}



#endif /* #if defined(ColibriiMX6_PLATFORM) */

/* end of file */
