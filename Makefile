KVERSION := `uname -r`
KDIR := /lib/modules/${KVERSION}/build
MODULE_LOADED := $(shell lsmod | grep hid_universal_pidff)

default: clean
	$(MAKE) -C $(KDIR) M=$$PWD

debug: clean
	$(MAKE) -C $(KDIR) M=$$PWD EXTRA_CFLAGS="-g -DDEBUG"

openffb: clean
	$(MAKE) -C $(KDIR) M=$$PWD EXTRA_CFLAGS="-DOPENFFBOARD"

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

install: default
	$(MAKE) -C $(KDIR) M=$$PWD modules_install
	depmod -A

unload:
	@if [ "$(MODULE_LOADED)" != "" ]; then\
		rmmod hid_universal_pidff;\
	fi

load: unload
	insmod hid_universal_pidff.ko
