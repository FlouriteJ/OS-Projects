#! /bin/bash
sudo dmesg -C;
sudo rmmod taskinfo;
sudo insmod taskinfo.ko;
echo "100" > /proc/taskinfo;
cat /proc/taskinfo;
echo "1" > /proc/taskinfo;
cat /proc/taskinfo;
echo "10000" > /proc/taskinfo;
cat /proc/taskinfo;
dmesg;