/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __HID_PIDFF_H
#define __HID_PIDFF_H

/* PIDFF Quirks to solve issues with certain devices */
/*
 * Ignore direction and always set 16384 (0x4000)
 */
#define PIDFF_QUIRK_FIX_WHEEL_DIRECTION		BIT(0)

int hid_pidff_init_quirks(struct hid_device *hid, const struct hid_device_id *id);

#endif
