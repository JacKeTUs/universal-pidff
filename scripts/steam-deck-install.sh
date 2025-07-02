#! /usr/bin/env bash

ro_status=$(steamos-readonly status)
install_folder_name="universal-pidff-install"

if [[ $ro_status == "enabled" ]]; then
    echo "Disabling readonly"
    echo ""
    steamos-readonly disable
fi

pacman-key --init
pacman-key --populate archlinux
pacman-key --populate holo

mkdir "$install_folder_name"
cd "$install_folder_name" || exit 1

AUR_LINK="https://aur.archlinux.org/cgit/aur.git/plain/PKGBUILD?h="
curl "${AUR_LINK}universal-pidff-dkms-git" -o PKGBUILD_PIDFF

chown deck:deck .
chmod 777 .

echo ""
echo "Don't worry about \"error: command failed to execute correctly\""
echo ""
linux=$(pacman -Qsq linux-neptune | grep -e "[0-9]$" | tail -n 1)
pacman -Syu --noconfirm base-devel fakeroot gcc glibc git \
    $linux $linux-headers linux-api-headers

pacman -S --asdeps --noconfirm dkms

sudo -u deck makepkg -Cc -p PKGBUILD_PIDFF
pacman -U --noconfirm universal-pidff-dkms-git-*.tar.zst

echo ""
echo "Again, don't worry about this ^"

cd ..
rm -rf "$install_folder_name"

echo ""
echo ""
echo "Done!"
echo "Just reboot your Deck :)"
