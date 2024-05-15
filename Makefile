KVERSION := `uname -r`
KDIR := /lib/modules/${KVERSION}/build

default:
	$(MAKE) -C $(KDIR) M=$$PWD

install: default
	$(MAKE) -C $(KDIR) M=$$PWD modules_install
	depmod -A

clean:
	$(MAKE) -C $(KDIR) M=$$PWD clean

