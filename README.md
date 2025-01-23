# Universal Force Feedback driver for Linux

Linux PIDFF driver with useful patches for initialization of FFB devices. Primarily targeting Direct Drive wheelbases.

Check out Linux Simracing community channel on [Matrix](https://matrix.to/#/#simracing:matrix.org)

## What's different between this and native pidff driver?
That driver allows most DirectDrive wheelbases to initialize and work.
Most of the DirectDrive wheelbases are basically DirectInput wheels, but with some caveats, which Windows allows, but pidff doesn't.
In that repository - pidff driver with some changes, which allows most of the DirectDrive wheelbases to work.

1. Added multiple quirks for better initialization rules for different wheelbases
2. Fixes for infinite-length effects
3. Fixes for out-of-bounds values (no more spam in kernel logs)

And that's basically it

## What devices are supported?
### Bases:
1. MOZA R3, R5, R9, R12, R16, R21
2. Cammus C5, C12
3. VRS DirectForce Pro
4. FFBeast Wheel, Joystick, Rudder
5. PXN V10, V12, V12 Lite
6. Lite Star GT978 FF
7. ...

## What works?
1. FFB (all effects from device descriptor)
2. All inputs (wheel axis, buttons)


## What does not work?
1. Telemetry functions. They are handeled by [Monocoque](https://github.com/Spacefreak18/monocoque)
2. `Firmware Update` function. Use Windows PC or Windows VM at the moment.
3. Setup through proprietary software. May require [some tweaking](#how-to-set-up-a-base-parameters)

## How to install this driver?
You can install it through AUR package, through DKMS or manually.

On SecureBoot enabled systems you will need additional steps for load this driver into the system. See [Signing](docs/SIGNING.md#signing) section.

### AUR package
There's an [AUR package](https://aur.archlinux.org/packages/universal-pidff-dkms-git) for Arch Linux maintained by [@Lawstorant](https://github.com/Lawstorant).

### DKMS
DKMS will install module into system, and will update it every time you update your kernel. Module will persist after reboots. It's the preferrable way to install it on the most distros.

1. Install `dkms` package from your distro package manager
2. Clone repository to `/usr/src/universal-pidff`
3. Install the module:
`sudo dkms install /usr/src/universal-pidff`
4. Update initramfs:
`sudo update-initramfs -u`
5. Reboot

To remove module:
`sudo dkms remove universal-pidff/<version> --all`

### Manually
Best for debugging purposes, where you need frequently change codebase/branches
1. Install `linux-headers-$(uname -r)` and `build-essential` packages from your distro package manager
2. Clone repository anywhere you want and `cd` into that directory
3. `make`. Alternatively, you can enable debug logs from the driver with `make debug`
4. Load module into system with `sudo insmod hid-universal-pidff.ko`

To unload module:
`sudo rmmod hid_universal_pidff`

### Testing
To test the supported effects, use ffbplay from [ffbtools](https://github.com/berarma/ffbtools) and play the included [effect-test.ffb](./effect-test.ffb) file

## How to set up a base parameters  (max rotation degree, max power, filters, etc)?
### MOZA
**[Boxflat](https://github.com/Lawstorant/boxflat)** is a Linux Pit House alternative made by [@Lawstorant](https://github.com/Lawstorant)

**[Android App](https://play.google.com/store/apps/details?id=com.gudsen.mozapithouse)**

### Cammus
**[Android App](https://play.google.com/store/apps/details?id=com.cammus.simulator)**

### VRS DirectForce Pro
You can do it through VRS with Wine. You need to tweak Wine prefix for them.

That software uses hidraw to set up a base. You need to create `udev` rule for allow access to hidraw device:
```
# VRS DFP
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="a355", MODE="0666", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-vrs.rules
# VRS pedals
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="a3be", MODE="0666", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-vrs.rules

udevadm control --reload-rules && udevadm trigger
```

Then you need to force VRS software to use hidraw, not SDL, to find devices:
1. Create new Wine prefix for them:

      `mkdir ~/.vrs-wine`
1. Launch regedit in prefix:

      `WINEPREFIX=$HOME/.vrs-wine wine regedit`
1. Create two keys in
  `HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\winebus`:

    * `DisableInput` (DWORD) - set to `1`;
    * `Enable SDL` (DWORD) - set to `0`; (yes, variable name contains space)
1. Install important packages for VRS config tool to work:
      `WINEPREFIX=$HOME/.vrs-wine winetricks win7`
      `WINEPREFIX=$HOME/.vrs-wine winetricks dotnet48`

1. Now you can launch soft through that WINEPREFIX:

    `WINEPREFIX=$HOME/.vrs-wine wine VRS.exe` - launch VRS software from installation directory.


## Known issues with the driver
- Current limit of usable buttons is 160 (up from the Linux default of 80). Create an issue if you want this increased further.

## Known issues with the firmware
You tell me please

## DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
