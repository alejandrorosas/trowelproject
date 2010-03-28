/**
 * \file
 *         MAC interface for low-power low-memory EZ430 devices
 *
 * \author
 *         Clément Burin des Roziers
 */

#ifndef __EZMAC_H__
#define __EZMAC_H__

#include "net/mac/mac.h"
#include "dev/radio.h"

extern const struct mac_driver ezmac_driver;

const struct mac_driver *ezmac_init(const struct radio_driver *d);

#endif /* __SICSLOWMAC_H__ */
