/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __HID_PIDFF_H
#define __HID_PIDFF_H

/* PIDFF Quirks to solve issues with certain devices */

/*
 * Ignore direction and always set 16384 (0x4000)
 */
#define PIDFF_QUIRK_FIX_WHEEL_DIRECTION		BIT(0)

/*
 * Ignore PARAM_BLOCK_OFFSET (Axis number).
 * Most of the wheelbases have only one Axis anyway
 * Fixes VRS DFP
*/
#define PIDFF_QUIRK_NO_PID_PARAM_BLOCK_OFFSET   BIT(2)

/*
 * Some wheelbases don't have some PID_CONTROL fields.
 * PID standard does not define fields that MUST exist, but
 *  that driver was strict about them. This quirk disables it.
 * Fixes VRS DFP
*/
#define PIDFF_QUIRK_NO_STRICT_PID_CONTROL       BIT(3)

int hid_pidff_init_quirks(struct hid_device *hid, const struct hid_device_id *id);

#endif
