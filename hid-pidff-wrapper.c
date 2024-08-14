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

#define MOZA_ADDITIONAL_AXES	{ABS_Y, ABS_Z, ABS_RX, ABS_RY, ABS_RZ, ABS_THROTTLE, ABS_RUDDER}
#define CAMMUS_ADDITIONAL_AXES	{}

static const struct hid_device_id pidff_wheel_devices[] = {
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R3),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R5),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R9),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R12),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION },
	{ HID_USB_DEVICE(USB_VENDOR_ID_MOZA, USB_DEVICE_ID_MOZA_R16_R21),
		.driver_data = PIDFF_QUIRK_FIX_WHEEL_DIRECTION },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CAMMUS, USB_DEVICE_ID_CAMMUS_C5),
		.driver_data = PIDFF_QUIRK_NO_DELAY_EFFECT },
	{ HID_USB_DEVICE(USB_VENDOR_ID_CAMMUS, USB_DEVICE_ID_CAMMUS_C12),
		.driver_data = PIDFF_QUIRK_NO_DELAY_EFFECT },
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

	ret = hid_pidff_init_quirks(hdev, id);
	if (ret) {
		hid_warn(hdev, "Force feedback is not supported\n");
		goto err;
	}

	return 0;
err:
	return ret;
}

static int universal_pidff_input_configured(struct hid_device *hdev,
				struct hid_input *hidinput)
{
	// Remove fuzz and deadzone from the wheel axis
	struct input_dev *input = hidinput->input;
	input_set_abs_params(input, ABS_X,
		input->absinfo[ABS_X].minimum, input->absinfo[ABS_X].maximum, 0, 0);

	// Decrease fuzz and deadzone on additional axes
	// Default Linux values are 255 for fuzz and 4096 for flat (deadzone)
	short *additional_axes;
	short moza_axes[] = MOZA_ADDITIONAL_AXES;
	short cammus_axes[] = CAMMUS_ADDITIONAL_AXES;
	short axes_cnt = 0;

	switch (hdev->vendor)
	{
	case USB_VENDOR_ID_MOZA:
		hid_dbg(hdev, "Defuzzing MOZA wheelbase");
		additional_axes = moza_axes;
		axes_cnt = sizeof(moza_axes) / sizeof(moza_axes[0]);
		break;

	case USB_VENDOR_ID_CAMMUS:
		hid_dbg(hdev, "Defuzzing CAMMUS wheelbase");
		additional_axes = cammus_axes;
		axes_cnt = sizeof(cammus_axes) / sizeof(cammus_axes[0]);
		break;

	default:
		break;
	}

	// Perform defuzzing
	for (short i = 0; i < axes_cnt; i++)
		input_set_abs_params(input, additional_axes[i],
			input->absinfo[additional_axes[i]].minimum,
			input->absinfo[additional_axes[i]].maximum, 32, 32);

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
