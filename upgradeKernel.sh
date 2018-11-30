#!/bin/sh

cpKernelFile()
{
	echo "Prepare to upgrade kernel."
	sleep 2
	cd /update
	cp /mnt/udisk/rbctrl/update/upgradeKernel.eku .
	if [ $? -ne 0 ]; then
		echo "Prepare upgrade kernel failed."
		sleep 2
		exit 1
	fi
	cd /
	sync
	sleep 2
}

update()
{
	cd /update
	echo "Start to upgrade kernel."
	sleep 2
	./upgradeKernel.eku
	if [ $? -ne 0 ]; then
		echo "upgrade kernel failed."
		sleep 2
		exit 2
	fi
	cd /
}

delTmpFile()
{
	echo "Delete temporary data."
	sleep 2
	cd /update
	rm upgradeKernel.eku
	if [ $? -ne 0 ]; then
		echo "remove temporary data failed."
		sleep 2
		exit 3
	fi
	cd /
}

cpKernelFile
update
delTmpFile
echo "upgrade kernel success."
sleep 2


