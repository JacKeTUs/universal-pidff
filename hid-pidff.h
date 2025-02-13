/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __HID_PIDFF_H
#define __HID_PIDFF_H

#include <linux/hid.h>

/* PIDFF Quirks to solve issues with certain devices */

/*
 * Skip initialization of 0xA7 descriptor (Delay effect)
 * Fixes VRS DFP, Cammus, old Simagic wheelbases
*/
#define HID_PIDFF_QUIRK_MISSING_DELAY		BIT(0)

/*
 * Ignore PARAM_BLOCK_OFFSET (Axis number).
 * Most of the wheelbases have only one Axis anyway
 * Fixes VRS DFP
*/
#define HID_PIDFF_QUIRK_MISSING_PBO		BIT(1)

/*
 * Some devices implement DEVICE_CONTROL as a bit mask variable
 * instead of an array, which leads to a logical_minimum for this
 * usage being possible different from 1. hid-pidff was strict
 * about it. This quirk disables this check.
 * Fixes VRS DFP
*/
#define HID_PIDFF_QUIRK_PERMISSIVE_CONTROL	BIT(2)

/*
 * Ignore direction and always set 16384 (0x4000)
 */
#define HID_PIDFF_QUIRK_FIX_WHEEL_DIRECTION	BIT(3)

/*
 * Some devices have all the periodic effects in their descriptors
 * but only sine effect actually works properly.
 * this forces every periodic effect to be created as SINE
 * Fixes PXN (LITE STAR)
*/
#define HID_PIDFF_QUIRK_PERIODIC_SINE_ONLY	BIT(4)

/* Kernel ifndef not included as we have our own copy of hid-pidff */
int hid_pidff_init(struct hid_device *hid);
int hid_pidff_init_with_quirks(struct hid_device *hid, u32 initial_quirks);

#endif
