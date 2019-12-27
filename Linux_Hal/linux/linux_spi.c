#include "linux_spi.h"
#include "pinCtl.h"

static const char *device;
static uint8_t mode8;
static uint8_t bits;
static uint32_t speed;
static uint16_t delay;
static int fd;
static int pd_pin;
static int cs_pin;


void pabort(const char *s)
{
    perror(s);
    abort();
}

void SPI_transfer(uint8_t const *tx, uint8_t const *rx, size_t len)
{

    struct spi_ioc_transfer tr ;
    memset(&tr, 0, sizeof(tr));

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = len;
    tr.delay_usecs = delay;
    tr.speed_hz = speed;
    tr.bits_per_word = bits;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        pabort("can't send spi message");


}


void SPI_init(const char * path, uint32_t spd)
{

    device = path;
    fd = open(device, O_RDWR);
    if (fd < 0)
        pabort("can't open device");

    int ret = 0;
    mode8 = SPI_MODE_0;
    bits = 8;
    speed = spd;



    /*
     * spi mode
     */
    // mode8 |= SPI_CPHA;
    // mode8 |= SPI_CPOL;
    ret = ioctl(fd, SPI_IOC_WR_MODE, &mode8);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &mode8);
    if (ret == -1)
        pabort("can't get spi mode");

    /*
     * bits per word
     */
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        pabort("can't get bits per word");

    /*
     * max speed hz
     */
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        pabort("can't get max speed hz");



}


void SPI_end()
{
    close(fd);
    pinCtl_pdSet(LOW);
    pinCtl_pd_disconnect();
    pinCtl_cs_disconnect();
}
