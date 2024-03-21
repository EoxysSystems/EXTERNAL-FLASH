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
#ifndef NDP120_BRINGUP_TESTS_H
#define NDP120_BRINGUP_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <syntiant_ndp.h>

enum ndp120_bringup_tests_sizes_e {
    NDP120_BRINGUP_TESTS_ID_SIZE = 1,
    NDP120_BRINGUP_TESTS_SPI_SIZE = 4,
    NDP120_BRINGUP_TESTS_ROM_SIZE = 8,
    NDP120_BRINGUP_TESTS_RAM_SIZE = 8
};

enum ndp120_bringup_tests_fail_location_e {
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_ID_READ,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_ID_MISCOMPARE,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_SPI_WRITE,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_SPI_READ,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_SPI_MISCOMPARE,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_ROM_READ,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_ROM_MISCOMPARE,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_RAM_WRITE,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_RAM_READ,
    NDP120_BRINGUP_TESTS_FAIL_LOCATION_RAM_MISCOMPARE
};

#define NDP120_BRINGUP_TESTS_FAIL_LOCATION_NAMES { \
        "id read", "id miscompare", "spi write", "spi read", "spi miscompare", \
        "rom read", "rom miscompare", "ram write", "ram read", \
        "ram miscompare" }

/**
 * @brief contains the location of last reported test failure
 */
extern int ndp120_bringup_tests_fail_location;

/**
 * @brief 0-terminated list of valid NDP120 device IDs
 */
extern uint8_t ndp120_bringup_tests_ids[];
    
/**
 * @brief test pattern for SPI and RAM tests
 */
extern uint8_t ndp120_bringup_tests_pattern[];
    
/**
 * @brief expected pattern for ROM test
 */
extern uint8_t ndp120_bringup_tests_rom_pattern[];


/**
 * @brief read the chip ID SPI register and compare with expected
 *
 * @param ndp NDP state object 
 * @param idp pointer to @c NDP120_BRINGUP_TESTS_ID_SIZE (1) array to hold id
 * @return a @c SYNTIANT_NDP_ERROR_* code. @c ndp120_bringup_tests_fail_location
 *     will also be set when failure is reported
 */
extern int ndp120_bringup_tests_id(struct syntiant_ndp_device_s *ndp,
                                   uint8_t *idp);

/**
 * @brief write, read and confirm a pattern to SPI registers
 *
 * @param ndp NDP state object 
 * @param read pointer to @c NDP120_BRINGUP_TESTS_SPI_SIZE (4) array to
 *     hold the read registers
 * @return a @c SYNTIANT_NDP_ERROR_* code. @c ndp120_bringup_tests_fail_location
 *     will also be set when failure is reported
 */
extern int ndp120_bringup_tests_spi(struct syntiant_ndp_device_s *ndp,
                                    uint8_t *read);
    
/**
 * @brief read and confirm a pattern from the MCU ROM
 *
 * @param ndp NDP state object 
 * @param read pointer to @c NDP120_BRINGUP_TESTS_ROM_SIZE (8) array to
 *     hold the read ROM contents
 * @return a @c SYNTIANT_NDP_ERROR_* code. @c ndp120_bringup_tests_fail_location
 *     will also be set when failure is reported
 */
extern int ndp120_bringup_tests_rom(struct syntiant_ndp_device_s *ndp,
                                    uint8_t *rom);

/**
 * @brief write, read and confirm a pattern to MCU RAM
 *
 * @param ndp NDP state object 
 * @param read pointer to @c NDP120_BRINGUP_TESTS_RAM_SIZE (8) array to
 *     hold the read RAM contents
 * @return a @c SYNTIANT_NDP_ERROR_* code. @c ndp120_bringup_tests_fail_location
 *     will also be set when failure is reported
 */
extern int ndp120_bringup_tests_ram(struct syntiant_ndp_device_s *ndp,
                                    uint8_t *read);
    
/**
 * @brief run all tests
 *
 * @param ndp NDP state object 
 * @return a @c SYNTIANT_NDP_ERROR_* code. @c ndp120_bringup_tests_fail_location
 *     will also be set when failure is reported
 */
extern int ndp120_bringup_tests_all(struct syntiant_ndp_device_s *ndp);
    
#ifdef __cplusplus
extern "C" {
#endif

#endif
