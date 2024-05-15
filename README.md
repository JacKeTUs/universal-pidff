# Force feedback support for MOZA steering wheels

Linux module driver for MOZA driving wheels.

MOZA wheels are basically DirectInput wheels.
In that repository - copy of pidff driver from 6.6 kernel with some changes around infinite length effects (like that https://github.com/berarma/ffbtools/issues/26)

And that's basically it

## What devices are supported?
### Bases:
1. MOZA R3
1. MOZA R5
1. MOZA R9
1. MOZA R12
1. MOZA R16
1. MOZA R21

### Wheel rims:
Not tested

## What works?
1. FFB (all effects from device descriptor)
2. All inputs (wheel axis, buttons)
3. Setup through MOZA PitHouse with [some tweaking](#how-to-set-up-a-base-parameters))

## What does not work?
1. Telemetry functions (Shift lights, etc), mostly because telemetry works only with proprietary soft, which can't get access to shared memory chunks from games.
2. `Firmware Update` function. Use Windows PC or Windows VM at the moment.

## How to use that driver?
You can install it through DKMS or manually.
### DKMS
1. Install `dkms`
2. Clone repository to `/usr/src/moza-ff`
3. Install the module: 
`sudo dkms install /usr/src/moza-ff`
4. Update initramfs:
`sudo update-initramfs -u`
5. Reboot

To remove module:
`sudo dkms remove moza-ff/<version> --all`
### Manually 

1. Install `linux-headers-$(uname -r)`
2. Clone repository
3. `make`
4. `sudo insmod hid-moza-ff.ko`

To unload module:
`sudo rmmod hid_moza_ff`

## How to set up a base parameters?

You can do it through MOZA PitHouse with Wine. You need to tweak Wine prefix for them.

That soft uses hidraw to set up a base. You need to create `udev` rule for allow access to hidraw device:
```
echo 'KERNEL=="hidraw*", ATTRS{idVendor}=="346e", MODE="0660", TAG+="uaccess"' | sudo tee /etc/udev/rules.d/11-moza.rules

udevadm control --reload-rules && udevadm trigger
```

Then you need to force MOZA soft to use hidraw, not SDL, to find devices:
1. Create new Wine prefix for them:

      `mkdir ~/moza-wine`
2. Launch regedit in prefix:

      `WINEPREFIX=$HOME/moza-wine wine regedit`
3. Create two keys in
  `HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\winebus`:

    * `DisableInput` (DWORD) - set to `1`;
    * `Enable SDL` (DWORD) - set to `0`; (yes, variable name contains  space)
4. Now you can launch soft through that WINEPREFIX:

    `WINEPREFIX=$HOME/moza-wine wine MOZA\ Pit\ House.exe` - launch your PitHouse from installation directory.

## Known issues with the driver
1. Firmware update does not work. Please use Windows machine or Windows VM for any firmware updates

## Known issues with the firmware 
You tell me please

## DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.