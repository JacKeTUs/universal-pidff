#! /usr/bin/env bash

linux=$(pacman -Qsq linux-neptune | grep -e "[0-9]$" | tail -n 1)

sudo pacman -Rcns hid-universal-pidff-dkms-git
sudo pacman -Rcns $linux-headers fakeroot

steamos-readonly enable

echo ""
echo ""
echo "Done!"
echo "Just reboot your Deck :)"
