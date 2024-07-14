// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * HID PIDFF wrapper
 * First of all targeting steering wheels and wheelbases
 *
 * Copyright (c) 2024 Makarenko Oleg
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include "hid-ids.h"
#include "hid-pidff.h"

static const struct hid_device_id pidff_wheel_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R3),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION | PIDFF_QUIRK_FIX_PERIODIC_ENVELOPE },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R5),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION | PIDFF_QUIRK_FIX_PERIODIC_ENVELOPE },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R9),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION | PIDFF_QUIRK_FIX_PERIODIC_ENVELOPE },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R12),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION | PIDFF_QUIRK_FIX_PERIODIC_ENVELOPE },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R16_R21),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION | PIDFF_QUIRK_FIX_PERIODIC_ENVELOPE },
	{ }
};
MODULE_DEVICE_TABLE(hid, pidff_wheel_devices);


static u8 *moza_report_fixup(struct hid_device *hdev, __u8 *rdesc,
                                        unsigned int *rsize)
{
		// Fix data type on PID Device Control
        if (rdesc[1002] == 0x91 && rdesc[1003] == 0x02) {
			rdesc[1003] = 0x00; // Fix header, it needs to be Array.
        }
        return rdesc;
}


static u8 *universal_pidff_report_fixup(struct hid_device *hdev, __u8 *rdesc,
                                        unsigned int *rsize)
{
		if (hdev->vendor == USB_VENDOR_ID_MOZA) {
			return moza_report_fixup(hdev, rdesc, rsize);
		}
		return rdesc;
}


/*
 * Check if the device is PID and initialize it
 * Add quirks after initialisation
 */
static int universal_pidff_probe(struct hid_device *hdev,
				const struct hid_device_id *id)
{
	int ret;
	ret = hid_parse(hdev);
	if (ret) {
		hid_err(hdev, "parse failed\n");
		goto err;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT & ~HID_CONNECT_FF);
	if (ret) {
		hid_err(hdev, "hw start failed\n");
		goto err;
	}

	ret = hid_pidff_init_with_quirks(hdev, id);
	if (ret) {
		hid_warn(hdev, "Force feedback not supported\n");
		goto err;
	}

	return 0;
err:
	return ret;
}

static int universal_pidff_input_configured(struct hid_device *hdev,
				struct hid_input *hidinput)
{
	struct input_dev *input = hidinput->input;
	input_set_abs_params(input, ABS_X,
		input->absinfo[ABS_X].minimum, input->absinfo[ABS_X].maximum, 0, 0);

	return 0;
}

static struct hid_driver universal_pidff = {
	.name = "hid-universal-pidff",
	.id_table = pidff_wheel_devices,
	.probe = universal_pidff_probe,
	.input_configured = universal_pidff_input_configured,
	.report_fixup = universal_pidff_report_fixup
};
module_hid_driver(universal_pidff);

MODULE_AUTHOR("Oleg Makarenko <oleg@makarenk.ooo>");
MODULE_DESCRIPTION("Universal HID PIDFF Driver");
MODULE_LICENSE("GPL");
