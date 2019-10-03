#include "colibri_spi.h"

//Spi::Spi(QObject *parent) : QObject(parent)
//{

//}

static const char *device;
static uint32_t mode;
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

void Spi_transfer(uint8_t const *tx, uint8_t const *rx, size_t len)
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
void Spi_pd_connect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/export", O_WRONLY);
    write(fff, PDPIN, sizeof PDPIN);
    close(fff);

    // Configure as output
    fff = open("/sys/class/gpio/gpio" PDPIN "/direction", O_WRONLY);
    write(fff, "out", 3);
    close(fff);

    pd_pin = open("/sys/class/gpio/gpio" PDPIN "/value", O_WRONLY | O_SYNC);
}
void Spi_pd_disconnect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fff, PDPIN, sizeof PDPIN);
    close(fff);
    close(pd_pin);
}

void Spi_cs_connect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/export", O_WRONLY);
    write(fff, CSPIN, sizeof CSPIN);
    close(fff);

    // Configure as output
    fff = open("/sys/class/gpio/gpio" CSPIN "/direction", O_WRONLY);
    write(fff, "out", 3);
    close(fff);

    cs_pin = open("/sys/class/gpio/gpio" CSPIN "/value", O_WRONLY | O_SYNC);
}
void Spi_cs_disconnect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fff, CSPIN, sizeof CSPIN);
    close(fff);
    close(cs_pin);
}


void Spi_init(const char * path, uint32_t spd)
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
    // mode |= SPI_CPHA;
    // mode |= SPI_CPOL;
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
void Spi_pdSet(const char *c)
{
    write(pd_pin, c, 1);
}
void Spi_csSet(const char *c)
{
    write(cs_pin, c, 1);
}


//Spi::~Spi()
//{
//    close(fd);
//    pdSet(LOW);
//    pd_disconnect();
//    cs_disconnect();
//}





