/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __HID_PIDFF_H
#define __HID_PIDFF_H

/* PIDFF Quirks to solve issues with certain devices */

/*
 * Ignore direction and always set 16384 (0x4000)
 */
#define PIDFF_QUIRK_FIX_WHEEL_DIRECTION		BIT(0)

/*
 * Skip initialization of 0xA7 descriptor (Delay effect)
 * Fixes VRS DFP, Cammus, old Simagic wheelbases
*/
#define PIDFF_QUIRK_NO_DELAY_EFFECT		BIT(1)

/*
 * Ignore PARAM_BLOCK_OFFSET (Axis number).
 * Most of the wheelbases have only one Axis anyway
 * Fixes VRS DFP
*/
#define PIDFF_QUIRK_NO_PID_PARAM_BLOCK_OFFSET	BIT(2)

/*
 * Some wheelbases don't have some PID_CONTROL fields.
 * PID standard does not define fields that MUST exist, but
 *  that driver was strict about them. This quirk disables it.
 * Fixes VRS DFP
*/
#define PIDFF_QUIRK_NO_STRICT_PID_CONTROL	BIT(3)

/*
 * Some devices have all the periodic effects in their descriptors
 * but only sine effect actually works properly.
 * this forces every periodic effect to be created as SINE
 * Fixes PXN (LITE STAR)
*/
#define PIDFF_QUIRK_PERIODIC_SINE_ONLY		BIT(4)

int hid_pidff_init_quirks(struct hid_device *hid, const struct hid_device_id *id);

#endif
