#ifndef SYN_SPI_H_
#define SYN_SPI_H_

#define SPI_SPEED       4000000

typedef struct spi_transaction_s{
    uint32_t length;
    uint8_t * tx_buffer;
	uint8_t * rx_buffer;
}spi_transaction_t;

extern HAL_StatusTypeDef ndp_spi_rxtx(uint8_t *tx_buf, uint8_t *rx_buf, uint32_t length);

#endif  /* SYN_SPI_H_ */
