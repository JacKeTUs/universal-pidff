
# Signing

## Signing module for SecureBoot
Latest kernels forbid loading unsigned custom kernel modules into the system with SecureBoot enabled.

For SecureBoot enabled system you have a choice:
1. Disable SecureBoot in your UEFI/BIOS
2. Use generated Machine Owner Key from DKMS (supports automatic signing)
3. Create Machine Owner Key and load it into your UEFI/BIOS, and sign kernel module with it.

### Using DKMS MOK key
MOK private key and certificates are generated the first time DKMS is run. You just need to import it to your system.

The location as well can be changed by setting the appropriate variables in /etc/dkms/framework.conf. For example, to allow usage of the system default Ubuntu update-secureboot-policy set the configuration file as follows:
```
mok_signing_key="/var/lib/shim-signed/mok/MOK.priv"
mok_certificate="/var/lib/shim-signed/mok/MOK.der"
```

```
# Find where keys are on your distro
ls -al /var/lib/dkms/mok*
# OR (on Ubuntu)
ls -al /var/lib/shim-signed/mok/MOK*

# Enroll keys into system
sudo mokutil --import /var/lib/dkms/mok.pub
# OR (on Ubuntu)
sudo mokutil --import /var/lib/shim-signed/mok/MOK.der
```
You need to reboot your PC after that, you will be greeted with blue screen dialog.
Choose "Enroll MOK", then "Continue" and "Yes". After that choose "Reboot system".

Now DKMS should sign updated modules automatically as they updated.

[Reference](https://github.com/dell/dkms/blob/master/README.md#module-signing)

### Manually create MOK key and manually sign kernel module
```
# This creates Machine Owner Key
openssl req -new -x509 -newkey rsa:2048 -keyout mok.key -outform DER -out mok.pub -nodes -days 36500 -subj "/CN=$hostname kernel module signing key/"

# This loads it into UEFI
sudo mokutil --import mok.pub
```

You need to reboot your PC after that, you will be greeted with blue screen dialog
Choose "Enroll MOK", enter your MOK password if exists, then "Continue", "Yes", and then reboot your system.

After that you can manually sign your built kernel module like so (feel free to adjust paths to keys/certificate/modules):
```
sudo /usr/src/linux-headers-$(uname -r)/scripts/sign-file sha256 mok.key mok.pub hid-universal-pidff.ko
```

Then you should be able to load driver like so:
```
sudo insmod hid-universal-pidff.ko
```