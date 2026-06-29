# Linux driver for Virgoo Turbocharger

## AI DISCLOSURE:
This fork used AI in order to make the changes and the outcome was verified with my own hardware. Use this module at your own risk.

## Background
This is a fork of [turbocharger-hid-linux](https://github.com/Z1ni/turbocharger-hid-linux) by Z1ni.
The original fork worked amazingly however, some of the button mappings and knob mappings were less universal.

For example, FX-R and BT-D were both mapped to B20 and B21 respectively as seen on [Hardware Tester](https://hardwaretester.com/gamepad).
Additionally, the knobs are not treated like standard Axis inputs for controllers.

This made it so that certain programs/games would not be able to read that high of buttons or not be able to recognize knob inputs.

This fork utilized AI in order to remap these inputs to more standardized buttons (B0-B9 ranges) as well as making the knobs control the left stick on the X and Y axis for easy detection in games.

## Updated Installation Method (DKMS)

1. Download the latest version .zip from [this fork's releases](https://github.com/Yuberz/turbocharger-hid-linux/releases).
2. Install DKMS if you do not already have it
3. Exctract the archive to /usr/src
4. Run `sudo dkms install -m hid-turbocharger -v {version}` where `{version}` is something like 1.1.0.
5. Verify that it was installed succesfully with `dkms status`
6. Create a new udev rule: `sudo nano /etc/udev/rules.d/99-turbocharger.rules`
7. Paste in this line `SUBSYSTEM=="input", ATTRS{idVendor}=="c252", ATTRS{idProduct}=="1f01", ENV{ID_INPUT_JOYSTICK}="1"`
8. Save the file.
9. Reload udev rules:
```
sudo udevadm control --reload-rules
sudo udevadm trigger
```
10. Unplug / plug in your controller if needed
11. It should now be properly recognized on sites like [Hardware Tester](https://hardwaretester.com/gamepad)

Note:
The manual installation method may also work, but is untested.
The udev rule is also experimental and if it does not work, please open an issue.
