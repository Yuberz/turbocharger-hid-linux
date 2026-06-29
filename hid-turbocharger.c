// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  USB HID driver for Virgoo / Rainbow Turbocharger controller, which has an
 *  invalid report descriptor.
 *
 *  Copyright (c) 2024 Mark Mäkinen <marktmakinen@gmail.com>
 */

#include <linux/hid.h>
#include <linux/module.h>

MODULE_AUTHOR("Mark Mäkinen <marktmakinen@gmail.com>");
MODULE_DESCRIPTION("HID driver for Virgoo / Rainbow Turbocharger controller");

/*
 * Virgoo (reported as Rainbow) Turbocharger controller specifies an incorrect
 * value for Logical Minimum for the Slider and Dial for the Game Pad, has its
 * two knobs declared as Slider/Dial instead of X/Y, and has two of its twelve
 * buttons declared on legacy joystick-only Button usages (3 and 6, which the
 * kernel maps to BTN_C/BTN_Z) that "standard gamepad" software doesn't
 * recognize. This replacement descriptor fixes all three issues:
 *
 *  - Logical Minimum for the knobs is corrected to -127 (was parsed as 129).
 *  - The two knobs are relabeled Usage (X) / Usage (Y) instead of
 *    Usage (Slider) / Usage (Dial), so they map to ABS_X/ABS_Y like a
 *    standard gamepad stick, instead of ABS_THROTTLE/ABS_RUDDER (matching
 *    how the Gamo2 Faucetwo reports its knobs).
 *  - Button usages 3 and 6 (which map to the legacy BTN_C/BTN_Z and aren't
 *    part of any standard gamepad button set) are swapped with usages 11
 *    and 12 (BTN_SELECT/BTN_START), which the original descriptor declares
 *    but never actually wires to a physical button.
 *
 * Because the button fix changes a compact Usage Minimum/Maximum range into
 * an explicit per-button Usage list, the descriptor grows from 138 to 158
 * bytes, so unlike a same-size byte patch this needs a full replacement
 * descriptor rather than in-place edits.
 */
