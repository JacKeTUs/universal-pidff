KVERSION := `uname -r`
KDIR := /lib/modules/${KVERSION}/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

debug:
	$(MAKE) -C $(KDIR) M=$$PWD EXTRA_CFLAGS="-g -DDEBUG"


install: default
	$(MAKE) -C $(KDIR) M=$$PWD modules_install
	depmod -A

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

