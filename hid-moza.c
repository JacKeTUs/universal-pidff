// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * HID driver for Moza Steering Wheels
 *
 * Copyright (c) 2024 Makarenko Oleg
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include "hid-moza.h"
#include "hid-pidff.h"

static const struct hid_device_id moza_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R3) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R5) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R9) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R12) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R16) },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R21) },
	{ }
};
MODULE_DEVICE_TABLE(hid, moza_devices);

// Fix data type on PID Device Control
static u8 *moza_report_fixup(struct hid_device *hdev, __u8 *rdesc,
                                        unsigned int *rsize)
{
        if (rdesc[1002] == 0x91 && rdesc[1003] == 0x02) {
			rdesc[1003] = 0x00; // Fix header, it needs to be Array.
        }
        return rdesc;
}


static int moza_probe(struct hid_device *hdev,
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

	/* set PIDFF quirks for moza wheelbases */
	unsigned quirks = 0;
	quirks |= PIDFF_QUIRK_FIX_0_INFINITE_LENGTH;
	quirks |= PIDFF_QUIRK_FIX_WHEEL_DIRECTION;

	ret = hid_pidff_init_with_quirks(hdev, quirks);
	if (ret) {
		hid_warn(hdev, "Force feedback not supported\n");
		goto err;
	}

	return 0;
err:
	return ret;
}

static int moza_input_configured(struct hid_device *hdev,
				struct hid_input *hidinput)
{
	struct input_dev *input = hidinput->input;
	input_set_abs_params(input, ABS_X,
		input->absinfo[ABS_X].minimum, input->absinfo[ABS_X].maximum, 0, 0);

	return 0;
}

static struct hid_driver moza_ff = {
	.name = "moza-ff",
	.id_table = moza_devices,
	.probe = moza_probe,
	.input_configured = moza_input_configured,
	.report_fixup = moza_report_fixup
};
module_hid_driver(moza_ff);

MODULE_AUTHOR("Oleg Makarenko <oleg@makarenk.ooo>");
MODULE_DESCRIPTION("MOZA HID FF Driver");
MODULE_LICENSE("GPL");
