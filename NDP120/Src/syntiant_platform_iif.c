#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINUX
#ifndef LINUX
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"   
#else 
//#include <unistd.h>
#include "cmsis_os.h"
#include "XNO_UTILS.h"
#include "XNO_DEBUG_TASK.h"
#endif

#include "syntiant_platform.h"
#include "syntiant_platform_iif.h"
#include <syntiant_ndp.h>
#include <syntiant_ndp_error.h>

#include <ndp120_spi_regs.h> 

#define SPI_TX_RX_BUF_SIZE  2048

static uint8_t tx_buf[SPI_TX_RX_BUF_SIZE + 5];  // Max(5, 1 + read_delay)
static uint8_t rx_buf[SPI_TX_RX_BUF_SIZE + 5];
static int evb_platform_spi_transfer_debug = 0;

void syntiant_platform_sleep_ms(int ms)
{
#ifndef LINUX
    vTaskDelay(ms / portTICK_PERIOD_MS);
#else 
    osDelay(ms);
#endif    
}

/* iif malloc */
static void *evb_platform_malloc(int size)
{
    return malloc(size);
}

/* iif free */
static void evb_platform_free(void *p)
{
    free(p);
}

/* iif sync & unsync functions
 * since this sample code handles interrupts in-line (everything in one single
 * thread)
 * we can leave these to NOP
 */
static int evb_platform_sync(void *d)
{
    (void)d;
    return 0;
}

static int evb_platform_unsync(void *d)
{
    (void)d;
    return 0;
}

/* iif mailbox-exchange wait. this sample implementation just polls for mailbox
 * completion */
int evb_platform_mbwait(void *d)
{
    struct syntiant_platform_app_s *ndp120 = (struct syntiant_platform_app_s *) d;
    int s;
    uint32_t notifications;

    do {
        s = syntiant_ndp_poll(ndp120->ndp, &notifications, 1);
        if (syntiant_platform_check_status("poll", s, 1)){
            return s;
        }
        osDelay(10);
    } while (((notifications & SYNTIANT_NDP_NOTIFICATION_MAILBOX_IN) == 0)
        && ((notifications & SYNTIANT_NDP_NOTIFICATION_EXTRACT_READY) == 0)
        && ((notifications & SYNTIANT_NDP_NOTIFICATION_MAILBOX_OUT) == 0) );

    return 0;
}

int evb_platform_get_type(void *d, unsigned int *type)
{
    struct syntiant_platform_app_s *ndp120 = (struct syntiant_platform_app_s *) d;
    int s = SYNTIANT_NDP_ERROR_NONE;
    uint8_t type_byte = 0x34;
    uint8_t data = 0;

    if (ndp120->device_type > 0) {
        type_byte = ndp120->device_type;
    } else {
        s = evb_platform_transfer(ndp120, 0, NDP120_SPI_ID0, NULL, &data, 1);
        if (!s) {
            if (data) {
                type_byte = data;
            } else {
                SYNTIANT_TRACE("SPI ID read as 0, assuming 0x%x\n", type_byte);
            }
        }
    }

    if (!s) {
        *type = type_byte;
    }
    return s;
}

static int evb_platform_spi_setup(void *d, int spi_speed,  syntiant_spi_transfer_f spi_func)
{
    struct syntiant_platform_evb_io_s *io = (struct syntiant_platform_evb_io_s *)d;

    io->spi_speed = spi_speed;
    io->spi_txrx= spi_func;
    if (spi_speed >= 8000000)
    {
        io->spi_read_delay = 3;
    }
    else if(spi_speed >= 3000000)
    {
        io->spi_read_delay = 2;
    }
    else
    {
        io->spi_read_delay = 1;
    }

    return SYNTIANT_NDP_ERROR_NONE;
}

