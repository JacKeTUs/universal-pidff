# Universal Force Feedback driver for Linux

Linux PIDFF driver with useful patches for initialization of FFB devices. Primarily targeting Direct Drive wheelbases.

Check out Linux Simracing community channel on [Matrix](https://matrix.to/#/#simracing:matrix.org)

## What's different between this and native pidff driver?
> [!NOTE]
> This driver was upstreamed into kernel from versions 6.12.24+, 6.13.12+, 6.14.3+. 6.15+ and greater, so with recent kernel there is no functional differences. You can install driver from this repo if you want to test some new functions/devices and/or debug something specific.

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
7. Asetek Invicta, Forte, La Prima, Tony Kanaan
8. ...

Kernel `pidff` driver will try to probe all devices with HID PID descriptor, so even if your device isn't listed here, it can probably just work (with recent kernel version).
Version of the driver in this repository will probe only devices with selected VID/PIDs.

## What works?
1. FFB (all effects defined from device descriptor)
2. All inputs (wheel axis, buttons, passthrough axis/buttons for wheel (e.g. Moza))


## What does not work?
1. Telemetry functions. It does not depend on the driver itself. Telemetry LEDs/shakers can be handled by [Monocoque](https://github.com/Spacefreak18/monocoque).
2. `Firmware Update` function. Use Windows PC or Windows VM at the moment.
3. Setup through proprietary software may or may not work. It does not depend on the driver itself. May require [some tweaking](#how-to-set-up-a-base-parameters) in corresponding Wine prefix.

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

### Steam Deck/SteamOS
#### Automatic install
```bash
# first, let's set a password
# (optional, skip if you already done this in the past)
passwd deck

# run installation script
sudo sh -c "$(curl -fsSL https://raw.githubusercontent.com/JacKeTUs/universal-pidff/main/scripts/steam-deck-install.sh)"
```
#### Automatic uninstall:
```bash
# and this is uninstall script if needed:
sudo sh -c "$(curl -fsSL https://raw.githubusercontent.com/JacKeTUs/universal-pidff/main/scripts/steam-deck-uninstall.sh)"

# optionally, remove your password
# enter current one and leave the new password blank
passwd deck
```

#### Manual install
```bash
# first, let's set a password
# (optional, skip if you already done this in the past)
passwd deck

# then, let's disable read only state
sudo steamos-readonly disable

# initialise and populate keyring
sudo pacman-key --init
sudo pacman-key --populate archlinux
sudo pacman-key --populate holo

# install necessary stuff
linux=$(pacman -Qsq linux-neptune | tail -n 1)
sudo pacman -Syu --noconfirm base-devel fakeroot glibc git \
    $linux $linux-headers linux-api-headers

# download AUR packege
wget https://aur.archlinux.org/cgit/aur.git/snapshot/universal-pidff-dkms-git.tar.gz
tar -xf universal-pidff-dkms-git.tar.gz

# finally install the driver itself
cd universal-pidff-dkms-git
makepkg -scri --noconfirm

# optionally, remove things we needed during installation
cd ..
rm -rf universal-pidff-dkms-git*
```

And now, just reboot and enjoy!

#### If you need to uninstall this driver and lock your deck again:
```bash
linux=$(pacman -Qsq linux-neptune | grep -e "[0-9]$" | tail -n 1)

sudo pacman -Rcns hid-universal-pidff-dkms-git
sudo pacman -Rcns $linux-headers fakeroot

sudo steamos-readonly enable

# optionally, remove your password
# enter current one and leave the new password blank
passwd deck
```

### Testing
1. Use the `test` make target as root. `sudo make test` builds module with debug, loads it and cleans the working directory.
2. Start new terminal and run `journalctl -f -k` to monitor new kernel messages
3. Connect wheelbase via USB to your PC
4. To test the supported effects, use ffbplay from [ffbtools](https://github.com/berarma/ffbtools) and play the included [effect-test.ffb](./effect-test.ffb) file e.g.
```
<path to built ffbplay>/ffbplay -d /dev/input/by-id/usb-<wheelbase-id> ./effect-test.ffb
```
Make sure that all effects were played and the wheelbase reacted accordingly.
5. Switch back to terminal with running `journalctl` command, it should have a lot of debug messages


## How to set up a base parameters  (max rotation degree, max power, filters, etc)?
### MOZA
**[Boxflat](https://github.com/Lawstorant/boxflat)** is a Linux Pit House alternative made by [@Lawstorant](https://github.com/Lawstorant)

**[Android App](https://play.google.com/store/apps/details?id=com.gudsen.mozapithouse)**

### Cammus
**[Android App](https://play.google.com/store/apps/details?id=com.cammus.simulator)**

### VRS DirectForce Pro
You can do it through VRS config tool through Steam as non-Steam game or with modified Wine prefix.

> [!WARNING]
> Firmware update doesn't work at this time, puts the wheelbase onto bootloop. If you need to update the wheelbase, do it from Windows (VM with USB passthrough or dualboot) for now. It is known issue, and will be fixed in long term.

#### Prerequisites:
You need to install udev rules, which will open hidraw descriptors to the wheelbase, wheels, pedals.

```
# VRS DFP
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="a355", MODE="0666", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-vrs.rules
# VRS pedals
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="a3be", MODE="0666", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-vrs.rules
# VRS wheel
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="0483", ATTRS{idProduct}=="a44c", MODE="0666", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-vrs.rules

udevadm control --reload-rules && udevadm trigger
```

#### Steam method
1. Add VRS config tool to Steam as non-Steam game. Rename it as you like.
2. Compatibility -> Force the use of specific Steam Play compatibility tool -> set recent enough Proton on it. Proton 10, Proton Experimental, Proton GE worked fine.
3. Change Shortcut -> Launch options like so:
```
PROTON_ENABLE_HIDRAW=1 %command%
```

#### Modified Wine prefix
You need to force VRS software to use hidraw, not SDL, to communicate with devices:
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

Note: In order to play Damping/Friction/Inertia/Spring effects by games, you must enable `Use device and game effects` from dropdown menu for these in DirectForce configuration tool and save it to the wheelbase.

### Asetek
You need to enable "high torque mode" after device is turned on/plugged in.
More info here: [asetek_wheelbase_cli repo](https://github.com/moonrail/asetek_wheelbase_cli)

## Known issues with the driver
- Current limit of usable buttons is 160 (up from the Linux default of 80). Create an issue if you want this increased further.

## Known issues with the firmware
You tell me please

## DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
