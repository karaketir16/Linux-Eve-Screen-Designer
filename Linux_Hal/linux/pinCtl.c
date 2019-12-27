#include "pinCtl.h"

static int pd_pin;
static int cs_pin;

void pinCtl_pd_connect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/export", O_WRONLY);
    write(fff, pinCtl_PDPIN, sizeof pinCtl_PDPIN);
    close(fff);

    // Configure as output
    fff = open("/sys/class/gpio/gpio" pinCtl_PDPIN "/direction", O_WRONLY);
    write(fff, "out", 3);
    close(fff);

    pd_pin = open("/sys/class/gpio/gpio" pinCtl_PDPIN "/value", O_WRONLY | O_SYNC);
}
void pinCtl_pd_disconnect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fff, pinCtl_PDPIN, sizeof pinCtl_PDPIN);
    close(fff);
    close(pd_pin);
}

void pinCtl_cs_connect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/export", O_WRONLY);
    write(fff, pinCtl_CSPIN, sizeof pinCtl_CSPIN);
    close(fff);

    // Configure as output
    fff = open("/sys/class/gpio/gpio" pinCtl_CSPIN "/direction", O_WRONLY);
    write(fff, "out", 3);
    close(fff);

    cs_pin = open("/sys/class/gpio/gpio" pinCtl_CSPIN "/value", O_WRONLY | O_SYNC);
}
void pinCtl_cs_disconnect()
{
    int fff = 0;
    fff = open("/sys/class/gpio/unexport", O_WRONLY);
    write(fff, pinCtl_CSPIN, sizeof pinCtl_CSPIN);
    close(fff);
    close(cs_pin);
}


void pinCtl_pdSet(const char *c)
{
    write(pd_pin, c, 1);
}
void pinCtl_csSet(const char *c)
{
    write(cs_pin, c, 1);
}

