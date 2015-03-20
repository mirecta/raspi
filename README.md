# raspi


## masquerade for raspi 

```
echo 1 > /proc/sys/net/ipv4/ip_forward
/sbin/iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
```
## compile own kernel on pc

```
http://elinux.org/Raspberry_Pi_Kernel_Compilation

export CCPREFIX="/home/miro/mDATA/raspi/tools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin/arm-bcm2708hardfp-linux-gnueabi-"
make ARCH=arm CROSS_COMPILE=${CCPREFIX} && make ARCH=arm CROSS_COMPILE=${CCPREFIX} modules

export MODULES_TEMP=~/mDATA/raspi/modules
make ARCH=arm CROSS_COMPILE=${CCPREFIX} INSTALL_MOD_PATH=${MODULES_TEMP} modules_install


sudo modprobe fbtft dma
sudo modprobe fbtft_device custom name=fb_ili9341  speed=18000000  gpios=led:18,reset:25,dc:24 rotate=90 bgr=1
```
