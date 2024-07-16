/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __HID_PIDFF_H
#define __HID_PIDFF_H

/* PIDFF Quirks to solve issues with certain devices */

/*
 * Always set a value > 0 for PERIODIC envelope attack and fade level
*/
#define PIDFF_QUIRK_FIX_PERIODIC_ENVELOPE   BIT(0)

/*
 * Ignore direction and always set 16384 (0x4000)
*/
#define PIDFF_QUIRK_FIX_WHEEL_DIRECTION     BIT(1)

/*
 * Skip initialization of 0xA7 descriptor (Delay effect)
*/
#define PIDFF_QUIRK_NO_DELAY_EFFECT         BIT(2)


int hid_pidff_init_quirks(struct hid_device *hid, const struct hid_device_id *id);

#endif
