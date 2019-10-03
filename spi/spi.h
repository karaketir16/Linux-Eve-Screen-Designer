#ifndef Spi_H
#define Spi_H

//#include <QObject>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

#define PDPIN "35" //SODIMM_133 (GPIO)
#define CSPIN "15" //SODIMM_98 (GPIO)


#define LOW "0"
#define HIGH "1"

#define SPI_PATH "/dev/spidev3.0"




    void Spi_pd_connect();
    void Spi_pd_disconnect();
    void Spi_cs_connect();
    void Spi_cs_disconnect();
    void Spi_pdSet(const char *);
    void Spi_csSet(const char *);

    void Spi_init(const char * path, uint32_t spd);

    void Spi_transfer(uint8_t const *tx, uint8_t const *rx, size_t len);





#endif // Spi_H


// SPDX-License-Identifier: GPL-2.0-only
/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

// #include <avr/pgmspace.h>