static const __u8 turbocharger_rdesc_fixed[] = {
	0x05, 0x01,		/*   Usage Page (Generic Desktop)        */
	0x09, 0x05,		/*   Usage (Game Pad)                    */
	0xa1, 0x01,		/*   Collection (Application)            */
	0x85, 0x01,		/*     Report ID (1)                     */
	0x75, 0x04,		/*     Report Size (4)                   */
	0x95, 0x01,		/*     Report Count (1)                  */
	0x81, 0x03,		/*     Input (Cnst,Var,Abs)              */
	0x05, 0x09,		/*     Usage Page (Button)                */
	0x09, 0x01,		/*     Usage (1)  -> BTN_SOUTH  (unused)  */
	0x09, 0x02,		/*     Usage (2)  -> BTN_EAST   (FX-L)    */
	0x09, 0x0b,		/*     Usage (11) -> BTN_SELECT (FX-R)    */
	0x09, 0x04,		/*     Usage (4)  -> BTN_NORTH  (SELECT)  */
	0x09, 0x05,		/*     Usage (5)  -> BTN_WEST   (BT-A)    */
	0x09, 0x0c,		/*     Usage (12) -> BTN_START  (BT-D)    */
	0x09, 0x07,		/*     Usage (7)  -> BTN_TL     (BT-B)    */
	0x09, 0x08,		/*     Usage (8)  -> BTN_TR     (BT-C)    */
	0x09, 0x09,		/*     Usage (9)  -> BTN_TL2    (unused)  */
	0x09, 0x0a,		/*     Usage (10) -> BTN_TR2    (START)   */
	0x09, 0x03,		/*     Usage (3)  -> BTN_C      (dead bit)*/
	0x09, 0x06,		/*     Usage (6)  -> BTN_Z      (dead bit)*/
	0x15, 0x00,		/*     Logical Minimum (0)                */
	0x25, 0x01,		/*     Logical Maximum (1)                */
	0x75, 0x01,		/*     Report Size (1)                    */
	0x95, 0x0c,		/*     Report Count (12)                  */
	0x81, 0x02,		/*     Input (Data,Var,Abs)                */
	0x05, 0x01,		/*     Usage Page (Generic Desktop)        */
	0x09, 0x30,		/*     Usage (X)        [was Slider]       */
	0x09, 0x31,		/*     Usage (Y)        [was Dial]         */
	0x16, 0x81, 0xff,	/*     Logical Minimum (-127)              */
	0x26, 0x7f, 0x00,	/*     Logical Maximum (127)               */
	0x75, 0x08,		/*     Report Size (8)                     */
	0x95, 0x02,		/*     Report Count (2)                    */
	0x81, 0x02,		/*     Input (Data,Var,Abs)                */
	0xc0,			/*   End Collection                        */
	0x05, 0x01,		/*   Usage Page (Generic Desktop)          */
	0x09, 0x02,		/*   Usage (Mouse)                         */
	0xa1, 0x01,		/*   Collection (Application)              */
	0x85, 0x02,		/*     Report ID (2)                       */
	0x09, 0x01,		/*     Usage (Pointer)                     */
	0xa1, 0x00,		/*     Collection (Physical)               */
	0x05, 0x09,		/*       Usage Page (Button)                */
	0x19, 0x01,		/*       Usage Minimum (1)                  */
	0x29, 0x03,		/*       Usage Maximum (3)                  */
	0x15, 0x00,		/*       Logical Minimum (0)                */
	0x25, 0x01,		/*       Logical Maximum (1)                */
	0x75, 0x01,		/*       Report Size (1)                    */
	0x95, 0x03,		/*       Report Count (3)                   */
	0x81, 0x02,		/*       Input (Data,Var,Abs)                */
	0x75, 0x05,		/*       Report Size (5)                    */
	0x95, 0x01,		/*       Report Count (1)                   */
	0x81, 0x03,		/*       Input (Cnst,Var,Abs)                */
	0x05, 0x01,		/*       Usage Page (Generic Desktop)        */
	0x09, 0x30,		/*       Usage (X)                           */
	0x09, 0x31,		/*       Usage (Y)                           */
	0x15, 0x81,		/*       Logical Minimum (-127)              */
	0x25, 0x7f,		/*       Logical Maximum (127)               */
	0x75, 0x08,		/*       Report Size (8)                     */
	0x95, 0x02,		/*       Report Count (2)                    */
	0x81, 0x06,		/*       Input (Data,Var,Rel)                */
	0xc0,			/*     End Collection                        */
	0xc0,			/*   End Collection                          */
	0x06, 0x00, 0xff,	/*   Usage Page (Vendor Defined FF00)        */
	0x09, 0x01,		/*   Usage (1)                               */
	0xa1, 0x01,		/*   Collection (Application)                */
	0x85, 0x03,		/*     Report ID (3)                          */
	0x06, 0x00, 0xff,	/*     Usage Page (Vendor Defined FF00)       */
	0x09, 0x01,		/*     Usage (1)                              */
	0x15, 0x00,		/*     Logical Minimum (0)                    */
	0x25, 0xff,		/*     Logical Maximum (255)                  */
	0x75, 0x08,		/*     Report Size (8)                        */
	0x95, 0x10,		/*     Report Count (16)                      */
	0x81, 0x02,		/*     Input (Data,Var,Abs)                   */
	0x09, 0x01,		/*     Usage (1)                              */
	0x15, 0x00,		/*     Logical Minimum (0)                    */
	0x25, 0xff,		/*     Logical Maximum (255)                  */
	0x75, 0x08,		/*     Report Size (8)                        */
	0x95, 0x3f,		/*     Report Count (63)                      */
	0x91, 0x02,		/*     Output (Data,Var,Abs)                  */
	0xc0,			/*   End Collection                           */
};

static const __u8 *turbocharger_report_fixup(struct hid_device *hdev, __u8 *rdesc,
		unsigned int *rsize)
{
	/* If this is the original 138-byte descriptor with the Logical Minimum bug,
	 * replace it entirely with our fixed 158-byte descriptor.
	 */
	if (*rsize == 138 && rdesc[36] == 0x16 && rdesc[37] == 0x81 && rdesc[38] == 0) {
		hid_info(hdev, "Replacing Turbocharger descriptor with full fixed version\n");
		*rsize = sizeof(turbocharger_rdesc_fixed);
		return turbocharger_rdesc_fixed;
	}

	/* If it doesn't match, return the original unchanged */
	return rdesc;
}

static const struct hid_device_id turbocharger_devices[] = {
	{ HID_USB_DEVICE(0xC252, 0x1F01) },
	{ }
};
MODULE_DEVICE_TABLE(hid, turbocharger_devices);

static struct hid_driver turbocharger_driver = {
	.name = "turbocharger",
	.id_table = turbocharger_devices,
	.report_fixup = turbocharger_report_fixup
};

module_hid_driver(turbocharger_driver);

MODULE_LICENSE("GPL");
