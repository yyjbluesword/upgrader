#!/bin/sh

cpKernelFile()
{
	echo "Prepare to upgrade kernel."
	cd /update
	cp /mnt/udisk/rbctrl/update/upgradeKernel.eku .
	if [ $? -ne 0 ]; then
		echo "Prepare upgrade kernel failed."
		echo "failed"
		exit 1
	fi
	cd /
	sync
}

update()
{
	cd /update
	echo "Start to upgrade kernel."
	./upgradeKernel.eku
	if [ $? -ne 0 ]; then
		echo "Upgrade kernel failed."
		echo "failed"
		exit 2
	fi
	cd /
}

delTmpFile()
{
	echo "Delete temporary data."
	cd /update
	rm upgradeKernel.eku
	if [ $? -ne 0 ]; then
		echo "Delete temporary data failed."
		echo "failed"
		exit 3
	fi
	cd /
}

cpKernelFile
update
delTmpFile
echo "Upgrade kernel success."
echo "success"


