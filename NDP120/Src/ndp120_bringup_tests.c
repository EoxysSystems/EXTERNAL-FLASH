/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2020 Syntiant Corporation
 *   All Rights Reserved.
 *
 *  NOTICE:  All information contained herein is, and remains the property of
 *  Syntiant Corporation and its suppliers, if any.  The intellectual and
 *  technical concepts contained herein are proprietary to Syntiant Corporation
 *  and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 *  process, and are protected by trade secret or copyright law.  Dissemination
 *  of this information or reproduction of this material is strictly forbidden
 *  unless prior written permission is obtained from Syntiant Corporation.
 */
#include <syntiant_portability.h>
#include <syntiant_ndp.h>
#include <ndp120_spi_regs.h>
#include <ndp120_regs.h>
#include <syntiant_ndp_error.h>
#include <ndp120_bringup_tests.h>

enum ndp120_bringup_tests_constants_e {
    NDP120_BRINGUP_TESTS_RAM_ADDRESS = NDP120_BOOTRAM_REMAP + 0x6000  // TODO
};

int ndp120_bringup_tests_fail_location;

uint8_t ndp120_bringup_tests_ids[] = {0x30, 0x32, 0x33, 0x34, 0x36, 0x37, 0};

uint8_t ndp120_bringup_tests_pattern[] =
    {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};

uint8_t ndp120_bringup_tests_rom_pattern[] =
    {0x80, 0x6f, 0x00, 0x20, 0x01, 0x01, 0x00, 0x01};


int
ndp120_bringup_tests_id(struct syntiant_ndp_device_s *ndp,
                        uint8_t *idp)
{
    int i, s;
    uint8_t id;
        
    s = syntiant_ndp_read_block(ndp, 0, NDP120_SPI_ID0, &id, 1);
    if (s) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_ID_READ;
        return s;
    }

    *idp = id;
    for (i = 0; ndp120_bringup_tests_ids[i]; i++) {
        if (id == ndp120_bringup_tests_ids[i]) {
            break;
        }
    }
    
    if (!ndp120_bringup_tests_ids[i]) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_ID_MISCOMPARE;
        return SYNTIANT_NDP_ERROR_FAIL;
    }

    return s;
}

int
ndp120_bringup_tests_spi(struct syntiant_ndp_device_s *ndp,
                         uint8_t *read)
{
    int i, s;
    
    s = syntiant_ndp_write_block(ndp, 0, NDP120_SPI_MADDR(0),
                                 ndp120_bringup_tests_pattern,
                                 NDP120_BRINGUP_TESTS_SPI_SIZE);
    if (s) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_SPI_WRITE;
        return s;
    }
    
    s = syntiant_ndp_read_block(ndp, 0, NDP120_SPI_MADDR(0), read,
                                NDP120_BRINGUP_TESTS_SPI_SIZE);
    if (s) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_SPI_READ;
        return s;
    }

    for (i = 0; i < NDP120_BRINGUP_TESTS_SPI_SIZE; i++) {
        if (ndp120_bringup_tests_pattern[i] != read[i]) {
            ndp120_bringup_tests_fail_location =
                NDP120_BRINGUP_TESTS_FAIL_LOCATION_SPI_MISCOMPARE;
            return SYNTIANT_NDP_ERROR_FAIL;
        }
    }

    return s;
}

int
ndp120_bringup_tests_rom(struct syntiant_ndp_device_s *ndp,
                         uint8_t *rom)
{
    int i, s;
        
    s = syntiant_ndp_read_block(ndp, 1, NDP120_BOOTROM, rom,
                                NDP120_BRINGUP_TESTS_ROM_SIZE);
    if (s) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_ROM_READ;
        return s;
    }

    for (i = 0; i < NDP120_BRINGUP_TESTS_ROM_SIZE; i++) {
        if (ndp120_bringup_tests_rom_pattern[i] != rom[i]) {
            ndp120_bringup_tests_fail_location =
                NDP120_BRINGUP_TESTS_FAIL_LOCATION_ROM_MISCOMPARE;
            return SYNTIANT_NDP_ERROR_FAIL;
        }
    }
    
    return s;
}

int
ndp120_bringup_tests_ram(struct syntiant_ndp_device_s *ndp,
                         uint8_t *read)
{
    int i, s;
    
    s = syntiant_ndp_write_block(ndp, 1, NDP120_BRINGUP_TESTS_RAM_ADDRESS,
                                 ndp120_bringup_tests_pattern,
                                 NDP120_BRINGUP_TESTS_RAM_SIZE);
    if (s) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_RAM_WRITE;
        return s;
    }
    
    s = syntiant_ndp_read_block(ndp, 1, NDP120_BRINGUP_TESTS_RAM_ADDRESS, read,
                                NDP120_BRINGUP_TESTS_RAM_SIZE);
    if (s) {
        ndp120_bringup_tests_fail_location =
            NDP120_BRINGUP_TESTS_FAIL_LOCATION_RAM_READ;
        return s;
    }

    for (i = 0; i < NDP120_BRINGUP_TESTS_RAM_SIZE; i++) {
        if (ndp120_bringup_tests_pattern[i] != read[i]) {
            ndp120_bringup_tests_fail_location =
                NDP120_BRINGUP_TESTS_FAIL_LOCATION_RAM_MISCOMPARE;
            return SYNTIANT_NDP_ERROR_FAIL;
        }
    }

    return s;
}

int
ndp120_bringup_tests_all(struct syntiant_ndp_device_s *ndp)
{
    uint8_t buf[NDP120_BRINGUP_TESTS_RAM_SIZE];
    int s;

    s = ndp120_bringup_tests_id(ndp, buf);
    if (s) {
        return s;
    }
    
    s = ndp120_bringup_tests_spi(ndp, buf);
    if (s) {
        return s;
    }

    s = ndp120_bringup_tests_rom(ndp, buf);
    if (s) {
        return s;
    }
    
    s = ndp120_bringup_tests_ram(ndp, buf);
    
    return s;
}
