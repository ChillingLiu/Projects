#!/bin/sh

echo "\r\n--------------------Make--------------------"
make
echo "\r\n---------------------ls---------------------"
ls
echo "\r\n----------------sudo dmesg -C---------------"
echo "This program asks for your passcode for the 'sudo' procedures, so don't be afraid of it.^^"
sudo dmesg -C


echo "\r\n\n\n----------------------------------------------------------------------------------------"
echo "Problem 1: Design a kernel module jiffies:"
echo "How many seconds do you want to sleep after insmod is done before doing rmmod?"
read time1
echo "\r\n-----------sudo insmod jiffies.ko-----------"
sudo insmod jiffies.ko
echo "\r\n--------------------dmesg-------------------"
sudo dmesg
echo "\r\n--------------cat /proc/jiffies-------------"
cat /proc/jiffies

echo "\r\n--------Sleeping for $time1 seconds.--------"
sleep $time1
echo "\r\n--------------cat /proc/jiffies-------------"
cat /proc/jiffies
echo "\r\n-------------sudo rmmod jiffies-------------"
sudo rmmod jiffies
echo "\r\n--------------------dmesg-------------------"
sudo dmesg


echo "\r\n\n\n----------------------------------------------------------------------------------------"
echo "Problem 2: Design a kernel module seconds:"
echo "How many seconds do you want to sleep after insmod is done before doing rmmod?"
read time2
echo "\r\n-----------sudo insmod seconds.ko-----------"
sudo insmod seconds.ko
echo "\r\n--------------------dmesg-------------------"
sudo dmesg
echo "\r\n--------------cat /proc/seconds-------------"
cat /proc/seconds

echo "\r\n--------Sleeping for $time2 seconds.--------"
sleep $time2
echo "\r\n--------------cat /proc/seconds-------------"
cat /proc/seconds
echo "\r\n-------------sudo rmmod seconds-------------"
sudo rmmod seconds
echo "\r\n--------------------dmesg-------------------"
sudo dmesg

echo "\r\n\n\n------------------Done------------------"
echo "-------------rm files except .ko------------"
rm jiffies.mod* jiffies.o
rm seconds.mod* seconds.o
rm modules.order Module.symvers
