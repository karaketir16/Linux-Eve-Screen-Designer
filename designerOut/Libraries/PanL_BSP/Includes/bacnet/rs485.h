/*####COPYRIGHTBEGIN####
 -------------------------------------------
 Copyright (C) 2004 Steve Karg

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to:
 The Free Software Foundation, Inc.
 59 Temple Place - Suite 330
 Boston, MA  02111-1307
 USA.

 As a special exception, if other files instantiate templates or
 use macros or inline functions from this file, or you compile
 this file and link it with other works to produce a work based
 on this file, this file does not by itself cause the resulting
 work to be covered by the GNU General Public License. However
 the source code for this file must still be made available in
 accordance with section (3) of the GNU General Public License.

 This exception does not invalidate any other reasons why a work
 based on this file might be covered by the GNU General Public
 License.
 -------------------------------------------
####COPYRIGHTEND####*/

#ifndef RS485_H
#define RS485_H

#include <stdint.h>
#include "mstp.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
//#if defined (__FT900__)
//#define GPIO_UART0_TX 	48
//#define GPIO_UART0_RX 	49
//#define GPIO_UART1_TX 	52
//#define GPIO_UART1_RX 	53
//#elif defined (__FT930__)
//#define GPIO_UART0_TX 	23
//#define GPIO_UART0_RX 	22
//#define GPIO_UART1_TX 	27
//#define GPIO_UART1_RX 	26
//#endif
#define UART_NO		1

#if (UART_NO == 1)
#define UART_ID 		UART1
#define UART_SYS_DEV	sys_device_uart1
#define UART_INT_NUM	interrupt_uart1
#define GPIO_UART_RX	GPIO_UART1_RX
#define GPIO_UART_TX	GPIO_UART1_TX
#define PAD_UART_RX		pad_uart1_txd
#define PAD_UART_TX		pad_uart1_rxd
#elif (UART_NO == 0)
#define UART_ID 		UART0
#define UART_SYS_DEV	sys_device_uart0
#define UART_INT_NUM	interrupt_uart0
#define GPIO_UART_RX	GPIO_UART0_RX
#define GPIO_UART_TX	GPIO_UART0_TX
#define PAD_UART_RX		pad_uart0_txd
#define PAD_UART_TX		pad_uart0_rxd
#endif


typedef struct {
	//                       R    W   650  950  lcr7
	/* 0 */union {
		uint8_t rhr, thr, dll;
	};
	/* 1 */union {
		uint8_t ier, asr, dlm;
	};
	/* 2 */union {
		uint8_t isr, fcr, efr;
	};
	/* 3 */union {
		uint8_t lcr, rfl;
	};
	/* 4 */union {
		uint8_t mcr, tfl;
	};
	/* 5 */union {
		uint8_t lsr, icr;
	};
	/* 6 */union {
		uint8_t msr;
	};
	/* 7 */union {
		uint8_t spr;
	};
	uint8_t __pad[8];
} uart_hw_t;

typedef struct {
	volatile uint16_t len;
	volatile uint16_t siz;
	volatile uint8_t* pbuf;
	volatile bool tx_on; // to indicate if transmission is ongoing

} uart_tx;

#define FIFOSIZE    128   // Always running in 128 byte FIFO mode
#define TX_THRESH   0    // Interrupt when TX FIFO reaches this
#define RX_THRESH   64    // Interrupt when RX FIFO reaches this
#define MEMBAR()		__asm__("" ::: "memory");

#define BUFF_SIZE               1024u
#define BUFF_MOD                (BUFF_SIZE - 1)
#define MIN(x,y)  ((x) > (y) ? (y) : (x))

void rs485_init(uint32_t baud);
bool rs485_byte_available(volatile struct mstp_port_struct_t *mstp_port);
bool rs485_receive_error(void);
bool rs485_bytes_send(uint8_t * buffer,       /* data to send */
						uint16_t nbytes);       /* number of bytes of data */
bool rs485_is_transmitting(void);
void rs485_stop_rx(void);

void RS485_Send_Frame(
    volatile struct mstp_port_struct_t *mstp_port,  /* port specific data */
    uint8_t * buffer,       /* frame to send (up to 501 bytes of data) */
    uint16_t nbytes);       /* number of bytes of data (up to 501) */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
