/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __HID_PIDFF_H
#define __HID_PIDFF_H

/* PIDFF Quirks to solve issues with certain devices */

/*
 * Substitute 0 length with 0xffff to resolve issues with
 * infinite effects coming from windows API
*/
#define PIDFF_QUIRK_FIX_0_INFINITE_LENGTH   BIT(0)

/*
 * Ignore direction for spring/damping/friction/inertia effects
 * and always set 16384
*/
#define PIDFF_QUIRK_FIX_WHEEL_DIRECTION     BIT(1)

/*
 * Skip initialization of 0xA7 descriptor (Delay effect)
*/
#define PIDFF_QUIRK_NO_DELAY_EFFECT         BIT(2)


int hid_new_pidff_init(struct hid_device *hid, const struct hid_device_id *id);

#endif
