#ifndef __LINUX_SPI_H__
#define __LINUX_SPI_H__

#include <stdio.h>
#include <stdint.h>

#define SPI_SPEED       2000000

int spi_open(const char * com_path);
int spi_close(int fd);
int spi_write_buffer(int fd, uint8_t* buffer, uint32_t size);
int spi_read_buffer(int fd, uint8_t *buffer, uint32_t size);
int spi_trans_buffer(int fd, uint8_t *buff_wr, uint32_t size, uint8_t *buff_rd);


#endif // !__LINUX_SPI_H__
