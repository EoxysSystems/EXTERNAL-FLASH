/*
 * SYNTIANT CONFIDENTIAL
 * _____________________
 *
 *   Copyright (c) 2018-2020 Syntiant Corporation
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

#include <syntiant_ndp_ilib_version.h>
#include <syntiant_ndp.h>
#include <syntiant_ndp_driver.h>
#ifndef EXCLUDE_SYNTIANT_CORE_1
#include <syntiant_ndp10x_driver.h>
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
#include <syntiant_ndp120_driver.h>
#endif
#include <syntiant_ndp_error.h>

static struct syntiant_ndp_driver_s *syntiant_ndp_drivers[] = {
#ifndef EXCLUDE_SYNTIANT_CORE_1
    &syntiant_ndp10x_driver,
#endif
#ifndef EXCLUDE_SYNTIANT_CORE_2
    &syntiant_ndp120_driver,
#endif
    NULL
};

static struct syntiant_ndp_driver_s *
syntiant_ndp_get_driver(unsigned int device_id);

char *
syntiant_ndp_ilib_version(void) {
    static char *version = SYNTIANT_NDP_ILIB_VERSION;

    return version;
}

char *syntiant_ndp_ilib_release_hash(void) {
    static char *version = SYNTIANT_NDP_ILIB_RELEASE_HASH;

    return version;
}

int syntiant_ndp_ilib_enumerate_family(unsigned int family,
                                       unsigned int *next_family)
{
    int s = SYNTIANT_NDP_ERROR_MORE;
    int i = 0;
    struct syntiant_ndp_driver_s *driver;

    if (family != SYNTIANT_NDP_FAMILY_UNKNOWN) {
        for (i = 0; (driver = syntiant_ndp_drivers[i]); i++) {
            if (driver->device_family == family) {
                i++;
                break;
            }
        }
        if (!driver) {
            s = SYNTIANT_NDP_ERROR_FAIL;
            goto done;
        }
    }

    driver = syntiant_ndp_drivers[i];

    if (!driver) {
        *next_family = SYNTIANT_NDP_FAMILY_UNKNOWN;
        s = SYNTIANT_NDP_ERROR_NONE;
        goto done;
    }

    *next_family = driver->device_family;

done:
    return s;
}

int
syntiant_ndp_ilib_family_version(unsigned int family, unsigned int *major,
                                 unsigned int *minor, unsigned int *patch)
{
    int s = SYNTIANT_NDP_ERROR_NONE;
    struct syntiant_ndp_driver_s **driverp;
    struct syntiant_ndp_driver_s *driver;

    for (driverp = syntiant_ndp_drivers; (driver = *driverp); driverp++) {
        if (driver->device_family == family) {
            break;
        }
    }

    if (!driver) {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }

    *major = driver->major;
    *minor = driver->minor;
    *patch = driver->patch;

error:
    return s;
}

static struct syntiant_ndp_driver_s *
syntiant_ndp_get_driver(unsigned int device_id)
{
    int i, j;
    struct syntiant_ndp_driver_s *driver;

    for (i = 0; syntiant_ndp_drivers[i]; i++) {
        driver = syntiant_ndp_drivers[i];
        for (j = 0; driver->device_types[j]; j++) {
            if (driver->device_types[j] == device_id) {
                return driver;
            }
        }
    }

    return NULL;
}

int
syntiant_ndp_init(struct syntiant_ndp_device_s **ndpp,
    struct syntiant_ndp_integration_interfaces_s *iif,
    enum syntiant_ndp_init_mode_e init_mode)
{
    int s, s0;
    unsigned int id;
    struct syntiant_ndp_device_s *ndp = *ndpp;
    int alloced = !ndp;

    if (!ndp) {
        ndp = (iif->malloc)(sizeof(struct syntiant_ndp_device_s));
        if (!ndp) {
            s = SYNTIANT_NDP_ERROR_NOMEM;
            goto error;
        }
        /* ndpp may be used by the client during a restart init */
        *ndpp = ndp;
    }

    ndp->init = 0;

    if (iif) {
        ndp->iif.d = iif->d;
        ndp->iif.malloc = iif->malloc;
        ndp->iif.free = iif->free;
        ndp->iif.mbwait = iif->mbwait;
        ndp->iif.get_type = iif->get_type;
        ndp->iif.sync = iif->sync;
        ndp->iif.unsync = iif->unsync;
        ndp->iif.transfer = iif->transfer;
    }

    s = (ndp->iif.get_type)(ndp->iif.d, &id);
    if (s) goto error;
    ndp->device_type = id;

    ndp->driver = syntiant_ndp_get_driver(id);
    if (!ndp->driver) {
        s = SYNTIANT_NDP_ERROR_UNSUP;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->init)(ndp, init_mode);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

