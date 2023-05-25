#include <stdint.h>     /* C99 types */
#include <stdio.h>      /* printf fprintf */
#include <stdlib.h>     /* malloc free */
#include <unistd.h>     /* lseek, close */
#include <fcntl.h>      /* open */
#include <string.h>     /* memset */

#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "linux_spi.h"

int spi_open(const char * com_path)
{
    int i, dev;
    int a=0, b=0;

    if(com_path == NULL) { return -1; }

    /* open SPI device */
    dev = open(com_path, O_RDWR);
    if(dev < 0)
    {
        return -1;
    }

    /* setting SPI mode to 'mode 0' */
    i = SPI_MODE_0;
    a = ioctl(dev, SPI_IOC_WR_MODE, &i);
    b = ioctl(dev, SPI_IOC_RD_MODE, &i);
    if ((a < 0) || (b < 0)) 
    {
        close(dev);
        return -1;
    }

    /* setting SPI max clk (in Hz) */
    i = SPI_SPEED;
    a = ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &i);
    b = ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &i);
    if ((a < 0) || (b < 0)) 
    {
        close(dev);
        return -1;
    }

    /* setting SPI to MSB first */
    i = 0;
    a = ioctl(dev, SPI_IOC_WR_LSB_FIRST, &i);
    b = ioctl(dev, SPI_IOC_RD_LSB_FIRST, &i);
    if ((a < 0) || (b < 0)) 
    {
        close(dev);
        return -1;
    }

    /* setting SPI to 8 bits per word */
    i = 0;
    a = ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &i);
    b = ioctl(dev, SPI_IOC_RD_BITS_PER_WORD, &i);
    if ((a < 0) || (b < 0)) 
    {
        close(dev);
        return -1;
    }

    return dev;
}

int spi_close(int fd)
{
    return close(fd);
}

int spi_write_buffer(int fd, uint8_t* buffer, uint32_t size)
{
    int spi_device = fd;
    struct spi_ioc_transfer k;
    int a;

    /* I/O transaction */
    memset(&k, 0, sizeof(k)); /* clear k */
    k.tx_buf = (unsigned long) buffer;
    k.len = size;
    k.speed_hz = SPI_SPEED;
    k.cs_change = 0;
    k.bits_per_word = 8;
    a = ioctl(spi_device, SPI_IOC_MESSAGE(1), &k);

    /* determine return code */
    if (a != (int)k.len) 
    {
        return -1;
    }

    return a;
}

int spi_read_buffer(int fd, uint8_t *buffer, uint32_t size)
{
    int spi_device = fd;
    struct spi_ioc_transfer k;
    int a;

    /* I/O transaction */
    memset(&k, 0, sizeof(k)); /* clear k */
    k.rx_buf = (unsigned long) buffer; // defalut trans zero
    k.len = size;
    k.speed_hz = SPI_SPEED;
    k.cs_change = 0;
    k.bits_per_word = 8;
    a = ioctl(spi_device, SPI_IOC_MESSAGE(1), &k);

    /* determine return code */
    if (a != (int)k.len) 
    {
        return -1;
    }

    return a;
}

int spi_trans_buffer(int fd, uint8_t *buff_wr, uint32_t size, uint8_t *buff_rd)
{
    int spi_device = fd;
    struct spi_ioc_transfer k;
    int a;

    /* I/O transaction */
    memset(&k, 0, sizeof(k)); /* clear k */
    k.tx_buf = (unsigned long) buff_wr;
    k.rx_buf = (unsigned long) buff_rd;
    k.len = size;
    k.speed_hz = SPI_SPEED;
    k.cs_change = 0;
    k.bits_per_word = 8;
    a = ioctl(spi_device, SPI_IOC_MESSAGE(1), &k);

    /* determine return code */
    if (a != (int)k.len) 
    {
        return -1;
    }

    return a;
}


