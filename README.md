# Universal Force Feedback driver for Linux

Linux PIDFF driver with useful patches for initialization of FFB devices. Primarily targeting Direct Drive wheelbases.

## What's different between this and native pidff driver?
That driver allows most DirectDrive wheelbases to initialize and work.
Most of the DirectDrive wheelbases are basically DirectInput wheels, but with some caveats, which Windows allows, but pidff doesn't.
In that repository - pidff driver with some changes, which allows most of the DirectDrive wheelbases to work.

1. Added multiple quirks for better initialization rules for different wheelbases (MOZA, VRS, Cammus)
2. Fixes for infinite-length effects
3. Fixes for out-of-bounds values (no more spam in kernel logs)

And that's basically it

## What devices are supported?
### Bases:
1. MOZA R3, R5, R9, R12, R16, R21
2. Cammus C5
3. VRS DirectForce Pro
4. ...

## What works?
1. FFB (all effects from device descriptor)
2. All inputs (wheel axis, buttons)


## What does not work?
1. Telemetry functions (Shift lights, displays, SimHub, etc), mostly because telemetry works only with proprietary soft, which can't get access to shared memory chunks from games.
2. `Firmware Update` function. Use Windows PC or Windows VM at the moment.
3. Setup through proprietary software. May require [some tweaking](#how-to-set-up-a-base-parameters))

## How to use this driver?
There's an [AUR packege](https://aur.archlinux.org/packages/universal-ff-dkms-git) for Arch Linux maintained by @Lawstorant

Alternatively, you can install it through DKMS or manually.
### DKMS
1. Install `dkms`
2. Clone repository to `/usr/src/universal-pidff`
3. Install the module:
`sudo dkms install /usr/src/universal-pidff`
4. Update initramfs:
`sudo update-initramfs -u`
5. Reboot

To remove module:
`sudo dkms remove universal-pidff/<version> --all`
### Manually

1. Install `linux-headers-$(uname -r)`
2. Clone repository
3. `make`
4. `sudo insmod hid-universal-pidff.ko`

To unload module:
`sudo rmmod hid_universal_pidff`

## How to set up a wheelbase parameters (max rotation degree, max power, filters, etc)?
### MOZA
**[Boxflat](https://github.com/Lawstorant/boxflat)** is a Linux Pit House alternative made by [@Lawstorant](https://github.com/Lawstorant)

**[Android App](https://play.google.com/store/apps/details?id=com.gudsen.mozapithouse)**

### Cammus
**[Android App](https://play.google.com/store/apps/details?id=com.cammus.simulator)**

### VRS DirectForce Pro
You can do it through VRS with Wine. You need to tweak Wine prefix for them.

That soft uses hidraw to set up a base. You need to create `udev` rule for allow access to hidraw device:
```
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="a355", MODE="0666", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-vrs.rules

udevadm control --reload-rules && udevadm trigger
```

Then you need to force VRS soft to use hidraw, not SDL, to find devices:
1. Create new Wine prefix for them:

      `mkdir ~/vrs-wine`
2. Launch regedit in prefix:

      `WINEPREFIX=$HOME/vrs-wine wine regedit`
3. Create two keys in
  `HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\winebus`:

    * `DisableInput` (DWORD) - set to `1`;
    * `Enable SDL` (DWORD) - set to `0`; (yes, variable name contains  space)
4. Now you can launch soft through that WINEPREFIX:

    `WINEPREFIX=$HOME/vrs-wine wine VRS.exe` - launch your soft from installation directory.


## Known issues with the driver
### MOZA
- Buttons above 80 simply don't show up. This is a Linux limitation and we're trying to fix that in the upstream

## Known issues with the firmware
You tell me please

## DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
