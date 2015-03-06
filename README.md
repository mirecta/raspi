# raspi


masquerade for raspi 

echo 1 > /proc/sys/net/ipv4/ip_forward/sbin/iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
/sbin/iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