error:
    if (!s) {
        ndp->init = 1;
    } else if (alloced && ndp) {
        (ndp->iif.free)(ndp);
        *ndpp = NULL;
    }

    return s;
}

int
syntiant_ndp_uninit(struct syntiant_ndp_device_s *ndp, int free,
    enum syntiant_ndp_init_mode_e init_mode)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        return SYNTIANT_NDP_ERROR_UNINIT;
    }

    ndp->init = 0;

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->uninit)(ndp, init_mode);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;

error:
    if (free) {
        (ndp->iif.free)(ndp);
    }
    return s;
}

int
syntiant_ndp_op_size(struct syntiant_ndp_device_s *ndp, int mcu,
                     unsigned int *size)
{
    int s;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->driver->op_size)(ndp->iif.d, mcu, size);

error:
    return s;
}

int
syntiant_ndp_read_block(struct syntiant_ndp_device_s *ndp, int mcu,
    uint32_t address, void *value, unsigned int count)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->read_block)(ndp, mcu, address, value, count);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_write_block(struct syntiant_ndp_device_s *ndp, int mcu,
                         uint32_t address, void *value, unsigned int count)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->write_block)(ndp, mcu, address, value, count);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_write(struct syntiant_ndp_device_s *ndp, int mcu, uint32_t address,
                   uint32_t value)
{
    int s;
    unsigned int size;

    s = syntiant_ndp_op_size(ndp, mcu, &size);
    if (!s) {
        s = syntiant_ndp_write_block(ndp, mcu, address, &value, size);
    }
    return s;
}

int
syntiant_ndp_read(struct syntiant_ndp_device_s *ndp, int mcu, uint32_t address,
                  void *value)
{
    int s;
    unsigned int size;

    s = syntiant_ndp_op_size(ndp, mcu, &size);
    if (!s) {
        s = syntiant_ndp_read_block(ndp, mcu, address, value, size);
    }
    return s;
}

int
syntiant_ndp_interrupts(struct syntiant_ndp_device_s *ndp, int *on)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->interrupts)(ndp, on);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_poll(
    struct syntiant_ndp_device_s *ndp, uint32_t *notifications, int clear)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->poll)(ndp, notifications, clear);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_load(struct syntiant_ndp_device_s *ndp, void *package, int len)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->load)(ndp, package, len);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_device_family(unsigned int device_id,
                               unsigned int *device_family)
{
    struct syntiant_ndp_driver_s *driver;

    driver = syntiant_ndp_get_driver(device_id);

    if (!driver) {
        return SYNTIANT_NDP_ERROR_UNSUP;
    }

    *device_family = driver->device_family;

    return SYNTIANT_NDP_ERROR_NONE;
}

int
syntiant_ndp_get_config(
    struct syntiant_ndp_device_s *ndp, struct syntiant_ndp_config_s *config)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    config->device_id = ndp->device_type;
    config->device_family = ndp->driver->device_family;
    s = (ndp->driver->get_config)(ndp, config);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_send_data(
    struct syntiant_ndp_device_s *ndp, uint8_t *data, unsigned int len,
    int type, uint32_t offset)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    if (len < 1) {
        s = SYNTIANT_NDP_ERROR_ARG;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->send_data)(ndp, data, len, type, offset);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_extract_data(struct syntiant_ndp_device_s *ndp, int type,
                          int from, uint8_t *data, unsigned int *len)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->extract_data)(ndp, type, from, data, len);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_match_summary(
    struct syntiant_ndp_device_s *ndp, uint32_t *summary)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->get_match_summary)(ndp, summary);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_match_binary(
    struct syntiant_ndp_device_s *ndp, uint8_t *matches, unsigned int len)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->get_match_binary)(ndp, matches, len);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}

int
syntiant_ndp_get_match_strength(
    struct syntiant_ndp_device_s *ndp, uint8_t *strengths, unsigned int len,
    int type)
{
    int s, s0;

    if (!ndp || !ndp->init) {
        s = SYNTIANT_NDP_ERROR_UNINIT;
        goto error;
    }

    s = (ndp->iif.sync)(ndp->iif.d);
    if (s) goto error;

    s = (ndp->driver->get_match_strength)(ndp, strengths, len, type);

    s0 = (ndp->iif.unsync)(ndp->iif.d);
    s = s ? s : s0;
error:
    return s;
}
