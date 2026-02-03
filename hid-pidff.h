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
#define HID_PIDFF_QUIRK_FIX_CONDITIONAL_DIRECTION	BIT(3)

/*
 * Some devices have all the periodic effects in their descriptors
 * but only sine effect actually works properly.
 * this forces every periodic effect to be created as SINE
 * Fixes PXN (LITE STAR)
 */
#define HID_PIDFF_QUIRK_PERIODIC_SINE_ONLY	BIT(4)

/*
 * Windows allows devices with missing negative coefficient for conditional
 * effects. Negative coefficient is ignored in such cases. Do not fail
 * set_condition usage search if negative coefficient is missing. Fixes
 * conditional effect playback on Asetek wheelbases.
 *
 * https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416601(v=vs.85)
 */
#define HID_PIDFF_QUIRK_MISSING_NEG_COEFFICIENT	BIT(5)

/*
 * This is the same case as for Negative coefficient
 */
#define HID_PIDFF_QUIRK_MISSING_NEG_SATURATION	BIT(6)

/*
 * Some devices (mainly Asetek) do not have deadband field in set conditional
 * usage. Do not fail set conditional usage search if it's missing.
 * Fixes conditional effect playback on Asetek wheelbases.
 */
#define HID_PIDFF_QUIRK_MISSING_DEADBAND	BIT(7)

/* Kernel ifndef not included as we have our own copy of hid-pidff */
int hid_pidff_init(struct hid_device *hid);
int hid_pidff_init_with_quirks(struct hid_device *hid, u32 initial_quirks);

#endif
