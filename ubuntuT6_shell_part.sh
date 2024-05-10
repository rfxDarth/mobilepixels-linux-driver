#!/bin/sh
lines=100
KDIR=$(uname -r)
if [ "$1" = "uninstall" ]; then
	echo "uninstall T6evdi"
	killall T6evdi
	service evdi_t6 stop
	systemctl stop T6evdi.service 
	systemctl stop T6poweron.service
	systemctl stop T6powerdown.service
	rmmod evdi
	rm  /usr/lib/libevdi.so
	rm  /usr/lib/libevdi.so.0
	rm  /usr/include/evdi_lib.h 
	rm  /etc/init.d/evdi_t6
	rm  /usr/sbin/T6evdi
	rm  /usr/sbin/T6contrl
	rm  /etc/systemd/system/T6evdi.service 
	rm  /etc/systemd/system/T6poweron.service
	rm  /etc/systemd/system/T6powerdown.service
	rm  /lib/modules/$KDIR/kernel/drivers/video/evdi.ko 
	echo "restart system.............."
	reboot 
	return
fi
EXE=$(ps aux|grep T6evdi |grep -v grep)
if [ "$EXE" != "" ]; then
	killall T6evdi
	service evdi_t6 stop
	systemctl stop T6evdi.service 
	systemctl stop T6poweron.service
	systemctl stop T6powerdown.service 
	rmmod evdi
	rm  /usr/lib/libevdi.so
	rm  /usr/lib/libevdi.so.0
	rm  /usr/include/evdi_lib.h 
	rm  /etc/init.d/evdi_t6
	rm  /usr/sbin/T6evdi
	rm  /usr/sbin/T6contrl
	rm  /etc/systemd/system/T6evdi.service 
	rm  /etc/systemd/system/T6poweron.service
	rm  /etc/systemd/system/T6powerdown.service
	rm  /lib/modules/$KDIR/kernel/drivers/video/evdi.ko 
fi
apt-mark hold linux-generic linux-image-generic linux-headers-generic
apt-get install libdrm-dev libusb-1.0-0-dev libelf-dev
apt-get install libturbojpeg0-dev build-essential
#apt-get install libx11-dev libxrandr-dev libxinerama-dev libpulse-dev libjpeg-turbo8-dev libjpeg8-dev
tail -n+$lines $0 >/tmp/packet.tar.gz
tar zxvf /tmp/packet.tar.gz 
tar zxvf evdi_t6.tar.gz
tar zxvf evdi.tar.gz
cd  evdi
make
kmodsign sha256 key.asc cert.der module/evdi.ko
SECURE=$(mokutil --sb-state |grep enabled)
if [ "$SECURE" != "" ]; then  
	echo "Input enroll MOK key password!!"
	mokutil --import cert.der
fi
cp module/evdi.ko /lib/modules/$KDIR/kernel/drivers/video/
cp library/libevdi.so  /usr/lib/
cp library/libevdi.so  /usr/lib/libevdi.so.0
cp library/evdi_lib.h /usr/include/
depmod
cd ..
cd evdi_t6_1
make
cp T6evdi /usr/sbin/
cp T6contrl /usr/sbin/
#cp evdi_t6.sh /etc/init.d/evdi_t6
echo evdi >> /etc/modules
cp evdi.conf /etc/modprobe.d/
#cp xorg.conf.new /etc/X11/xorg.conf
#chmod 755 /etc/init.d/evdi_t6
#update-rc.d evdi_t6 defaults
cp T6evdi.service /etc/systemd/system/
chmod 755 /etc/systemd/system/T6evdi.service
systemctl enable T6evdi.service 
cp T6poweron.service /etc/systemd/system/
chmod 755 /etc/systemd/system/T6poweron.service
systemctl enable T6poweron.service 
cp T6powerdown.service /etc/systemd/system/
chmod 755 /etc/systemd/system/T6powerdown.service
systemctl enable T6powerdown.service 
cd ..
#echo default-sample-rate = 48000 >> /etc/pulse/daemon.conf
#echo load-module module-alsa-sink device=hw:Loopback,0 >>/etc/pulse/default.pa
#service evdi_t6 start
rm -rf evdi evdi_t6_1 evdi_t6.tar.gz evdi.tar.gz
DRV=$(ls /lib/modules/$KDIR/kernel/drivers/video/ |grep evdi.ko |grep -v grep)
EXE2=$(ls /usr/sbin/ |grep T6evdi |grep -v grep)
if [ "$EXE2" != "" ] && [ "$DRV" != "" ]; then
	echo "restart system.............."
	reboot 
else
	echo "Driver Installation Failed"
fi
exit 0