static int evb_platform_spi_transfer(void *d, int mcu, uint32_t addr, 
                                    void *out, void *in, unsigned int count)
{
    uint32_t length;
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_platform_evb_io_s *io = (struct syntiant_platform_evb_io_s *)d;
    int read_delay = io->spi_read_delay;

    if (in && out) {
        s = SYNTIANT_NDP_ERROR_ARG;
        SYNTIANT_TRACE("transfer wrong in %p or out %p\r\n", in, out);
        return s;
    }

    if (evb_platform_spi_transfer_debug) {
        SYNTIANT_TRACE("transfer mcu %d, count %d, addr 0x%08x, out %p, in %p\r\n",
            mcu, count, (unsigned int)addr, out, in);
    }

    if (mcu) {
        if ((count & 0x3) != 0) {
            s = SYNTIANT_NDP_ERROR_ARG;
            return s;
        }

        if(in) {
            // one command + 4 bytes MCU address
            tx_buf[0] = NDP120_SPI_MADDR(0);
            memcpy(&tx_buf[1], &addr, sizeof(addr));
            length = 5;  // one byte spi_cmd + 4 bytes MCU address
            s = io->spi_txrx(tx_buf, rx_buf, length);
            if (syntiant_platform_check_status("SPI write failed", s, 1)) {
                return s;
            }

            /* for reads, need to restart a read command packet */
            if (read_delay <= 4) {
                tx_buf[0] = 0x80 | (NDP120_SPI_MDATA(0) - read_delay);
            } else {
                tx_buf[0] = 0x80 | (NDP120_SPI_MDUMMY(11) - read_delay + 5);
            }

            length = 1 + read_delay + count; // one byte spi_cmd + delay bytes + count
            s = io->spi_txrx(tx_buf, rx_buf, length);
            if (syntiant_platform_check_status("SPI read failed", s, 1)) {
                return s;
            }
            
            /* skip one spi_cmd + the read_delay dummy bytes */
            memcpy(in, &rx_buf[1 + read_delay], count);
        }
        else {
            // one byte command + 4 bytes MCU address + tx data
            tx_buf[0] = NDP120_SPI_MADDR(0);
            memcpy(&tx_buf[1], &addr, sizeof(addr));
            length = 5;  // one byte spi_cmd + 4 bytes MCU address  
            memcpy(&tx_buf[length], out, count);
            length = count + 5;      
            s = io->spi_txrx(tx_buf, rx_buf, length);
            if (syntiant_platform_check_status("SPI write failed", s, 1)) {
                return s;
            }
        }
    } else {
        if (0xff < addr) {
            s = SYNTIANT_NDP_ERROR_ARG;
            return s;
        }
        
        if (in) {
            tx_buf[0] = 0x80 | addr;
            length = 1 + count;
            s = io->spi_txrx(tx_buf, rx_buf, length);
            if (syntiant_platform_check_status("SPI read failed", s, 1)) {
                return s;
            }
            
            /* skip one spi_cmd*/
            memcpy(in, &rx_buf[1], count);
        }
        else {
            tx_buf[0] = addr;
            memcpy(&tx_buf[1], out, count);  // Fill tx data
            length = 1 + count;
            s = io->spi_txrx(tx_buf, rx_buf, length);
            if (syntiant_platform_check_status("SPI write failed", s, 1)) {
                return s;
            }
        }
    }
    return s;
}


/* SPI access function */
int evb_platform_transfer(
    void *d, int mcu, uint32_t addr, void *out, void *in, unsigned int count)
{
    struct syntiant_platform_app_s *ndp120 = (struct syntiant_platform_app_s *) d;
    int s;

    s = evb_platform_spi_transfer(&ndp120->evb_io_handle, mcu, addr, out, in, count);
    return s;
}

int evb_platform_iif_setup(void *d, struct syntiant_ndp_integration_interfaces_s *iif, 
                           int spi_speed,  syntiant_spi_transfer_f spi_func)
{
    struct syntiant_platform_app_s *ndp120 = (struct syntiant_platform_app_s *) d;
    int s = SYNTIANT_NDP_ERROR_NONE;

    evb_platform_spi_setup(&ndp120->evb_io_handle, spi_speed, spi_func);

    /* setup the integration interface functions */
    iif->d = ndp120;
    iif->malloc   = evb_platform_malloc;
    iif->free     = evb_platform_free;
    iif->mbwait   = evb_platform_mbwait;
    iif->get_type = evb_platform_get_type;
    iif->sync     = evb_platform_sync;
    iif->unsync   = evb_platform_unsync;
    iif->transfer = evb_platform_transfer;

    return s;
}

/* error handling function */
int syntiant_platform_check_status(char *message, int s, int do_exit)
{
    (void)do_exit;
    char loc_str[50]={'\0'};
    const char *er_str;
    if (s) {
    	strcpy(loc_str,"NDP_ERR: ");
    	strcat(loc_str,message);
    	strcat(loc_str,": ");
    	er_str = syntiant_ndp_error_name(s);
    	strcat(loc_str,er_str);
		XNO_Printf((char*)loc_str);
    }
    return s;
}
