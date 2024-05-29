# Force feedback support for MOZA steering wheels

Linux module driver for MOZA driving wheels.

MOZA wheels are basically DirectInput wheels.
In that repository - copy of pidff driver from 6.6 kernel with some changes around infinite length effects (like that https://github.com/berarma/ffbtools/issues/26)

And that's basically it

## What devices are supported?
### Bases:
1. MOZA R3, R5, R9, R12, R16, R21
1. ...

### Wheel rims (others yet untested):
1. MOZA RS V2 (with Moza wheelbases)

## What works?
1. FFB (all effects from device descriptor)
2. All inputs (wheel axis, buttons)


## What does not work?
1. Telemetry functions (Shift lights, displays, SimHub, etc), mostly because telemetry works only with proprietary soft, which can't get access to shared memory chunks from games.
2. `Firmware Update` function. Use Windows PC or Windows VM at the moment.
3. Setup through MOZA PitHouse even with [some tweaking](#how-to-set-up-a-base-parameters))

## How to use this driver?
There's an [AUR packege](https://aur.archlinux.org/packages/moza-ff-dkms-git) for Arch Linux maintained by @Lawstorant

Alternatively, you can install it through DKMS or manually.
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

### MOZA
For now, please, use [Android App](https://play.google.com/store/apps/details?id=com.gudsen.mozapithouse)


## Known issues with the driver
1. Firmware update does not work. Please use Windows machine or Windows VM for any firmware updates

## Known issues with the firmware
You tell me please

## DISCLAIMER
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
