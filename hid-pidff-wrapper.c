// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * HID PIDFF wrapper
 * First of all targeting steering wheels and wheelbases
 *
 * Copyright (c) 2024 Makarenko Oleg
 * Copyright (c) 2024 Tomasz Pakuła
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>
#include <linux/input-event-codes.h>
#include "hid-ids.h"
#include "hid-pidff.h"

#define JOY_RANGE (BTN_DEAD - BTN_JOYSTICK + 1)

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
 * Map buttons manually to extend the default joystick buttn limit
 */
static int universal_pidff_input_mapping(struct hid_device *hdev, struct hid_input *hi,
		struct hid_field *field, struct hid_usage *usage,
		unsigned long **bit, int *max)
{
	// Let the default behavior handle mapping if usage is not a button
	if ((usage->hid & HID_USAGE_PAGE) != HID_UP_BUTTON)
		return 0;

	int button = ((usage->hid - 1) & HID_USAGE);
	int code = button + BTN_JOYSTICK;

	// Detect the end of JOYSTICK buttons range
	// KEY_AUDIO_DESC = 0x270
	if (code > BTN_DEAD)
		code = button + KEY_NEXT_FAVORITE - JOY_RANGE;

	// Map overflowing buttons to KEY_RESERVED for the upcoming new input event
	// It will handle button presses differently and won't depend on defined
	// ranges. KEY_RESERVED usage is needed for the button to not be ignored.
	if (code > KEY_MAX)
		code = KEY_RESERVED;

	hid_map_usage(hi, usage, bit, max, EV_KEY, code);
	hid_dbg(hdev, "Button %d: usage %d", button, code);
	return 1;
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
	short axis;
	for (axis = ABS_Y; axis <= ABS_BRAKE, axis++) {
		if (!test_bit(input->absbit, axis))
			continue;

		input_set_abs_params(input, axis,
			input->absinfo[axis].minimum,
			input->absinfo[axis].maximum, 8, 0);
	}

	return 0;
}

static struct hid_driver universal_pidff = {
	.name = "hid-universal-pidff",
	.id_table = pidff_wheel_devices,
	.input_mapping = universal_pidff_input_mapping,
	.probe = universal_pidff_probe,
	.input_configured = universal_pidff_input_configured,
	.report_fixup = universal_pidff_report_fixup
};
module_hid_driver(universal_pidff);

MODULE_AUTHOR("Oleg Makarenko <oleg@makarenk.ooo>");
MODULE_AUTHOR("Tomasz Pakuła <tomasz.pakula.oficjalny@gmail.com>");
MODULE_DESCRIPTION("Universal HID PIDFF Driver");
MODULE_LICENSE("GPL");
