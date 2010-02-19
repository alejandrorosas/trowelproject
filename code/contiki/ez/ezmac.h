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
#define MAC_BROADCAST 0xFF

extern const struct mac_driver ezmac_driver;

// radio driver is not needed, as EZMAC calls directly the CC2500
const struct mac_driver *ezmac_init(const struct radio_driver *r);

#endif /* __SICSLOWMAC_H__ */
