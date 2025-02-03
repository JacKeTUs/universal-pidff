#! /usr/bin/env bash

ro_status=$(steamos-readonly status)

if [[ $ro_status == "enabled" ]]; then
    echo "Disabling readonly"
    echo ""
    steamos-readonly disable
fi

pacman-key --init
pacman-key --populate archlinux
pacman-key --populate holo

mkdir universal-pidff-install
cd universal-pidff-install

wget https://aur.archlinux.org/cgit/aur.git/snapshot/universal-pidff-dkms-git.tar.gz
tar -xf universal-pidff-dkms-git.tar.gz

# to ABSOLUTELY make sure we have acces when running sudo -u deck
chown -R deck:deck universal-pidff-dkms-git
chmod 777 universal-pidff-dkms-git

echo ""
echo "Don't worry about \"error: command failed to execute correctly\""
echo ""
linux=$(pacman -Qsq linux-neptune | grep -e "[0-9]$" | tail -n 1)
pacman -Syu --noconfirm base-devel fakeroot glibc git \
    $linux $linux-headers linux-api-headers

echo ""
echo "You'll be asked for password again :("
echo ""
sleep 2s

cd universal-pidff-dkms-git
sudo -u deck makepkg -scri --noconfirm

cd ../..
rm -rf universal-pidff-install

echo ""
echo ""
echo "Done!"
echo "Just reboot your Deck :)"
